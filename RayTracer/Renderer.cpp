#include "Renderer.h"
#include <iostream>
#include <execution>
#include <cstring>
#include "Materials/Material.h"


uint32_t convert2RGB(const glm::vec4 &color) {
    uint8_t r = (uint8_t) (std::sqrt(color.r) * 255.0f);
    uint8_t g = (uint8_t) (std::sqrt(color.g) * 255.0f);
    uint8_t b = (uint8_t) (std::sqrt(color.b) * 255.0f);
    uint8_t a = (uint8_t) (std::sqrt(color.a) * 255.0f);

    uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;
    return result;
}

void Renderer::onResize(uint32_t width, uint32_t height) {
    if (m_final_image) {
        if (m_final_image->GetWidth() == width && m_final_image->GetHeight() == height)
            return;
        m_final_image->Resize(width, height);
    } else {
        m_final_image = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
    }

    delete[] m_image_data;
    m_image_data = new uint32_t[width * height];

    delete[] m_accumulation_data;
    m_accumulation_data = new glm::vec3[width * height];

    m_image_horizontal_iter.resize(width);
    m_image_vertical_iter.resize(height);
    for (uint32_t i = 0; i < width; i++)
        m_image_horizontal_iter[i] = i;
    for (uint32_t i = 0; i < height; i++)
        m_image_vertical_iter[i] = i;
}

Renderer::Renderer() {
//#ifdef MULTI_THREAD
//    m_threads.resize(THREAD_NUM);
//
//    m_block_shading_func = [](Renderer *renderer, int startLine, int endLine) {
//        uint32_t image_width = renderer->getFinalImage()->GetWidth();
//        for (int y = startLine; y < endLine; y++) {
//            for (int x = 0; x < image_width; x++) {
//                uint32_t pixel_index = y * renderer->m_final_image->GetWidth() + x;
//
//                glm::vec3 color{0.f};
//                for (int i = 0; i < renderer->spp; i++) {
//                    Ray ray(renderer->m_active_camera->getPosition(),
//                            renderer->m_active_camera->calSuperSamplingOffset(pixel_index));
//                    color += renderer->traceRay(*renderer->m_active_scene, ray, renderer->m_depth);
//                }
//                color *= (1.f / (float) renderer->spp);
//                renderer->m_accumulation_data[pixel_index] += color;
//
//                glm::vec3 accumulated_color = renderer->m_accumulation_data[pixel_index];
//                accumulated_color /= (float) renderer->m_frame_index;
//                accumulated_color = glm::clamp(accumulated_color, glm::vec3(0.0f),
//                                               glm::vec3(1.0f));
//                renderer->m_image_data[pixel_index] = convert2RGB(glm::vec4(accumulated_color, 1.f));
//            }
//        }
//        renderer->m_render_frame_lock.unlock();
//        ++renderer->m_finished_threads;
//        if (renderer->m_finished_threads == THREAD_NUM) {
//            renderer->m_wait_all_threads_finished.notify_one();
//        }
//    };
//#endif
}

void Renderer::render(const Scene &scene, Camera &camera) {
    m_active_scene = &scene;
    m_active_camera = &camera;

    m_active_camera->setSpp(m_spp);

    float pixel_delta_x = 2.f / (float) m_final_image->GetWidth();
    float pixel_delta_y = 2.f / (float) m_final_image->GetHeight();
    glm::vec3 pixel_delta(pixel_delta_x * .5, pixel_delta_y * .5, 0.f);

    if (m_frame_index == 1) {
        memset(m_accumulation_data, 0,
               m_final_image->GetWidth() * m_final_image->GetHeight() * sizeof(glm::vec3));
    }

//#if MULTI_THREAD
//    m_finished_threads = 0;
//    int thread_lines_count = std::ceil((float)m_final_image->GetHeight() / THREAD_NUM);
//    int last_thread_lines = (int)m_final_image->GetHeight() - thread_lines_count * THREAD_NUM;
//
//    int start, end;
//    for (int i = 0; i < THREAD_NUM-1; i++) {
//        start = i * thread_lines_count;
//        end = (i + 1) * thread_lines_count;
//        m_threads[i] = std::thread(m_block_shading_func, this, start, end);
//        m_threads[i].detach();
//    }
//    start = THREAD_NUM * thread_lines_count;
//    end = THREAD_NUM * thread_lines_count + last_thread_lines;
//    m_threads[THREAD_NUM-1] = std::thread(m_block_shading_func, this, start, end);
//    m_threads[THREAD_NUM-1].detach();
//
//    std::unique_lock<std::mutex> locker(m_render_frame_lock);
//    m_wait_all_threads_finished.wait(locker, [this]() { return this->m_finished_threads == THREAD_NUM; });
//#endif

#if MULTI_THREAD
    std::for_each(std::execution::par_unseq, m_image_vertical_iter.begin(),
                  m_image_vertical_iter.end(),
                  [this](uint32_t y) {
                      std::for_each(std::execution::par_unseq, m_image_horizontal_iter.begin(),
                                    m_image_horizontal_iter.end(),
                                    [this, y](uint32_t x) {
                                        uint32_t pixel_index = y * m_final_image->GetWidth() + x;

                                        glm::vec3 color{0.f};
                                        for (int i = 0; i < m_spp; i++) {
                                            uint32_t cached_ray_index =
                                                    (y * m_final_image->GetWidth() + x) * m_spp + i;
                                            const glm::vec3 &cached_ray_dir =
                                                    m_active_camera->getRayDirections()[cached_ray_index];
                                            Ray ray(m_active_camera->getPosition(), cached_ray_dir);
                                            color += traceRay(*m_active_scene, ray, m_depth);
                                        }
                                        color *= (1.f / (float) m_spp);
                                        m_accumulation_data[pixel_index] += color;

                                        glm::vec3 accumulated_color = m_accumulation_data[pixel_index];
                                        accumulated_color /= (float) m_frame_index;
                                        accumulated_color = glm::clamp(accumulated_color,
                                                                       glm::vec3(0.0f),
                                                                       glm::vec3(1.0f));
                                        m_image_data[pixel_index] = convert2RGB(
                                                glm::vec4(accumulated_color, 1.f));
                                    });
                  });
#else
    for (uint32_t y = 0; y < m_final_image->GetHeight(); y++) {
        for (uint32_t x = 0; x < m_final_image->GetWidth(); x++) {
            uint32_t pixel_index = y * m_final_image->GetWidth() + x;

            const glm::vec3& cached_dir = m_active_camera->getRayDirections()[pixel_index];
            glm::vec3 color{0.f};
            for (int i = 0; i < spp; i++) {
                Ray ray(m_active_camera->getPosition(), cached_dir);
                color += traceRay(*m_active_scene, ray, m_depth);
            }
            color *= (1.f / (float) spp);
            m_accumulation_data[pixel_index] += color;

            glm::vec3 accumulated_color = m_accumulation_data[pixel_index];
            accumulated_color /= (float) m_frame_index;
            accumulated_color = glm::clamp(accumulated_color, glm::vec3(0.0f),
                                           glm::vec3(1.0f));
            m_image_data[pixel_index] = convert2RGB(glm::vec4(accumulated_color, 1.f));
        }
    }
#endif

    m_final_image->SetData(m_image_data);
    ++m_frame_index;
}

glm::vec3 Renderer::traceRay(const Scene &scene, const Ray &ray, int depth) {
    if (depth <= 0) {
        return scene.getBackgroundColor();
    }

    Intersection isect{};
    if (!scene.intersect(ray, isect)) {
        return scene.getBackgroundColor();
    }

    glm::vec3 color_from_emission = isect.mat->emit(isect.u, isect.v, isect.p);

    BxDFSample sample = isect.mat->scatter(ray, isect);
    Ray scattered(isect.p, sample.wi);

    // Note: Monte-Carlo Integration
    glm::vec3 color_from_scatter =
            traceRay(scene, scattered, depth - 1) * sample.lightTransport;

    return color_from_emission + color_from_scatter;
}
