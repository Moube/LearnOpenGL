cmake_minimum_required(VERSION 3.8)
project(Component)

list(APPEND ComponentAllSubDir "Template")

foreach(dirname ${ComponentAllSubDir})
    add_subdirectory(${dirname})
endforeach()

