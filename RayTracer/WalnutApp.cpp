#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Timer.h"
#include "Core/Renderer.h"
#include "Core/Camera.h"
#include "Core/Scene.h"
#include "glm/gtc/type_ptr.hpp"


using namespace Walnut;

class ExampleLayer : public Walnut::Layer {
public:
    ExampleLayer() : m_camera(60, .01f, 1000.f) {}

    void OnUpdate(float ts) override {
        if (m_camera.onUpdate(ts)) {
            m_renderer.resetFrameIndex();
        }
    }

    void OnUIRender() override {
        ImGui::Begin("Settings");
        ImGui::Text("Last frame elapsed: %.2fms", m_last_render_time);
        ImGui::Text("Accumulated frames: %i", m_renderer.getFrameCount());
        ImGui::InputInt("Samples Per Pixel", &m_renderer.getSpp(), 1, 16);
        if (ImGui::Button("Reset")) {
            m_renderer.resetFrameIndex();
        }
        ImGui::ColorPicker3("Background", glm::value_ptr(m_scene.getBackgroundColor()));
        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0., 0.});
        ImGui::Begin("Viewport");
        m_viewport_width = static_cast<uint32_t>(ImGui::GetContentRegionAvail().x);
        m_viewport_height = static_cast<uint32_t>(ImGui::GetContentRegionAvail().y);

        Walnut::Timer timer;
        m_camera.onResize(m_viewport_width, m_viewport_height);
        m_renderer.onResize(m_viewport_width, m_viewport_height);

        m_renderer.render(m_scene, m_camera);
        m_last_render_time = timer.ElapsedMillis();

        const std::shared_ptr<Walnut::Image> &image = m_renderer.getFinalImage();
        if (m_renderer.getFinalImage()) {
            ImGui::Image(image->GetDescriptorSet(), {(float) image->GetWidth(), (float) image->GetHeight()},
                         ImVec2(0, 1), ImVec2(1, 0));
        }

        ImGui::End();
        ImGui::PopStyleVar();
    }

private:
    Renderer m_renderer;
    uint32_t m_viewport_width, m_viewport_height;
    float m_last_render_time;
    Camera m_camera;
    Scene m_scene;
};


Walnut::Application *Walnut::CreateApplication(int argc, char **argv) {
    Walnut::ApplicationSpecification spec;
    spec.Name = "Real-Time Ray Tracer";

    Walnut::Application *app = new Walnut::Application(spec);
    app->PushLayer<ExampleLayer>();
    app->SetMenubarCallback([app]() {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Exit")) {
                app->Close();
            }
            ImGui::EndMenu();
        }
    });
    return app;
}
