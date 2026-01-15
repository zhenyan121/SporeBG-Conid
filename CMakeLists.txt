cmake_minimum_required(VERSION 3.20)
project(SporeBG-Conid LANGUAGES C CXX)

# C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 输出目录 - 修复配置问题
if(CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
endif()

include(${CMAKE_SOURCE_DIR}/cmake/ThirdParty.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/SDL.cmake)

# ========== 源码 ==========
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
    src/scenes/gameplay/OnlineGameScene.cpp
    src/ui/managers/OnlineGameUIManager.cpp
    src/network/client/Client.cpp
    src/network/server/GameServer.cpp
    src/network/NetworkManager.cpp
    src/graphics/texture/TextureManager.cpp
)



add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
    ${asio_SOURCE_DIR}/asio/include
)

# ========== 平台特定设置 ==========
if (WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        ASIO_STANDALONE
        _WIN32_WINNT=0x0A00
    )
    
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
            SDL3::SDL3
            SDL3_ttf::SDL3_ttf
            SDL3_image::SDL3_image
            nlohmann_json::nlohmann_json
            ws2_32
            mswsock
            advapi32
    )
    
    # Windows: DLL 复制逻辑
    if(MINGW)
        # ... (保持 MinGW DLL 复制逻辑)
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
    
else()  # Linux/macOS 等 Unix-like 系统
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        ASIO_STANDALONE
    )
    
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
            SDL3::SDL3
            SDL3_ttf::SDL3_ttf
            SDL3_image::SDL3_image
            nlohmann_json::nlohmann_json
            pthread  # Linux 需要 pthread
            ${CMAKE_DL_LIBS}  # 动态链接库
    )
    
    # 可选：添加额外的 Linux 特定链接库
    find_package(Threads REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
endif()

# ========== 跨平台资产复制 ==========
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
