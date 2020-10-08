.PHONY: all libs staticlib dynamiclib

LIBNAME=sequencer

LIBVERSION=$(shell grep '<version>1' ../../../pom.xml | sed 's/    <version>//g' | sed 's/<\/version>//g')
LIBMAJOR=$(shell echo $(LIBVERSION) | sed 's/\([1-9]\).\([0-9]\).\([0-9]\)/\1/g')
LIBMINOR=$(shell echo $(LIBVERSION) | sed 's/\([1-9]\).\([0-9]\).\([0-9]\)/\2/g')
LIBMAINT=$(shell echo $(LIBVERSION) | sed 's/\([1-9]\).\([0-9]\).\([0-9]\)/\3/g')

SONAME=lib$(LIBNAME).so.$(LIBMAJOR)
REALNAME=lib$(LIBNAME).so.$(LIBVERSION)

TARGET=../../../target

LIB_DIR := $(TARGET)/lib
BUILD_DIR_STAT := ./build_stat
BUILD_DIR_DYN := ./build_dyn

BUILD_TYPE = Release

CMAKE_FLAGS = -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)

all: libs

libs: staticlib dynamiclib

staticlib: $(BUILD_DIR_STAT)/lib$(LIBNAME).a
	@mkdir -p $(LIB_DIR)
	cp $(BUILD_DIR_STAT)/lib$(LIBNAME).a $(LIB_DIR)/lib$(LIBNAME).a

dynamiclib: $(BUILD_DIR_DYN)/lib$(LIBNAME).so
	@mkdir -p $(LIB_DIR)
	cp $(BUILD_DIR_DYN)/lib$(LIBNAME).so $(LIB_DIR)/$(REALNAME)
	cd $(LIB_DIR);ln -sf $(REALNAME) lib$(LIBNAME).so
	cd $(LIB_DIR);ln -sf $(REALNAME) $(SONAME)

$(BUILD_DIR_STAT)/lib$(LIBNAME).a:
	@mkdir -p $(BUILD_DIR_STAT)
	cmake3 -B$(BUILD_DIR_STAT) -DBUILD_SHARED_LIBS=OFF $(CMAKE_FLAGS)
	$(MAKE) -C $(BUILD_DIR_STAT)

$(BUILD_DIR_DYN)/lib$(LIBNAME).so:
	@mkdir -p $(BUILD_DIR_DYN)
	cmake3 -B$(BUILD_DIR_DYN) -DBUILD_SHARED_LIBS=ON $(CMAKE_FLAGS)
	$(MAKE) -C $(BUILD_DIR_DYN)
