#pragma once

#include "game_object.hpp"
#include <lib2k/types.hpp>

struct GameSettings {
    usize num_planets{ 20 };
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GameSettings, num_planets);

class Galaxy final {
private:
    std::vector<GameObject> m_game_objects;
    GameSettings m_game_settings;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Galaxy, m_game_objects, m_game_settings);

public:
    [[nodiscard]] std::vector<GameObject> const& game_objects() const {
        return m_game_objects;
    }

    [[nodiscard]] std::vector<GameObject>& game_objects() {
        return m_game_objects;
    }

    [[nodiscard]] GameSettings const& game_settings() const {
        return m_game_settings;
    }
};
