# DRAY

**DRAY** (Display Rendering librarY) is a C++ library for driving ST7789 display modules. It provides a minimal interface for rendering basic graphics on embedded systems.

---

## Getting Started

1. Clone the repository:

   ```bash
   git clone https://github.com/yourusername/dray.git
   ```

2. Include the library in your project.

---

## Example Usage

```cpp id="u82kdl"
#include "lcd.hpp"

int main() {
    lcd display(RESX, CSX, DCX, SDA, SCL, BL);

    display.start();

    display.set_color(COLOR::RED);
    display.fill_screen();

    display.set_color(COLOR::WHITE);
    display.draw_rect(10, 100, 10, 100);

    while (1) {
    }
}
```

---

## Interface

### Constructor

```cpp id="n1z8qp"
lcd(uint16_t RESX, uint16_t CSX, uint16_t DCX, uint16_t SDA, uint16_t SCL, uint16_t BL);
```

Initializes the display object with the required GPIO pins:

* `RESX` – Reset pin
* `CSX`  – Chip Select pin
* `DCX`  – Data/Command pin
* `SDA`  – SPI MOSI pin
* `SCL`  – SPI Clock pin
* `BL`   – Backlight control pin

---

### Methods

#### `void start();`

Initializes the ST7789 display and prepares it for rendering. Must be called before any drawing operations.

---

#### `void set_color(COLOR col);`

Sets the current drawing color. All subsequent draw operations will use this color.

---

#### `void fill_screen();`

Fills the entire display with the currently selected color.

---

#### `void draw_rect(uint16_t ys, uint16_t ye, uint16_t xs, uint16_t xe);`

Draws a filled rectangle using the current color.

* `ys` – Start Y coordinate
* `ye` – End Y coordinate
* `xs` – Start X coordinate
* `xe` – End X coordinate
