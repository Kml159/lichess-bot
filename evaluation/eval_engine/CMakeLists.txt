cmake_minimum_required(VERSION 3.5)
project(eval_engine)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "-g")  # FOR DEBUGGING !!!
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find the Python3 interpreter and create a virtual environment
find_package(Python3 COMPONENTS Interpreter)
execute_process(COMMAND "${Python3_EXECUTABLE}" -m venv "venv/")

# Set the venv
set(ENV{VIRTUAL_ENV} "${CMAKE_SOURCE_DIR}/venv")
set(Python3_FIND_VIRTUALENV FIRST)
unset(Python3_EXECUTABLE)

# Launch a new search for Python3 components
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
include_directories(${Python3_INCLUDE_DIRS})

# Add the executable for main.cpp
add_executable(output.o src/main.cpp)
target_link_libraries(output.o ${Python3_LIBRARIES})

# Add the shared library for interface.cpp
add_library(eval_engine SHARED src/interface.cpp)
target_link_libraries(eval_engine ${Python3_LIBRARIES})