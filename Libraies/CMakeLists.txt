cmake_minimum_required(VERSION 3.8)	
project("Libraies")

LIST(APPEND LibraiesSubDir "glad")
LIST(APPEND LibraiesSubDir "glm")

set(PROJECT_LIBRAIES_DIR ${CMAKE_CURRENT_SOURCE_DIR})
foreach(dirname ${LibraiesSubDir}) 
	add_subdirectory(${dirname})
endforeach()

