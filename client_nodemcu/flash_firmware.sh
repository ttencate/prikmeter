#!/bin/bash

# Exit on error
set -e

cd "$(dirname "$0")"

firmware=$(ls nodemcu-firmware/bin/nodemcu_integer__*.bin | tail -n1)
echo "Flashing latest firmware build: $firmware"
./esptool/esptool.py --port /dev/ttyUSB0 write_flash -fm dio 0x00000 "$firmware"
