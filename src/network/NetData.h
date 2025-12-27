#pragma once
#include <utility>
struct NetData {
    std::pair<int, int> clickPosition = {0, 0};
    // 序列化，转换成字节数组
    void serialize(char* buffer) const {
        // 暴力转换
        int* ptr = reinterpret_cast<int*>(buffer);
        ptr[0] = clickPosition.first;
        ptr[1] = clickPosition.second;
    }
    // 反序列化，从字节数组恢复
    static NetData deserialize(const char* buffer) {
        // 暴力转换
        const int* ptr = reinterpret_cast<const int*>(buffer);
        return NetData{{ptr[0], ptr[1]}};
    }
    // 大小（固定16字节：4个int）
    static constexpr size_t size() { return 4 * sizeof(int); }
};

enum class NetType {
    HOST,
    CLIENT

};