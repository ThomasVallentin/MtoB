cmake_minimum_required(VERSION 3.15)
project(MtoB)

set(CMAKE_CXX_STANDARD 11)

set(LIBRARY_OUTPUT_PATH  ${CMAKE_CURRENT_SOURCE_DIR}/libraries)

file(GLOB_RECURSE SOURCES_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*)
file(GLOB_RECURSE INCLUDES_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*)

# We're building the plugin library
add_library(MtoB SHARED ${SOURCES_FILES} ${INCLUDES_FILES})

# Find Maya include directory
find_path(MAYA_INCLUDE_DIR maya/MFn.h
    PATHS
        $ENV{MAYA_LOCATION}
    PATH_SUFFIXES
        "include/"
        )
message(${MAYA_INCLUDE_DIR})

# Find OpenMaya
find_library(MAYA_LIBRARY
    NAMES
        OpenMaya
    PATHS
        $ENV{MAYA_LOCATION}
    PATH_SUFFIXES
        "lib/"
        )
set(MAYA_LIBRARIES "${MAYA_LIBRARY}")

# Find other OpenMaya libraries
set(_MAYA_LIBRARIES Foundation OpenMayaRender OpenMayaUI)
foreach (MAYA_LIB ${_MAYA_LIBRARIES})
    find_library(MAYA_${MAYA_LIB}_LIBRARY
        NAMES
            ${MAYA_LIB}
        PATHS
            $ENV{MAYA_LOCATION}
        PATH_SUFFIXES
            "lib/"
            )
    message(${MAYA_${MAYA_LIB}_LIBRARY})
    set(MAYA_LIBRARIES ${MAYA_LIBRARIES} "${MAYA_${MAYA_LIB}_LIBRARY}")
endforeach()

set(BOUNCE_RENDERER_DIR $ENV{BOUNCE_ROOT}/bounceRenderer)

# Add Bounce includes
set(BOUNCE_RENDERER_INCLUDE_DIR ${BOUNCE_RENDERER_DIR}/include)

# Find Bounce library
find_library(BOUNCE_LIBRARY
    NAMES
        bounceRendererLib
    PATHS
        $ENV{BOUNCE_RENDERER_DIR}
    PATH_SUFFIXES
        "libraries"
        )

set_target_properties(MtoB PROPERTIES SUFFIX ".mll")

target_include_directories(MtoB PUBLIC
        ${MAYA_INCLUDE_DIR}
        ${BOUNCE_RENDERER_INCLUDE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(MtoB ${MAYA_LIBRARIES} ${BOUNCE_LIBRARY})

# Set deployment directories
set(MAYA_RELEASE_DIR C:/Users/Thomas\ Vallentin/Documents/maya/PLUGINS/MtoB)
set(PLUGIN_RELEASE_DIR ${MAYA_RELEASE_DIR}/plug-ins)

# Create deployed hierarchy
add_custom_command(TARGET MtoB
        PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${MAYA_RELEASE_DIR})

# Copy scripts to maya plug-ins
add_custom_target(TARGET MtoB
        PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory  ${CMAKE_CURRENT_SOURCE_DIR}/scripts ${MAYA_RELEASE_DIR}/scripts)

# Copy README to maya plug-ins
add_custom_command(TARGET MtoB
        PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/README.md ${MAYA_RELEASE_DIR})

# Copy plugin to maya plug-ins
add_custom_command(TARGET MtoB
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${PLUGIN_RELEASE_DIR})

add_custom_command(TARGET MtoB
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:MtoB> ${PLUGIN_RELEASE_DIR})
