.PHONY: all staticlib dynamiclib

LIBVERSION=$(shell grep '<version>1' ../../../pom.xml | sed 's/    <version>//g' | sed 's/<\/version>//g')
LIBMAJOR=$(shell echo $(LIBVERSION) | sed 's/\([1-9]\).\([0-9]\).\([0-9]\)/\1/g')
LIBMINOR=$(shell echo $(LIBVERSION) | sed 's/\([1-9]\).\([0-9]\).\([0-9]\)/\2/g')
LIBMAINT=$(shell echo $(LIBVERSION) | sed 's/\([1-9]\).\([0-9]\).\([0-9]\)/\3/g')

TARGET=../../../target

LIB_DIR := $(TARGET)/lib
EXE_DIR := $(TARGET)/bin
BUILD_DIR := ./build
EPICS_INSTR_BUILD_DIR := $(BUILD_DIR)/epics

BUILD_TYPE = RelWithDebInfo

CMAKE_FLAGS = -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -DLIBVERSION=$(LIBVERSION) -DLIBSOVERSION=$(LIBMAJOR)
CMAKE_FLAGS += -DCMAKE_INSTALL_PREFIX=$(TARGET)

all: dynamiclib staticlib

dynamiclib:
	@mkdir -p $(BUILD_DIR)
	cmake3 -B$(BUILD_DIR) -DBUILD_SHARED_LIBS=ON $(CMAKE_FLAGS)
	cmake3 --build $(BUILD_DIR)
	make -C $(BUILD_DIR) install
	@mkdir -p $(LIB_DIR)
	cp -P $(EPICS_INSTR_BUILD_DIR)/*.so* $(LIB_DIR)/

staticlib: dynamiclib
	@mkdir -p $(BUILD_DIR)
	cmake3 -B$(BUILD_DIR) -DBUILD_SHARED_LIBS=OFF $(CMAKE_FLAGS)
	cmake3 --build $(BUILD_DIR)
	make -C $(BUILD_DIR) install
	@mkdir -p $(LIB_DIR)
	cp $(EPICS_INSTR_BUILD_DIR)/*.a $(LIB_DIR)/
