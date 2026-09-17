cmake_minimum_required(VERSION 3.16)

project(FlightIntelligenceDashboard LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Network)

add_executable(FlightIntelligenceDashboard

)

target_link_libraries(FlightIntelligenceDashboard
	PRIVATE
	Qt6::Widgets
	Qt6::Network
	) 
