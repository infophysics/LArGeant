cmake_minimum_required(VERSION 3.19 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 20)

project(LArGeant)

set(CMAKE_BUILD_TYPE Debug)

# Debug mode
option(DEBUG_MODE "Compile with debug options" OFF)
if(DEBUG_MODE)
    add_compile_options(-g)
endif()

# Multithreading mode
option(MULTITHREADING "Compile with multithreading mode" OFF)
if(MULTITHREADING)
    if(DEFINED ENV{G4MULTITHREADED})
        add_definitions(-DLARTPC_MULTITHREADED)
        message(STATUS "Compiling with multithreading enabled.")
    else()
        message(STATUS "Attempt to use multithreading failed - G4MULTITHREADED is not defined! Maybe you didn't compile G4 with multithreading.")
    endif()
endif()

# use the visualization manager
option(USE_VIS "Whether to use the visualization." OFF)
if(USE_VIS)
    add_definitions(-DLARTPC_USE_VIS)
    message(STATUS "Compiling with the visualization enabled.")
endif()

# use the ui executive/manager
option(USE_UI "Whether to use the ui interface." OFF)
if(USE_UI)
    add_definitions(-DLARTPC_USE_UI)
    message(STATUS "Compiling with the ui enabled.")
endif()

find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})
message("Geant4 installation directory -- ${Geant4_DIR}")

# Get all header directories
MACRO(HEADER_DIRECTORIES return_list)
    FILE(GLOB_RECURSE new_list *.hh)
    SET(dir_list "")
    FOREACH(file_path ${new_list})
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
        SET(dir_list ${dir_list} ${dir_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES dir_list)
    SET(${return_list} ${dir_list})
ENDMACRO()

HEADER_DIRECTORIES(header_list)

include_directories(
    ${header_list}
    ${PROJECT_SOURCE_DIR}/lib/nest/include/Detectors/
    ${PROJECT_SOURCE_DIR}/lib/nest/include/NEST/
    ${PROJECT_SOURCE_DIR}/lib/gcem/include
)

###############        Get NEST version from git        #####################
execute_process(COMMAND git describe --tag
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib/nest/
        OUTPUT_VARIABLE NEST_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE)

function(get_versions versionString version)
    if ("${versionString}" STREQUAL "")
        set(version "0.0.0" PARENT_SCOPE)
        return()
    endif ()

    string(REGEX REPLACE "v([0-9]*)([.][0-9]*[.][0-9]*-?.*)$" "\\1" numbers ${versionString})
    set(major ${numbers})
    string(REGEX REPLACE "v([0-9]*[.])([0-9]*)([.][0-9]*-?.*)$" "\\2" numbers ${versionString})
    set(minor ${numbers})
    string(REGEX REPLACE "v([0-9]*[.][0-9]*[.])([0-9]*)(-?.*)$" "\\2" numbers ${versionString})
    set(patch ${numbers})
    set(version "${major}.${minor}.${patch}" PARENT_SCOPE)
endfunction()

get_versions("${NEST_VERSION}" version)
set(NEST_VERSION ${version})
# Set source directory
set(NEST_SOURCE_DIR "lib/nest/src")
set(NEST_SOURCES 
    "${NEST_SOURCE_DIR}/NEST.cpp" 
    "${NEST_SOURCE_DIR}/LArNEST.cpp" 
    "${NEST_SOURCE_DIR}/RandomGen.cpp"  
    "${NEST_SOURCE_DIR}/VDetector.cpp"
    "${NEST_SOURCE_DIR}/LArDetector.cpp"
    "${NEST_SOURCE_DIR}/TestSpectra.cpp" 
    "${NEST_SOURCE_DIR}/GammaHandler.cpp" 
    "${NEST_SOURCE_DIR}/ValidityTests.cpp"
)

# generate sources and headers
file(GLOB_RECURSE sources 
    ${PROJECT_SOURCE_DIR}/src/*.cxx
)
file(GLOB_RECURSE headers 
    ${PROJECT_SOURCE_DIR}/include/*.hh
)

#-----------create programs-----------#
# argon sphere
add_executable(argon_sphere 
    src/programs/ArgonSphere.cc 
    ${sources} ${headers} ${NEST_SOURCES}
)
target_link_libraries(argon_sphere ${Geant4_LIBRARIES})

# argon cube
add_executable(argon_cube 
    src/programs/ArgonCube.cc 
    ${sources} ${headers} ${NEST_SOURCES}
)
target_link_libraries(argon_cube ${Geant4_LIBRARIES})

# argon cube
add_executable(detector_test 
    src/programs/DetectorTest.cc 
    ${sources} ${headers} ${NEST_SOURCES}
)
target_link_libraries(detector_test ${Geant4_LIBRARIES})

# copy macro and data files
file(GLOB MACRO_FILES
    "macros/*.mac"
)
file(GLOB DATA_FILES
    "data/*.dat"
)
file(GLOB ANALYSIS_FILES
    "analysis/*.py"
)
file(COPY ${MACRO_FILES} DESTINATION ${PROJECT_BINARY_DIR})
file(COPY ${DATA_FILES} DESTINATION ${PROJECT_BINARY_DIR})
file(COPY ${ANALYSIS_FILES} DESTINATION ${PROJECT_BINARY_DIR})

add_custom_target(LArGeant DEPENDS largeant)

# Generate Python module
add_subdirectory(lib/pybind11)
pybind11_add_module(lar_tpc ${SOURCES} "src/bindings.cxx")