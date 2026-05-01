#include "gpio.hpp"

uint16_t GPIO::setPortPin( port po, pin pi ){
  return (po << 8 | pi);
}

port GPIO::getPort( uint16_t portPin ){
   return static_cast<port>((portPin >> 8) & 0xFF);
}

pin GPIO::getPin( uint16_t portPin ){
   return static_cast<pin>((portPin) & 0xFF);
}

