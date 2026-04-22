#include "BMPDraw.h"

using namespace BMPDraw;

int main() {
	BMPDrawer drawer(800, 600);
	
	drawer.clear(BMP_WHITE);
	
	drawer.line(Point(50, 50), Point(750, 50), BMP_RED);
	drawer.rect(Point(100, 100), Point(300, 300), BMP_GREEN);
	drawer.circ(Point(400, 200), 100, BMP_BLUE);
	
	drawer.poly(
		BMP_YELLOW,
		Point(200, 400),
		Point(100, 500),
		Point(300, 500)
	);
	
	drawer.poly(
		BMP_MAGENTA,
		Point(500, 350),
		Point(450, 450),
		Point(550, 500),
		Point(650, 450),
		Point(600, 350)
	);
	
	drawer.saveBMP("output.bmp");
	
	return 0;
}
