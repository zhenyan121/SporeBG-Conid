#pragma once
#include <SDL3/SDL.h>
#include <unordered_map>
#include <string>

class TextureManager {
public:

    TextureManager(SDL_Renderer* renderer);
    ~TextureManager();


    SDL_Texture* createTextureFromRect(SDL_FRect& rect, SDL_Color& coler);

    void cleanupAllTexture();

    SDL_Texture* getTexture(SDL_FRect& rect, SDL_Color& color);

    bool destoryTexture(SDL_FRect& rect, SDL_Color& color);

private:

    SDL_Renderer* m_renderer = nullptr;

    std::unordered_map<size_t, SDL_Texture*> m_cacheTexture;

    size_t makeHash(SDL_FRect& rect, SDL_Color& color);

};