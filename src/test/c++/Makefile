.PHONY: all

PROGNAME=unit-tests

TARGET=../../../target

BIN_DIR := $(TARGET)/bin
BUILD_DIR := ./build

EXECUTABLE := $(BUILD_DIR)/$(PROGNAME)

BUILD_TYPE = Release
CMAKE_FLAGS = -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)

all: executable

executable: $(EXECUTABLE)
	@mkdir -p $(BIN_DIR)
	cp $(EXECUTABLE) $(BIN_DIR)/

$(EXECUTABLE):
	@mkdir -p $(BUILD_DIR)
	cmake3 -B$(BUILD_DIR) $(CMAKE_FLAGS)
	$(MAKE) -C $(BUILD_DIR)
