# Collection of functions used for handling dependencies

if(NOT COA_FETCH_DEPS)
  return()
endif()

include(FetchContent)

FetchContent_Declare(
  sup-dto
  GIT_REPOSITORY https://github.com/oac-tree/sup-dto.git
  GIT_TAG b08a4d6749e7ee6982511b1517edfa064bfb06b0 # v1.5
  OVERRIDE_FIND_PACKAGE
)

# sup-utils currently has a clash in target name unit-tests (with sup-dto)
# FetchContent_Declare(
#   sup-utils
#   GIT_REPOSITORY https://github.com/oac-tree/sup-utils.git
#   GIT_TAG 41070c713f4aa71da498f7325600d26abccbfaf2 # v1.5
#   OVERRIDE_FIND_PACKAGE
# )

message(VERBOSE "Building dependencies from source")
