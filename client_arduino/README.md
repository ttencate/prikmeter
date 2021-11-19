Client
======

Prerequisites
-------------

Install PlatformIO Core by following [the
instructions](https://docs.platformio.org/en/latest//core/installation.html).
This will take care of downloading everything else.

Configuration
-------------

You need to create a directory `data/` and copy the file
`config/prikmeter.example.json` to `data/prikmeter.json`, then edit it
according to your settings (WiFi network name and password, server hostname and
certificate, and your account's authentication token). Remove the comments.

To get your server's certificate fingerprint, a command like this might be
useful:

    $ true | openssl s_client -connect upload.your-prikmeter-server.example.com:443 \
           | openssl x509 -noout -fingerprint

Upload the configuration to the device:

    $ pio run -t uploadfs

Building and running
--------------------

Use `pio` to drive the build and upload process, using the commands from
[PlatformIO](https://docs.platformio.org/en/latest//core/userguide/index.html);
type `pio -h` to get the full list. The commonly used ones:

    $ pio check           # run cppcheck static analysis
    $ pio run             # build
    $ pio run -t upload   # build and upload
    $ pio device monitor  # monitor serial output
    $ pio run -t clean    # remove build outputs

Unit testing
------------

To run unit tests on the local machine (needs GCC):

    $ pio test -t native

Debugging
---------

For debugging and testing without an actual smart meter connected, the
following arguments can be added to the `pio` command line:

* `READ_FROM_SERIAL=1` reads the telegram input from the serial port, instead
  of the P1 port.
* `PRINT_TELEGRAM=1` prints telegram bytes verbatim to the serial port (among
  the other debugging info).
* `DONT_SEND_TELEGRAM=1` skips uploading the telegram.
