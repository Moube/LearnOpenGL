cmake_minimum_required(VERSION 3.8)
project(Libraries)

list(APPEND LibrariesAllSubDir "glad")
list(APPEND LibrariesAllSubDir "glfw")
list(APPEND LibrariesAllSubDir "stb_image")
list(APPEND LibrariesAllSubDir "glm")
list(APPEND LibrariesAllSubDir "assimp")
list(APPEND LibrariesAllSubDir "freetype")

foreach(dirname ${LibrariesAllSubDir})
    add_subdirectory(${dirname})
endforeach()

