cmake_minimum_required(VERSION 3.13.4)
project(pylittleocio)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/")

set(CMAKE_CXX_FLAGS "-std=c++14 ${CMAKE_CXX_FLAGS}")

find_package(Python COMPONENTS Interpreter Development REQUIRED)
include_directories(${Python_INCLUDE_DIRS})

find_package(pybind11 REQUIRED)

find_package(OCIO REQUIRED)


set(LIB_NAME ${PROJECT_NAME})
add_definitions(-DLIB_NAME=${LIB_NAME})

add_library(${LIB_NAME} SHARED pylittleocio.cpp)

SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES PREFIX "")
target_link_libraries(${LIB_NAME} PRIVATE ${Python_LIBRARIES})
target_link_libraries(${LIB_NAME} PRIVATE ${OCIO_LIBRARIES})

install(TARGETS ${LIB_NAME} DESTINATION lib/python${Python_VERSION_MAJOR}.${Python_VERSION_MINOR})
