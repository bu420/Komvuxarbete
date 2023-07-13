#pragma once

#include <vector>
#include <functional>

#include "math.hpp"

namespace komvux {
	struct byte3 {
		unsigned char x, y, z;
	};

	template <typename T> class buffer2d {
	public:
		buffer2d(int width, int height) :
			width(width), height(height) {
			data.resize(width * height);
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

	void render_triangle(
		color_buffer& color_buf,
		depth_buffer& depth_buf,
		const vec4f& pos0,
		const vec4f& pos1,
		const vec4f& pos2,
		const std::function<byte3()>& pixel_shader_callback);
}