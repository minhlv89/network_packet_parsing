cmake_minimum_required(VERSION 3.0)
set (CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
project(axelspacepacketparsing)
file(GLOB headers *.h)
file(GLOB sources *.cpp)
add_executable(
	${PROJECT_NAME}
	${headers}
	${sources}
)
target_link_libraries(
	${PROJECT_NAME}
)
if(OS_WINDOWS)
target_link_libraries(
	Ws2_32.lib
)
endif()
