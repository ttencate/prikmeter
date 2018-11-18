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

# TESTING

TEST_ROOT := test
TEST_BUILD_ROOT := $(BUILD_ROOT)/test
TEST_EXECUTABLE := $(TEST_BUILD_ROOT)/main
IMPL_CPPS := \
  telegram_reader.cpp
TEST_CPPS := \
  test/main.cpp \
  test/telegram_reader_test.cpp

.PHONY: test
test: $(TEST_EXECUTABLE)
	"$(TEST_EXECUTABLE)"

$(TEST_BUILD_ROOT)/%.o: %.cpp $(TEST_ROOT)/catch2/catch.hpp
	mkdir -p $(dir $@)
	g++ -c -I. -o$@ $<

$(TEST_EXECUTABLE): $(patsubst %.cpp, $(TEST_BUILD_ROOT)/%.o, $(IMPL_CPPS) $(TEST_CPPS))
	mkdir -p $(dir $@)
	g++ -I. -o$@ $^
