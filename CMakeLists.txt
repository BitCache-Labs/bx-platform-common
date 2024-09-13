add_library(bx-platform-common STATIC 
    "src/bx/platform/file.cpp"
    "src/bx/platform/thread.cpp"
)
target_include_directories (bx-platform-common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)