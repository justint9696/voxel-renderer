#pragma once

#include <unordered_map>

template <typename Key, typename Tp>
class ResourceManager {
public:
    ResourceManager();
    ~ResourceManager();

    template <typename ...Args>
    void create(const Key& key, Args&& ...args) {
        this->map.try_emplace(key, std::forward<Args>(args)...);
    }

    const Tp& get(Key key) {
        return this->map.at(key);
    }

protected:
    std::unordered_map<Key, Tp> map;
};
