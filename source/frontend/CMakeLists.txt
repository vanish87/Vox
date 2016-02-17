set(FRONTEND_SRCS
   "${CMAKE_CURRENT_SOURCE_DIR}/FrontendManager.h"
   "${CMAKE_CURRENT_SOURCE_DIR}/FrontendManager.cpp"
   "${CMAKE_CURRENT_SOURCE_DIR}/FrontendPage.h"
   "${CMAKE_CURRENT_SOURCE_DIR}/FrontendPage.cpp"
   PARENT_SCOPE)

source_group("frontend" FILES ${FRONTEND_SRCS})
