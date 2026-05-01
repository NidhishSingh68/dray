#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/timer.h>
#include "uart.hpp"
#include "printf.h"
#include "lcd.hpp"

namespace {
  uart logger;
}

extern "C" void usart2_isr(){
  logger.isr();
}

extern "C" void _putchar(char c){
  logger.write(c);
  if ( (USART_SR(uart_config::usart) & USART_SR_TXE) != 0 ) {
    usart_enable_tx_interrupt(uart_config::usart);
  }
}

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
    logger.setup();
    printf("Hello from the world");
    lcd disp(GPIO::setPortPin(port::PORTA,pin::PIN9),GPIO::setPortPin(port::PORTA,pin::PIN4),GPIO::setPortPin(port::PORTB,pin::PIN5),GPIO::setPortPin(port::PORTA,pin::PIN7),GPIO::setPortPin(port::PORTA,pin::PIN5),GPIO::setPortPin(port::PORTA,pin::PIN10));
    disp.start();
    disp.fill_screen(COLOR::GREEN);

    //port po = GPIO::getPort(GPIO::setPortPin(port::PORTA,pin::PIN10));
    //pin pi = GPIO::getPin(GPIO::setPortPin(port::PORTA,pin::PIN10));
    //BL_ON(GPIO_PORT[po],GPIO_PIN[pi]);
    while(true){
      __asm__("nop");
    }
}
