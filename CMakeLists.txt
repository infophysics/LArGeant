cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 20)

project(LArGeant)

find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})

# grab include directories
include_directories(LArGeant PUBLIC 
    ${PROJECT_SOURCE_DIR}/include
)
# generate sources and headers
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cxx)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#-----------create programs-----------#
# argon sphere
add_executable(argon_sphere src/programs/ArgonSphere.cc ${sources} ${headers})
target_link_libraries(argon_sphere ${Geant4_LIBRARIES})
# argon cube
add_executable(argon_cube src/programs/ArgonCube.cc ${sources} ${headers})
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