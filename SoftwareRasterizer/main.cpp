#include "math.hpp"
#include "gfx.hpp"
#include "platform.hpp"

#include <numbers>

#define W 600
#define H 600

int main() {
    namespace kv = komvux;

    kv::initialize();

    kv::window win("Roterande Kub", W, H);

    kv::color_buffer color_buf(W, H);
    kv::depth_buffer depth_buf(W, H);

    const kv::vec3f cube_vertices[36] = {
        {-0.5f,-0.5f,-0.5f}, {-0.5f,0.5f,-0.5f}, {0.5f,0.5f,-0.5f},   {-0.5f,-0.5f,-0.5f}, {0.5f,0.5f,-0.5f},   {0.5f,-0.5f,-0.5f},
        {0.5f,-0.5f,-0.5f},  {0.5f,0.5f,-0.5f},  {0.5f,0.5f,0.5f},    {0.5f,-0.5f,-0.5f},  {0.5f,0.5f,0.5f},    {0.5f,-0.5f,0.5f},
        {0.5f,-0.5f,0.5f},   {0.5f,0.5f,0.5f},   {-0.5f,0.5f,0.5f},   {0.5f,-0.5f,0.5f},   {-0.5f,0.5f,0.5f},   {-0.5f,-0.5f,0.5f},
        {-0.5f,-0.5f,0.5f},  {-0.5f,0.5f,0.5f},  {-0.5f,0.5f,-0.5f},  {-0.5f,-0.5f,0.5f},  {-0.5f,0.5f,-0.5f},  {-0.5f,-0.5f,-0.5f},
        {-0.5f,0.5f,-0.5f},  {-0.5f,0.5f,0.5f},  {0.5f,0.5f,0.5f},    {-0.5f,0.5f,-0.5f},  {0.5f,0.5f,0.5f},    {0.5f,0.5f,-0.5f},
        {0.5f,-0.5f,0.5f},   {-0.5f,-0.5f,0.5f}, {-0.5f,-0.5f,-0.5f}, {0.5f,-0.5f,0.5f},   {-0.5f,-0.5f,-0.5f}, {0.5f,-0.5f,-0.5f}
    };

    const kv::mat4 projection = kv::perspective(
        H / static_cast<float>(W), 
        70 * (static_cast<float>(std::numbers::pi) / 180), 
        .1f, 
        1000.f);

    const kv::vec3f camera_pos(1.f, 1.f, 1.f);
    const kv::mat4 view = kv::look_at(camera_pos, { 0.f, 0.f, 0.f }, { 0.f, -1.f, 0.f });

    while (!win.get_should_close()) {
        win.poll_events();

        color_buf.clear(kv::byte3{ 255, 255, 255 });
        depth_buf.clear(0);

        // Model matrix with a rotation animation.
        kv::mat4 model(1);
        model = model.rotate_y(kv::get_elapsed_time() * std::numbers::pi / 2000);
        model = model.rotate_z(kv::get_elapsed_time() * std::numbers::pi / 4000);

        kv::mat4 mvp = model * view * projection;

        // For each of of the cube's triangles.
        for (int i = 0; i < 12; i++) {
            std::array<kv::vec4f, 3> vertices;

            for (int j = 0; j < 3; j++) {
                kv::vec3f vertex = cube_vertices[i * 3 + j];
                vertices[j] = { vertex.x(), vertex.y(), vertex.z(), 1.f };

                // Multiply position with MVP matrix, world space -> clip space.
                vertices[j] *= mvp;
            }

            kv::render_triangle(color_buf, depth_buf, vertices,
                []() -> kv::byte3 {
                    return { 255, 0, 255 };
                });
        }

        win.swap_buffers(color_buf);
    }
}