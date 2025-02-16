#pragma once

#define BYTES_PER_PIXEL 6

struct Pixel {
	char sym;
	int row, col, r, g, b;
};