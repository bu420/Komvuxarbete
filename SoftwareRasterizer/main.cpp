#include "math.hpp"
#include "gfx.hpp"
#include "platform.hpp"

#define W 600
#define H 600

using namespace komvux;

int main() {
	initialize();

	window win("Roterande Kub", W, H);

	color_buffer color_buf(W, H);
	depth_buffer depth_buf(W, H);

	for (int x = 0; x < W; x++) {
		for (int y = 0; y < H; y++) {
			color_buf.at(x, y) = { (unsigned char)(255 - x / (float)H * 255.f), (unsigned char)(y / (float)H * 255.f), 0 };
		}
	}

	while (!win.get_should_close()) {
		win.poll_events();

		win.swap_buffers(color_buf);
	}
}