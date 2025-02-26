# Collection of functions used for handling dependencies

if(NOT COA_FETCH_DEPS)
  return()
endif()

include(FetchContent)

FetchContent_Declare(
  sup-dto
  GIT_REPOSITORY https://github.com/oac-tree/sup-dto.git
  GIT_TAG 6000d1e11b8fd44ed835a4923d7d135529e5e04d # v1.7dev
  OVERRIDE_FIND_PACKAGE
)

sup-utils currently has a clash in target name unit-tests (with sup-dto)
FetchContent_Declare(
  sup-utils
  GIT_REPOSITORY https://github.com/oac-tree/sup-utils.git
  GIT_TAG a289e1fcd8e33638d505226d6a2ae6ce9321bdfd # v1.6dev
  OVERRIDE_FIND_PACKAGE
)

message(VERBOSE "Building dependencies from source")
