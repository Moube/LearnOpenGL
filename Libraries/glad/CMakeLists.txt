cmake_minimum_required(VERSION 3.8)
project(glad)
option(GROUP_BY_EXPLORER ON)
option(USE_SOLUTION_FOLDERS ON)
set(CMAKE_CXX_STANDARD 11)

file(GLOB_RECURSE HEADER_FILES *.h *.hpp *.ini)
file(GLOB_RECURSE SOURCE_FILES *.cpp *.c)
file(GLOB_RECURSE SHADER_FILES *.hlsl)

set(CppFile ${HEADER_FILES} ${SOURCE_FILES})
set(AllFile ${CppFile} ${SHADER_FILES})

foreach(fileItem ${AllFile})
    get_filename_component(PARENT_DIR "${fileItem}" DIRECTORY)
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")
    string(REPLACE "/" "\\" GROUP "${GROUP}")
    set(GROUP "${GROUP}")
    source_group("${GROUP}" FILES "${fileItem}")
endforeach()

add_library(${PROJECT_NAME} ${AllFile})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Libraries")
set_property(SOURCE ${SHADER_FILES} PROPERTY VS_TOOL_OVERRIDE "shader")
set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

# 头文件目录
target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_LIBRARIES_DIR}
)