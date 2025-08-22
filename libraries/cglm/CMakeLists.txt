add_library(cglm INTERFACE)
target_include_directories(cglm
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
set_target_properties(log PROPERTIES FOLDER "Libraries")