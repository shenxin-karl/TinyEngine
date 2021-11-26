cmake_minimum_required(VERSION 3.8)	
set(CMAKE_CXX_STANDARD 20)
project("Libraies")
LIST(APPEND LibraiesSubDir "glad")
set(PROJECT_LIBRAIES_DIR ${CMAKE_CURRENT_SOURCE_DIR})
foreach(dirname ${LibraiesSubDir}) 
	add_subdirectory(${dirname})
endforeach()

