#include <algorithm>
#include <game/galaxy.hpp>

[[nodiscard]] tl::optional<GameObject const&> Galaxy::find_game_object(uuids::uuid const uuid) const {
    auto const it = std::ranges::find_if(m_game_objects, [&](GameObject const& game_object) {
        return game_object.uuid() == uuid;
    });
    if (it == m_game_objects.end()) {
        return tl::nullopt;
    }
    return *it;
}

[[nodiscard]] tl::optional<GameObject const&> Galaxy::find_game_object(std::string_view const name) const {
    auto const it = std::ranges::find_if(m_game_objects, [&](GameObject const& game_object) {
        return game_object.name() == name;
    });
    if (it == m_game_objects.end()) {
        return tl::nullopt;
    }
    return *it;
}
