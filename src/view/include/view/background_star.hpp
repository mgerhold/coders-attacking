#pragma once
#include "camera.hpp"
#include <gfx/renderer.hpp>
#include <numbers>
#include <utils/color.hpp>
#include <utils/vec2.hpp>

namespace view {
    class BackgroundStar final {
        utils::Vec2f m_position;
        float m_size;
        float m_base_brightness;
        float m_period;
        float m_amplitude;
        float m_distance;
        utils::Color m_color;

        BackgroundStar(
                utils::Vec2f const position,
                float const size,
                float const base_brightness,
                float const period,
                float const amplitude,
                float const distance,
                utils::Color const color
        )
            : m_position{ position },
              m_size{ size },
              m_base_brightness{ base_brightness },
              m_period{ period },
              m_amplitude{ amplitude },
              m_distance{ distance },
              m_color{ color } { }

    public:
        // clang-format off
        void render(
            gfx::Renderer& renderer,
            Camera const& camera,
            float const elapsed_time
        ) const { // clang-format on
            using namespace utils;
            static constexpr auto pi = std::numbers::pi_v<float>;
            auto const brightness = std::clamp(
                    std::sin(elapsed_time * 2.0f * pi / m_period) * m_amplitude + m_base_brightness,
                    0.0f,
                    1.0f
            );
            auto const color = Color{
                static_cast<u8>(static_cast<float>(m_color.r) * brightness),
                static_cast<u8>(static_cast<float>(m_color.g) * brightness),
                static_cast<u8>(static_cast<float>(m_color.b) * brightness),
            };
            auto const screen_coords = camera.world_to_screen_coords(m_position, m_distance);
            if (not camera.viewport().contains(screen_coords)) {
                return;
            }
            renderer.draw_circle(screen_coords, m_size, color);
        }

        [[nodiscard]] static BackgroundStar random(c2k::Random& random) {
            auto const position = utils::Vec2f{
                (random.next_float() - 0.5f) * 1.2f,
                (random.next_float() - 0.5f) * 1.2f,
            };
            auto const size = random.next_float() * 2.0f;
            auto const brightness = random.next_float() * 0.5f + 0.1f;
            auto const period = random.next_float() * 10.0f + 5.0f;
            auto const amplitude = random.next_float() * 0.4f;
            auto const distance = random.next_float() * 10.0f + 0.1f;
            auto const planet_type = random.next_integral<int>(3);
            auto const color = std::invoke([&] {
                switch (planet_type) {
                    case 0:
                        // yellow
                        return utils::Color{
                            static_cast<u8>(random.next_integral(240, 256)),
                            static_cast<u8>(random.next_integral(240, 256)),
                            static_cast<u8>(random.next_integral(220, 240)),
                        };
                    case 1:
                        // blue
                        return utils::Color{
                            static_cast<u8>(random.next_integral(220, 240)),
                            static_cast<u8>(random.next_integral(220, 240)),
                            static_cast<u8>(random.next_integral(240, 256)),
                        };
                    case 2:
                        // white
                        return utils::Color{
                            static_cast<u8>(random.next_integral(240, 256)),
                            static_cast<u8>(random.next_integral(240, 256)),
                            static_cast<u8>(random.next_integral(240, 256)),
                        };
                }
                assert(false and "unreachable");
                return utils::Color{ utils::Color::Red };
            });
            return BackgroundStar{ position, size, brightness, period, amplitude, distance, color };
        }
    };
} // namespace view
