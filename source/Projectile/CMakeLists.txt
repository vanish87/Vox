set(PROJECTILE_SRCS
   "${CMAKE_CURRENT_SOURCE_DIR}/Projectile.h"
   "${CMAKE_CURRENT_SOURCE_DIR}/Projectile.cpp"
   "${CMAKE_CURRENT_SOURCE_DIR}/ProjectileManager.h"
   "${CMAKE_CURRENT_SOURCE_DIR}/ProjectileManager.cpp"
   PARENT_SCOPE)

source_group("Projectile" FILES ${PROJECTILE_SRCS})