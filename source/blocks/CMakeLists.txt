set(BLOCKS_SRCS
	"${CMAKE_CURRENT_SOURCE_DIR}/ChunkManager.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/ChunkManager.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Chunk.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Chunk.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/BiomeManager.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/BiomeManager.cpp"
	PARENT_SCOPE)

source_group("blocks" FILES ${BLOCKS_SRCS})