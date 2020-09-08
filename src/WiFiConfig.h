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

#include <ArduinoJson.h>

#include "typename.h"

#define WIFI_OFFSET 2
#define CONFIG_OFFSET 98

#if defined(ARDUINO_ARCH_ESP8266) //ESP8266
using WebServer = ESP8266WebServer;
#endif

enum Mode
{
    MODE_AP,
    MODE_API
};

enum ParameterMode
{
    PARAMETER_GET = 0x01,
    PARAMETER_SET = 0x02,
    PARAMETER_BOTH = 0x03
};

/**
 * @brief Parameter or Parameter Group metadata
 *
 * Allows to assign additional properties for form generation
 */
class Metadata
{
  public:
    /**
     * @brief Metadata constructor
     *
     * @param label form label
     * @param description form description, optional
     */
    Metadata(const char *label, const char *description = NULL)
    {
        this->_label = label;
        this->_description = description;
    }

    /**
     * @brief Parameter label
     *
     * @return label
     */
    const char *label()
    {
        return this->_label;
    }

    /**
     * @brief Parameter description
     *
     * @return description | NULL
     */
    const char *description()
    {
        return this->_description;
    }

  private:
    const char *_label;
    const char *_description;
};

/**
 * @brief Definition of configuration parameter interface
 */
class ConfigParameterInterface
{
  public:
    /**
     * @brief
     *
     * @param json
     */
    virtual void fromJson(JsonObject *json) = 0;

    /**
     * @brief
     *
     * @param json
     */
    virtual void toJson(JsonObject *json) = 0;

    /**
     * @brief
     *
     * @param json
     */
    virtual void toJsonSchema(JsonObject *json) = 0;

    /**
     * @brief Get the Mode object
     *
     * @return ParameterMode
     */
    virtual ParameterMode getMode() = 0;
};

/**
 * @brief Config Parameter
 */
template <typename T>
class ConfigParameter : public ConfigParameterInterface
{
  public:
    /**
     * @brief ConfigParameter constructor
     *
     * @param name parameter name
     * @param ptr parameter value
     * @param metadata additional parameter configuration, optional
     * @param mode parameter mode, optional
     * @param cb callback, optional
     */
    ConfigParameter(const char *name, T *ptr, Metadata *metadata = NULL, ParameterMode mode = PARAMETER_BOTH, std::function<void(String)> cb = NULL)
    {
        this->name = name;
        this->ptr = ptr;
        this->metadata = metadata;
        this->cb = cb;
        this->mode = mode;
    }

    /**
     * @brief Get parameter mode
     *
     * @return parameter mode
     */
    ParameterMode getMode()
    {
        return this->mode;
    }

    /**
     * @brief Parse configuration parameter value from JSON object
     *
     * @param json JSON object
     */
    void fromJson(JsonObject *json)
    {
        if (json->containsKey(name) && json->getMember(name).is<T>()) {
        {
            this->update(json->getMember(name).as<T>());
        }
    }

    /**
     * @brief Fill JSON object with parameter value
     *
     * @param json pointer to JSON object
     */
    void toJson(JsonObject *json)
    {
        // json->set(name, *ptr);

        // if (cb)
        // {
        //     cb(name);
        // }
        json->getOrAddMember(name).set((const char*) ptr);
    }

    /**
     * @brief Fill JSON object with parameter configuration schema
     *
     * @param json pointer to JSON object
     */
    void toJsonSchema(JsonObject *json)
    {
        // json->set("name", name);
        json->getOrAddMember(name).set((const char*) name);
        json.getOrAddMember(type).set(GetTypeName<T>());
        // json->set("type", GetTypeName<T>());
        if (metadata != NULL)
        {
            // json->set("label", metadata->label());
            json->getOrAddMember(label).set((const char*) this->metadata->label());

            if (metadata->description() != NULL)
            {
                // json->set("description", metadata->description());
                json->getOrAddMember(description).set((const char*) this->metadata->description());

            }
        }
    }

  private:
    String name;
    T *ptr;
    Metadata *metadata;
    std::function<void(String)> cb;
    ParameterMode mode;
};

/**
 * @brief ConfigParameter of String type
 */
class ConfigStringParameter : public ConfigParameterInterface
{
  public:
    /**
     * @brief Constructor of ConfigStringParameter
     *
     * @param name parameter name
     * @param ptr parameter value
     * @param length parameter string buffer value
     * @param metadata optional parameter metadata
     * @param mode parameter mode
     */
    ConfigStringParameter(const char *name, char *ptr, size_t length, Metadata *metadata = NULL, ParameterMode mode = PARAMETER_BOTH)
    {
        this->name = name;
        this->ptr = ptr;
        this->length = length;
        this->metadata = metadata;
        this->mode = mode;
    }

    /**
     * @brief Get the Mode object
     *
     * @return ParameterMode
     */
    ParameterMode getMode()
    {
        return this->mode;
    }

    /**
     * @brief Get parameter value from JSON object
     *
     * @param json pointer to JSON object
     */
    void fromJson(JsonObject *json)
    {
        // if (json->containsKey(name) && json->is<char *>(name))
        if (json->containsKey(name) && json->getMember(name).is<T>()) 
        {
            // const char *value = json->get<const char *>(name);
            this->update(json->getMember(name).as<T>());

            memset(ptr, '\0', length);
            strncpy(ptr, const_cast<char *>(value), length - 1);
        }
    }

    /**
     * @brief Fill JSON object with parameter value
     *
     * @param json pointer to JSON object
     */
    void toJson(JsonObject *json)
    {
        // json->set(name, ptr);
        json.getOrAddMember(name).set((const char*) ptr);
    }

    /**
     * @brief Fill JSON object with parameter schema definition
     *
     * @param json
     */
    void toJsonSchema(JsonObject *json)
    {
        // json->set("name", name);
        json.getOrAddMember(name).set((const char*) name);
        // json->set("type", "string");
        json.getOrAddMember(type).set((const char*) "string");

        if (metadata != NULL)
        {
            // json->set("label", metadata->label());
            json.getOrAddMember(label).set((const char*) metadata->label());

            if (metadata->description() != NULL)
            {
                // json->set("description", metadata->description());
                json.getOrAddMember(description).set((const char*)description())
            }
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
 * @brief Group of parameters
 */
class ConfigParameterGroup
{
  public:
    /**
     * @brief Construct a new Config Parameter Group object
     *
     * @param name parameter name
     * @param metadata parameter metadata, optional
     */
    ConfigParameterGroup(const char *name, Metadata *metadata = NULL)
    {
        this->name = name;
        this->metadata = metadata;
    }

    /**
     * @brief Add parameter to the group
     *
     * @tparam T parameter type
     * @param name parameter name
     * @param variable parameter value
     * @param metadata parameter metadata
     * @param mode parameter mode
     * @return ConfigParameterGroup&
     */
    template <typename T>
    ConfigParameterGroup &addParameter(const char *name, T *variable, Metadata *metadata = NULL, ParameterMode mode = PARAMETER_BOTH)
    {
        parameters.push_back(new ConfigParameter<T>(name, variable, metadata, mode));

        return *this;
    }

    /**
     * @brief Add string parameter to the group
     *
     * @param name parameter name
     * @param variable parameter value
     * @param size parameter buffer size
     * @param metadata parameter metadata
     * @param mode parameter mode
     * @return ConfigParameterGroup&
     */
    ConfigParameterGroup &addParameter(const char *name, char *variable, size_t size, Metadata *metadata = NULL, ParameterMode mode = PARAMETER_BOTH)
    {
        parameters.push_back(new ConfigStringParameter(name, variable, size, metadata, mode));

        return *this;
    }

    /**
     * @brief Fill JSON object with values of this group
     *
     * @param json pointer to JSON object
     */
    void toJson(JsonObject *json);

    /**
     * @brief Fill JSON object with schema definition of the group
     *
     * @param json pointer to JSON object
     */
    void toJsonSchema(JsonObject *json);

    /**
     * @brief Get parameter values from JSON object
     *
     * @param json pointer to JSON object
     */
    void fromJson(JsonObject *json);

    /**
     * @brief Get the Name of the group
     *
     * @return const char*
     */
    const char *getName()
    {
        return name;
    }

  private:
    const char *name;
    Metadata *metadata;

    std::list<ConfigParameterInterface *> parameters;
};

/**
 * @brief Configuration Manager
 */
class ConfigManager
{
  public:
    /**
     * @brief Construct a new Config Manager object
     *
     */
    ConfigManager();

  public:
    /**
     * @brief Set Access Point name
     *
     * @param name
     */
    void setAPName(const char *name);

    /**
     * @brief Set Access Point password, empty by default
     *
     * Sets the password used for the access point. For WPA2-PSK network it should be at least 8 character long.
     * If not specified, the access point will be open for anybody to connect to.
     *
     * @param password
     */
    void setAPPassword(const char *password);

    /**
     * @brief Set filename of HTML configurator
     *
     * Sets the path in SPIFFS to the webpage to be served by the access point.
     *
     * @param filename
     */
    void setAPFilename(const char *filename);

    /**
     * @brief Set timeout
     *
     * Sets the access point timeout, in seconds (default 0, no timeout).
     *
     * Note: The timeout starts when the access point is started, but is evaluated in the loop function.
     *
     * @param timeout
     */
    void setAPTimeout(const int timeout);

    /**
     * @brief Set number of WiFi retries
     *
     * Sets the number of Wifi connection retries. Defaults to 20.
     *
     * @param retries
     */
    void setWifiConnectRetries(const int retries);

    /**
     * @brief Set number of WiFi connection retries
     *
     * Sets the interval (in milliseconds) between Wifi connection retries. Defaults to 500ms.
     *
     * @param interval
     */
    void setWifiConnectInterval(const int interval);

    /**
     * @brief
     *
     * Sets a callback allowing customized http endpoints to be set when the access point is setup.
     *
     * @param callback
     */
    void setAPCallback(std::function<void(WebServer *)> callback);

    /**
     * @brief
     *
     * Sets a callback allowing customized http endpoints to be set when the api is setup.
     *
     * @param callback
     */
    void setAPICallback(std::function<void(WebServer *)> callback);

    /**
     * @brief
     *
     * @param name
     * @param metadata
     * @return ConfigParameterGroup&
     *
     * @code
     *   configManager.addParameterGroup("app", new Metadata("Application", "Example of application properties"))
     * @endcode
     */
    ConfigParameterGroup &addParameterGroup(const char *name, Metadata *metadata = NULL)
    {
        ConfigParameterGroup *group = new ConfigParameterGroup(name, metadata);
        groups.push_back(group);

        return *group;
    }

    /**
     * @brief
     *
     * Starts the configuration manager. The config parameter will be saved into and retrieved from the EEPROM.
     *
     * @tparam T
     * @param config
     */
    template <typename T>
    void begin(T &config)
    {
        this->config = &config;
        this->configSize = sizeof(T);

        EEPROM.begin(CONFIG_OFFSET + this->configSize);

        setup();
    }

    /**
     * @brief Handle incoming API requests
     *
     * This function should be called from the main loop()
     */
    void loop();

    /**
     * @brief Save configuation values
     *
     * Saves the config passed to the begin function to the EEPROM.
     *
     */
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
    /**
     * @brief
     *
     */
    void handleGetRoot();

    /**
     * @brief
     */
    void handleReboot();

    /**
     * @brief
     *
     */
    void handleGetWifi();

    /**
     * @brief
     *
     */
    void handleGetWifiScan();

    /**
     * @brief
     *
     */
    void handlePostConnect();

    /**
     * @brief
     *
     */
    void handlePostDisconnect();

    /**
     * @brief
     *
     */
    void handleGetSettings();

    /**
     * @brief
     *
     */
    void handleGetSettingsSchema();

    /**
     * @brief
     *
     */
    void handlePostSettings();

    /**
     * @brief clean settings
     */
    void handleDeleteSettings();

    /**
     * @brief
     *
     */
    void handleAPPost();

    /**
     * @brief
     *
     */
    void handleRESTGet();

    /**
     * @brief
     *
     */
    void handleRESTPut();

    /**
     * @brief
     *
     */
    void handleNotFound();

  private:
    /**
     * @brief
     *
     * @param jsonString
     * @return JsonObject&
     */
    JsonObject decodeJson(String jsonString);

    /**
     * @brief
     *
     */
    void readConfig();

    /**
     * @brief
     *
     */
    void writeConfig();

    /**
     * @brief
     *
     * @return true
     * @return false
     */
    bool wifiConnected();

    /**
     * @brief
     *
     */
    void setup();

    /**
     * @brief Start Access Point mode
     *
     */
    void startAP();

    /**
     * @brief
     *
     */
    void startApi(const char* ssid);

    /**
     * @brief
     *
     */
    void startWebServer();

    /**
     * @brief
     *
     * @param str
     * @return boolean
     */
    boolean isIp(String str);

    /**
     * @brief
     *
     * @param ip
     * @return String
     */
    String toStringIP(IPAddress ip);
};

#endif /* __CONFIGMANAGER_H__ */
