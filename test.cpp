#include "BMPDraw.h"

int main() {
	BMPDraw::BMPDrawer gui(800, 600);
	
	gui.line(50,50, 700,500, BMPDraw::Color(255,0,0));
	gui.rect(100,100, 200,150, BMPDraw::Color(0,255,0));
	gui.circ(400, 300, 100, BMPDraw::Color(255, 255, 0));
	
	gui.saveBMP("output.bmp");
	return 0;
}
