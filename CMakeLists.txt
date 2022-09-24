cmake_minimum_required(VERSION 3.8)
project(LearnOpenGL)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(PROJECT_COMPONENT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Component)
set(PROJECT_LIBRARIES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Libraries)

list(APPEND allSubDir "Component")
list(APPEND allSubDir "Libraries")
list(APPEND allSubDir "TemplateApp")
list(APPEND allSubDir "HelloWindow")
list(APPEND allSubDir "HelloTriangle")
list(APPEND allSubDir "Shaders")
list(APPEND allSubDir "Textures")
list(APPEND allSubDir "Transformations")
list(APPEND allSubDir "CoordinateSystems")
list(APPEND allSubDir "Camera")
list(APPEND allSubDir "Colors")
list(APPEND allSubDir "BasicLighting")

foreach(dirname ${allSubDir})
    add_subdirectory(${dirname})
endforeach()

set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "Other")