#pragma once

#include "font_type.hpp"
#include <gfx/font.hpp>
#include <memory>
#include <unordered_map>

class ServiceProvider;

class ResourceManager final {
private:
    // we store unique pointers to avoid trouble with dangling references outside of this class
    std::unordered_map<FontType, std::unique_ptr<gfx::Font>> m_fonts;

public:
    [[nodiscard]] explicit ResourceManager(ServiceProvider const& service_provider);

    [[nodiscard]] gfx::Font const& font(FontType type) const;
};
