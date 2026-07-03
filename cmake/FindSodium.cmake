include(FindPackageHandleStandardArgs)

find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(PC_Sodium QUIET libsodium)
endif()

find_path(Sodium_INCLUDE_DIR
    NAMES sodium.h
    HINTS ${PC_Sodium_INCLUDE_DIRS}
)

find_library(Sodium_LIBRARY
    NAMES sodium libsodium
    HINTS ${PC_Sodium_LIBRARY_DIRS}
)

set(Sodium_VERSION "${PC_Sodium_VERSION}")

find_package_handle_standard_args(Sodium
    REQUIRED_VARS
        Sodium_LIBRARY
        Sodium_INCLUDE_DIR
    VERSION_VAR
        Sodium_VERSION
)

if(Sodium_FOUND AND NOT TARGET Sodium::Sodium)
    add_library(Sodium::Sodium UNKNOWN IMPORTED)
    set_target_properties(Sodium::Sodium PROPERTIES
        IMPORTED_LOCATION "${Sodium_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${Sodium_INCLUDE_DIR}"
    )
endif()

mark_as_advanced(Sodium_INCLUDE_DIR Sodium_LIBRARY)
