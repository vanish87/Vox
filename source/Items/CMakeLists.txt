set(ITEMS_SRCS
   "${CMAKE_CURRENT_SOURCE_DIR}/Item.h"
   "${CMAKE_CURRENT_SOURCE_DIR}/Item.cpp"
   "${CMAKE_CURRENT_SOURCE_DIR}/ItemManager.h"
   "${CMAKE_CURRENT_SOURCE_DIR}/ItemManager.cpp"
   "${CMAKE_CURRENT_SOURCE_DIR}/StatAttribute.h"
   "${CMAKE_CURRENT_SOURCE_DIR}/StatAttribute.cpp"
   "${CMAKE_CURRENT_SOURCE_DIR}/ItemUtils.cpp"
   PARENT_SCOPE)

source_group("Items" FILES ${ITEMS_SRCS})