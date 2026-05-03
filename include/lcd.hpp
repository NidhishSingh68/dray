#pragma once 

#include "gpio.hpp"
#include "timer.hpp"
#include <libopencm3/stm32/spi.h>

enum COLOR {
  RED = 0xF800U,
  GREEN = 0x07E0U,
  BLUE = 0x001FU,
  WHITE = 0xFFFFU,
  BLACK = 0x00000
};

#define TRANSMIT_DATA(port,pin) gpio_set(port,pin);
#define TRANSMIT_COMMAND(port,pin) gpio_clear(port,pin);

#define BL_ON(port,pin) gpio_set(port,pin);
#define BL_OFF(port,pin) gpio_clear(port,pin);

#define CS_ON(port,pin) gpio_clear(port,pin);
#define CS_OFF(port,pin) gpio_set(port,pin);

#define TFT_WIDTH 240
#define TFT_HEIGHT 320

// -------------------------Display Commands-------------------------------------
#define NOP 0x00
#define SWRESET 0x01

#define SLPOUT 0x11
#define NORON 0x13

#define INVOFF 0x20
#define INVON 0x21

#define DISPOFF 0x28
#define DISPON 0x29

#define CASET 0x2A
#define RASET 0x2B

#define RAMWR 0x2C

#define MADCTL 0x36

#define COLMOD 0x3A
// -----------------------------------------------------------------------------

class lcd{
  public:
    lcd( uint16_t RESX, uint16_t CSX, uint16_t DCX, uint16_t SDA, uint16_t SCL, uint16_t BL );
    void start();
    void set_color( COLOR col );
    void fill_screen();
    void draw_rect( uint16_t ys, uint16_t ye, uint16_t xs, uint16_t xe );
  
  private:
    void spi_setup();
    void hw_reset();
    void select();
    void deselect();
    void init_sequence();
    void send_command(uint8_t command);
    void send_data(uint8_t data);
    void set_rect(uint16_t ys, uint16_t ye, uint16_t xs, uint16_t xe);
    void fill_rect(uint16_t ys, uint16_t ye, uint16_t xs, uint16_t xe);

  private:
    uint16_t RESX,DCX,CSX,SDA,SCL,BL;
    COLOR render_color;
};

