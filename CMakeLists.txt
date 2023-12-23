# ==============================================================================
# @AUTHOR Vik Pandher
# @DATE 2023-12-20

if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
    message(FATAL_ERROR "Prevented in-tree build. Create a build directory and run \"cmake -S ${CMAKE_SOURCE_DIR} -B .\" from there")
endif()

cmake_minimum_required(VERSION 3.22.0)

set(CMAKE_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/Bin")

option(CMAKE_SANITY_CHECK_EXTRA_CMAKE_DEBUG_OUTPUT "Cmake outputs extra dubug info." TRUE)

project("FileToHeaderConverter" # ${PROJECT_NAME}
    VERSION 0.1.0.0
    DESCRIPTION "This is used to convert files into C++ headers so they can be embedded in a binary."
    LANGUAGES C CXX
)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" PROPERTY VS_STARTUP_PROJECT "${PROJECT_NAME}")

set(SOURCE_FILES
    "Src/Main.cpp"
)

source_group("Src" FILES ${SOURCE_FILES})

# adding WIN32 makes WinMain() the entry point and so no conole appears
add_executable("${PROJECT_NAME}" # WIN32
    ${SOURCE_FILES}
)

target_compile_definitions("${PROJECT_NAME}" PRIVATE PROJECT_NAME="${PROJECT_NAME}")

# Set command line arguments for the executable
set_target_properties("${PROJECT_NAME}" PROPERTIES VS_DEBUGGER_COMMAND_ARGUMENTS
    "/s ../Test/Cube.obj ../Test/Cube_obj.h g_cube_obj"
)

target_include_directories("${PROJECT_NAME}"
    PRIVATE 
    "Include"
    "Src"
)

target_link_directories("${PROJECT_NAME}"
    PRIVATE
    "Lib"
)

# Getting a linker warning when building debug only:
    # LNK4098 defaultlib 'MSVCRT' confilics with use of other libs; use
    # /NODEFAULTLIB:library

target_link_libraries("${PROJECT_NAME}"
    PRIVATE
)

set_target_properties("${PROJECT_NAME}" PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_OUTPUT_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_OUTPUT_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_OUTPUT_DIR}"
    OUTPUT_NAME_DEBUG "${PROJECT_NAME}_debug"
    OUTPUT_NAME_RELEASE "${PROJECT_NAME}"
)

if(${CMAKE_SANITY_CHECK_EXTRA_CMAKE_DEBUG_OUTPUT})
    message("--------------------------------------------------------------------------------")

    message("CMAKE_OUTPUT_DIR: ${CMAKE_OUTPUT_DIR}")

    message("PROJECT_NAME: ${PROJECT_NAME}")
    message("PROJECT_VERSION: ${PROJECT_VERSION}")
    message("PROJECT_DESCRIPTION: ${PROJECT_DESCRIPTION}")

    get_property(_ENABLED_LANGUAGES GLOBAL PROPERTY ENABLED_LANGUAGES)
    message("ENABLED_LANGUAGES:")
    foreach(VARIABLE ${_ENABLED_LANGUAGES})
        message("    ${VARIABLE}")
    endforeach()

    message("CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
    message("CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
    message("CMAKE_GENERATOR: ${CMAKE_GENERATOR}")

    message("CMAKE_CONFIGURATION_TYPES:")
    foreach(VARIABLE ${CMAKE_CONFIGURATION_TYPES})
        message("    ${VARIABLE}")
    endforeach()

    get_property(_USE_FOLDERS GLOBAL PROPERTY USE_FOLDERS)
    message("USE_FOLDERS: ${_USE_FOLDERS}")
    
    get_property(_VS_STARTUP_PROJECT DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" PROPERTY VS_STARTUP_PROJECT)
    message("VS_STARTUP_PROJECT: ${_VS_STARTUP_PROJECT}")

    get_property(_SOURCES TARGET "${PROJECT_NAME}" PROPERTY SOURCES)
    message("SOURCES:")
    foreach(VARIABLE ${_SOURCES})
        message("    ${VARIABLE}")
    endforeach()

    get_target_property(_COMPILE_DEFINITIONS "${PROJECT_NAME}" COMPILE_DEFINITIONS)
    message("COMPILE_DEFINITIONS:")
    foreach(VARIABLE ${_COMPILE_DEFINITIONS})
        message("    ${VARIABLE}")
    endforeach()

    get_property(_TARGET_INCLUDE_DIRECTORIES TARGET "${PROJECT_NAME}" PROPERTY INCLUDE_DIRECTORIES)
    message("TARGET_INCLUDE_DIRECTORIES:")
    foreach(VARIABLE ${_TARGET_INCLUDE_DIRECTORIES})
        message("    ${VARIABLE}")
    endforeach()

    get_property(_TARGET_LINK_DIRECTORIES TARGET "${PROJECT_NAME}" PROPERTY LINK_DIRECTORIES)
    message("TARGET_LINK_DIRECTORIES:")
    foreach(VARIABLE ${_TARGET_LINK_DIRECTORIES})
        message("    ${VARIABLE}")
    endforeach()

    get_property(_TARGET_LINK_LIBRARIES TARGET "${PROJECT_NAME}" PROPERTY LINK_LIBRARIES)
    message("TARGET_LINK_LIBRARIES:")
    foreach(VARIABLE ${_TARGET_LINK_LIBRARIES})
        message("    ${VARIABLE}")
    endforeach()

    get_property(_VS_DEBUGGER_WORKING_DIRECTORY TARGET "${PROJECT_NAME}" PROPERTY VS_DEBUGGER_WORKING_DIRECTORY)
    message("VS_DEBUGGER_WORKING_DIRECTORY: ${_VS_DEBUGGER_WORKING_DIRECTORY}")

    get_property(_RUNTIME_OUTPUT_DIRECTORY_DEBUG TARGET "${PROJECT_NAME}" PROPERTY RUNTIME_OUTPUT_DIRECTORY_DEBUG)
    message("RUNTIME_OUTPUT_DIRECTORY_DEBUG: ${_RUNTIME_OUTPUT_DIRECTORY_DEBUG}")

    get_property(_RUNTIME_OUTPUT_DIRECTORY_RELEASE TARGET "${PROJECT_NAME}" PROPERTY RUNTIME_OUTPUT_DIRECTORY_RELEASE)
    message("RUNTIME_OUTPUT_DIRECTORY_RELEASE: ${_RUNTIME_OUTPUT_DIRECTORY_RELEASE}")

    get_property(_OUTPUT_NAME_DEBUG TARGET "${PROJECT_NAME}" PROPERTY OUTPUT_NAME_DEBUG)
    message("OUTPUT_NAME_DEBUG: ${_OUTPUT_NAME_DEBUG}")

    get_property(_OUTPUT_NAME_RELEASE TARGET "${PROJECT_NAME}" PROPERTY OUTPUT_NAME_RELEASE)
    message("OUTPUT_NAME_RELEASE: ${_OUTPUT_NAME_RELEASE}")

    message("--------------------------------------------------------------------------------")
endif()
