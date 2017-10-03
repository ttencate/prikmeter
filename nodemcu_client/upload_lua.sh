#!/bin/sh

# Exit on error
set -e

cd "$(dirname "$0")"

./nodemcu-uploader/nodemcu-uploader.py --port /dev/ttyUSB0 --baud 115200 upload *.lua
