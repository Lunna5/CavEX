add_library(parson STATIC parson.c parson.h)
target_include_directories(parson PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(parson PROPERTIES FOLDER "Libraries")