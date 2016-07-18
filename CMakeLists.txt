cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 14)
find_package(ROOT COMPONENTS Geom MathCore RIO REQUIRED)

message(R1 ${ROOT_INCLUDE_DIR})
message(R2 ${ROOT_INCLUDE_DIRS})

#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})
find_package(DD4hep COMPONENTS DDG4 DDEve REQUIRED)
find_package(Geant4)
find_package(podio)
find_package(fccedm)

include_directories(${DD4hep_INCLUDE_DIRS} ${ROOT_INCLUDE_DIR} ${Geant4_INCLUDE_DIRS} ${podio_INCLUDE_DIRS} ${FCCEDM_INCLUDE_DIRS})


add_executable(fcc2dd4hit fcc2dd4vis.cpp)
target_link_libraries(fcc2dd4hit  ${DD4hep_LIBRARIES} ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} ${DD4hep_LIBRARIES}  ${DD4hep_DDG4_LIBRARY} ${podio_LIBRARIES} ${FCCEDM_CMAKE_DIR}/../lib/libdatamodel.so)
