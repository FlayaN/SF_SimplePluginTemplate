cmake_minimum_required(VERSION 3.26)

SET(PROJECT_AUTHOR "FlayaN")
project(
	SF_SimplePluginTemplate
	VERSION 0.0.1
	LANGUAGES CXX
)

# ########################################################################################################################
# ## Source build guard
# ########################################################################################################################

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
	message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.")
endif()

# ########################################################################################################################
# ## Build options
# ########################################################################################################################

macro(set_from_environment VARIABLE)
	if (NOT DEFINED ${VARIABLE} AND DEFINED ENV{${VARIABLE}})
		set(${VARIABLE} $ENV{${VARIABLE}})
	endif ()
endmacro()

set_from_environment(SFDATA_PATH)

message("Options:")
option(COPY_BUILD "Copy the build output to the Starfield directory, also need SFDataPath" OFF)
option(ZIP_TO_DIST "Zip the mod to 7z file in dist." ON)
message("\tCopy build output: ${COPY_BUILD}")
message("\t\tCopy build output path SFDATA_PATH: ${SFDATA_PATH}")
message("\tZip to dist: ${ZIP_TO_DIST}")

# #######################################################################################################################
# # Add CMake features
# #######################################################################################################################

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/Version.rc.in
	${CMAKE_CURRENT_BINARY_DIR}/cmake/version.rc
	@ONLY
)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_DEBUG OFF)
set(COMMONLIBSF_CMAKE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/extern/CommonLibSF/CommonLibSF/cmake")

# #######################################################################################################################
# # Find dependencies
# #######################################################################################################################

find_package(unordered_dense CONFIG REQUIRED)
find_path(CLIB_UTIL_INCLUDE_DIRS "ClibUtil/detail/SimpleIni.h")

# #######################################################################################################################
# # Headers and sources
# #######################################################################################################################

file(GLOB_RECURSE INCLUDE_HEADERS
	LIST_DIRECTORIES false
	CONFIGURE_DEPENDS
	"include/*.h"
	"include/*.hpp"
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include
	PREFIX "Header Files"
	FILES ${INCLUDE_HEADERS}
)

file(GLOB_RECURSE SOURCE_HEADERS
	LIST_DIRECTORIES false
	CONFIGURE_DEPENDS
	"src/*.h"
	"src/*.hpp"
)
source_group(
	TREE ${CMAKE_CURRENT_SOURCE_DIR}/src
	PREFIX "Header Files"
	FILES ${SOURCE_HEADERS}
)

file(GLOB_RECURSE SOURCES
	LIST_DIRECTORIES false
	CONFIGURE_DEPENDS
	"src/*.cpp"
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src
	PREFIX "Source Files"
	FILES ${SOURCES}
)

# #######################################################################################################################
# # CommonlibSF Plugin
# #######################################################################################################################

include(${COMMONLIBSF_CMAKE_DIR}/CommonLibSF.cmake)
add_subdirectory(extern/CommonLibSF)
add_commonlibsf_plugin(
	${PROJECT_NAME} 
	AUTHOR ${PROJECT_AUTHOR}
	SOURCES ${INCLUDE_HEADERS} ${SOURCE_HEADERS} ${SOURCES}
)

# #######################################################################################################################
# # Post target setup
# #######################################################################################################################

target_precompile_headers(
	${PROJECT_NAME}
	PRIVATE
		include/PCH.h
)

target_sources(
	${PROJECT_NAME}
	PRIVATE
		${CMAKE_CURRENT_BINARY_DIR}/cmake/version.rc
)

target_compile_features(
	${PROJECT_NAME}
	PRIVATE
		cxx_std_23
)

target_include_directories(
	${PROJECT_NAME}
	PRIVATE
		${CLIB_UTIL_INCLUDE_DIRS}
)

target_link_libraries(
	${PROJECT_NAME}
	PRIVATE
		unordered_dense::unordered_dense
)

# https://gitlab.kitware.com/cmake/cmake/-/issues/24922
set_target_properties(
	${PROJECT_NAME}
	PROPERTIES
		VS_USER_PROPS ${COMMONLIBSF_CMAKE_DIR}/build_stl_modules.props
)

# #######################################################################################################################
# # Automatic deployment
# #######################################################################################################################

if (COPY_BUILD)
	if (DEFINED SFDataPath)
		add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${SFDataPath}/SFSE/Plugins/
			COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_PDB_FILE:${PROJECT_NAME}> ${SFDataPath}/SFSE/Plugins/
		)
	else ()
		message(
			WARNING
			"Variable SFDataPath is not defined. Skipping post-build copy command."
		)
	endif ()
endif ()

if(ZIP_TO_DIST)
	set(ZIP_DIR "${CMAKE_CURRENT_BINARY_DIR}/zip")
	add_custom_target(build-time-make-directory ALL
		COMMAND ${CMAKE_COMMAND} -E remove_directory "${ZIP_DIR}" ${CMAKE_SOURCE_DIR}/dist
		COMMAND ${CMAKE_COMMAND} -E make_directory "${ZIP_DIR}/SFSE/Plugins" ${CMAKE_SOURCE_DIR}/dist
	)

	message("Copying mod to ${ZIP_DIR}.")
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> "${ZIP_DIR}/SFSE/Plugins/"
		COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_PDB_FILE:${PROJECT_NAME}> "${ZIP_DIR}/SFSE/Plugins/"
	)

	set(TARGET_ZIP "${PROJECT_NAME}${PROJECT_VERSION}.7z")
	message("Zipping ${ZIP_DIR} to ${CMAKE_SOURCE_DIR}/dist/${TARGET_ZIP}")
	ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E tar cf ${CMAKE_SOURCE_DIR}/dist/${TARGET_ZIP} --format=7zip -- .
		WORKING_DIRECTORY ${ZIP_DIR}
	)
endif()
