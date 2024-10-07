# oac-tree

`oac-tree` is a software framework for Operation, Automation and Control, based on behavior trees. This repository contains the main library and command line executables. Plugins are provided in separate repositories.

Note that all executables and documentation refer to the project as the `Sequencer`, since this was the name used internally. This may change in the future.

## Building and installation

### Prerequisites

All software components require `CMake` and `Google Test` for building and unit testing respectively.

To be able to build and install `oac-tree`, be sure to first install its dependencies:

* `sup-dto`: library for structured Data Transfer Objects; no extra dependencies.
* `sup-utils`: utility libraries: xml parsing, command line argument parsing, etc. Depends also on `libxml2` for XML parsing.

### Build and install

To build `oac-tree` or any of its dependencies, check out the source code and execute the following step:

```bash
mkdir <BUILD_DIR>
cd <BUILD_DIR>
cmake -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR> <SOURCE_DIR>
cmake --build .
cmake --install .
```

where the following paths should be specified:

* `<BUILD_DIR>`: the folder where all build files will be generated;
* `<SOURCE_DIR>`: the folder where the source code was checked out;
* `<INSTALL_DIR>`: the root of the folders where the executable, libraries and header files will be installed.

### Building documentation

To build the documentation, the following extra dependencies are required:

* `doxygen`;
* `sphinx` and its components `breathe` and `sphinx_rtd_theme`.

Use the following lines instead during the build generation:

```bash
mkdir <BUILD_DIR>
cd <BUILD_DIR>
cmake -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR> -DCOA_BUILD_DOCUMENTATION=On <SOURCE_DIR>
cmake --build .
cmake --install .
```

The documentation will then be installed in `<INSTALL_DIR>/share/doc/sup-sequencer/`.

## Running some simple procedures

Some simple procedures can be found in the folder `<SOURCE_DIR>/test/resources`. To execute one of these procedures, e.g. `wait_for_variable.xml`, run the following command in a shell:

```bash
<INSTALL_DIR>/bin/sequencer-cli -f <SOURCE_DIR>/test/resources/wait_for_variable.xml
```
