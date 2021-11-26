cmake_minimum_required(VERSION 3.8)	
set(CMAKE_CXX_STANDARD 20)
project(GLContext)

file(GLOB_RECURSE HEADER_FILES *.h *.hpp *.ini)
file(GLOB_RECURSE SOURCE_FILES *.c *.cpp)
SET(AllFile ${HEADER_FILES} ${SOURCE_FILES})

foreach(fileItem ${AllFile})       
	# Get the directory of the source file
	get_filename_component(PARENT_DIR "${fileItem}" DIRECTORY)
	# Remove common directory prefix to make the group
	string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")
	# Make sure we are using windows slashes
	string(REPLACE "/" "\\" GROUP "${GROUP}")
	# Group into "Source Files" and "Header Files"
	set(GROUP "${GROUP}")
	source_group("${GROUP}" FILES "${fileItem}")
endforeach()

add_library("GLContext" ${AllFile})
set_target_properties("GLContext" PROPERTIES FOLDER "Component")

target_include_directories("GLContext" PUBLIC 
	${PROJECT_COMPONENT_DIR}/
	${PROJECT_LIBRAIES_DIR}/
)
