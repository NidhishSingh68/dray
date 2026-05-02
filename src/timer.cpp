#include "timer.hpp"

timer::timer( TIMER tim ) : tim(tim){}

void timer::init(){
  bus_freq_mhz = 2*rcc_apb1_frequency/ONEMHZ;

  // Configures the timer in upcounting mode
  rcc_periph_clock_enable(TIM::RCC_TIM[tim]);
  timer_direction_up(TIM::TIMER[tim]);
  nvic_enable_irq(TIM::NVIC_TIM_IRQ[tim]);
  timer_enable_update_event(TIM::TIMER[tim]);
  timer_enable_irq(TIM::TIMER[tim],TIM_DIER_UIE);
  timer_set_prescaler(TIM::TIMER[tim],(bus_freq_mhz-1)); 
  timer_disable_preload(TIM::TIMER[tim]);
  timer_generate_event(TIM::TIMER[tim],TIM_EGR_UG);
}

void timer::setPeriodms( uint32_t ms ){
  timer_set_counter(TIM::TIMER[tim],0);
  timer_set_period(TIM::TIMER[tim], (1000*ms)-1);
}

void timer::setPeriodus( uint32_t us ){
  timer_set_counter(TIM::TIMER[tim],0);
  timer_set_period(TIM::TIMER[tim], us-1);
}

void timer::startTimer(){
  timer_enable_counter(TIM::TIMER[tim]);
  shouldLoop = true;
}

void timer::stopTimer(){
  timer_disable_counter(TIM::TIMER[tim]);
  shouldLoop = false;
}

void timer::delayUs( uint32_t us ){
  setPeriodus(us);
  startTimer();
  while(shouldLoop){}
}

void timer::delayMs( uint32_t ms ){
  setPeriodms(ms);
  startTimer();
  while(shouldLoop){}
}
