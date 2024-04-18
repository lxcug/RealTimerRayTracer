#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Random.h"
#include "Walnut/Timer.h"


using namespace Walnut;

class ExampleLayer : public Walnut::Layer {
public:
    virtual void OnUIRender() override {
        ImGui::Begin("Settings");

        ImGui::Text("Last render elapsed: %.3fms", m_last_render_time);

        if (ImGui::Button("Render")) {
            Render();
        }
        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0., 0.});
        ImGui::Begin("Viewport");
        m_viewport_width = static_cast<uint32_t>(ImGui::GetContentRegionAvail().x);
        m_viewport_height = static_cast<uint32_t>(ImGui::GetContentRegionAvail().y);

        if (m_image) {
            ImGui::Image(m_image->GetDescriptorSet(), {(float) m_image->GetWidth(), (float) m_image->GetHeight()});
        }

        ImGui::End();
        ImGui::PopStyleVar();

        Render();
    }

    void Render() {
        Timer timer;

        if (!m_image || m_viewport_width != m_image->GetWidth() || m_viewport_height != m_image->GetHeight()) {
            m_image = std::make_shared<Image>(m_viewport_width, m_viewport_height, ImageFormat::RGBA);
            delete[] m_image_data;
            m_image_data = new uint32_t[m_viewport_width * m_viewport_height];
        }

        for (uint32_t i = 0; i < m_viewport_width * m_viewport_height; i++) {
            m_image_data[i] = Random::UInt();
            m_image_data[i] |= 0xff000000;
        }

        m_image->SetData(m_image_data);

        m_last_render_time = timer.ElapsedMillis();
    };

private:
    std::shared_ptr<Image> m_image;
    uint32_t m_viewport_width, m_viewport_height;
    uint32_t *m_image_data = nullptr;
    float m_last_render_time;
};

Walnut::Application *Walnut::CreateApplication(int argc, char **argv) {
    Walnut::ApplicationSpecification spec;
    spec.Name = "Real-Time Ray Tracing";

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