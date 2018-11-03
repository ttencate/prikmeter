-include config.in

ifeq ($(READ_FROM_SERIAL), 1)
  BUILD_EXTRA_FLAGS := -DREAD_FROM_SERIAL
endif

# TODO escape variables properly for use in C++ string literals
config.h: config.in
	echo '#define WIFI_SSID "$(WIFI_SSID)"' > $@
	echo '#define WIFI_PASSWORD "$(WIFI_PASSWORD)"' >> $@
	echo '#define SERVER_HOST "$(SERVER_HOST)"' >> $@
	echo '#define SERVER_CERTIFICATE_FINGERPRINT "$(SERVER_CERTIFICATE_FINGERPRINT)"' >> $@
	echo '#define AUTH_TOKEN "$(AUTH_TOKEN)"' >> $@

# https://github.com/plerup/makeEspArduino
SKETCH := arduino_client.ino
CHIP := esp8266
BOARD := nodemcuv2
UPLOAD_SPEED := 115200
ESPTOOL_PY := ../esptool/esptool.py
BUILD_ROOT := ./build
include makeEspArduino.mk
$(USER_OBJ): config.h
