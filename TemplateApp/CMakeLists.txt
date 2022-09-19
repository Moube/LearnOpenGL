cmake_minimum_required(VERSION 3.8)
project(TemplateApp)
option(USE_SOLUTION_FOLDERS ON)
option(GROUP_BY_EXPLORER ON)

file(GLOB_RECURSE HEADER_FILES *.h *.hpp *.ini)
file(GLOB_RECURSE SOURCE_FILES *.cpp *.c)
file(GLOB_RECURSE SHADER_FILES *.vert *.frag)

set(CppFile ${HEADER_FILES} ${SOURCE_FILES})
set(AllFile ${CppFile} ${SHADER_FILES})

foreach(fileItem ${AllFile})
    get_filename_component(PARENT_DIR "${fileItem}" DIRECTORY)
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")
    string(REPLACE "/" "\\" GROUP "${GROUP}")
    set(GROUP "${GROUP}")
    source_group("${GROUP}" FILES "${fileItem}")
endforeach()

# 编译为 可执行文件
add_executable(${PROJECT_NAME} ${AllFile})
set_property(SOURCE ${SHADER_FILES} PROPERTY VS_TOOL_OVERRIDE "shader")

# 头文件目录
target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${PROJECT_COMPONENT_DIR}/Template
    ${PROJECT_LIBRARIES_DIR}/glad
    ${PROJECT_LIBRARIES_DIR}/glfw
    ${PROJECT_LIBRARIES_DIR}/stb_image
    ${PROJECT_LIBRARIES_DIR}/glm
)

# 链接组件库
target_link_libraries(${PROJECT_NAME} PUBLIC
    Template
    glad
    glfw
    stb_image
    glm
)
