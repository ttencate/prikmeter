Client
======

Prerequisites
-------------

Install PlatformIO Core by following [the
instructions](https://docs.platformio.org/en/latest//core/installation.html).
This will take care of downloading everything else.

Configuration
-------------

You need to copy the file `config/config.example.h` to a filename of your
choice, say `config/config.home.h`, and edit it to fill in the required details
(WiFi network name and password, server hostname and certificate, and your
account's authentication token).

This file is parsed as C++ code, so escape your values accordingly if needed.

To get your server's certificate fingerprint, a command like this might be
useful:

    $ true | openssl s_client -connect upload.your-prikmeter-server.example.com:443 \
           | openssl x509 -noout -fingerprint

Building
--------

To tell the `platformio.ini` where your configuration file is, set the variable
`PRIKMETER_CONFIG_H` to contain the name of the file you just created, for
example:

    $ export PRIKMETER_CONFIG_H=config.home.h

Note that this should be relative to the `config/` directory.

Then you can just use `pio` to drive the build and upload process, using the
commands from
[PlatformIO](https://docs.platformio.org/en/latest//core/userguide/index.html);
type `pio -h` to get the full list. The commonly used ones:

    $ pio check           # run cppcheck static analysis
    $ pio run             # build
    $ pio run -t upload   # build and upload
    $ pio device monitor  # monitor serial output
    $ pio run -t clean    # remove build outputs

Testing
-------

// TODO figure out how to make this work with `pio test`

Debugging
---------

// TODO update this to use `pio device monitor`

After connecting the ESP8266 via USB to your computer, find out its device name
(Linux) like so:

    $ dmesg | tail
    ...
    [ 9087.382595] usb 1-5.1.3: ch341-uart converter now attached to ttyUSB0

The USB serial port is opened at 115200 baud, so you can connect to it like
this:

    $ minicom -b 115200 -D /dev/ttyUSB0

If this doesn't work, it might help to run `pio run -t upload` first.
Presumably this sets up the serial port in the right state.

For debugging and testing without an actual smart meter connected, the
following arguments can be added to the `pio` command line:

* `READ_FROM_SERIAL=1` reads the telegram input from the serial port, instead
  of the P1 port.
* `PRINT_TELEGRAM=1` prints telegram bytes verbatim to the serial port (among
  the other debugging info).
* `DONT_SEND_TELEGRAM=1` skips uploading the telegram.
