#行位带 "#"的语句是自己加的
cmake_minimum_required(VERSION 3.8)#
project(glm)#
option(GROUP_BY_EXPLORER ON)#
option(USE_SOLUTION_FOLDERS ON)#
set(CMAKE_CXX_STANDARD 11)#
set(BUILD_STATIC_LIBS true)#

file(GLOB ROOT_SOURCE *.cpp)
file(GLOB ROOT_INLINE *.inl)
file(GLOB ROOT_HEADER *.hpp)
file(GLOB ROOT_TEXT ../*.txt)
file(GLOB ROOT_MD ../*.md)
file(GLOB ROOT_NAT ../util/glm.natvis)

file(GLOB_RECURSE CORE_SOURCE ./detail/*.cpp)
file(GLOB_RECURSE CORE_INLINE ./detail/*.inl)
file(GLOB_RECURSE CORE_HEADER ./detail/*.hpp)

file(GLOB_RECURSE EXT_SOURCE ./ext/*.cpp)
file(GLOB_RECURSE EXT_INLINE ./ext/*.inl)
file(GLOB_RECURSE EXT_HEADER ./ext/*.hpp)

file(GLOB_RECURSE GTC_SOURCE ./gtc/*.cpp)
file(GLOB_RECURSE GTC_INLINE ./gtc/*.inl)
file(GLOB_RECURSE GTC_HEADER ./gtc/*.hpp)

file(GLOB_RECURSE GTX_SOURCE ./gtx/*.cpp)
file(GLOB_RECURSE GTX_INLINE ./gtx/*.inl)
file(GLOB_RECURSE GTX_HEADER ./gtx/*.hpp)

file(GLOB_RECURSE SIMD_SOURCE ./simd/*.cpp)
file(GLOB_RECURSE SIMD_INLINE ./simd/*.inl)
file(GLOB_RECURSE SIMD_HEADER ./simd/*.h)

source_group("Text Files" FILES ${ROOT_TEXT} ${ROOT_MD})
source_group("Core Files" FILES ${CORE_SOURCE})
source_group("Core Files" FILES ${CORE_INLINE})
source_group("Core Files" FILES ${CORE_HEADER})
source_group("EXT Files" FILES ${EXT_SOURCE})
source_group("EXT Files" FILES ${EXT_INLINE})
source_group("EXT Files" FILES ${EXT_HEADER})
source_group("GTC Files" FILES ${GTC_SOURCE})
source_group("GTC Files" FILES ${GTC_INLINE})
source_group("GTC Files" FILES ${GTC_HEADER})
source_group("GTX Files" FILES ${GTX_SOURCE})
source_group("GTX Files" FILES ${GTX_INLINE})
source_group("GTX Files" FILES ${GTX_HEADER})
source_group("SIMD Files" FILES ${SIMD_SOURCE})
source_group("SIMD Files" FILES ${SIMD_INLINE})
source_group("SIMD Files" FILES ${SIMD_HEADER})

#add_library(glm INTERFACE)
#target_include_directories(glm INTERFACE ../)
add_library(${PROJECT_NAME} INTERFACE)#
target_include_directories(${PROJECT_NAME} INTERFACE ../)#
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)#
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Libraries")#
set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")#

if(BUILD_STATIC_LIBS)
add_library(glm_static STATIC ${ROOT_TEXT} ${ROOT_MD} ${ROOT_NAT}
	${ROOT_SOURCE}    ${ROOT_INLINE}    ${ROOT_HEADER}
	${CORE_SOURCE}    ${CORE_INLINE}    ${CORE_HEADER}
	${EXT_SOURCE}     ${EXT_INLINE}     ${EXT_HEADER}
	${GTC_SOURCE}     ${GTC_INLINE}     ${GTC_HEADER}
	${GTX_SOURCE}     ${GTX_INLINE}     ${GTX_HEADER}
	${SIMD_SOURCE}    ${SIMD_INLINE}    ${SIMD_HEADER})
	target_link_libraries(glm_static PUBLIC glm)
	add_library(glm::glm_static ALIAS glm_static)
	set_target_properties(glm_static PROPERTIES FOLDER "Libraries")#
	set_target_properties(glm_static PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BUILD_DIR}/Binary)#
	set_target_properties(glm_static PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BUILD_DIR}/Binary)#
endif()

if(BUILD_SHARED_LIBS)
add_library(glm_shared SHARED ${ROOT_TEXT} ${ROOT_MD} ${ROOT_NAT}
	${ROOT_SOURCE}    ${ROOT_INLINE}    ${ROOT_HEADER}
	${CORE_SOURCE}    ${CORE_INLINE}    ${CORE_HEADER}
	${EXT_SOURCE}     ${EXT_INLINE}     ${EXT_HEADER}
	${GTC_SOURCE}     ${GTC_INLINE}     ${GTC_HEADER}
	${GTX_SOURCE}     ${GTX_INLINE}     ${GTX_HEADER}
	${SIMD_SOURCE}    ${SIMD_INLINE}    ${SIMD_HEADER})
	target_link_libraries(glm_shared PUBLIC glm)
	add_library(glm::glm_shared ALIAS glm_shared)
	set_target_properties(glm_shared PROPERTIES FOLDER "Libraries")#
	set_target_properties(glm_shared PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BUILD_DIR}/Binary)#
	set_target_properties(glm_shared PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BUILD_DIR}/Binary)#
endif()