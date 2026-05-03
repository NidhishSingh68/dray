#include "lcd.hpp"

//----------------------------------------------------------- 
namespace {
  timer tim(TIMER::T2);
}

extern "C" void tim2_isr(){
  timer_clear_flag(TIM::TIMER[tim.tim],TIM_SR_UIF);
  tim.stopTimer();
  tim.shouldLoop = false;
}
//----------------------------------------------------------- 

// Private Functions
void lcd::spi_setup(){
  rcc_periph_reset_pulse(RST_SPI1);
  rcc_periph_clock_enable(RCC_SPI1);
  spi_init_master(SPI1,SPI_CR1_BAUDRATE_FPCLK_DIV_8,SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE,SPI_CR1_CPHA_CLK_TRANSITION_1,SPI_CR1_DFF_8BIT,SPI_CR1_MSBFIRST);
  spi_enable_software_slave_management(SPI1);
  spi_set_nss_high(SPI1);
  spi_enable(SPI1);
}

void lcd::hw_reset(){
  port po = GPIO::getPort(RESX);
  pin pi = GPIO::getPin(RESX);
  gpio_set(GPIO_PORT[po],GPIO_PIN[pi]);
  tim.delayMs(10);
  gpio_clear(GPIO_PORT[po],GPIO_PIN[pi]);
  tim.delayMs(20);
  gpio_set(GPIO_PORT[po],GPIO_PIN[pi]);
  tim.delayMs(120);
}

void lcd::select(){
  port csPort = GPIO::getPort(CSX);
  pin csPin = GPIO::getPin(CSX);

  CS_ON(GPIO_PORT[csPort],GPIO_PIN[csPin]) 
}

void lcd::deselect(){
  while (!(SPI_SR(SPI1) & SPI_SR_BSY));

  port csPort = GPIO::getPort(CSX);
  pin csPin = GPIO::getPin(CSX);

  CS_OFF(GPIO_PORT[csPort],GPIO_PIN[csPin]) 
}

void lcd::init_sequence(){
  
  select();
  send_command(SWRESET);
  deselect();
  tim.delayMs(120);

  select();
  send_command(SLPOUT);
  deselect();
  tim.delayMs(255);

  select();
  send_command(COLMOD);
  send_data(0x55); //65k colors @16 bits
  deselect();

  select();
  send_command(MADCTL);
  send_data(0);
  deselect();

  select();
  send_command(INVON);
  deselect();

  select();
  send_command(NORON);
  deselect();

  select();
  send_command(DISPON);
  deselect();
  tim.delayMs(100);

}

void lcd::send_command(uint8_t command){
  port dcxPort = GPIO::getPort(DCX);
  pin dcxPin = GPIO::getPin(DCX);

  TRANSMIT_COMMAND(GPIO_PORT[dcxPort],GPIO_PIN[dcxPin])
  spi_send(SPI1,command);
}

void lcd::send_data(uint8_t data){
  port dcxPort = GPIO::getPort(DCX);
  pin dcxPin = GPIO::getPin(DCX);

  TRANSMIT_DATA(GPIO_PORT[dcxPort],GPIO_PIN[dcxPin])
  spi_send(SPI1,data);
}

void lcd::set_rect(uint16_t ys, uint16_t ye, uint16_t xs, uint16_t xe){
  select();
  send_command(CASET);
  send_data(xs>>8);
  send_data(xs & 0xFF);
  send_data(xe>>8);
  send_data(xe & 0xFF);

  send_command(RASET);
  send_data(ys >> 8 );
  send_data(ys & 0xFF);
  send_data(ye >> 8 );
  send_data(ye & 0xFF);
  deselect();
}

void lcd::fill_rect( uint16_t ys, uint16_t ye, uint16_t xs, uint16_t xe ){
  uint16_t width = xe - xs;  
  uint16_t height = ye - ys;

  select();
  send_command(RAMWR);
  for (int i = 0 ; i < width ; i++){
    for (int j = 0 ; j < height ; j++){
      send_data(render_color>>8);
      send_data(render_color & 0xFF);
    }
  }
  deselect();  
}

// Public Functions
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
  
  // NSS: Managed Manually
  po = GPIO::getPort(CSX);
  pi = GPIO::getPin(CSX);
  rcc_periph_clock_enable(RCC_PORT[po]);
  gpio_mode_setup(GPIO_PORT[po],GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,GPIO_PIN[pi]);

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

void lcd::start(){
  port po = GPIO::getPort(BL);
  pin pi = GPIO::getPin(BL);
  spi_setup();
  hw_reset();
  init_sequence();
  BL_ON(GPIO_PORT[po],GPIO_PIN[pi]);
}

void lcd::set_color( COLOR col ){
  render_color = col;
}

void lcd::fill_screen(){
  set_rect(0,TFT_HEIGHT-1,0,TFT_WIDTH-1);
  fill_rect(0,TFT_HEIGHT,0,TFT_WIDTH); 
}

void lcd::draw_rect( uint16_t ys, uint16_t ye, uint16_t xs, uint16_t xe ){
  set_rect( ys, ye, xs, xe );
  fill_rect( ys, ye, xs, xe );
}

