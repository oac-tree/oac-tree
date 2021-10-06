.PHONY: all

LIBVERSION=$(shell cat ../VERSION)
LIBMAJOR=$(shell cat ../VERSION | cut -d'.' -f1)
LIBMINOR=$(shell cat ../VERSION | cut -d'.' -f2)
LIBMAINT=$(shell cat ../VERSION | cut -d'.' -f3)

TARGET=../../../target

BUILD_DIR := ./build

BUILD_TYPE = RelWithDebInfo

CMAKE_FLAGS := -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -DLIBVERSION=$(LIBVERSION) -DLIBSOVERSION=$(LIBMAJOR)
CMAKE_FLAGS += -DCMAKE_INSTALL_PREFIX=$(TARGET)

ifeq ($(COVERAGE),true)
CMAKE_FLAGS += -DCOVERAGE=true
else
CMAKE_FLAGS += -DCOVERAGE=false
endif

all: cmake_targets

cmake_targets:
	@mkdir -p $(BUILD_DIR)
	cmake -B$(BUILD_DIR) $(CMAKE_FLAGS)
	$(MAKE) -C $(BUILD_DIR) install
