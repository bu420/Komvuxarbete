#include "gfx.hpp"

#include <iostream>

using namespace komvux;

struct line3d {
    vec3f start;
    vec3f end;
};

enum class line_type {
    any,
    vertical,
    horizontal
};

struct line3d_stepper {
    vec3f current;
    vec3f increment;

    int steps = -1;
    int i = 0;
    line_type type;

    line3d_stepper(line3d line, line_type type) : type(type) {
        // Round X and Y to nearest integer (pixel position).
        line.start.x() = std::round(line.start.x());
        line.start.y() = std::round(line.start.y());
        line.end.x() = std::round(line.end.x());
        line.end.y() = std::round(line.end.y());

        current = line.start;

        vec3f difference(line.end - line.start);

        // Calculate steps (total number of increments).
        switch (type) {
        case line_type::any: 
            steps = static_cast<int>(std::max(std::abs(difference.x()), std::abs(difference.y())));
            break;

        case line_type::vertical:
            steps = static_cast<int>(std::abs(difference.y()));
            break;

        case line_type::horizontal:
            steps = static_cast<int>(std::abs(difference.x()));
        }

        // Calculate how much to increment each step.
        if (steps > 0) {
            increment = difference / static_cast<float>(steps);
        }
    }

    bool step() {
        if (i == steps) {
            return false;
        }

        i++;
        current += increment;
        return true;
    }
};

void render_triangle_from_lines(
    color_buffer& color_buf,
    depth_buffer& depth_buf, 
    line3d a, 
    line3d b,
    const std::function<byte3()>& pixel_shader_callback) {
    // Sort lines based on X.
    if (a.start.x() > b.start.x()) {
        std::swap(a, b);
    }

    line3d_stepper line_a(a, line_type::vertical);
    line3d_stepper line_b(b, line_type::vertical);

    do {
        assert(line_a.current.y() == line_b.current.y() && "Big failure.");

        line3d_stepper line_x(line3d{ line_a.current, line_b.current }, line_type::horizontal);
        
        do {
            int x = static_cast<int>(line_x.current.x());
            int y = static_cast<int>(line_x.current.y());

            color_buf.at(x, y) = pixel_shader_callback();
        } while (line_x.step());
    } while (line_a.step() && line_b.step());
}

void komvux::render_triangle(
	color_buffer& color_buf,
	depth_buffer& depth_buf,
    std::array<vec4f, 3> positions_vec4,
	const std::function<byte3()>& pixel_shader_callback) {
    // W division (homogeneous clip space -> NDC space).
    for (auto& position : positions_vec4) {
        position.r() /= position.w();
        position.g() /= position.w();
        position.b() /= position.w();
    }

    // Discard W component since we do not need it anymore.
    std::array<vec3f, 3> positions = {
        vec3f(positions_vec4[0].x(), positions_vec4[0].y(), positions_vec4[0].z()),
        vec3f(positions_vec4[1].x(), positions_vec4[1].y(), positions_vec4[1].z()),
        vec3f(positions_vec4[2].x(), positions_vec4[2].y(), positions_vec4[2].z())
    };

    // Viewport transformation. 
    // Scale from [-1, 1] to color buffer size.
    for (auto& position : positions) {
        position.x() = (position.x() + 1) / 2.f * color_buf.get_width();
        position.y() = (position.y() + 1) / 2.f * color_buf.get_height();
    }
    
    // Floor X and Y, otherwise there's missing pixel artifacts.
    for (auto& position : positions) {
        position.x() = std::floorf(position.x());
        position.y() = std::floorf(position.y());
    }

    // Sort vertices by Y.
    if (positions[0].y() > positions[1].y()) {
        std::swap(positions[0], positions[1]);
    }
    if (positions[0].y() > positions[2].y()) {
        std::swap(positions[0], positions[2]);
    }
    if (positions[1].y() > positions[2].y()) {
        std::swap(positions[1], positions[2]);
    }

    // Check if the top of the triangle is flat.
    if (positions[0].y() == positions[1].y()) {
        render_triangle_from_lines(
            color_buf,
            depth_buf,
            line3d{ positions[0], positions[2] }, 
            line3d{ positions[1], positions[2] },
            pixel_shader_callback);
    }
    // Check if the bottom is flat.
    else if (positions[1].y() == positions[2].y()) {
        render_triangle_from_lines(
            color_buf,
            depth_buf,
            line3d{ positions[0], positions[1] }, 
            line3d{ positions[0], positions[2] },
            pixel_shader_callback);
    }
    // Else split into two smaller triangles.
    else {
        float alpha_split = (positions[1].y() - positions[0].y()) / (positions[2].y() - positions[0].y());

        vec3f position3(
            lerp(positions[0].x(), positions[2].x(), alpha_split),
            positions[1].y(),
            lerp(positions[0].z(), positions[2].z(), alpha_split));

        // Top (flat bottom).
        render_triangle_from_lines(
            color_buf, 
            depth_buf, 
            line3d{ positions[0], positions[1] }, 
            line3d{ positions[0], position3 }, 
            pixel_shader_callback);

        // Bottom (flat top).
        render_triangle_from_lines(
            color_buf,
            depth_buf,
            line3d{ positions[1], positions[2] }, 
            line3d{ position3, positions[2] },
            pixel_shader_callback);
    }
}