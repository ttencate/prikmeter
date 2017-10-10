Install the Arduino core for ESP8266 by following [the
instructions](https://github.com/esp8266/Arduino), except you need at least
version `2.4.0-rc1`, so if `2.3.0` is still the latest stable release, use the
following Board Manager URL:

    https://github.com/esp8266/Arduino/releases/download/2.4.0-rc1/package_esp8266com_index.json

You also need to create a file named `config.in` which looks like this:

    WIFI_SSID = your_network_name
    WIFI_PASSWORD = your_wifi_password
    SERVER_HOST = prikmeter.your-server.example.com
    AUTH_TOKEN = your_auth_token

Then you can just use the commands from
[makeEspArduino](https://github.com/plerup/makeEspArduino); type `make help` to
get the full list. Some of the more useful ones:

    make build    # build the code
    make flash    # upload the code
