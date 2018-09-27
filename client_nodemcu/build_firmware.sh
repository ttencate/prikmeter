#!/bin/bash

# Exit on error
set -e

cd "$(dirname "$0")"

cp -v firmware_config/* nodemcu-firmware/app/include/

docker pull marcelstoer/nodemcu-build
docker run --rm -ti -e INTEGER_ONLY=1 -v $(pwd)/nodemcu-firmware:/opt/nodemcu-firmware marcelstoer/nodemcu-build
