cmake_minimum_required(VERSION 3.9)
project(project1)
set(CMAKE_CXX_STANDARD 11)

set(Python_ADDITIONAL_VERSIONS 3.7)
find_package(PythonLibs 3 REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})


find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(project1 main.cpp math1.cpp math1.h someOPs/math2.h someOPs/math2.cpp
        cpp4pymeanshift/ms.cpp cpp4pymeanshift/ms.h cpp4pymeanshift/msImageProcessor.cpp cpp4pymeanshift/msImageProcessor.h
        cpp4pymeanshift/RAList.cpp cpp4pymeanshift/RAList.h
        cpp4pymeanshift/rlist.cpp cpp4pymeanshift/rlist.h
        cpp4pymeanshift/tdef.h
        )

target_link_libraries(project1 ${PYTHON_LIBRARIES})

target_link_libraries(project1 ${OpenCV_LIBS})