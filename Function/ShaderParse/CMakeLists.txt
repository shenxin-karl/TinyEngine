cmake_minimum_required(VERSION 3.8)	
project(ShaderParse)

# 开启多线程编译 和 使用 c++latest 版本
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /std:c++latest")

file(GLOB_RECURSE L_SOURCES *.l)
file(GLOB_RECURSE Y_SOURCES *.y)

message("L_SOURCES: ${L_SOURCES}")
message("L_SOURCES: ${Y_SOURCES}")

set(FLEX_OUPUT "${CMAKE_CURRENT_SOURCE_DIR}/ParseCode/lex.yy.cpp")
add_custom_command(OUTPUT ${FLEX_OUPUT}
				   PRE_BUILD
                   COMMAND ${PROJECT_DIR}/Tool/win_flex_bison/win_flex.exe -o ${FLEX_OUPUT} --wincompat ${L_SOURCES} 
				   MAIN_DEPENDENCY ${L_SOURCES}
                   COMMENT "Generate ${FLEX_OUPUT}"
)

set(BISON_OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/ParseCode/sysy.tab.cpp")
add_custom_command(OUTPUT ${BISON_OUTPUT}
				   PRE_BUILD
                   COMMAND ${PROJECT_DIR}/Tool/win_flex_bison/win_bison.exe -d -v -b y ${Y_SOURCES}  -o ${BISON_OUTPUT}
				   MAIN_DEPENDENCY ${Y_SOURCES}
                   COMMENT "Generate ${BISON_OUTPUT}"
)

add_compile_options(/wd4005)
add_compile_options(/wd4819)
add_compile_options(/wd4065)


file(GLOB_RECURSE SOURCE_FILES *.cpp *.c)
file(GLOB_RECURSE HEADER_FILES *.hpp *.h *.ini)
set(CppFile ${SOURCE_FILES} ${HEADER_FILES} ${FLEX_OUPUT} ${BISON_OUTPUT})
set(AllFile ${CppFile} ${L_SOURCES} ${Y_SOURCES} ${FLEX_OUPUT} ${BISON_OUTPUT})


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

add_library(${PROJECT_NAME} STATIC ${AllFile})
set_property(SOURCE ${SHADER_FILES} PROPERTY VS_TOOL_OVERRIDE "yacc")	# 设置 yacc 资源文件

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Function")

target_include_directories(${PROJECT_NAME} PUBLIC 
	${PROJECT_FUNCTION_DIR}/ShaderParse/ParseCode
)


