cmake_minimum_required(VERSION 3.20)
project(SporeBG-Conid LANGUAGES C CXX)

# C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# ========== 引入 SDL 构建脚本 ==========
include(${CMAKE_SOURCE_DIR}/cmake/SDL.cmake)

# ========== 源码 ==========
#file(GLOB_RECURSE SOURCE_FILES
#    src/*.cpp
#    src/*.h
#)

set(SOURCE_FILES
    src/main.cpp
    src/core/GameApplication.cpp
    src/core/WindowManager.cpp
    src/game/GameSession.cpp
    src/game/Board.cpp
    src/game/Piece.cpp
    src/game/Rule.cpp
    src/game/ComponentManager.cpp
    src/input/InputManager.cpp
    src/scenes/base/SceneManager.cpp
    src/scenes/gameplay/GameScene.cpp
    src/scenes/menu/MainMenuScene.cpp
    src/graphics/game/BoardRenderer.cpp
    src/graphics/game/CoordinateConverter.cpp
    src/utils/CoordinateTools.cpp
    src/graphics/font/FontManager.cpp
    src/graphics/font/TextRenderer.cpp
    src/ui/components/Button.cpp
    src/ui/components/Label.cpp
    src/ui/managers/GameUIManager.cpp
    src/ui/managers/MainMenuUIManager.cpp
    src/graphics/ui/UIRenderer.cpp
    src/graphics/font/BitmapFont.cpp
    src/utils/ConfigLoader.cpp
    src/core/DebugManager.cpp
    src/ui/managers/debug/DebugOverlay.cpp
    src/core/Time.cpp
)


add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        SDL3::SDL3
        SDL3_ttf::SDL3_ttf
        SDL3_image::SDL3_image
        nlohmann_json::nlohmann_json
)

# ========== Windows: 复制 DLL ==========
if (WIN32)
# 查找 MinGW 运行时库
    if(MINGW)
        # 查找 libgcc DLL
        execute_process(
            COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=libgcc_s_seh-1.dll
            OUTPUT_VARIABLE GCC_DLL_PATH
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        
        # 查找 libstdc++ DLL
        execute_process(
            COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=libstdc++-6.dll
            OUTPUT_VARIABLE STDCPP_DLL_PATH
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        
        # 复制编译器运行时库
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${GCC_DLL_PATH}
                $<TARGET_FILE_DIR:${PROJECT_NAME}>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${STDCPP_DLL_PATH}
                $<TARGET_FILE_DIR:${PROJECT_NAME}>
             COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:SDL3_image::SDL3_image>
                $<TARGET_FILE_DIR:${PROJECT_NAME}>
            COMMENT "Copying MinGW runtime DLLs"
        )
    endif()

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL3::SDL3>
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL3_ttf::SDL3_ttf>
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMENT "Copying SDL3 runtime DLLs"
    )




# ========== 仅复制修改过的 assets ==========
set(ASSETS_SRC_DIR "${CMAKE_SOURCE_DIR}/assets")
set(ASSETS_DST_DIR "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets")

# 收集所有 asset 文件
file(GLOB_RECURSE ASSET_FILES
    "${ASSETS_SRC_DIR}/*"
)

# 为每个文件生成复制规则
foreach(ASSET_FILE ${ASSET_FILES})
    # 计算目标路径
    file(RELATIVE_PATH REL_PATH "${ASSETS_SRC_DIR}" "${ASSET_FILE}")
    set(OUT_FILE "${ASSETS_DST_DIR}/${REL_PATH}")

    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${ASSET_FILE}" "${OUT_FILE}"
        COMMENT "Copying changed asset: ${REL_PATH}"
    )
endforeach()
endif()
