#include <Arduino.h>

#include <WiFiConfig.h>

struct Config
{
    char name[20] = {0};
    bool enabled = false;
    int hour = 0;
} config;

ConfigManager configManager;

void setup()
{
    Serial.begin(115200);

    configManager.setAPName("Config Demo");
    configManager.setAPFilename("/index.html");

    configManager.addParameterGroup("app", new Metadata("Application", "Example of application properties"))
        .addParameter("name", config.name, 20, new Metadata("Name"))
        .addParameter("enabled", &config.enabled, new Metadata("Enabled"))
        .addParameter("hour", &config.hour, new Metadata("Hour"));

    configManager.begin(config);
}

void loop()
{
    configManager.loop();

    delay(1000);
}
