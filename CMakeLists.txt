cmake_minimum_required(VERSION 3.9)
project(cpp-ext-python)

set(SDK_VERSION 0_0_1)

#set(CMAKE_BUILD_TYPE "Release")				# 指定生成的版本
#set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
#set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")


set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED TRUE)

find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})

####
set(Python_ADDITIONAL_VERSIONS 3.9)
find_package(PythonLibs 3 REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

set(PYTHON_ROOT "/usr/local/Cellar/python@3.9/3.9.2_1/Frameworks/Python.framework/Versions/3.9")
message("python root: " ${PYTHON_ROOT})
include_directories(${PYTHON_ROOT}/include/)
include_directories(${PYTHON_ROOT}/lib/)
link_directories(${PYTHON_ROOT}/lib/)
####

include_directories("/Users/shanmu/CLionProjects/project7")

add_executable(cpp-ext-python main3.cpp
        cpp4pymeanshift/ms.cpp cpp4pymeanshift/ms.h cpp4pymeanshift/msImageProcessor.cpp cpp4pymeanshift/msImageProcessor.h
        cpp4pymeanshift/RAList.cpp cpp4pymeanshift/RAList.h
        cpp4pymeanshift/rlist.cpp cpp4pymeanshift/rlist.h
        cpp4pymeanshift/tdef.h)

target_link_libraries(cpp-ext-python ${PYTHON_LIBRARIES})
target_link_libraries(cpp-ext-python ${OpenCV_LIBS})