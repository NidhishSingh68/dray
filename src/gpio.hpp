#pragma once

#include <cstdint>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include "printf.h"

enum port{
  PORTA,
  PORTB,
  PORTC,
  PORTD,
  PORTE,
  PORTF,
  PORTG,
  PORTH
};

enum pin{
  PIN0,
  PIN1,
  PIN2,
  PIN3,
  PIN4,
  PIN5,
  PIN6,
  PIN7,
  PIN8,
  PIN9,
  PIN10,
  PIN11,
  PIN12,
  PIN13,
  PIN14,
  PIN15
};

const rcc_periph_clken RCC_PORT[] = { RCC_GPIOA, RCC_GPIOB, RCC_GPIOC, RCC_GPIOD, RCC_GPIOE, RCC_GPIOF, RCC_GPIOG, RCC_GPIOH};
const uint32_t GPIO_PIN[] = {GPIO0,GPIO1,GPIO2,GPIO3,GPIO4,GPIO5,GPIO6,GPIO7,GPIO8,GPIO9,GPIO10,GPIO11,GPIO12,GPIO13,GPIO14,GPIO15};
const uint32_t GPIO_PORT[] = {GPIOA,GPIOB,GPIOC,GPIOD,GPIOE,GPIOF,GPIOG,GPIOH};

namespace GPIO{

  uint16_t setPortPin( port po, pin pi );
  port getPort( uint16_t portPin );
  pin getPin( uint16_t portPin );

}
