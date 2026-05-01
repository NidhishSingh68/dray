#pragma once 
#include "gpio.hpp"
#include "timer.hpp"
#include <libopencm3/stm32/spi.h>

enum COLOR {
  RED = 0xF800U,
  GREEN = 0x07E0U,
  BLUE = 0x001FU,
  WHITE = 0xFFFFU
};

#define TRANSMIT_DATA(port,pin) gpio_set(port,pin);
#define TRANSMIT_COMMAND(port,pin) gpio_clear(port,pin);

#define BL_ON(port,pin) gpio_set(port,pin);
#define BL_OFF(port,pin) gpio_clear(port,pin);

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
    void fill_screen( COLOR col );
    void init_sequence();
    void spi_setup();
    void send_command(uint8_t command);
    void send_data(uint8_t data);
  private:
    void hw_reset();
    void set_rect(uint16_t ys, uint16_t ye, uint16_t xs, uint16_t xe);
    uint16_t RESX,DCX,CSX,SDA,SCL,BL;
};
