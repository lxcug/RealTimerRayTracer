// @author: lx
// @time: 2024/1/11
// @brief: 

#ifndef REAL_TIME_RAY_TRACER_RENDERER_H
#define REAL_TIME_RAY_TRACER_RENDERER_H

#include "Walnut/Image.h"
#include <memory>
#include "glm/glm.hpp"
#include "Camera.h"
#include "Ray.h"
#include "Scene.h"
#include <thread>
#include <functional>
#include <condition_variable>
#include <mutex>

#define MULTI_THREAD 1
#define THREAD_NUM 12


class Renderer {
public:
    Renderer() = default;

    ~Renderer() {
        delete[] m_image_data;
        delete[] m_accumulation_data;
    }

    void onResize(uint32_t width, uint32_t height);

    void render(const Scene &scene, Camera &camera);

    [[nodiscard]] const std::shared_ptr<Walnut::Image> &getFinalImage() const {
        return m_final_image;
    }

    void resetFrameIndex() {
        m_frame_index = 1;
    }

    [[nodiscard]] int getFrameCount() const {
        return m_frame_index;
    }

    int& getSpp() {
        return m_spp;
    }

    void setRussianRoulette(float prob) {
        m_russianRoulette = prob;
    }

private:
    glm::vec3 traceRay(const Scene &scene, const Ray &ray, int depth);

private:
    std::shared_ptr<Walnut::Image> m_final_image;  // image data in gpu side
    uint32_t *m_image_data = nullptr;  // image data in cpu side
    int m_depth = 20;
    float m_russianRoulette = .95f;
    int m_spp = 1;

    const Scene *m_active_scene = nullptr;
    Camera *m_active_camera = nullptr;

    glm::vec3 *m_accumulation_data = nullptr;
    uint32_t m_frame_index = 1;

    std::vector<uint32_t> m_image_vertical_iter, m_image_horizontal_iter;

//#if MULTI_THREAD
//    std::vector<std::thread> m_threads;
//    std::function<void(Renderer *, int, int)> m_block_shading_func;
//    std::atomic<int> m_finished_threads;
//    std::condition_variable m_wait_all_threads_finished;
//    std::mutex m_render_frame_lock;
//#endif
};

static uint32_t convert2RGB(const glm::vec4 &color);


#endif //REAL_TIME_RAY_TRACER_RENDERER_H
