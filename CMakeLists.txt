cmake_minimum_required(VERSION 3.26)

SET(PROJECT_AUTHOR "FlayaN")
project(
	SF_SimplePluginTemplate
	VERSION 0.0.1
	LANGUAGES CXX
)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# ########################################################################################################################
# ## Build options
# ########################################################################################################################
message("CommonLibSFPlugin Options:")
option(COPY_BUILD "Copy the build output to the Starfield directory, also need ENV:SFDataPath" OFF)
option(ZIP_TO_DIST "Zip the mod to 7z file in dist." ON)
message("\tCopy build output to ENV:SFDataPath: ${COPY_BUILD}")
message("\tZip to dist: ${ZIP_TO_DIST}")

# #######################################################################################################################
# # Add CMake features
# #######################################################################################################################
include(CommonLibSFPlugin)

# #######################################################################################################################
# # Find dependencies
# #######################################################################################################################
find_package(unordered_dense CONFIG REQUIRED)
find_path(CLIB_UTIL_INCLUDE_DIRS "ClibUtil/detail/SimpleIni.h")

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