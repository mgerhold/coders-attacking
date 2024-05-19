#pragma once

#include "gfx/window.hpp"
#include "scene.hpp"
#include "scene_manager.hpp"

#include <memory>
#include <vector>

class SceneStack final : public SceneManager {
private:
    std::vector<std::unique_ptr<Scene>> m_scenes;
    bool m_should_delete_current_scene{ false };
    std::unique_ptr<Scene> m_scene_to_enqueue{ nullptr };
    bool m_scene_manager_calls_allowed{ false };
    utils::IntRect m_window_area;

public:
    explicit SceneStack(gfx::Window const& window) : m_window_area{ window.area() } { }

    template<std::derived_from<Scene> T, typename... Args>
    void emplace(Args&&... args) {
        m_scenes.emplace_back(std::make_unique<T>(*this, std::forward<Args>(args)...));
        m_scenes.back()->recalculate_layout(m_window_area);
    }

    void update(float delta_seconds);
    void handle_event(ui::Event const& event);
    void render(gfx::Renderer& renderer) const;
    void recalculate_layout(utils::IntRect area);

    [[nodiscard]] usize size() const {
        return m_scenes.size();
    }

    [[nodiscard]] bool is_empty() const {
        return size() == 0;
    }

    void delete_scene() override;
    void enqueue_scene(std::unique_ptr<Scene> scene) override;

    [[nodiscard]] utils::IntRect viewport() const override {
        return m_window_area;
    }
};
