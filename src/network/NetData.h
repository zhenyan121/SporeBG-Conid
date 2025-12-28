#pragma once
#include <utility>

enum class NetDataType {
    CLICK_POSITION,
    GAME_START
};

struct NetData {
    std::pair<int, int> clickPosition = {0, 0};
    int firstPlayer = 1;
    NetDataType type = NetDataType::CLICK_POSITION;
    // 序列化，转换成字节数组
    void serialize(char* buffer) const {
        // 暴力转换
        int* ptr = reinterpret_cast<int*>(buffer);

        ptr[0] = static_cast<int>(type);
        ptr[1] = clickPosition.first;
        ptr[2] = clickPosition.second;
        ptr[3] = firstPlayer;
    }
    // 反序列化，从字节数组恢复
    static NetData deserialize(const char* buffer) {
        // 暴力转换
        const int* ptr = reinterpret_cast<const int*>(buffer);
        NetData data;
        data.type = static_cast<NetDataType>(ptr[0]);
        data.clickPosition = {ptr[1], ptr[2]};
        data.firstPlayer = ptr[3];
        return data;
    }
    // 大小（固定16字节：4个int）
    static constexpr size_t size() { return 4 * sizeof(int); }
};

enum class NetType {
    HOST,
    CLIENT

};