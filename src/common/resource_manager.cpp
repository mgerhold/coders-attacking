#include <array>
#include <common/resource_manager.hpp>
#include <common/service_provider.hpp>
#include <gfx/window.hpp>

[[nodiscard]] ResourceManager::ResourceManager(ServiceProvider const& service_provider) {
    static constexpr auto fonts_to_load = std::array{
        std::tuple{ FontType::Roboto, "assets/fonts/Roboto/Roboto-Regular.ttf", 40 },
    };

    for (auto const& [type, filename, font_size] : fonts_to_load) {
        m_fonts.insert({
                type,
                std::make_unique<gfx::Font>(service_provider.window().load_font(filename, font_size).value()),
        });
    }
}

[[nodiscard]] gfx::Font const& ResourceManager::font(FontType const type) const {
    return *m_fonts.at(type);
}
