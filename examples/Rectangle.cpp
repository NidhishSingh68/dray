#include <libopencm3/stm32/rcc.h>
#include "lcd.hpp"

// PA10: Backlight
// PB5: DCX
// PA9: Reset
// --------SPI----------
// PA4: NSS
// PA5: SCK
// PA7: MOSI
int main(){
    rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_84MHZ]);

    lcd disp(GPIO::setPortPin(port::PORTA,pin::PIN9),GPIO::setPortPin(port::PORTA,pin::PIN4),GPIO::setPortPin(port::PORTB,pin::PIN5),GPIO::setPortPin(port::PORTA,pin::PIN7),GPIO::setPortPin(port::PORTA,pin::PIN5),GPIO::setPortPin(port::PORTA,pin::PIN10));
    disp.start();
    disp.set_color(COLOR::BLUE);
    disp.fill_screen();

    disp.set_color(COLOR::RED);
    disp.draw_rect(TFT_HEIGHT/2, (TFT_HEIGHT/2 + 20),TFT_WIDTH/2,(TFT_WIDTH/2)+20);

    while(true){
      __asm__("nop");
    }
}
