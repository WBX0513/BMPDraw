# BMPDraw
A lightweight, dependency-free C++ software rendering library for generating 24-bit BMP images.

## Version
26.1.0

## Features
- No external graphics libraries required
- Software framebuffer rendering
- Basic 2D drawing primitives:
  - Pixel
  - Line
  - Rectangle
  - Circle
- Export to standard 24-bit Windows BMP file

## Usage
Include `BMPDraw.h` in your project:

```cpp
#include "BMPDraw.h"

int main() {
    BMPDraw::BMPDrawer drawer(800, 600);

    drawer.clear(BMPDraw::Color(0, 0, 0));
    drawer.line(100, 100, 700, 500, BMPDraw::Color(255, 0, 0));
    drawer.rect(200, 200, 150, 100, BMPDraw::Color(0, 255, 0));
    drawer.circ(400, 300, 80, BMPDraw::Color(0, 0, 255));

    drawer.saveBMP("output.bmp");
    return 0;
}
```

## Compile
No linking required, header-only library.

## License
This project is licensed under the MIT License.