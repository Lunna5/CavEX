file(GLOB SRC_FILES source/*.c)
add_library(cNBT ${SRC_FILES})
target_include_directories(cNBT
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
set_target_properties(log PROPERTIES FOLDER "Libraries")