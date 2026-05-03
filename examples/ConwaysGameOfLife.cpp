#include <libopencm3/stm32/rcc.h>
#include "lcd.hpp"
#include <bitset>

#define HEIGHT 320 
#define WIDHT 240

#define GAP 2
#define BOXSIZE 8 

#define ROWS HEIGHT/(BOXSIZE+GAP)
#define COLS WIDHT/(BOXSIZE+GAP)

std::bitset<ROWS*COLS> front;
std::bitset<ROWS*COLS> back;

int mod(int a, int b)
{
    int r = a % b;
    return r < 0 ? r + b : r;
}

uint8_t countNeightbours(int row, int col){
  uint8_t count = 0;
  int lowerN = mod(row+1,ROWS);
  int upperN = mod(row-1,ROWS);
  int rightN = mod(col+1,COLS);
  int leftN = mod(col-1,COLS);

  // Right Neightbour
  if ( front[(row*COLS)+rightN] ) count++;
  // Left Neightbour
  if ( front[(row*COLS)+leftN] ) count++;
  // Upper Neightbour
  if ( front[(upperN*COLS)+col] ) count++;
  // Lower Neightbour
  if ( front[(lowerN*COLS)+col] ) count++;
  // Upper Left 
  if ( front[(upperN*COLS)+leftN] ) count++;
  // Upper Right
  if ( front[(upperN*COLS)+rightN] ) count++;
  // Lower Left
  if ( front[(lowerN*COLS)+leftN] ) count++;
  // Lower Right
  if ( front[(lowerN*COLS)+rightN] ) count++;

  return count;
}

void updateGrid(){
  for ( int i = 0 ; i < ROWS ; i++){
    for (int j = 0 ; j < COLS ; j++){
      int aliveNegihbours = countNeightbours(i,j);
      if (front[i*COLS + j]){
        // Rules For Alive Cells

        if ( aliveNegihbours < 2 ){
          // Die due to underpopulation 
          back[i*COLS + j] = false;
        } else if ( aliveNegihbours >=2 && aliveNegihbours <= 3 ){
          // Live if 2 or 3 neightbours alive
          back[i*COLS + j] = true;
        } else if ( aliveNegihbours > 3 ){
          // Die due to overpopulation 
          back[i*COLS + j] = false;
        }
      } else{
        // Rules For Dead Cells

        if (aliveNegihbours  == 3){
          // Come Alive due to reproduction
          back[i*COLS + j] = true;
        }
      }
    }
  }
  // Transfer content into framebuffer 
  front.reset();
  front |= back;
}


void renderGrid( lcd& disp ){

  for (int i = 0 ; i<ROWS;i++){
    for (int j = 0; j < COLS;j++){
      unsigned int xs = (j*(BOXSIZE+GAP)) + GAP;
      unsigned int ys = (i*(BOXSIZE+GAP)) + GAP;
      unsigned int xe = xs + (BOXSIZE);
      unsigned int ye = ys + (BOXSIZE);

      if ( front[i*COLS+ j]){ 
        disp.set_color(COLOR::RED);
        disp.draw_rect(ys,ye, xs,xe);
      }
      else{
        disp.set_color(COLOR::WHITE);
        disp.draw_rect(ys,ye, xs,xe);
      }
    }
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

    lcd disp(GPIO::setPortPin(port::PORTA,pin::PIN9),GPIO::setPortPin(port::PORTA,pin::PIN4),GPIO::setPortPin(port::PORTB,pin::PIN5),GPIO::setPortPin(port::PORTA,pin::PIN7),GPIO::setPortPin(port::PORTA,pin::PIN5),GPIO::setPortPin(port::PORTA,pin::PIN10));
    disp.start();

    //Glider
    front[((ROWS/2)*COLS)+COLS/2] = 1;
    front[((ROWS/2)*COLS)+(COLS/2)+1] = 1;
    front[((ROWS/2)*COLS)+(COLS/2)+2] = 1;
    front[(((ROWS/2)-1)*COLS)+(COLS/2)+2] = 1;
    front[(((ROWS/2)-2)*COLS)+(COLS/2)+1] = 1;

    // Spinner
    front[(((ROWS/2)+5)*COLS)+COLS/2] = 1;
    front[(((ROWS/2)+6)*COLS)+COLS/2] = 1;
    front[(((ROWS/2)+7)*COLS)+COLS/2] = 1;
    
    disp.set_color(COLOR::BLACK);
    disp.fill_screen();

    while(true){
      renderGrid(disp);
      updateGrid();
    }

}
