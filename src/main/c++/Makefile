.PHONY: all libs staticlib dynamiclib

LIBNAME=sequencer
CLI_NAME=sequencer-cli

LIBVERSION=$(shell grep '<version>1' ../../../pom.xml | sed 's/    <version>//g' | sed 's/<\/version>//g')
LIBMAJOR=$(shell echo $(LIBVERSION) | sed 's/\([1-9]\).\([0-9]\).\([0-9]\)/\1/g')
LIBMINOR=$(shell echo $(LIBVERSION) | sed 's/\([1-9]\).\([0-9]\).\([0-9]\)/\2/g')
LIBMAINT=$(shell echo $(LIBVERSION) | sed 's/\([1-9]\).\([0-9]\).\([0-9]\)/\3/g')

SONAME=lib$(LIBNAME).so.$(LIBMAJOR)
REALNAME=lib$(LIBNAME).so.$(LIBVERSION)

TARGET=../../../target

LIB_DIR := $(TARGET)/lib
EXE_DIR := $(TARGET)/bin
BUILD_DIR := ./build
THIRD_PARTY_BUILD_DIR := $(BUILD_DIR)/instructions/ThirdParty

BUILD_TYPE = RelWithDebInfo

CMAKE_FLAGS = -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -DLIBVERSION=$(LIBVERSION) -DLIBSOVERSION=$(LIBMAJOR)

all: libs executable

libs: dynamiclib staticlib

dynamiclib: $(BUILD_DIR)/$(REALNAME)
	@mkdir -p $(LIB_DIR)
	cp -P $(BUILD_DIR)/lib$(LIBNAME).so* $(LIB_DIR)/
	cp -P $(THIRD_PARTY_BUILD_DIR)/*.so* $(LIB_DIR)/

staticlib: $(BUILD_DIR)/lib$(LIBNAME).a
	@mkdir -p $(LIB_DIR)
	cp $(BUILD_DIR)/lib$(LIBNAME).a $(LIB_DIR)/
	cp $(THIRD_PARTY_BUILD_DIR)/*.a $(LIB_DIR)/

$(BUILD_DIR)/$(REALNAME):
	@mkdir -p $(BUILD_DIR)
	cmake3 -B$(BUILD_DIR) -DBUILD_SHARED_LIBS=ON $(CMAKE_FLAGS)
	$(MAKE) -C $(BUILD_DIR)

$(BUILD_DIR)/lib$(LIBNAME).a: $(BUILD_DIR)/$(REALNAME)
	@mkdir -p $(BUILD_DIR)
	cmake3 -B$(BUILD_DIR) -DBUILD_SHARED_LIBS=OFF $(CMAKE_FLAGS)
	$(MAKE) -C $(BUILD_DIR)

executable: dynamiclib
	@mkdir -p $(EXE_DIR)
	cp $(BUILD_DIR)/$(CLI_NAME) $(EXE_DIR)/$(CLI_NAME)
