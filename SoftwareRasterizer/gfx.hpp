#pragma once

#include <vector>
#include <array>
#include <optional>
#include <functional>

#include "math.hpp"

namespace komvux {
	template<typename T> using optional_reference = 
		std::optional<std::reference_wrapper<T>>;

	struct byte3 {
		unsigned char x, y, z;
	};

	template <typename T> class buffer2d {
	public:
		buffer2d(int width, int height) :
			width(width), height(height) {
			data.resize(width * height);
		}

		void clear(T value) {
			std::fill(data.begin(), data.end(), value);
		}

		int get_width() const { return width; }
		int get_height() const { return height; }

		T& operator [] (int i) { return data.at(i); }
		const T& operator [] (int i) const { return data.at(i); }

		T& at(int x, int y) { return data.at(y * width + x); }
		const T& at(int x, int y) const { return data.at(y * width + x); }

	protected:
		int width;
		int height;
		std::vector<T> data;
	};

	using color_buffer = buffer2d<byte3>;
	using depth_buffer = buffer2d<float>;

	struct attribute {
		vec4f data;
		int count;

        attribute() : count(0) {}
        attribute(float value) : data{ value, 0.f, 0.f, 0.f }, count(1) {}
        attribute(const vec2f& value) : data{ value.x(), value.y(), 0.f, 0.f }, count(2) {}
        attribute(const vec3f& value) : data{ value.x(), value.y(), value.z(), 0.f }, count(3) {}
        attribute(const vec4f& value) : data{ value.x(), value.y(), value.z(), value.w() }, count(4) {}

		attribute lerp(const attribute& other, float amount) const;
	};

	struct vertex {
		vec4f position;

		std::array<attribute, 4> attributes;
		int attribute_count;
    
        vertex() = default;

        template <typename... T> vertex(const vec4f& position, T... attributes) : 
            position(position), attributes{ attributes... }, attribute_count(sizeof...(T)) {
        }

		vertex lerp(const vertex& other, float amount) const;
	};
    
    void render_triangle(
        optional_reference<color_buffer> color_buf,
        optional_reference<depth_buffer> depth_buf,
        std::array<vertex, 3> vertices,
        std::function<byte3(const vertex&)> pixel_shader_callback);
}