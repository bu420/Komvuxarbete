#include "math.hpp"
#include "gfx.hpp"
#include "platform.hpp"

#include <numbers>
#include <algorithm>
#include <iostream>

#define W 600
#define H 600

int main() {
    namespace kv = komvux;

    kv::initialize();

    kv::window win("Roterande Kub", W, H);

    kv::color_buffer color_buf(W, H);
    kv::depth_buffer depth_buf(W, H);

    const kv::vertex cube_vertices[36] = {
        kv::vertex({-0.5f,-0.5f,-0.5f,1.f},kv::attribute(kv::vec3f(255.f,0.f,0.f))),
        kv::vertex({-0.5f,0.5f,-0.5f,1.f},kv::attribute(kv::vec3f(255.f,0.f,0.f))),
        kv::vertex({0.5f,0.5f,-0.5f,1.f},kv::attribute(kv::vec3f(255.f,0.f,0.f))),
        kv::vertex({-0.5f,-0.5f,-0.5f,1.f},kv::attribute(kv::vec3f(255.f,0.f,0.f))),
        kv::vertex({0.5f,0.5f,-0.5f,1.f},kv::attribute(kv::vec3f(255.f,0.f,0.f))),
        kv::vertex({0.5f,-0.5f,-0.5f,1.f},kv::attribute(kv::vec3f(255.f,0.f,0.f))),

        kv::vertex({0.5f,-0.5f,-0.5f,1.f},kv::attribute(kv::vec3f(0.f,255.f,0.f))),
        kv::vertex({0.5f,0.5f,-0.5f,1.f},kv::attribute(kv::vec3f(0.f,255.f,0.f))),
        kv::vertex({0.5f,0.5f,0.5f,1.f},kv::attribute(kv::vec3f(0.f,255.f,0.f))),
        kv::vertex({0.5f,-0.5f,-0.5f,1.f},kv::attribute(kv::vec3f(0.f,255.f,0.f))),
        kv::vertex({0.5f,0.5f,0.5f,1.f},kv::attribute(kv::vec3f(0.f,255.f,0.f))),
        kv::vertex({0.5f,-0.5f,0.5f,1.f},kv::attribute(kv::vec3f(0.f,255.f,0.f))),

        kv::vertex({0.5f,-0.5f,0.5f,1.f},kv::attribute(kv::vec3f(0.f,0.f,255.f))),
        kv::vertex({0.5f,0.5f,0.5f,1.f},kv::attribute(kv::vec3f(0.f,0.f,255.f))),
        kv::vertex({-0.5f,0.5f,0.5f,1.f},kv::attribute(kv::vec3f(0.f,0.f,255.f))),
        kv::vertex({0.5f,-0.5f,0.5f,1.f},kv::attribute(kv::vec3f(0.f,0.f,255.f))),
        kv::vertex({-0.5f,0.5f,0.5f,1.f},kv::attribute(kv::vec3f(0.f,0.f,255.f))),
        kv::vertex({-0.5f,-0.5f,0.5f,1.f},kv::attribute(kv::vec3f(0.f,0.f,255.f))),

        kv::vertex({-0.5f,-0.5f,0.5f,1.f},kv::attribute(kv::vec3f(255.f,0.f,255.f))),
        kv::vertex({-0.5f,0.5f,0.5f,1.f},kv::attribute(kv::vec3f(255.f,0.f,255.f))),
        kv::vertex({-0.5f,0.5f,-0.5f,1.f},kv::attribute(kv::vec3f(255.f,0.f,255.f))),
        kv::vertex({-0.5f,-0.5f,0.5f,1.f},kv::attribute(kv::vec3f(255.f,0.f,255.f))),
        kv::vertex({-0.5f,0.5f,-0.5f,1.f},kv::attribute(kv::vec3f(255.f,0.f,255.f))),
        kv::vertex({-0.5f,-0.5f,-0.5f,1.f},kv::attribute(kv::vec3f(255.f,0.f,255.f))),

        kv::vertex({-0.5f,0.5f,-0.5f,1.f},kv::attribute(kv::vec3f(0.f,255.f,255.f))),
        kv::vertex({-0.5f,0.5f,0.5f,1.f},kv::attribute(kv::vec3f(0.f,255.f,255.f))),
        kv::vertex({0.5f,0.5f,0.5f,1.f},kv::attribute(kv::vec3f(0.f,255.f,255.f))),
        kv::vertex({-0.5f,0.5f,-0.5f,1.f},kv::attribute(kv::vec3f(0.f,255.f,255.f))),
        kv::vertex({0.5f,0.5f,0.5f,1.f},kv::attribute(kv::vec3f(0.f,255.f,255.f))),
        kv::vertex({0.5f,0.5f,-0.5f,1.f},kv::attribute(kv::vec3f(0.f,255.f,255.f))),

        kv::vertex({0.5f,-0.5f,0.5f,1.f},kv::attribute(kv::vec3f(255.f,255.f,0.f))),
        kv::vertex({-0.5f,-0.5f,0.5f,1.f},kv::attribute(kv::vec3f(255.f,255.f,0.f))),
        kv::vertex({-0.5f,-0.5f,-0.5f,1.f},kv::attribute(kv::vec3f(255.f,255.f,0.f))),
        kv::vertex({0.5f,-0.5f,0.5f,1.f},kv::attribute(kv::vec3f(255.f,255.f,0.f))),
        kv::vertex({-0.5f,-0.5f,-0.5f,1.f},kv::attribute(kv::vec3f(255.f,255.f,0.f))),
        kv::vertex({0.5f,-0.5f,-0.5f,1.f},kv::attribute(kv::vec3f(255.f,255.f,0.f)))
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
        depth_buf.clear(1);

        // Model matrix with a rotation animation.
        kv::mat4 model(1);
        model = model.rotate_y(kv::get_elapsed_time() * std::numbers::pi / 2000);
        model = model.rotate_z(kv::get_elapsed_time() * std::numbers::pi / 4000);

        kv::mat4 mvp = model * view * projection;

        // For each of of the cube's triangles.
        for (int i = 0; i < 12; i++) {
            std::array<kv::vertex, 3> vertices;

            for (int j = 0; j < 3; j++) {
                vertices[j] = cube_vertices[i * 3 + j];

                // Multiply position with MVP matrix, world space -> clip space.
                vertices[j].position *= mvp;
            }

            kv::render_triangle(color_buf, depth_buf, vertices,
                [](const kv::vertex& vertex) -> kv::byte3 {
                    kv::vec4f pos = vertex.position;
                    
                    kv::vec3f color(
                        vertex.attributes[0].data[0],
                        vertex.attributes[0].data[1],
                        vertex.attributes[0].data[2]);

                    color *= std::sin(H - (pos.y() / H)) * 1.5;
                    color += H / 4.f;

                    if (pos.x() > 300) {
                        color.r() = 255;
                    }

                    color *= std::cos(pos.x() / W);

                    return { 
                        static_cast<unsigned char>(color.x()), 
                        static_cast<unsigned char>(color.y()), 
                        static_cast<unsigned char>(color.z())
                    };
                });
        }

        win.swap_buffers(color_buf);
    }
}