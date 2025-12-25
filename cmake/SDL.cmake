include(FetchContent)

# ===========================
# SDL3
# ===========================
FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.2.4
)

FetchContent_MakeAvailable(SDL3)

# ===========================
# SDL3_ttf（使用内置 FreeType + Harfbuzz）
# ===========================
set(SDLTTF_VENDORED ON CACHE BOOL "" FORCE)
set(SDLTTF_HARFBUZZ OFF CACHE BOOL "" FORCE)
set(SDLTTF_PLUTOSVG OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    SDL3_ttf
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
    GIT_TAG release-3.2.2   # ← 正确的版本
)

FetchContent_MakeAvailable(SDL3_ttf)

# ===========================
# SDL3_image
# ===========================
FetchContent_Declare(
    SDL3_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
    GIT_TAG release-3.2.0
)

FetchContent_MakeAvailable(SDL3_image)



