Client
======

Prerequisites
-------------

Install the Arduino core for ESP8266 by following [the
instructions](https://github.com/esp8266/Arduino), except you need at least
version `2.4.0-rc1`, so if `2.3.0` is still the latest stable release, use the
following Board Manager URL:

    https://github.com/esp8266/Arduino/releases/download/2.4.0-rc1/package_esp8266com_index.json

Configuration
-------------

You need to create a file named `config.in` which looks like this:

    WIFI_SSID = your_network_name
    WIFI_PASSWORD = your_wifi_password
    SERVER_HOST = upload.your-prikmeter-server.example.com
    SERVER_CERTIFICATE_FINGERPRINT = 9C:35:5A:83:7C:8A:5F:13:C8:D0:98:9B:D3:9E:84:27:1B:BD:FA:30
    AUTH_TOKEN = your_auth_token

This file is parsed as a `Makefile` snippet, so escape your values accordingly
if needed.

To get your server's certificate fingerprint, a command like this might be
useful:

    $ true | openssl s_client -connect upload.your-prikmeter-server.example.com:443 \
           | openssl x509 -noout -fingerprint

Building
--------

Then you can just use `make` to drive the build and upload process, using the
commands from [makeEspArduino](https://github.com/plerup/makeEspArduino); type
`make help` to get the full list. The commonly used ones:

    $ make          # build the code
    $ make flash    # upload the code
    $ make clean    # remove build outputs

Debugging
---------

After connecting the ESP8266 via USB to your computer, find out its device name
(Linux) like so:

    $ dmesg | tail
    ...
    [ 9087.382595] usb 1-5.1.3: ch341-uart converter now attached to ttyUSB0

The USB serial port is opened at 115200 baud, so you can connect to it like
this:

    $ minicom -b 115200 -D /dev/ttyUSB0

If this doesn't work, it might help to run `make flash` first. Presumably this
sets up the serial port in the right state.

For debugging and testing without an actual smart meter connected, it's useful
to add `READ_FROM_SERIAL=1` to the `make` command. That will let you send test
telegrams on the serial port.
