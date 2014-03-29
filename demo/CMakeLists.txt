#-------------------------------------------------------------------------------
# anyfx demo
#-------------------------------------------------------------------------------

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR}
					${CMAKE_SOURCE_DIR}/util/code
					${CMAKE_SOURCE_DIR}/api/code		
					${CMAKE_SOURCE_DIR}/exts/glew
					code)
					

FILE(GLOB SRC "code/*.cc")
FILE(GLOB HEAD "code/*.h")
SOURCE_GROUP("demo" FILES ${SRC} ${HEAD})
LIST(APPEND CODE ${SRC} ${HEAD})

FILE(GLOB SRC "../exts/glew/*.c")
FILE(GLOB HEAD "../exts/glew/GL/*.h")
SOURCE_GROUP("demo\\glew" FILES ${SRC} ${HEAD})
ADD_DEFINITIONS(-DGLEW_STATIC)
LIST(APPEND CODE ${SRC} ${HEAD})

FILE(GLOB_RECURSE HEAD "code/glm/*.hpp")
SOURCE_GROUP("demo\\glm" FILES ${SRC} ${HEAD})
LIST(APPEND CODE ${SRC} ${HEAD})

FILE(GLOB_RECURSE HEAD "code/IL/*.h")
SOURCE_GROUP("demo\\IL" FILES ${SRC} ${HEAD})
LIST(APPEND CODE ${SRC} ${HEAD})


ADD_EXECUTABLE(demo ${CODE})
SET_DEFAULT_OUTPUT(demo)

IF(WIN32)
	SET(DEVIL_LIBS ${CMAKE_SOURCE_DIR}/exts/devil/DevIL.lib
					${CMAKE_SOURCE_DIR}/exts/devil/ILU.lib
					${CMAKE_SOURCE_DIR}/exts/devil/ILUT.lib)
ELSEIF(UNIX)
	SET(DEVIL_LIBS IL ILU)
ENDIF()

IF (MSVC11)
	ADD_DEFINITIONS(-D_XKEYCHECK_H)
ENDIF()

ADD_DEPENDENCIES(demo
				util
				api)

TARGET_LINK_LIBRARIES(demo
					util
					api
					${OPENGL_LIBS}
					${DEVIL_LIBS})
					

