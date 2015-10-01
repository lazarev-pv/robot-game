cmake_minimum_required (VERSION 2.8)
project (Core)
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CmakeModules/")

ADD_DEFINITIONS(
   -std=c++0x
    -g
	-DMY_LINUX=1
	-Wall
	-pedantic
	-Wextra
	-Wno-reorder
)

set(USER_ROOT /usr)

FILE(GLOB_RECURSE SOURCES src *.cpp)

# work around for clion bug
FILE (GLOB_RECURSE clion_all_headers include/*.h)
ADD_CUSTOM_TARGET(all_clion
    SOURCES ${clion_all_headers}
)

add_library(chibiEngine STATIC  ${SOURCES})

find_package(PNG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Lua REQUIRED)

find_library(GLUT_LIBRARY
    NAMES glut GLUT freeglut freeglut_static
	HINTS ${USER_ROOT}/lib 
)

find_library(Freetype_LIBRARY
	NAMES Freetype freetype Freetype_static
	HINTS /usr/lib/x86_64-linux-gnu ${USER_ROOT}/lib
)

find_library(Box2D_LIBRARY
    NAMES Box2D Box2D_static
    HINTS ${USER_ROOT}/lib ${CMAKE_INSTALL_PREFIX}/lib
)

include_directories( 
	 src
	 include
	${USER_ROOT}/include/freetype2
	${USER_ROOT}/include
	${USER_ROOT}/local/include
    ${LUA_INCLUDE_DIR}
	${OPENGL_INCLUDE_DIRS}  
	${Freetype_INCLUDE_DIRS} 
	${PNG_INCLUDE_DIRS}
	${Box2D_INCLUDE_DIRS}	
)

