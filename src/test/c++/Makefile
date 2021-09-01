.PHONY: all

PROGNAME=unit-tests

TARGET=../../../target

BIN_DIR := $(TARGET)/bin
BUILD_DIR := ./build

UNIT_TEST := $(BUILD_DIR)/unit/$(PROGNAME)

BUILD_TYPE = RelWithDebInfo

CMAKE_FLAGS := -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)

ifeq ($(COVERAGE),true)
CMAKE_FLAGS += -DCOVERAGE=true
else
CMAKE_FLAGS += -DCOVERAGE=false
endif

all: unit-tests

unit-tests: $(UNIT_TEST)
	@mkdir -p $(BIN_DIR)
	cp $(UNIT_TEST) $(BIN_DIR)/

$(UNIT_TEST):
	@mkdir -p $(BUILD_DIR)
	cmake -B$(BUILD_DIR) $(CMAKE_FLAGS)
	$(MAKE) -C $(BUILD_DIR)
