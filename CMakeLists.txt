cmake_minimum_required(VERSION 3.8)	
set(CMAKE_CXX_STANDARD 20)
option(GROUP_BY_EXPLORER ON)					# 和目录相同结构
set_property(GLOBAL PROPERTY USE_FOLDERS ON)	# 运行CMAKE创建文件夹
project(AllProject)

set(PROJECT_LIBRAIES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Libraies)
set(PROJECT_COMPONENT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Component)

LIST(APPEND allSubDir "Component")
LIST(APPEND allSubDir "Libraies")
LIST(APPEND allSubDir "App")
foreach(dirname ${allSubDir})
	add_subdirectory(${dirname})
endforeach()

set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "Other")