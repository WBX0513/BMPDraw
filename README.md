# BMPDraw

A lightweight, dependency-free C++ software rendering library for generating 24-bit BMP images.(-std=c++11)

## Version

26.2.0

## Features

- No external graphics libraries required
- Software framebuffer rendering
- Basic 2D drawing primitives:
  - Pixel
  - Line
  - Rectangle
  - Circle
  - Polygon
- Export to standard 24-bit Windows BMP file
- **Important**: Image width and height must be multiples of 4

## Usage

Include `BMPDraw.h` in your project:

```cpp
#include "BMPDraw.h"

int main() {
	BMPDraw::BMPDrawer gui(800, 600);
	
	gui.line(50,50, 700,500, BMPDraw::Color(255,0,0));
	gui.rect(100,100, 200,150, BMPDraw::Color(0,255,0));
	gui.circ(400, 300, 100, BMPDraw::Color(255, 255, 0));
	
	gui.saveBMP("output.bmp");
	return 0;
}
```

## Compile

No linking required, header-only library.

## License

This project is licensed under the MIT License.
