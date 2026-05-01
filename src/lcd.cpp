#include "lcd.hpp"

namespace {
  timer tim(TIMER::T2);
}

extern "C" void tim2_isr(){
  timer_clear_flag(TIM::TIMER[tim.tim],TIM_SR_UIF);
  tim.stopTimer();
  tim.shouldLoop = false;
}

lcd::lcd(  uint16_t RESX, uint16_t NSS, uint16_t DCX, uint16_t MOSI, uint16_t SCL, uint16_t BL ): RESX(RESX),DCX(DCX), CSX(NSS),SDA(MOSI),SCL(SCL),BL(BL){
  tim.init();
  port po;
  pin pi;
  
  // Backlight
  po = GPIO::getPort(BL);
  pi = GPIO::getPin(BL);
  rcc_periph_clock_enable(RCC_PORT[po]);
  gpio_mode_setup(GPIO_PORT[po],GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,GPIO_PIN[pi]);

  // Reset Pin
  po = GPIO::getPort(RESX);
  pi = GPIO::getPin(RESX);
  rcc_periph_clock_enable(RCC_PORT[po]);
  gpio_mode_setup(GPIO_PORT[po],GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,GPIO_PIN[pi]);
  
  // Data Command Identifier
  // DC:=0 (Command), DC:=1 (Data)
  po = GPIO::getPort(DCX);
  pi = GPIO::getPin(DCX);
  rcc_periph_clock_enable(RCC_PORT[po]);
  gpio_mode_setup(GPIO_PORT[po],GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,GPIO_PIN[pi]);
  
  // NSS
  po = GPIO::getPort(CSX);
  pi = GPIO::getPin(CSX);
  rcc_periph_clock_enable(RCC_PORT[po]);
  gpio_mode_setup(GPIO_PORT[po],GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN[pi]);
  gpio_set_af(GPIO_PORT[po],GPIO_AF5,GPIO_PIN[pi]);

  // MOSI
  po = GPIO::getPort(SDA);
  pi = GPIO::getPin(SDA);
  rcc_periph_clock_enable(RCC_PORT[po]);
  gpio_mode_setup(GPIO_PORT[po],GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN[pi]);
  gpio_set_af(GPIO_PORT[po],GPIO_AF5,GPIO_PIN[pi]);

  // SCLK
  po = GPIO::getPort(SCL);
  pi = GPIO::getPin(SCL);
  rcc_periph_clock_enable(RCC_PORT[po]);
  gpio_mode_setup(GPIO_PORT[po],GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN[pi]);
  gpio_set_af(GPIO_PORT[po],GPIO_AF5,GPIO_PIN[pi]);
}

void lcd::spi_setup(){
  rcc_periph_reset_pulse(RST_SPI1);
  rcc_periph_clock_enable(RCC_SPI1);
  spi_init_master(SPI1,SPI_CR1_BAUDRATE_FPCLK_DIV_16,SPI_CR1_CPOL_CLK_TO_1_WHEN_IDLE,SPI_CR1_CPHA_CLK_TRANSITION_1,SPI_CR1_DFF_8BIT,SPI_CR1_MSBFIRST);
}

void lcd::send_command(uint8_t command){
  spi_enable(SPI1);
  spi_send(SPI1,command);
  while (!(SPI_SR(SPI1) & SPI_SR_TXE)); // Wait for transfer to finish before disabling
  spi_disable(SPI1);
}

void lcd::send_data(uint8_t data){
  spi_enable(SPI1);
  spi_send(SPI1,data);
  while (!(SPI_SR(SPI1) & SPI_SR_TXE)); // Wait for transfer to finish before disabling
  spi_disable(SPI1);
}

void lcd::init_sequence(){
  port dcxPort = GPIO::getPort(DCX);
  pin dcxPin = GPIO::getPin(DCX);
  
  TRANSMIT_COMMAND(GPIO_PORT[dcxPort],GPIO_PIN[dcxPin])
  send_command(SWRESET);
  tim.delayMs(120);

  TRANSMIT_COMMAND(GPIO_PORT[dcxPort],GPIO_PIN[dcxPin])
  send_command(SLPOUT);
  tim.delayMs(255);

  TRANSMIT_COMMAND(GPIO_PORT[dcxPort],GPIO_PIN[dcxPin])
  send_command(COLMOD);
  TRANSMIT_DATA(GPIO_PORT[dcxPort],GPIO_PIN[dcxPin]);
  send_data(0x55); //65k colors @16 bits
  tim.delayMs(10);

  TRANSMIT_COMMAND(GPIO_PORT[dcxPort],GPIO_PIN[dcxPin])
  send_command(MADCTL);
  TRANSMIT_DATA(GPIO_PORT[dcxPort],GPIO_PIN[dcxPin])
  send_data(0);
  tim.delayMs(10);

  TRANSMIT_COMMAND(GPIO_PORT[dcxPort],GPIO_PIN[dcxPin])
  send_command(CASET);
  TRANSMIT_DATA(GPIO_PORT[dcxPort],GPIO_PIN[dcxPin])
  send_data(0);
  send_data(0);
  send_data(TFT_WIDTH >> 8);
  send_data(TFT_WIDTH & 0xFF);

  TRANSMIT_COMMAND(GPIO_PORT[dcxPort],GPIO_PIN[dcxPin])
  send_command(RASET);
  TRANSMIT_DATA(GPIO_PORT[dcxPort],GPIO_PIN[dcxPin])
  send_data(0);
  send_data(0);
  send_data(TFT_HEIGHT >> 8);
  send_data(TFT_HEIGHT & 0xFF);

  TRANSMIT_COMMAND(GPIO_PORT[dcxPort],GPIO_PIN[dcxPin])
  gpio_clear(GPIOB,GPIO5);
  send_command(INVON);
  tim.delayMs(10);

  TRANSMIT_COMMAND(GPIO_PORT[dcxPort],GPIO_PIN[dcxPin])
  send_command(NORON);
  tim.delayMs(10);

  TRANSMIT_COMMAND(GPIO_PORT[dcxPort],GPIO_PIN[dcxPin])
  send_command(DISPON);
  tim.delayMs(10);
}

void lcd::start(){
  port po = GPIO::getPort(BL);
  pin pi = GPIO::getPin(BL);
  spi_setup();
  hw_reset();
  init_sequence();
  BL_ON(GPIO_PORT[po],GPIO_PIN[pi]);
}

void lcd::hw_reset(){
  port po = GPIO::getPort(RESX);
  pin pi = GPIO::getPin(RESX);
  tim.setPeriodus(10);
  gpio_set(GPIO_PORT[po],GPIO_PIN[pi]);
  tim.startTimer();
  while(tim.shouldLoop){}
  tim.setPeriodus(40);
  gpio_clear(GPIO_PORT[po],GPIO_PIN[pi]);
  tim.startTimer();
  while(tim.shouldLoop){}
  gpio_set(GPIO_PORT[po],GPIO_PIN[pi]);
}

void lcd::set_rect(uint16_t ys, uint16_t ye, uint16_t xs, uint16_t xe){
  
  port dcxPort = GPIO::getPort(DCX);
  pin dcxPin = GPIO::getPin(DCX);

  TRANSMIT_COMMAND(GPIO_PORT[dcxPort],GPIO_PIN[dcxPin])
  send_command(CASET);
  TRANSMIT_DATA(GPIO_PORT[dcxPort],GPIO_PIN[dcxPin])
  send_data(xs >> 8 );
  send_data(xs & 0xFF);
  send_data(xe >> 8);
  send_data(xe & 0xFF);

  TRANSMIT_COMMAND(GPIO_PORT[dcxPort],GPIO_PIN[dcxPin])
  send_command(RASET);
  TRANSMIT_DATA(GPIO_PORT[dcxPort],GPIO_PIN[dcxPin])
  send_data(ys >> 8);
  send_data(ys & 0xFF);
  send_data(ye >> 8);
  send_data(ye & 0xFF);
  
  TRANSMIT_COMMAND(GPIO_PORT[dcxPort],GPIO_PIN[dcxPin])
  send_command(RAMWR);
  TRANSMIT_DATA(GPIO_PORT[dcxPort],GPIO_PIN[dcxPin]);
}
void lcd::fill_screen( COLOR col ){

  set_rect(0,TFT_HEIGHT-1,0,TFT_WIDTH-1);
  spi_set_dff_16bit(SPI1);
  spi_enable(SPI1);

  for (int i = 0 ; i < TFT_HEIGHT ; i++){
    for (int j = 0 ; j < TFT_WIDTH ; j++){
      spi_send(SPI1,col);
    }
  }
  spi_disable(SPI1);
  spi_set_dff_8bit(SPI1);
}


