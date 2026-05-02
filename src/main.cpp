#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/timer.h>
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
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_USART2);
    lcd disp(GPIO::setPortPin(port::PORTA,pin::PIN9),GPIO::setPortPin(port::PORTA,pin::PIN4),GPIO::setPortPin(port::PORTB,pin::PIN5),GPIO::setPortPin(port::PORTA,pin::PIN7),GPIO::setPortPin(port::PORTA,pin::PIN5),GPIO::setPortPin(port::PORTA,pin::PIN10));
    disp.start();
    disp.set_color(COLOR::BLUE);
    disp.fill_screen();

    while(true){
      __asm__("nop");
    }
}
