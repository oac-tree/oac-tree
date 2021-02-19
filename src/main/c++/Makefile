.PHONY: all cmake_targets

LIBVERSION=$(shell grep '<version>1' ../../../pom.xml | sed 's/    <version>//g' | sed 's/<\/version>//g')
LIBMAJOR=$(shell echo $(LIBVERSION) | sed 's/\([1-9]\).\([0-9]\).\([0-9]\)/\1/g')
LIBMINOR=$(shell echo $(LIBVERSION) | sed 's/\([1-9]\).\([0-9]\).\([0-9]\)/\2/g')
LIBMAINT=$(shell echo $(LIBVERSION) | sed 's/\([1-9]\).\([0-9]\).\([0-9]\)/\3/g')

TARGET=../../../target

BUILD_DIR := ./build

BUILD_TYPE = RelWithDebInfo

CMAKE_FLAGS = -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -DLIBVERSION=$(LIBVERSION) -DLIBSOVERSION=$(LIBMAJOR)
CMAKE_FLAGS += -DCMAKE_INSTALL_PREFIX=$(TARGET)

all: cmake_targets

cmake_targets:
	@mkdir -p $(BUILD_DIR)
	cmake3 -B$(BUILD_DIR) $(CMAKE_FLAGS)
	cmake3 --build $(BUILD_DIR)
	make -C $(BUILD_DIR) install
