cmake_minimum_required(VERSION 3.8)
project(Libraries)

list(APPEND LibrariesAllSubDir "glad")
list(APPEND LibrariesAllSubDir "glfw")

foreach(dirname ${LibrariesAllSubDir})
    add_subdirectory(${dirname})
endforeach()

