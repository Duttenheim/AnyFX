#-------------------------------------------------------------------------------
# anyfx api
#-------------------------------------------------------------------------------

INCLUDE_DIRECTORIES(${ANYFX_SOURCE_DIR}
					${ANYFX_SOURCE_DIR}/util/code
					${ANYFX_GLEW_FOLDER}/include
					${ANYFX_SOURCE_DIR}/exts/eastl/include
					${ANYFX_SOURCE_DIR}/api
					${ANYFX_SOURCE_DIR}/api/lowlevel)
					

FILE(GLOB SRC "*.cc")
FILE(GLOB HEAD "*.h")
SOURCE_GROUP("lowlevel" FILES ${SRC} ${HEAD})
LIST(APPEND CODE ${SRC} ${HEAD})

FILE(GLOB SRC "../*.cc")
FILE(GLOB HEAD "../*.h")
SOURCE_GROUP("lowlevel\\common" FILES ${SRC} ${HEAD})
LIST(APPEND CODE ${SRC} ${HEAD})

FILE(GLOB SRC "base/*.cc")
FILE(GLOB HEAD "base/*.h")
SOURCE_GROUP("lowlevel\\base" FILES ${SRC} ${HEAD})
LIST(APPEND CODE ${SRC} ${HEAD})

FILE(GLOB SRC "gl4/*.cc")
FILE(GLOB HEAD "gl4/*.h")
SOURCE_GROUP("lowlevel\\gl4" FILES ${SRC} ${HEAD})
LIST(APPEND CODE ${SRC} ${HEAD})

FILE(GLOB SRC "vk/*.cc")
FILE(GLOB HEAD "vk/*.h")
SOURCE_GROUP("lowlevel\\vk" FILES ${SRC} ${HEAD})
LIST(APPEND CODE ${SRC} ${HEAD})

IF (ANYFX_HAS_METAL)
FILE(GLOB SRC "mtl/*.cc")
FILE(GLOB HEAD "mtl/*.h")
SOURCE_GROUP("lowlevel\\mtl" FILES ${SRC} ${HEAD})
LIST(APPEND CODE ${SRC} ${HEAD})
ENDIF()

IF (ANYFX_HAS_DX12)
FILE(GLOB SRC "dx12/*.cc")
FILE(GLOB HEAD "dx12/*.h")
SOURCE_GROUP("lowlevel\\dx12" FILES ${SRC} ${HEAD})
LIST(APPEND CODE ${SRC} ${HEAD})
ENDIF()

FILE(GLOB SRC "loaders/*.cc")
FILE(GLOB HEAD "loaders/*.h")
SOURCE_GROUP("lowlevel\\loaders" FILES ${SRC} ${HEAD})
LIST(APPEND CODE ${SRC} ${HEAD})

IF (MSVC11)
	ADD_DEFINITIONS(-D_XKEYCHECK_H)
ENDIF()

ADD_LIBRARY(lowlevel-api STATIC ${CODE})
ADD_DEPENDENCIES(lowlevel-api api)
