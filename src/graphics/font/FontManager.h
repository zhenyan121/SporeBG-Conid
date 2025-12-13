/**
 * @file    FontManager.h
 * @brief   字体加载和管理的实现，使用SDL的ttf库扩展
 * @author  zhenyan121
 * @date    2025-12-12
 */
#pragma once
#include <string>
#include <unordered_map>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

/**
 * @class   FontManager
 * @brief   字体管理的类
 * 
 * 用于加载字体，缓存字体
 * 提供接口，用与获取字体缓存
 */
class FontManager {
public:
    FontManager();

    /**
     * @brief   析构函数
     * 清理已经加载的字体资源
     */
    ~FontManager();

    /**
     * @brief   加载字体
     * 从字体文件中加载字体并将其缓存。不同大小的字体将被独立存储。
     * @param   fontID 字体文件名称的引用，不包含路径前缀
     * @param   ptSize 字体的磅值大小，不同的大小会产生不同的缓存键
     * @return  返回加载字体的指针，失败时抛出std::runtime_error异常
     * @note    该函数会自动检查字体是否已缓存，如已缓存则直接返回缓存的字体
     * @throw   std::runtime_error 当字体文件无法打开时抛出异常
     */
    TTF_Font* loadFont(const std::string& fontID, int ptSize);
    /**
     * @brief   获取字体
     * 获取指定字体的指针。如果字体未被加载过，将自动调用loadFont进行加载。
     * @param   fontID 字体文件名称的引用，不包含路径前缀
     * @param   ptSize 字体的磅值大小
     * @return  返回获取到的字体指针，保证非空；失败时会抛出异常
     * @note    该函数是获取字体的推荐方式，具有自动加载功能
     * @throw   std::runtime_error 当字体加载失败时抛出异常
     */
    TTF_Font* getFont(const std::string& fontID, int ptSize);

    

private:
    
    std::unordered_map<size_t, TTF_Font*> m_fonts; ///< 字体缓存表，使用哈希值作为键存储TTF_Font指针

    /**
     * @brief   计算组合哈希值
     * 根据字体名称和大小计算唯一的哈希键用于缓存。采用黄金比例乘法混合算法。
     * @param   fontID 字体文件名称的引用
     * @param   ptSize 字体的磅值大小
     * @return  返回计算得到的哈希值，作为m_fonts字典的键
     * @note    内部实现使用XOR运算和位移组合两个独立的哈希值
     */
    size_t makeHash(const std::string& fontID, int ptSize);
    
};