#pragma once

#include "widget.hpp"
#include <string>

namespace ui {
    class Label : public Widget {
    private:
        std::string m_caption;
        utils::Color m_color;

    public:
        Label(utils::FloatRect position, std::string caption, utils::Color color);

        bool handle_event([[maybe_unused]] Event event) override {
            return false;
        }

        void render(gfx::Renderer& renderer) const override;
    };
} // namespace ui
