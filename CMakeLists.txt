cmake_minimum_required(VERSION 3.8)

project("winapi")

file(GLOB_RECURSE WINAPI_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.ixx)

add_library(${PROJECT_NAME}
	${WINAPI_SOURCES}
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
