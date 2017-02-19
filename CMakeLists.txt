cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 11)

project(CA_standalone)

find_package(fccedm)
find_package(podio)
find_package(ROOT)

add_executable(fcc-hits fcc-hits.cpp)
include_directories(${FCCEDM_INCLUDE_DIRS} ${podio_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS} )

target_link_libraries(fcc-hits ${ROOT_LIBRARIES} ${FCCEDM_INCLUDE_DIRS}/../lib/libdatamodel.so ${podio_LIBRARIES})
