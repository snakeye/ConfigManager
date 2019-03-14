# WiFi configuration manager

Wifi connection and configuration manager for ESP8266 and ESP32.

Based on [ConfigManager](https://github.com/snakeye/ConfigManager) library. The major difference
is that the full configuration is provided by the device and configuration form is built
dynamically by javascript application.

This library was made to ease the complication of configuring Wifi and other
settings on an ESP8266 or ESP32. It is roughly split into two parts, Wifi configuration
and REST variable configuration.

# Requires

* [ArduinoJson 5.x](https://github.com/bblanchon/ArduinoJson)

# Quick Start

## Installing

You can install through the Arduino Library Manager. The package name is
**ConfigManager**.

## Usage

Include the library in your sketch

```cpp
#include <ConfigManager.h>
```

Initialize a global instance of the library

```cpp
ConfigManager configManager;
```

Initialize settings object

```cpp
struct Config
{
    char name[20] = {0};
    bool enabled = false;
    int hour = 0;
} config;
```

In your setup function define required parameters and start the manager.

```cpp
configManager.setAPName("Config Demo");
configManager.setAPFilename("/index.html");

configManager.addParameterGroup("app", new Metadata("Application", "Example of application properties"))
    .addParameter("name", config.name, 20, new Metadata("Name"))
    .addParameter("enabled", &config.enabled, new Metadata("Enabled"))
    .addParameter("hour", &config.hour, new Metadata("Hour"));

configManager.begin(config);
```

In your loop function, run the manager loop

```cpp
configManager.loop();
```

Upload the ```index.html``` file found in the ```data``` directory into the SPIFFS.
Instructions on how to do this vary based on your IDE. Below are links instructions
on the most common IDEs:

### ESP8266

* [Arduino IDE](http://arduino-esp8266.readthedocs.io/en/latest/filesystem.html#uploading-files-to-file-system)

* [Platform IO](http://docs.platformio.org/en/stable/platforms/espressif.html#uploading-files-to-file-system-spiffs)

### ESP32

* [Arduino IDE](https://github.com/me-no-dev/arduino-esp32fs-plugin)

* [Platform IO](http://docs.platformio.org/en/stable/platforms/espressif32.html#uploading-files-to-file-system-spiffs)

# Documentation

Class methods and properties are described in the [usage](/docs/usage.md) document.

# API Endpoints

API endpoints are described in [API Blueprint](/docs/dist/api.apib) and [rendered HTML](/docs/dist/api.html) files.

# Frontend development

Install javascript packages running command

```bash
yarn
```

Build develpment (`build:dev`) or production (`build:live`) HTML file:

```bash
yarn build:live
```