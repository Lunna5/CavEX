add_library(log STATIC log.c log.h)
target_include_directories(log PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(log PROPERTIES FOLDER "Libraries")