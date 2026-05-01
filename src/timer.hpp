#pragma once
#include <cstdint>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/nvic.h>

#define ONEMHZ 1000000

enum TIMER{
  T1,
  T2,
  T3,
  T4,
  T5,
  T6,
  T7,
  T8
};

namespace TIM{
  const rcc_periph_clken RCC_TIM[] = {RCC_TIM1,RCC_TIM2,RCC_TIM3,RCC_TIM3,RCC_TIM4,RCC_TIM4,RCC_TIM5,RCC_TIM6,RCC_TIM7,RCC_TIM8};
  const uint32_t TIMER[] = {TIM1,TIM2,TIM3,TIM4,TIM5,TIM6,TIM7,TIM8};
  const uint32_t NVIC_TIM_IRQ[] = {NVIC_TIM1_CC_IRQ,NVIC_TIM2_IRQ,NVIC_TIM2_IRQ,NVIC_TIM3_IRQ,NVIC_TIM4_IRQ}; // TODO: Add IRQ's for remaining timers
};

class timer{
  public:
    volatile uint8_t shouldLoop;
    TIMER tim;
    unsigned int bus_freq_mhz;

    timer( TIMER tim );

    void init();
    // UEV interrupt generated after ms milliseconds
    void setPeriodms( uint32_t ms );
    // UEV interrupt generated after us microseconds
    void setPeriodus( uint32_t us );
    void startTimer();
    void stopTimer();

    void delayUs(uint32_t us);
    void delayMs(uint32_t ms);
};
