cmake_minimum_required(VERSION 3.10)

project(RobotProject)

set(CMAKE_CXX_STANDARD 11)

#Füge hier alle deine Quellcode-Dateien hinzu
add_executable(my_robot_program
    Main.cpp
    LidarMatlab.cpp
    LidarProcessor.cpp
    SharedMemory.cpp
    TCPEchoClient.cpp
    LidarMatlab.cpp
    LidarProcessor.cpp
    OdometryClient.cpp
)
