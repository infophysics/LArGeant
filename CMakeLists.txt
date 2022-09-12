cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 20)

project(LArGeant)

find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})

# grab include directories
include_directories(LArGeant PUBLIC 
    ${PROJECT_SOURCE_DIR}/include
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
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cxx)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

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

# copy macro and data files
file(GLOB MACRO_FILES
    "macros/*.mac"
)
file(COPY ${MACRO_FILES} DESTINATION ${PROJECT_BINARY_DIR})
file(GLOB DATA_FILES
    "*.dat"
)
file(COPY ${DATA_FILES} DESTINATION ${PROJECT_BINARY_DIR})

add_custom_target(LArGeant DEPENDS largeant)

set(CMAKE_BUILD_TYPE Debug)