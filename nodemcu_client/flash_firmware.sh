#!/bin/bash

# Exit on error
set -e

cd "$(dirname "$0")"

./esptool/esptool.py --port /dev/ttyUSB0 write_flash -fm dio 0x00000 nodemcu-firmware/bin/nodemcu_integer__20171003-1254.bin
