/* 
 * BMPDraw.h - A lightweight, dependency-free BMP image drawing library
 * 
 * This header-only library provides a simple interface to create BMP image files
 * by drawing pixels, lines, rectangles, and circles using pure software rendering.
 * 
 * Features:
 * - No external graphics libraries or dependencies
 * - Software-based framebuffer rendering
 * - Basic 2D drawing functions: pixel, line, rectangle, circle
 * - Direct BMP file export (24-bit RGB format)
 * 
 * Author: Binxian Wang
 * Version: 26.1.0
 * License: MIT
 */

#ifndef BMPDRAW_H
#define BMPDRAW_H 1

#include <fstream>
#include <cstdint>
#include <cstring>

namespace BMPDraw {

struct Color {
    uint8_t r, g, b;
    Color(uint8_t r_ = 0, uint8_t g_ = 0, uint8_t b_ = 0) : r(r_), g(g_), b(b_) {  }
}; // class Color

class BMPDrawer {
private:
	int w, h;
	Color *fb;
	
public:
	BMPDrawer(int width, int height) : w(width), h(height) {
		fb = new Color[w * h];
		clear(Color(0,0,0));
	}
	
	~BMPDrawer() { delete[] fb; }
	
	void clear(Color c) {
		for (int i = 0; i < w * h; i++) fb[i] = c;
	}
	
	void putPixel(int x, int y, Color c) {
		if (x < 0 || x >= w || y < 0 || y >= h) return;
		fb[y * w + x] = c;
	}
	
	void line(int x1, int y1, int x2, int y2, Color c) {
		int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
		int dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
		int err = dx + dy;
		
		while (true) {
			putPixel(x1, y1, c);
			if (x1 == x2 && y1 == y2) break;
			int e2 = 2 * err;
			if (e2 >= dy) { err += dy; x1 += sx; }
			if (e2 <= dx) { err += dx; y1 += sy; }
        }
    }
	
	void rect(int x, int y, int bw, int bh, Color c) {
		for (int dx = 0; dx < bw; dx++) {
			putPixel(x + dx, y, c);
			putPixel(x + dx, y + bh - 1, c);
		}
		for (int dy = 0; dy < bh; dy++) {
			putPixel(x, y + dy, c);
			putPixel(x + bw - 1, y + dy, c);
		}
	}
	
	void circ(int cx, int cy, int r, Color c) {
		int x = r;
		int y = 0;
		int err = 1 - x;
		
		while (x >= y) {
			putPixel(cx + x, cy + y, c);
			putPixel(cx + y, cy + x, c);
			putPixel(cx - y, cy + x, c);
			putPixel(cx - x, cy + y, c);
			putPixel(cx - x, cy - y, c);
			putPixel(cx - y, cy - x, c);
			putPixel(cx + y, cy - x, c);
			putPixel(cx + x, cy - y, c);
			
			y++;
			if (err < 0) {
				err += 2 * y + 1;
			} else {
				x--;
				err += 2 * (y - x + 1);
			}
		}
	}
	
	void saveBMP(const char *path) {
		std::ofstream f(path, std::ios::binary);
		
		uint16_t bfType = 0x4D42;
		uint32_t bfSize = 54 + 3 * w * h;
		uint16_t bfRes1 = 0, bfRes2 = 0;
		uint32_t bfOffBits = 54;
		
		f.write((char*)&bfType, 2);
		f.write((char*)&bfSize, 4);
		f.write((char*)&bfRes1, 2);
		f.write((char*)&bfRes2, 2);
		f.write((char*)&bfOffBits, 4);
		
		uint32_t biSize = 40;
		int32_t  biWidth = w, biHeight = -h;
		uint16_t biPlanes = 1, biBitCount = 24;
		uint32_t biCompression = 0, biSizeImage = 0, biXPelsPerMeter = 0, biYPelsPerMeter = 0, biClrUsed = 0, biClrImportant = 0;
		
		f.write((char*)&biSize, 4);
		f.write((char*)&biWidth, 4);
		f.write((char*)&biHeight, 4);
		f.write((char*)&biPlanes, 2);
		f.write((char*)&biBitCount, 2);
		f.write((char*)&biCompression, 4);
		f.write((char*)&biSizeImage, 4);
		f.write((char*)&biXPelsPerMeter, 4);
		f.write((char*)&biYPelsPerMeter, 4);
		f.write((char*)&biClrUsed, 4);
		f.write((char*)&biClrImportant, 4);
		
		for (int i = 0; i < w * h; i++) {
			uint8_t bgr[] = { fb[i].b, fb[i].g, fb[i].r };
			f.write((char*)bgr, 3);
		}
	}
}; // class BMPDrawer

} // namespace BMPDraw

#endif /* BMPDRAW_H */
