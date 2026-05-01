#include "uart.hpp"

void uart::setup()
{
    gpio_mode_setup( GPIOA,GPIO_MODE_AF,GPIO_PUPD_PULLDOWN,GPIO2|GPIO3 );
    gpio_set_af(GPIOA,GPIO_AF7,GPIO2|GPIO3);

    usart_enable( uart_config::usart );
    usart_set_mode( uart_config::usart, USART_MODE_TX ); 
    usart_set_baudrate( uart_config::usart , uart_config::BAUDRATE );
    usart_set_stopbits( uart_config::usart,USART_STOPBITS_1 );
    usart_set_databits(uart_config::usart,8);
    usart_set_parity( uart_config::usart,USART_PARITY_NONE );

    nvic_enable_irq(NVIC_USART2_IRQ);
}

void uart::write(std::uint8_t c){
  buffer.push(c);
}

void uart::isr() {
  int ch = buffer.pop();
  if (ch == -1) usart_disable_tx_interrupt(uart_config::usart);
  else {
    usart_send(uart_config::usart,(uint8_t)ch);
  }
}
