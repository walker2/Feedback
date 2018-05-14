cmake_minimum_required(VERSION 3.5)
project(Feedback)

set(CMAKE_PREFIX_PATH $ENV{HOME}/Qt/5.10.0/gcc_64)
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_STANDARD 11)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Multimedia REQUIRED)
find_package(Qt5PrintSupport REQUIRED)

include_directories(${Qt5Widgets_INCLUDE_DIRS})
set(SOURCE_FILES Feedback/main.cpp Feedback/qcustomplot.h Feedback/qcustomplot.cpp)
add_executable(Feedback ${SOURCE_FILES})
target_link_libraries(Feedback Qt5::Gui Qt5::Widgets Qt5::Multimedia Qt5::Core Qt5::PrintSupport)