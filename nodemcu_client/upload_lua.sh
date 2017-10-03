#!/bin/sh

# Exit on error
set -e

cd "$(dirname "$0")"

./nodemcu-uploader/nodemcu-uploader.py upload *.lua
./nodemcu-uploader/nodemcu-uploader.py node restart
