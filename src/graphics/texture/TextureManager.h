#pragma once
#include <SDL3/SDL.h>
#include <unordered_map>
#include <string>

class TextureManager {
public:

    TextureManager(SDL_Renderer* renderer);
    ~TextureManager();


    SDL_Texture* createTextureFromRect(int x, int y, SDL_FRect& rect, SDL_Color& coler);

    void cleanupAllTexture();

    SDL_Texture* getTexture(int x, int y);

    bool destoryTexture(int x, int y);

private:

    SDL_Renderer* m_renderer = nullptr;

    std::unordered_map<size_t, SDL_Texture*> m_cacheTexture;

    size_t makeHash(int x, int y);

};