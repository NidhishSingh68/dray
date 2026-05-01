#include <cstdint>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>
#include "rb.hpp"

namespace uart_config{
  const uint32_t BAUDRATE = 115200;
  const uint32_t usart = USART2;
}

class uart{
  public:
    uart() = default;
    void setup();
    void write( std::uint8_t c );
    void isr();
    
  private:
    rb<200> buffer;
};
