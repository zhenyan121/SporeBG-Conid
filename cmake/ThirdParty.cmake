include(FetchContent)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.12.0   
)

FetchContent_MakeAvailable(nlohmann_json)

FetchContent_Declare(
    asio
    GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
    GIT_TAG asio-1-36-0 # 指定一个稳定版本标签
)
FetchContent_MakeAvailable(asio)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.3  # 使用稳定版本
    # 或者使用最新的稳定版本标签，例如：
    # GIT_TAG 1.0.1  # 请查看glm的发布页面获取最新稳定版本
)
FetchContent_MakeAvailable(glm)