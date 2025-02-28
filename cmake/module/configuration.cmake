# -----------------------------------------------------------------------------
# Common project configuration
# -----------------------------------------------------------------------------

include(COACommonConfig)
include(COASetupCodacEnvironment)

# -----------------------------------------------------------------------------
# Dependencies
# -----------------------------------------------------------------------------
include(FetchGithubDeps)

find_package(sup-dto REQUIRED)
find_package(sup-utils REQUIRED)
