cmake_minimum_required (VERSION 2.8)
project (Robot-Game)

ADD_DEFINITIONS(
    -std=c++0x
    -g
    -Wall
    -pedantic
    -Wno-reorder
)

FILE(GLOB_RECURSE SOURCES src *.cpp)

add_executable (robot ${SOURCES}) 

find_package(OpenGL REQUIRED) 
find_package(GLUT REQUIRED) 
find_package(PNG REQUIRED)

find_library(SFML_LIBRARY 
    NAMES sfml-audio
    HINTS /usr/local/lib 
)

find_library(Freetype_LIBRARY
    NAMES Freetype freetype Freetype_static
    HINTS /usr/lib/x86_64-linux-gnu /usr/lib
)

find_library(Box2D_LIBRARY
    NAMES Box2D Box2D_static
    HINTS /usr/lib 
)

find_library(DL_LIBRARY
    NAMES dl
    HINTS /lib/x86_64-linux-gnu
)

include_directories( 
    ../Core/include
	include
	/usr/include/freetype2
    ${LUA_INCLUDE_DIR}
	${OPENGL_INCLUDE_DIRS}  
	${GLUT_INCLUDE_DIRS} 
	${Freetype_INCLUDE_DIRS} 
	${PNG_INCLUDE_DIRS} 
)
target_link_libraries(robot
    chibiEngine
    ${SFML_LIBRARY}
	${OPENGL_LIBRARIES} 
	${GLUT_LIBRARY}
	${Freetype_LIBRARY} 
	${PNG_LIBRARY}  
	${Box2D_LIBRARY}
	${LUA_LIBRARY}
    ${DL_LIBRARY}
)

# work around for clion bug
FILE (GLOB_RECURSE clion_all_headers4 src/*.h)
FILE (GLOB_RECURSE clion_all_headers_4 include/*.h)
ADD_CUSTOM_TARGET(all_clion4
    SOURCES ${clion_all_headers4} ${clion_all_headers_4}
)
