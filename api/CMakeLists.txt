#-------------------------------------------------------------------------------
# anyfx api
#-------------------------------------------------------------------------------

INCLUDE_DIRECTORIES(${ANYFX_SOURCE_DIR}
					${ANYFX_SOURCE_DIR}/util/code
					${ANYFX_SOURCE_DIR}/exts/eastl/include
					${ANYFX_SOURCE_DIR}/exts/glslang
					${ANYFX_SOURCE_DIR}/api
					${ANYFX_SOURCE_DIR}/api/lowlevel
					${ANYFX_SOURCE_DIR}/api/highlevel
					${VULKAN_INCLUDE_DIR})
										
FILE(GLOB SRC "*.cc")
FILE(GLOB HEAD "*.h")
SOURCE_GROUP("api" FILES ${SRC} ${HEAD})
LIST(APPEND CODE ${SRC} ${HEAD})

FILE(GLOB SRC "lowlevel/*.cc")
FILE(GLOB HEAD "lowlevel/*.h")
SOURCE_GROUP("lowlevel" FILES ${SRC} ${HEAD})
LIST(APPEND CODE ${SRC} ${HEAD})

FILE(GLOB SRC "lowlevel/base/*.cc")
FILE(GLOB HEAD "lowlevel/base/*.h")
SOURCE_GROUP("lowlevel\\base" FILES ${SRC} ${HEAD})
LIST(APPEND CODE ${SRC} ${HEAD})

FILE(GLOB SRC "lowlevel/gl4/*.cc")
FILE(GLOB HEAD "lowlevel/gl4/*.h")
SOURCE_GROUP("lowlevel\\gl4" FILES ${SRC} ${HEAD})
LIST(APPEND CODE ${SRC} ${HEAD})

FILE(GLOB SRC "lowlevel/vk/*.cc")
FILE(GLOB HEAD "lowlevel/vk/*.h")
SOURCE_GROUP("lowlevel\\vk" FILES ${SRC} ${HEAD})
LIST(APPEND CODE ${SRC} ${HEAD})

IF (ANYFX_HAS_METAL)
FILE(GLOB SRC "lowlevel/mtl/*.cc")
FILE(GLOB HEAD "lowlevel/mtl/*.h")
SOURCE_GROUP("lowlevel\\mtl" FILES ${SRC} ${HEAD})
LIST(APPEND CODE ${SRC} ${HEAD})
ENDIF()

IF (ANYFX_HAS_DX12)
FILE(GLOB SRC "lowlevel/dx12/*.cc")
FILE(GLOB HEAD "lowlevel/dx12/*.h")
SOURCE_GROUP("lowlevel\\dx12" FILES ${SRC} ${HEAD})
LIST(APPEND CODE ${SRC} ${HEAD})
ENDIF()

FILE(GLOB SRC "lowlevel/loaders/*.cc")
FILE(GLOB HEAD "lowlevel/loaders/*.h")
SOURCE_GROUP("lowlevel\\loaders" FILES ${SRC} ${HEAD})
LIST(APPEND CODE ${SRC} ${HEAD})

FILE(GLOB SRC "highlevel/*.cc")
FILE(GLOB HEAD "highlevel/*.h")
SOURCE_GROUP("highlevel" FILES ${SRC} ${HEAD})
LIST(APPEND CODE ${SRC} ${HEAD})

FILE(GLOB SRC "highlevel/internal/*.cc")
FILE(GLOB HEAD "highlevel/internal/*.h")
SOURCE_GROUP("highlevel\\internal" FILES ${SRC} ${HEAD})
LIST(APPEND CODE ${SRC} ${HEAD})

FILE(GLOB SRC "highlevel/internal/glsl4/*.cc")
FILE(GLOB HEAD "highlevel/internal/glsl4/*.h")
SOURCE_GROUP("highlevel\\internal\\glsl4" FILES ${SRC} ${HEAD})
LIST(APPEND CODE ${SRC} ${HEAD})

FILE(GLOB SRC "highlevel/internal/vk/*.cc")
FILE(GLOB HEAD "highlevel/internal/vk/*.h")
SOURCE_GROUP("highlevel\\internal\\vk" FILES ${SRC} ${HEAD})
LIST(APPEND CODE ${SRC} ${HEAD})

FILE(GLOB SRC "highlevel/loaders/*.cc")
FILE(GLOB HEAD "highlevel/loaders/*.h")
SOURCE_GROUP("highlevel\\loaders" FILES ${SRC} ${HEAD})
LIST(APPEND CODE ${SRC} ${HEAD})

ADD_DEFINITIONS(-DGLEW_STATIC)

IF(ANYFX_TRANSPOSE_MATRICES)
	ADD_DEFINITIONS(-D__ANYFX_TRANSPOSE_MATRIX__)
ENDIF()

IF (MSVC11)
	ADD_DEFINITIONS(-D_XKEYCHECK_H)
ENDIF()

ADD_LIBRARY(api STATIC ${CODE})
ADD_DEPENDENCIES(api util)
TARGET_LINK_LIBRARIES(api util glew)

TARGET_INCLUDE_DIRECTORIES(api PUBLIC ${ANYFX_SOURCE_DIR}/api/lowlevel ${ANYFX_SOURCE_DIR}/api/highlevel)