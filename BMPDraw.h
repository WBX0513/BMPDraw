/*
 * BMPDraw.h - Lightweight BMP image drawing library
 * 
 * A header-only, dependency-free 2D graphics library for software rendering
 * and generating uncompressed 24-bit RGB BMP image files.
 * 
 * Features:
 * - Software framebuffer rendering
 * - Pixel, line, rectangle, circle, polygon drawing
 * - Boundary checking for safe drawing
 * - Direct BMP file export
 * - No external dependencies
 * 
 * Author:  Binxian Wang
 * Version: 26.2.0
 * License: MIT
 */

#ifndef BMPDRAW_H
#define BMPDRAW_H 1

#ifndef BMPDRAW_H_VERSION
#define BMPDRAW_H_VERSION 260200L
#endif /* BMPDRAW_H_VERSION */

#include <cstdint>
#include <cstring>
#include <fstream>

namespace BMPDraw {

struct Color {
    uint8_t r, g, b;
    Color(uint8_t r_ = 0, uint8_t g_ = 0, uint8_t b_ = 0) : r(r_), g(g_), b(b_) {  }
}; // class Color

#if BMPDRAW_H_VERSION >= 260102L

# if BMPDRAW_H_VERSION >= 260103L

#  define COLOR_MIN     0
#  define COLOR_MAX     255

#  define BMP_BLACK     Color(COLOR_MIN, COLOR_MIN, COLOR_MIN)
#  define BMP_WHITE     Color(COLOR_MAX, COLOR_MAX, COLOR_MAX)
#  define BMP_RED       Color(COLOR_MAX, COLOR_MIN, COLOR_MIN)
#  define BMP_GREEN     Color(COLOR_MIN, COLOR_MAX, COLOR_MIN)
#  define BMP_BLUE      Color(COLOR_MIN, COLOR_MIN, COLOR_MAX)
#  define BMP_YELLOW    Color(COLOR_MAX, COLOR_MAX, COLOR_MIN)
#  define BMP_CYAN      Color(COLOR_MIN, COLOR_MAX, COLOR_MAX)
#  define BMP_MAGENTA   Color(COLOR_MAX, COLOR_MIN, COLOR_MAX)

# endif /* BMPDRAW_H_VERSION >= 260103L */

struct Point {
	int x, y;
	Point(int x_, int y_) : x(x_), y(y_) {  }
}; // class Point

#endif /* BMPDRAW_H_VERSION >= 260102L */

class BMPDrawer {
private:
	int w, h;
	Color *fb;
	
#if BMPDRAW_H_VERSION >= 260102L
	/// One helper for BMPDrawer.
	inline int min(int __a, int __b) { return __a < __b ? __a : __b; }
#endif /* BMPDRAW_H_VERSION >= 260102L */
	
public:
	BMPDrawer(int width, int height) : w(width), h(height) {
		fb = new Color[w * h];
#if BMPDRAW_H_VERSION >= 260101L
		clear(Color(255, 255, 255));
#else /* BMPDRAW_H_VERSION < 260101L */
		clear(Color(0, 0, 0));
#endif /* BMPDRAW_H_VERSION >= 260101L */
	}
	
	~BMPDrawer() { delete[] fb; }
	
	void clear(Color c) {
		for (int i = 0; i < w * h; i++) fb[i] = c;
	}
	
#if BMPDRAW_H_VERSION >= 260102L
	void putPixel(Point p, Color c) { putPixel(p.x, p.y, c); }
	void line(Point p1, Point p2, Color c) { line(p1.x, p1.y, p2.x, p2.y, c); }
	void rect(Point p1, Point p2, Color c) { rect(min(p1.x, p2.x), min(p1.y, p2.y), abs(p1.x - p2.x), abs(p1.y - p2.y), c); }
	void circ(Point p, int r, Color c) { circ(p.x, p.y, r, c); }
#endif /* BMPDRAW_H_VERSION >= 260102L */
	
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
	
#if BMPDRAW_H_VERSION >= 260200L
	template<typename... _Points>
	void poly(Color c, _Points... points) {
		static_assert(sizeof...(points) >= 2, "Need at least 2 points!");
		Point ps[] = { points... };
		int n = sizeof...(points);
		for (int i = 0, j = 1; j < n; i++, j++) line(ps[i], ps[j], c);
		line(ps[n - 1], ps[0], c);
	}
#endif /* BMPDRAW_H_VERSION >= 260200L */
	
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
