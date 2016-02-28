#-------------------------------------------------------------------------------
# anyfx api
#-------------------------------------------------------------------------------

INCLUDE_DIRECTORIES(${ANYFX_SOURCE_DIR}
					${ANYFX_GLEW_FOLDER}/include
					${ANYFX_SOURCE_DIR}/exts/eastl/include
					${ANYFX_SOURCE_DIR}/api
					${ANYFX_SOURCE_DIR}/api/highlevel)
					

FILE(GLOB SRC "*.cc")
FILE(GLOB HEAD "*.h")
SOURCE_GROUP("highlevel" FILES ${SRC} ${HEAD})
LIST(APPEND CODE ${SRC} ${HEAD})

FILE(GLOB SRC "../*.cc")
FILE(GLOB HEAD "../*.h")
SOURCE_GROUP("highlevel\\common" FILES ${SRC} ${HEAD})
LIST(APPEND CODE ${SRC} ${HEAD})

FILE(GLOB SRC "internal/*.cc")
FILE(GLOB HEAD "internal/*.h")
SOURCE_GROUP("highlevel\\internal" FILES ${SRC} ${HEAD})
LIST(APPEND CODE ${SRC} ${HEAD})

FILE(GLOB SRC "internal/glsl4/*.cc")
FILE(GLOB HEAD "internal/glsl4/*.h")
SOURCE_GROUP("highlevel\\internal\\glsl4" FILES ${SRC} ${HEAD})
LIST(APPEND CODE ${SRC} ${HEAD})

FILE(GLOB SRC "internal/vk/*.cc")
FILE(GLOB HEAD "internal/vk/*.h")
SOURCE_GROUP("highlevel\\internal\\vk" FILES ${SRC} ${HEAD})
LIST(APPEND CODE ${SRC} ${HEAD})

FILE(GLOB SRC "loaders/*.cc")
FILE(GLOB HEAD "loaders/*.h")
SOURCE_GROUP("highlevel\\loaders" FILES ${SRC} ${HEAD})
LIST(APPEND CODE ${SRC} ${HEAD})

ADD_DEFINITIONS(-DGLEW_STATIC)

IF(ANYFX_TRANSPOSE_MATRICES)
	ADD_DEFINITIONS(-D__ANYFX_TRANSPOSE_MATRIX__)
ENDIF()

IF (MSVC11)
	ADD_DEFINITIONS(-D_XKEYCHECK_H)
ENDIF()

ADD_LIBRARY(highlevel-api STATIC ${CODE})
ADD_DEPENDENCIES(highlevel-api api)
TARGET_LINK_LIBRARIES(highlevel-api glew api)
