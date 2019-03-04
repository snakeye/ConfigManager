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
    ConfigParameter(const char *name, const char *label, T *ptr, ParameterMode mode = both, std::function<void(const char *)> cb = NULL)
    {
        this->name = name;
        this->label = label;
        this->ptr = ptr;
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
        json->set("label", label);
        json->set("type", GetTypeName<T>());
    }

  private:
    const char *name;
    const char *label;
    T *ptr;
    std::function<void(const char *)> cb;
    ParameterMode mode;
};

/**
 * Config String Parameter
 */
class ConfigStringParameter : public BaseParameter
{
  public:
    ConfigStringParameter(const char *name, const char *label, char *ptr, size_t length, ParameterMode mode = both)
    {
        this->name = name;
        this->label = label;
        this->ptr = ptr;
        this->length = length;
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
        json->set("label", label);
        json->set("type", "string");
    }

  private:
    const char *name;
    const char *label;
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
    ConfigParameterGroup(const char *name, const char *label)
    {
        this->name = name;
        this->label = label;
    }

    template <typename T>
    ConfigParameterGroup &addParameter(const char *name, const char *label, T *variable)
    {
        parameters.push_back(new ConfigParameter<T>(name, label, variable));

        return *this;
    }

    template <typename T>
    ConfigParameterGroup &addParameter(const char *name, const char *label, T *variable, ParameterMode mode)
    {
        parameters.push_back(new ConfigParameter<T>(name, label, variable, mode));

        return *this;
    }

    ConfigParameterGroup &addParameter(const char *name, const char *label, char *variable, size_t size)
    {
        parameters.push_back(new ConfigStringParameter(name, label, variable, size));

        return *this;
    }

    ConfigParameterGroup &addParameter(const char *name, const char *label, char *variable, size_t size, ParameterMode mode)
    {
        parameters.push_back(new ConfigStringParameter(name, label, variable, size, mode));

        return *this;
    }

    void toJsonSchema(JsonObject *json);

  private:
    const char *name;
    const char *label;

    std::list<BaseParameter *> parameters;
};

/**
 * Config Manager
 */
class ConfigManager
{
  public:
    ConfigManager() {}

    Mode getMode();

    void setAPName(const char *name);
    void setAPPassword(const char *password);
    void setAPFilename(const char *filename);
    void setAPTimeout(const int timeout);

    void setWifiConnectRetries(const int retries);
    void setWifiConnectInterval(const int interval);

    void setAPCallback(std::function<void(WebServer *)> callback);
    void setAPICallback(std::function<void(WebServer *)> callback);

    ConfigParameterGroup &addParameterGroup(const char *name, const char *label)
    {
        ConfigParameterGroup *group = new ConfigParameterGroup(name, label);
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
    JsonObject &decodeJson(String jsonString);

    void handleGetSettings();
    void handleGetSettingsSchema();
    void handleGetScan();

    void handleAPGet();
    void handleAPPost();
    void handleRESTGet();
    void handleRESTPut();
    void handleNotFound();

    bool wifiConnected();
    void setup();
    void startAP();
    void startApi();

    void startWebServer();

    void readConfig();
    void writeConfig();

    boolean isIp(String str);
    String toStringIP(IPAddress ip);
};

#endif /* __CONFIGMANAGER_H__ */
