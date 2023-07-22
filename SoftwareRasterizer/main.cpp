#include "math.hpp"
#include "gfx.hpp"
#include "platform.hpp"

#define W 600
#define H 600

int main() {
	namespace kv = komvux;

	kv::initialize();

	kv::window win("Roterande Kub", W, H);

	kv::color_buffer color_buf(W, H);
	kv::depth_buffer depth_buf(W, H);

	color_buf.clear(kv::byte3{ 255, 255, 255 });
	depth_buf.clear(0);

	const std::array<kv::vec4f, 3> tri0 = {
		kv::vec4f(-0.5f, -0.5f, 0.f, 1.f),
		kv::vec4f(-0.5f, 0.5f, 0.f, 1.f),
		kv::vec4f(0.5f, 0.5f, 0.f, 1.f)
	};

	const std::array<kv::vec4f, 3> tri1 = {
		kv::vec4f(0.5f, 0.5f, 0.f, 1.f),
		kv::vec4f(0.5f, -0.5f, 0.f, 1.f),
		kv::vec4f(-0.5f, -0.5f, 0.f, 1.f)
	};

	kv::render_triangle(color_buf, depth_buf, tri0,
		[]() -> kv::byte3 {
			return { 128, 0, 255 };
		});
	kv::render_triangle(color_buf, depth_buf, tri1,
		[]() -> kv::byte3 {
			return { 255, 128, 0 };
		});

	while (!win.get_should_close()) {
		win.poll_events();
		win.swap_buffers(color_buf);
	}
}