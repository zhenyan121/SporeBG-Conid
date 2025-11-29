cmake_minimum_required(VERSION 3.16)
project(SporeBG-Conid)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# 包含SDL3子项目
add_subdirectory(third_party/SDL3)

set(SOURCE_FILES
    src/main.cpp
    src/core/Game.cpp
    src/ui/Render.cpp
    src/core/Board.cpp
    src/core/Piece.cpp
    src/core/Rule.cpp
    src/core/ComponentManager.cpp
)

# 添加可执行文件
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(SporeBG-Conid PRIVATE ${CMAKE_SOURCE_DIR}/src)
# 使用正确的目标名称链接SDL3
# 方法1：使用别名（推荐）
target_link_libraries(SporeBG-Conid PRIVATE SDL3::SDL3)

# Windows下复制动态库 - 使用正确的目标名称
if(WIN32)
    add_custom_command(TARGET SporeBG-Conid POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL3::SDL3>  
            $<TARGET_FILE_DIR:SporeBG-Conid>
        COMMENT "Copying SDL3 DLL to output directory"
    )
endif()