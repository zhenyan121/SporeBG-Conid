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