# Programming interface description

## Methods

### getMode
```
Mode getMode()
```
> Gets the current mode, **ap** or **api**.

### setAPName
```
void setAPName(const char *name)
```
> Sets the name used for the access point.

### setAPPassword
```
void setAPPassword(const char *password)
```
> Sets the password used for the access point. For WPA2-PSK network it should be at least 8 character long.
> If not specified, the access point will be open for anybody to connect to.

### setAPFilename
```
void setAPFilename(const char *filename)
```
> Sets the path in SPIFFS to the webpage to be served by the access point.

### setAPTimeout
```
void setAPTimeout(const int timeout)
```
> Sets the access point timeout, in seconds (default 0, no timeout).
>
> **Note:** *The timeout starts when the access point is started, but is evaluated in the loop function.*

### setAPCallback
```
void setAPCallback(std::function<void(WebServer*)> callback)
```
> Sets a callback allowing customized http endpoints to be set when the access point is setup.

### setAPICallback
```
void setAPICallback(std::function<void(WebServer*)> callback)
```
> Sets a callback allowing customized http endpoints to be set when the api is setup.

### setWifiConnectRetries
```
void setWifiConnectRetries(const int retries)
```
> Sets the number of Wifi connection retires. Defaults to 20.

### setWifiConnectInterval
```
void setWifiConnectInterval(const int interval)
```
> Sets the interval (in milliseconds) between Wifi connection retries. Defaults to 500ms.

### addParameter
```
template<typename T>
void addParameter(const char *name, T *variable)
```
or
```
template<typename T>
void addParameter(const char *name, T *variable, ParameterMode mode)
```
> Adds a parameter to the REST interface. The optional mode can be set to ```set```
> or ```get``` to make the parameter read or write only (defaults to ```both```).

### addParameter (string)
```
void addParameter(const char *name, char *variable, size_t size)
```
or
```
void addParameter(const char *name, char *variable, size_t size, ParameterMode mode)
```
> Adds a character array parameter to the REST interface.The optional mode can be set to ```set```
> or ```get``` to make the parameter read or write only (defaults to ```both```).

### begin
```
template<typename T>
void begin(T &config)
```
> Starts the configuration manager. The config parameter will be saved into
> and retrieved from the EEPROM.

### save
```
void save()
```
> Saves the config passed to the begin function to the EEPROM.

### loop
```
void loop()
```
> Handles any waiting REST requests.
