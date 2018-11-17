# https://github.com/plerup/makeEspArduino
SKETCH := arduino_client.ino
CHIP := esp8266
BOARD := nodemcuv2
UPLOAD_SPEED := 115200
ESPTOOL_PY := ../esptool/esptool.py
BUILD_ROOT := ./build

ifeq ($(PRIKMETER_CONFIG_H),)
$(error You need to set PRIKMETER_CONFIG_H to the name of your configuration header. See README.md for details.)
endif
CPP_EXTRA += '-DPRIKMETER_CONFIG_INCLUDE="$(PRIKMETER_CONFIG_H)"'

BUILD_EXTRA_FLAGS := \
  $(if $(READ_FROM_SERIAL), -DREAD_FROM_SERIAL) \
  $(if $(PRINT_TELEGRAM), -DPRINT_TELEGRAM) \
  $(if $(DONT_SEND_TELEGRAM), -DDONT_SEND_TELEGRAM)

include makeEspArduino.mk

$(USER_OBJ): $(PRIKMETER_CONFIG_H)
