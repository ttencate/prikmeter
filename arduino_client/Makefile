-include config.in

.PHONY: all
all: config.h root_cert.cpp

# TODO escape variables properly for use in C++ string literals
config.h: config.in root.cert
	echo '#define WIFI_SSID "$(WIFI_SSID)"' > $@
	echo '#define WIFI_PASSWORD "$(WIFI_PASSWORD)"' >> $@
	echo '#define SERVER_HOST "$(SERVER_HOST)"' >> $@

# The first sed command prints everything between begin and end of each cert.
# The second sed command operates on reversed output (tac) and prints
# everything up to the end (now beginning) of the last (now first) cert.
root.cert: config.in
	true | openssl s_client -showcerts -connect '$(SERVER_HOST):443' 2>/dev/null \
		| sed -n -e '/-BEGIN CERTIFICATE-/,/-END CERTIFICATE-/p' \
		| tac \
		| sed -n -e '1,/-BEGIN CERTIFICATE-/p' \
		| tac \
		| openssl x509 -outform der -out $@

root_cert.cpp: root.cert
	xxd -i $< > $@

.PHONY: clean_certs
clean_certs:
	rm -f config.h root.cert root_cert.cpp

# https://github.com/plerup/makeEspArduino
SKETCH := arduino_client.ino
CHIP := esp8266
BOARD := nodemcuv2
UPLOAD_SPEED := 115200
ESPTOOL_PY := ../esptool/esptool.py
BUILD_ROOT := ./build
include makeEspArduino.mk
