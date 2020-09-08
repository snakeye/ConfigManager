#include "WiFiConfig.h"

const byte DNS_PORT = 53;
const char magicBytes[2] = {'C', 'M'};

const char mimeHTML[] PROGMEM = "text/html";
const char mimeJSON[] PROGMEM = "application/json";
const char mimePlain[] PROGMEM = "text/plain";

ConfigManager::ConfigManager()
{
    this->setAPFilename("/index.html");
}

void ConfigManager::setAPName(const char *name)
{
    this->apName = (char *)name;
}

void ConfigManager::setAPPassword(const char *password)
{
    this->apPassword = (char *)password;
}

void ConfigManager::setAPFilename(const char *filename)
{
    this->apFilename = (char *)filename;
}

void ConfigManager::setAPTimeout(const int timeout)
{
    this->apTimeout = timeout;
}

void ConfigManager::setWifiConnectRetries(const int retries)
{
    this->wifiConnectRetries = retries;
}

void ConfigManager::setWifiConnectInterval(const int interval)
{
    this->wifiConnectInterval = interval;
}

void ConfigManager::setAPCallback(std::function<void(WebServer *)> callback)
{
    this->apCallback = callback;
}

void ConfigManager::setAPICallback(std::function<void(WebServer *)> callback)
{
    this->apiCallback = callback;
}

/**
 * @brief
 *
 */
void ConfigManager::loop()
{
    if (mode == MODE_AP && apTimeout > 0 && ((millis() - apStart) / 1000) > apTimeout)
    {
        // ESP.restart();
    }

    if (dnsServer)
    {
        dnsServer->processNextRequest();
    }

    if (server)
    {
        server->handleClient();
    }
}

/**
 * @brief
 *
 */
void ConfigManager::save()
{
    this->writeConfig();
}

/**
 * @brief
 *
 * @param jsonString
 * @return JsonObject&
 */
JsonObject ConfigManager::decodeJson(String jsonString)
{
    DynamicJsonDocument jsonBuffer(1024);

    if (jsonString.length() == 0)
    {
        return jsonBuffer.createObject();
    }

    JsonObject obj = jsonBuffer.parseObject(jsonString);

    if (!obj.success())
    {
        return jsonBuffer.createObject();
    }

    return obj;
}

/**
 * Respond with content of index.html
 */
void ConfigManager::handleGetRoot()
{
    SPIFFS.begin();

    File f = SPIFFS.open(apFilename, "r");
    if (!f)
    {
        Serial.print(F("file "));
        Serial.print(apFilename);
        Serial.println(F(" open failed"));
        server->send(404, FPSTR(mimeHTML), F("File not found"));
        return;
    }

    server->streamFile(f, FPSTR(mimeHTML));

    f.close();
}

/**
 *
 */
void ConfigManager::handleReboot()
{
    server->send(204, FPSTR(mimePlain), F("Saved. Will attempt to reboot."));

    // Restart server
    ESP.restart();
}

/**
 * Return current WiFi mode
 */
void ConfigManager::handleGetWifi()
{
    DynamicJsonBuffer jsonBuffer;
    JsonObject res = jsonBuffer.createObject();

    // WiFi mode
    switch (WiFi.getMode())
    {
    case WIFI_OFF:
        res.set("mode", "off");
        break;
    case WIFI_STA:
        res.set("mode", "sta");
        break;
    case WIFI_AP:
        res.set("mode", "ap");
        break;
    case WIFI_AP_STA:
        res.set("mode", "ap_sta");
        break;
    default:
        res.set("mode", "");
        break;
    }

    // connection status
    res.set("connected", WiFi.isConnected());

    String body;
    res.printTo(body);

    server->send(200, FPSTR(mimeJSON), body);
}

/**
 * @brief Scan WiFi networks
 *
 */
void ConfigManager::handleGetWifiScan()
{
    DynamicJsonBuffer jsonBuffer;
    JsonArray &res = jsonBuffer.createArray();

    static long lastScan = 0;
    const long scanPeriod = 5000;
    long now = millis();

    if (now > lastScan + scanPeriod)
    {
        WiFi.scanDelete();
        WiFi.scanNetworks(true);
        lastScan = now;
    }

    int n = WiFi.scanComplete();
    if (n >= 0)
    {
        for (int i = 0; i < n; i++)
        {
            JsonObject obj = res.createNestedObject();

            obj.set("ssid", WiFi.SSID(i));
            obj.set("channel", WiFi.channel(i));
            obj.set("rssi", WiFi.RSSI(i));
            obj.set("open", WiFi.encryptionType(i) == ENC_TYPE_NONE);
        }
    }

    String body;
    res.printTo(body);

    server->send(200, FPSTR(mimeJSON), body);
}

/**
 * @brief Connect to the Access Point
 *
 */
void ConfigManager::handlePostConnect()
{
    bool isJson = server->header("Content-Type") == FPSTR(mimeJSON);
    String ssid;
    String password;
    char ssidChar[32];
    char passwordChar[64];

    if (isJson)
    {
        JsonObject obj = decodeJson(server->arg("plain"));

        ssid = obj.get<String>("ssid");
        password = obj.get<String>("password");
    }
    else
    {
        ssid = server->arg("ssid");
        password = server->arg("password");
    }

    if (ssid.length() == 0)
    {
        server->send(400, FPSTR(mimePlain), F("Invalid ssid or password."));
        return;
    }

    strncpy(ssidChar, ssid.c_str(), sizeof(ssidChar));
    strncpy(passwordChar, password.c_str(), sizeof(passwordChar));

    EEPROM.put(0, magicBytes);
    EEPROM.put(WIFI_OFFSET, ssidChar);
    EEPROM.put(WIFI_OFFSET + 32, passwordChar);
    EEPROM.commit();

    server->send(204, FPSTR(mimePlain), F("Saved. Will attempt to reboot."));

    // Restart server
    ESP.restart();
}

/**
 * @brief Disconnect from the access point
 *
 */
void ConfigManager::handlePostDisconnect()
{
    char ssidChar[32] = {0};
    char passwordChar[64] = {0};

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    // clear saved connection parameters
    EEPROM.put(0, magicBytes);
    EEPROM.put(WIFI_OFFSET, ssidChar);
    EEPROM.put(WIFI_OFFSET + 32, passwordChar);
    EEPROM.commit();

    server->send(204, FPSTR(mimePlain), F("Saved. Will attempt to reboot."));

    // Restart server
    ESP.restart();
}

/**
 * Return JSON with data schema
 */
void ConfigManager::handleGetSettingsSchema()
{
    DynamicJsonDocument jsonBuffer(1024);
    JsonArray res = jsonBuffer.createArray();

    std::list<ConfigParameterGroup *>::iterator it;
    for (it = groups.begin(); it != groups.end(); ++it)
    {
        JsonObject obj = res.createNestedObject();
        (*it)->toJsonSchema(&obj);
    }

    String body;
    res.printTo(body);

    server->send(200, FPSTR(mimeJSON), body);
}

/**
 * Return JSON with settings values
 */
void ConfigManager::handleGetSettings()
{
    DynamicJsonDocument jsonBuffer(1024);
    JsonObject res = jsonBuffer.createObject();

    std::list<ConfigParameterGroup *>::iterator it;
    for (it = groups.begin(); it != groups.end(); ++it)
    {
        (*it)->toJson(&res);
    }

    String body;
    res.printTo(body);

    server->send(200, FPSTR(mimeJSON), body);
}

/**
 * @brief Update settings
 *
 */
void ConfigManager::handlePostSettings()
{
    JsonObject obj = decodeJson(server->arg("plain"));
    if (!obj.success())
    {
        server->send(400, FPSTR(mimeJSON), "");
        return;
    }

    std::list<ConfigParameterGroup *>::iterator it;
    for (it = groups.begin(); it != groups.end(); ++it)
    {
        (*it)->fromJson(&obj);
    }

    writeConfig();

    server->send(204, FPSTR(mimeJSON), "");
}

/**
 * Clear settings
 */
void ConfigManager::handleDeleteSettings()
{
    EEPROM.put(0, magicBytes);
    EEPROM.commit();

    server->send(204, FPSTR(mimePlain), F("Saved. Will attempt to reboot."));

    // Restart server
    ESP.restart();
}

/**
 * @brief Handle 404 Not Found error
 *
 */
void ConfigManager::handleNotFound()
{
    if (!isIp(server->hostHeader()))
    {
        server->sendHeader("Location", String("http://") + toStringIP(server->client().localIP()), true);
        server->send(302, FPSTR(mimePlain), ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.
        server->client().stop();
        return;
    }

    server->send(404, FPSTR(mimePlain), "");
    server->client().stop();
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool ConfigManager::wifiConnected()
{
    Serial.print(F("Waiting for WiFi to connect"));

    int i = 0;
    while (i < wifiConnectRetries)
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            Serial.println("");
            return true;
        }

        Serial.print(".");

        delay(wifiConnectInterval);
        i++;
    }

    Serial.println("");
    Serial.println(F("Connection timed out"));

    return false;
}

/**
 * @brief
 *
 */
void ConfigManager::setup()
{
    char magic[2] = {0};
    char ssid[32] = {0};
    char password[64] = {0};

    //
    Serial.println(F("Reading saved configuration"));

    EEPROM.get(0, magic);

    if (memcmp(magic, magicBytes, 2) == 0)
    {
        // config read sucessfully
        EEPROM.get(WIFI_OFFSET, ssid);
        EEPROM.get(WIFI_OFFSET + 32, password);

        readConfig();

        Serial.println(F("Config read successfully"));
    }
    else
    {
        // config is incorrect, save default one
        EEPROM.put(0, magicBytes);
        EEPROM.put(WIFI_OFFSET, ssid);
        EEPROM.put(WIFI_OFFSET + 32, password);

        writeConfig();

        Serial.println(F("Config incorrect, overwriting"));
    }

    // try to connect to access point
    if (ssid[0] != '\0')
    {
        WiFi.begin(ssid, password[0] == '\0' ? NULL : password);

        delay(1000);

        if (wifiConnected())
        {
            startApi(ssid);
        }
    }

    // could not connect to AP or there is no configuration for it
    if (WiFi.status() != WL_CONNECTED)
    {
        apTimeout = 0;

        startAP();
    }

    //
    startWebServer();
}

/**
 * @brief
 *
 */
void ConfigManager::startWebServer()
{
    const char *headerKeys[] = {"Content-Type"};
    size_t headerKeysSize = sizeof(headerKeys) / sizeof(char *);

    server.reset(new WebServer(80));

    server->collectHeaders(headerKeys, headerKeysSize);

    server->on("/", HTTPMethod::HTTP_GET, std::bind(&ConfigManager::handleGetRoot, this));

    server->on("/reboot", HTTPMethod::HTTP_POST, std::bind(&ConfigManager::handleReboot, this));

    // wifi parameters
    server->on("/wifi", HTTPMethod::HTTP_GET, std::bind(&ConfigManager::handleGetWifi, this));
    server->on("/wifi/scan", HTTPMethod::HTTP_GET, std::bind(&ConfigManager::handleGetWifiScan, this));
    server->on("/wifi/connect", HTTPMethod::HTTP_POST, std::bind(&ConfigManager::handlePostConnect, this));
    server->on("/wifi/disconnect", HTTPMethod::HTTP_POST, std::bind(&ConfigManager::handlePostDisconnect, this));

    // configuration settings
    server->on("/settings", HTTPMethod::HTTP_OPTIONS, std::bind(&ConfigManager::handleGetSettingsSchema, this));
    server->on("/settings", HTTPMethod::HTTP_GET, std::bind(&ConfigManager::handleGetSettings, this));
    server->on("/settings", HTTPMethod::HTTP_POST, std::bind(&ConfigManager::handlePostSettings, this));
    server->on("/settings", HTTPMethod::HTTP_DELETE, std::bind(&ConfigManager::handleDeleteSettings, this));

    // not found handling
    server->onNotFound(std::bind(&ConfigManager::handleNotFound, this));

    if (apCallback)
    {
        apCallback(server.get());
    }

    server->begin();
}

/**
 * @brief
 *
 */
void ConfigManager::startAP()
{
    mode = MODE_AP;

    Serial.println(F("Starting Access Point"));

    WiFi.disconnect();
    WiFi.softAPdisconnect();
    WiFi.mode(WIFI_OFF);

    delay(500);

    WiFi.mode(WIFI_AP);

    WiFi.softAPConfig(IPAddress(192, 168, 1, 1), IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0));

    WiFi.softAP(apName, apPassword);

    delay(1000); // Need to wait to get IP

    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);

    dnsServer.reset(new DNSServer);
    dnsServer->setErrorReplyCode(DNSReplyCode::NoError);
    dnsServer->start(DNS_PORT, "*", IPAddress(192, 168, 1, 1));

    apStart = millis();
}

/**
 * @brief
 *
 * @param ssid
 */
void ConfigManager::startApi(const char *ssid)
{
    mode = MODE_API;

    Serial.print(F("Connected to "));
    Serial.print(ssid);
    Serial.print(F(" with IP "));
    Serial.println(WiFi.localIP());

    WiFi.mode(WIFI_STA);

    wifi_station_dhcpc_start();
}

/**
 * @brief
 *
 */
void ConfigManager::readConfig()
{
    byte *ptr = (byte *)config;

    for (unsigned int i = 0; i < configSize; i++)
    {
        *(ptr++) = EEPROM.read(CONFIG_OFFSET + i);
    }
}

/**
 * @brief
 *
 */
void ConfigManager::writeConfig()
{
    byte *ptr = (byte *)config;

    for (unsigned int i = 0; i < configSize; i++)
    {
        EEPROM.write(CONFIG_OFFSET + i, *(ptr++));
    }
    EEPROM.commit();
}

/**
 * @brief
 *
 * @param str
 * @return boolean
 */
boolean ConfigManager::isIp(String str)
{
    for (unsigned int i = 0; i < str.length(); i++)
    {
        int c = str.charAt(i);
        if (c != '.' && (c < '0' || c > '9'))
        {
            return false;
        }
    }
    return true;
}

/**
 * @brief
 *
 * @param ip
 * @return String
 */
String ConfigManager::toStringIP(IPAddress ip)
{
    String res = "";
    for (unsigned int i = 0; i < 3; i++)
    {
        res += String((ip >> (8 * i)) & 0xFF) + ".";
    }
    res += String((ip >> (8 * 3)) & 0xFF);
    return res;
}
