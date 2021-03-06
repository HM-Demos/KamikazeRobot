cmake_minimum_required(VERSION 3.7)
project(robot)

set(CMAKE_CXX_STANDARD 14)

# add pthreads
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(robot
        src/main.cpp
        src/controller.cpp
        src/controller.h
        src/communication.h
        src/communication.cpp
        src/engine.cpp
        src/engine.h
        src/detection_system.cpp
        src/detection_system.h)

target_link_libraries(robot PRIVATE Threads::Threads
        ${CMAKE_SOURCE_DIR}/lib/libev3dev.a
        ${CMAKE_SOURCE_DIR}/lib/libpaho-mqttpp3.a
        ${CMAKE_SOURCE_DIR}/lib/libpaho-mqtt3a.a
        ${CMAKE_SOURCE_DIR}/lib/libpaho-mqtt3c.a)

target_compile_options(robot PRIVATE -Wpedantic -Wall -Wextra)
