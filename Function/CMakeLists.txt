cmake_minimum_required(VERSION 3.8)	
project(Component)
option(GROUP_BY_EXPLORER ON)

LIST(APPEND ComponentAllSubDir "Function")
LIST(APPEND ComponentAllSubDir "Math")
LIST(APPEND ComponentAllSubDir "ShaderParse")

if (MSVC)
	add_definitions(-D_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING)	## enable wstring_convert
endif(MSVC)

foreach(dirname ${ComponentAllSubDir}) 
	if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${dirname}/CMakeLists.txt")
		add_subdirectory(${dirname})
	endif()
endforeach()

