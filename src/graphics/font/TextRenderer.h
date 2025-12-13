/**
 * @file    TextRenderer.h
 * @brief   文本渲染器类，用于将带有特定样式的文本渲染到屏幕上
 * @details 该类使用 SDL3 TTF 库进行文字渲染，并实现了纹理缓存机制以提高性能。
 *          支持获取文本尺寸和渲染文本到指定位置。
 * @author  zhenyan121
 * @date    2025-12-12
 */

#pragma once
#include <string>
#include <unordered_map>
#include <list>
#include <ctime>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "Textstyle.h"
#include "FontManager.h"

/**
 * @class   TextRenderer
 * @brief   文本渲染器
 * @details 负责管理文本渲染的生命周期，包括：
 *          - 文本纹理的创建和缓存
 *          - 文本尺寸的计算
 *          - 文本到指定屏幕位置的渲染
 *          
 *          使用哈希缓存机制存储已渲染的文本纹理，避免重复创建相同的纹理。
 */
class TextRenderer {

public:
    /**
     * @brief   构造函数
     * @param   renderer SDL 渲染器指针，用于创建纹理和执行渲染操作。不可为 nullptr
     * @param   fontManager 字体管理器指针，用于获取指定字体 ID 和大小的字体对象。不可为 nullptr
     * @details 初始化文本渲染器，设置必要的 SDL 渲染器和字体管理器引用
     */
    TextRenderer(SDL_Renderer* renderer, FontManager* fontManager);
    
    /**
     * @brief   析构函数
     * @details 自动释放所有缓存中的 SDL 纹理资源，防止内存泄漏
     */
    ~TextRenderer();

    /**
     * @brief   获取文本渲染后的尺寸
     * @param   text 要计算尺寸的文本内容。支持任意 UTF-8 字符串
     * @param   style 文本样式，包含字体 ID、大小、颜色等信息
     * @return  返回一个 pair，first 为宽度（像素），second 为高度（像素）；
     *          如果渲染失败，返回 {0, 0}
     * @details 该函数会检查缓存，如果文本纹理已存在则直接返回其尺寸；
     *          否则创建新的纹理并缓存，然后返回尺寸。
     *          这样可以避免重复渲染相同的文本。
     * @note    返回的尺寸是矩形的宽和高
     */
    std::pair<int, int> getTextSize(const std::string& text, TextStyle style);
    
    /**
     * @brief   将文本渲染到指定位置
     * @param   text 要渲染的文本内容。支持任意 UTF-8 字符串
     * @param   style 文本样式，包含字体 ID、大小、颜色等信息
     * @param   x 文本左上角的 X 坐标（像素）
     * @param   y 文本左上角的 Y 坐标（像素）
     * @details 该函数会检查缓存，如果文本纹理已存在则直接使用；
     *          否则创建新的纹理、缓存并渲染。
     *          使用浮点数坐标进行渲染，支持亚像素精度。
     * @note    (x, y) 为文本左上角的顶点坐标
     */
    void renderText(const std::string& text, TextStyle style, int x, int y);

    /**
     * @brief   为文本和样式生成哈希值
     * @param   text 文本内容
     * @param   style 文本样式
     * @return  返回组合哈希值，用于缓存键的生成
     * @details 利用文本内容、字体 ID、字体大小和颜色信息生成唯一的哈希值，
     *          用于在缓存中快速查找已渲染的文本纹理
     * @see     m_cache
     */
    size_t makeHash(const std::string& text, TextStyle style) const;

    /**
     * @brief   手动清理所有缓存
     * @details 释放所有缓存中的纹理资源，完全清空缓存表
     * @note    清理后任何文本都需要重新渲染
     */
    void clearCache();

private:
    /** @brief SDL 渲染器指针，用于创建纹理和执行 2D 渲染操作 */
    SDL_Renderer* m_renderer;
    
    /** @brief 字体管理器指针，用于管理和获取字体资源 */
    FontManager* m_fontManager;

    /**
     * @struct  CachedText
     * @brief   缓存的文本纹理信息
     * @details 存储已渲染文本的纹理及其尺寸信息，用于快速重用
     */
    struct CachedText {
        /** @brief 文本渲染后生成的 SDL 纹理指针 */
        SDL_Texture* texture;
        
        /** @brief 文本纹理的宽度（像素） */
        int width;
        
        /** @brief 文本纹理的高度（像素） */
        int height;
        
        /** @brief 该缓存项的最后访问时间戳（秒） */
        time_t lastAccessTime;
    };

    /**
     * @brief   文本纹理缓存容器
     * @details 使用无序哈希表存储已渲染的文本纹理。
     *          键是由文本内容、字体信息等生成的哈希值（通过 makeHash 生成）；
     *          值是对应的 CachedText 结构体，包含纹理指针和尺寸信息。
     *          
     *          缓存机制的优势：
     *          - 避免重复渲染相同的文本
     *          - 提高渲染性能
     *          - 加速尺寸查询
     * @see     CachedText, makeHash()
     */
    std::unordered_map<size_t, CachedText> m_cache;

    /** @brief 最大缓存条目数，超过此数量会触发自动清理 */
    static constexpr size_t MAX_CACHE_SIZE = 256;

    /** @brief 缓存清理时保留的最少条目数（清理时会保留最常用的条目） */
    static constexpr size_t MIN_CACHE_SIZE = 128;

    /** @brief 自动清理缓存（当缓存超过最大限制时调用） */
    void autoCleanCache();
    
    /**
     * @brief   创建文本纹理（暂未使用）
     * @param   text 文本内容
     * @param   fontID 字体 ID
     * @param   color 文本颜色
     * @return  返回创建的纹理指针，创建失败时返回 nullptr
     * @deprecated 该函数已被 createAndCacheTexture() 取代
     */
    //SDL_Texture* createTextTexture(const std::string& text, const std::string& fontID, SDL_Color color);
    
    /**
     * @brief   创建并缓存文本纹理
     * @param   text 文本内容
     * @param   style 文本样式
     * @return  返回 CachedText 结构体，包含创建的纹理和尺寸信息；
     *          创建失败时返回空结构体（texture 为 nullptr）
     * @details 该函数执行以下步骤：
     *          1. 从字体管理器获取指定字体
     *          2. 使用 TTF 库将文本渲染为表面（Surface）
     *          3. 将表面转换为 SDL 纹理
     *          4. 立即释放表面以节省内存
     *          5. 将纹理和尺寸信息存储到缓存中
     *          6. 返回结果
     * @see     CachedText, m_cache
     */
    CachedText createAndCacheTexture(const std::string& text, TextStyle style);

};
