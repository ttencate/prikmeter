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

You need to copy the file `config.example.h` to a filename of your choice, say
`config.home.h`, and edit it to fill in the required details (WiFi network name
and password, server hostname and certificate, and your account's
authentication token).

This file is parsed as C++ code, so escape your values accordingly if needed.

To get your server's certificate fingerprint, a command like this might be
useful:

    $ true | openssl s_client -connect upload.your-prikmeter-server.example.com:443 \
           | openssl x509 -noout -fingerprint

Building
--------

To tell the `Makefile` where your configuration file is, set the variable
`PRIKMETER_CONFIG` to contain the name of the file you just created, for
example:

    $ export PRIKMETER_CONFIG_H=config.home.h

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

For debugging and testing without an actual smart meter connected, the
following arguments can be added to the `make` command line:

* `READ_FROM_SERIAL=1` reads the telegram input from the serial port, instead
  of the P1 port.
* `PRINT_TELEGRAM=1` prints telegram bytes verbatim to the serial port (among
  the other debugging info).
* `DONT_SEND_TELEGRAM=1` skips uploading the telegram.
