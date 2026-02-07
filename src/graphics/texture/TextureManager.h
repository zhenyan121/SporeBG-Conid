#pragma once
#include <SDL3/SDL.h>
#include <unordered_map>
#include <string>

class TextureManager {
public:

    TextureManager(SDL_Renderer* renderer);
    ~TextureManager();


    SDL_Texture* createTextureFromRect(const SDL_FRect& rect, const SDL_Color& coler);

    void cleanupAllTexture();

    SDL_Texture* getTexture(const SDL_FRect& rect, const SDL_Color& color);

    bool destoryTexture(const SDL_FRect& rect, const SDL_Color& color);

private:

    SDL_Renderer* m_renderer = nullptr;

    std::unordered_map<size_t, SDL_Texture*> m_cacheTexture;

    size_t makeHash(const SDL_FRect& rect, const SDL_Color& color);

};