#ifndef __CONFIGMANAGER_H__
#define __CONFIGMANAGER_H__

#include <DNSServer.h>
#include <EEPROM.h>
#include <FS.h>

#if defined(ARDUINO_ARCH_ESP8266) //ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#elif defined(ARDUINO_ARCH_ESP32) //ESP32
#include <SPIFFS.h>
#include <WiFi.h>
#include <WebServer.h>
#endif

#include <functional>
#include <list>

#include "ArduinoJson.h"

#include "typename.h"

#define WIFI_OFFSET 2
#define CONFIG_OFFSET 98

#if defined(ARDUINO_ARCH_ESP8266) //ESP8266
using WebServer = ESP8266WebServer;
#endif

enum Mode
{
    ap,
    api
};

enum ParameterMode
{
    get,
    set,
    both
};

class Metadata
{
  public:
    Metadata(const char *label, const char *description = NULL)
    {
        this->_label = label;
        this->_description = description;
    }

    const char *label()
    {
        return this->_label;
    }

    const char *description()
    {
        return this->_description;
    }

  private:
    const char *_label;
    const char *_description;
};

/**
 * Base Parameter
 */
class BaseParameter
{
  public:
    virtual ParameterMode getMode() = 0;
    virtual void fromJson(JsonObject *json) = 0;
    virtual void toJson(JsonObject *json) = 0;
    virtual void toJsonSchema(JsonObject *json) = 0;
};

/**
 * Config Parameter
 */
template <typename T>
class ConfigParameter : public BaseParameter
{
  public:
    ConfigParameter(const char *name, T *ptr, Metadata *metadata = NULL, ParameterMode mode = both, std::function<void(const char *)> cb = NULL)
    {
        this->name = name;
        this->ptr = ptr;
        this->metadata = metadata;
        this->cb = cb;
        this->mode = mode;
    }

    ParameterMode getMode()
    {
        return this->mode;
    }

    void fromJson(JsonObject *json)
    {
        if (json->containsKey(name) && json->is<T>(name))
        {
            *ptr = json->get<T>(name);
        }
    }

    void toJson(JsonObject *json)
    {
        json->set(name, *ptr);

        if (cb)
        {
            cb(name);
        }
    }

    void toJsonSchema(JsonObject *json)
    {
        json->set("name", name);
        json->set("type", GetTypeName<T>());
        if (metadata != NULL)
        {
            json->set("label", metadata->label());
        }
    }

  private:
    const char *name;
    T *ptr;
    Metadata *metadata;
    std::function<void(const char *)> cb;
    ParameterMode mode;
};

/**
 * Config String Parameter
 */
class ConfigStringParameter : public BaseParameter
{
  public:
    ConfigStringParameter(const char *name, char *ptr, size_t length, Metadata *metadata = NULL, ParameterMode mode = both)
    {
        this->name = name;
        this->ptr = ptr;
        this->length = length;
        this->metadata = metadata;
        this->mode = mode;
    }

    ParameterMode getMode()
    {
        return this->mode;
    }

    void fromJson(JsonObject *json)
    {
        if (json->containsKey(name) && json->is<char *>(name))
        {
            const char *value = json->get<const char *>(name);

            memset(ptr, '\n', length);
            strncpy(ptr, const_cast<char *>(value), length - 1);
        }
    }

    void toJson(JsonObject *json)
    {
        json->set(name, ptr);
    }

    void toJsonSchema(JsonObject *json)
    {
        json->set("name", name);
        json->set("type", "string");
        if (metadata != NULL)
        {
            json->set("label", metadata->label());
        }
    }

  private:
    const char *name;
    Metadata *metadata;
    char *ptr;
    size_t length;
    ParameterMode mode;
};

/**
 * Parameter group
 */
class ConfigParameterGroup
{
  public:
    ConfigParameterGroup(const char *name, Metadata *metadata = NULL)
    {
        this->name = name;
        this->metadata = metadata;
    }

    template <typename T>
    ConfigParameterGroup &addParameter(const char *name, T *variable, Metadata *metadata = NULL, ParameterMode mode = both)
    {
        parameters.push_back(new ConfigParameter<T>(name, variable, metadata, mode));

        return *this;
    }

    ConfigParameterGroup &addParameter(const char *name, char *variable, size_t size, Metadata *metadata = NULL, ParameterMode mode = both)
    {
        parameters.push_back(new ConfigStringParameter(name, variable, size, metadata, mode));

        return *this;
    }

    void toJson(JsonObject *json);
    void toJsonSchema(JsonObject *json);
    void fromJson(JsonObject *json);

    const char *getName()
    {
        return name;
    }

  private:
    const char *name;
    Metadata *metadata;

    std::list<BaseParameter *> parameters;
};

/**
 * Config Manager
 */
class ConfigManager
{
  public:
    ConfigManager() {}

    void setAPName(const char *name);
    void setAPPassword(const char *password);
    void setAPFilename(const char *filename);
    void setAPTimeout(const int timeout);

    void setWifiConnectRetries(const int retries);
    void setWifiConnectInterval(const int interval);

    void setAPCallback(std::function<void(WebServer *)> callback);
    void setAPICallback(std::function<void(WebServer *)> callback);

    ConfigParameterGroup &addParameterGroup(const char *name, Metadata *metadata = NULL)
    {
        ConfigParameterGroup *group = new ConfigParameterGroup(name, metadata);
        groups.push_back(group);

        return *group;
    }

    void loop();

    template <typename T>
    void begin(T &config)
    {
        this->config = &config;
        this->configSize = sizeof(T);

        EEPROM.begin(CONFIG_OFFSET + this->configSize);

        setup();
    }

    void save();

  private:
    Mode mode;
    void *config;
    size_t configSize;

    char *apName = (char *)"Thing";
    char *apPassword = NULL;
    char *apFilename = (char *)"/index.html";
    unsigned long apTimeout = 0;
    unsigned long apStart = 0;

    int wifiConnectRetries = 20;
    int wifiConnectInterval = 500;

    std::unique_ptr<DNSServer> dnsServer;
    std::unique_ptr<WebServer> server;

    std::list<ConfigParameterGroup *> groups;

    std::function<void(WebServer *)> apCallback;
    std::function<void(WebServer *)> apiCallback;

  private:
    void handleGetRoot();

    void handleGetWifi();
    void handleGetWifiScan();
    void handlePostConnect();
    void handlePostDisconnect();

    void handleGetSettings();
    void handleGetSettingsSchema();
    void handlePostSettings();

    void handleAPPost();
    void handleRESTGet();
    void handleRESTPut();
    void handleNotFound();

  private:
    JsonObject &decodeJson(String jsonString);

    void readConfig();
    void writeConfig();
    bool wifiConnected();
    void setup();
    void startAP();
    void startApi();

    void startWebServer();

    boolean isIp(String str);
    String toStringIP(IPAddress ip);
};

#endif /* __CONFIGMANAGER_H__ */
