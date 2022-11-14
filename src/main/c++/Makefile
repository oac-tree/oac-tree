.PHONY: all

TARGET=../../../target

BUILD_DIR := $(TARGET)/build

CMAKE_FLAGS += -DCMAKE_INSTALL_PREFIX=$(TARGET)

ifeq ($(COVERAGE),true)
CMAKE_FLAGS += -DCOVERAGE=true
else
CMAKE_FLAGS += -DCOVERAGE=false
endif

all: cmake_targets

cmake_targets:
	cp $(TARGET)/../src/CMakeLists.txt $(TARGET)/.
	@mkdir -p $(BUILD_DIR)
	cmake -B$(BUILD_DIR) $(CMAKE_FLAGS) $(TARGET)
	$(MAKE) -C $(BUILD_DIR) install
