/**
  ******************************************************************************
  * @file      startup_stm32f446xx.s
  * @author    MCD Application Team, NidhishSingh68
  * @brief     STM32F446xx Devices vector table for GCC based toolchains. 
  *            This module performs:
  *                - Set the initial SP
  *                - Set the initial PC == Reset_Handler,
  *                - Set the vector table entries with the exceptions ISR address
  *                - Branches to main in the C library (which eventually
  *                  calls main()).
  *            After Reset the Cortex-M4 processor is in Thread mode,
  *            priority is Privileged, and the Stack is set to Main.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
    
  .syntax unified
  .cpu cortex-m4
  .fpu softvfp
  .thumb

.global  g_pfnVectors
.global  Default_Handler

/* start address for the initialization values of the .data section. 
defined in linker script */
.word  _data_loadaddr
/* start address for the .data section. defined in linker script */  
.word  _data
/* end address for the .data section. defined in linker script */
.word  _edata
/* start address for the .bss section. defined in linker script */
.word  _sbss
/* end address for the .bss section. defined in linker script */
.word  _ebss
/* stack used for SystemInit_ExtMemCtl; always internal RAM used */

/**
 * @brief  This is the code that gets called when the processor first
 *          starts execution following a reset event. Only the absolutely
 *          necessary set is performed, after which the application
 *          supplied main() routine is called. 
 * @param  None
 * @retval : None
*/

    .section  .text.reset_handler
  .weak  reset_handler
  .type  reset_handler, %function
reset_handler:  
  ldr   sp, =_stack      /* set stack pointer */
  
/* Copy the data segment initializers from flash to SRAM */  
  ldr r0, =_data
  ldr r1, =_edata
  ldr r2, = _data_loadaddr
  movs r3, #0
  b LoopCopyDataInit

CopyDataInit:
  ldr r4, [r2, r3]
  str r4, [r0, r3]
  adds r3, r3, #4

LoopCopyDataInit:
  adds r4, r0, r3
  cmp r4, r1
  bcc CopyDataInit
  
/* Zero fill the bss segment. */
  ldr r2, =_sbss
  ldr r4, =_ebss
  movs r3, #0
  b LoopFillZerobss

FillZerobss:
  str  r3, [r2]
  adds r2, r2, #4

LoopFillZerobss:
  cmp r2, r4
  bcc FillZerobss
  
/* Call static constructors */
    bl __libc_init_array
/* Call the application's entry point.*/
  bl  main
  bx  lr    
.size  reset_handler, .-reset_handler

/**
 * @brief  This is the code that gets called when the processor receives an 
 *         unexpected interrupt.  This simply enters an infinite loop, preserving
 *         the system state for examination by a debugger.
 * @param  None     
 * @retval None       
*/
    .section  .text.Default_Handler,"ax",%progbits
Default_Handler:
Infinite_Loop:
  b  Infinite_Loop
  .size  Default_Handler, .-Default_Handler
/******************************************************************************
*
* The minimal vector table for a Cortex M3. Note that the proper constructs
* must be placed on this to ensure that it ends up at physical address
* 0x0000.0000.
* 
*******************************************************************************/
   .section  .isr_vector,"a",%progbits
  .type  g_pfnVectors, %object
   
   
g_pfnVectors:
  .word  _stack
  .word  reset_handler

  /* Cortex-M exceptions */
  .word  nmi_isr
  .word  hard_fault_isr
  .word  mem_manage_isr
  .word  bus_fault_isr
  .word  usage_fault_isr
  .word  0
  .word  0
  .word  0
  .word  0
  .word  sv_call_isr
  .word  debug_monitor_isr
  .word  0
  .word  pend_sv_isr
  .word  sys_tick_isr

  /* External interrupts */
  .word  wwdg_isr
  .word  pvd_isr
  .word  tamp_stamp_isr
  .word  rtc_wkup_isr
  .word  flash_isr
  .word  rcc_isr
  .word  exti0_isr
  .word  exti1_isr
  .word  exti2_isr
  .word  exti3_isr
  .word  exti4_isr
  .word  dma1_stream0_isr
  .word  dma1_stream1_isr
  .word  dma1_stream2_isr
  .word  dma1_stream3_isr
  .word  dma1_stream4_isr
  .word  dma1_stream5_isr
  .word  dma1_stream6_isr
  .word  adc_isr
  .word  can1_tx_isr
  .word  can1_rx0_isr
  .word  can1_rx1_isr
  .word  can1_sce_isr
  .word  exti9_5_isr
  .word  tim1_brk_tim9_isr
  .word  tim1_up_tim10_isr
  .word  tim1_trg_com_tim11_isr
  .word  tim1_cc_isr
  .word  tim2_isr
  .word  tim3_isr
  .word  tim4_isr
  .word  i2c1_ev_isr
  .word  i2c1_er_isr
  .word  i2c2_ev_isr
  .word  i2c2_er_isr
  .word  spi1_isr
  .word  spi2_isr
  .word  usart1_isr
  .word  usart2_isr
  .word  usart3_isr
  .word  exti15_10_isr
  .word  rtc_alarm_isr
  .word  otg_fs_wkup_isr
  .word  tim8_brk_tim12_isr
  .word  tim8_up_tim13_isr
  .word  tim8_trg_com_tim14_isr
  .word  tim8_cc_isr
  .word  dma1_stream7_isr
  .word  fmc_isr
  .word  sdio_isr
  .word  tim5_isr
  .word  spi3_isr
  .word  uart4_isr
  .word  uart5_isr
  .word  tim6_dac_isr
  .word  tim7_isr
  .word  dma2_stream0_isr
  .word  dma2_stream1_isr
  .word  dma2_stream2_isr
  .word  dma2_stream3_isr
  .word  dma2_stream4_isr
  .word  0
  .word  0
  .word  can2_tx_isr
  .word  can2_rx0_isr
  .word  can2_rx1_isr
  .word  can2_sce_isr
  .word  otg_fs_isr
  .word  dma2_stream5_isr
  .word  dma2_stream6_isr
  .word  dma2_stream7_isr
  .word  usart6_isr
  .word  i2c3_ev_isr
  .word  i2c3_er_isr
  .word  otg_hs_ep1_out_isr
  .word  otg_hs_ep1_in_isr
  .word  otg_hs_wkup_isr
  .word  otg_hs_isr
  .word  dcmi_isr
  .word  0
  .word  0
  .word  fpu_isr
  .word  0
  .word  0
  .word  spi4_isr
  .word  0
  .word  0
  .word  sai1_isr
  .word  0
  .word  0
  .word  0
  .word  sai2_isr
  .word  quadspi_isr
  .word  cec_isr
  .word  spdif_rx_isr
  .word  fmpi2c1_ev_isr
  .word  fmpi2c1_er_isr

  .size g_pfnVectors, .-g_pfnVectors

/*******************************************************************************
*
* Provide weak aliases for each Exception handler to the Default_Handler. 
* As they are weak aliases, any function with the same name will override 
* this definition.
* 
*******************************************************************************/

.weak nmi_isr
.thumb_set nmi_isr, Default_Handler

.weak hard_fault_isr
.thumb_set hard_fault_isr, Default_Handler

.weak mem_manage_isr
.thumb_set mem_manage_isr, Default_Handler

.weak bus_fault_isr
.thumb_set bus_fault_isr, Default_Handler

.weak usage_fault_isr
.thumb_set usage_fault_isr, Default_Handler

.weak sv_call_isr
.thumb_set sv_call_isr, Default_Handler

.weak debug_monitor_isr
.thumb_set debug_monitor_isr, Default_Handler

.weak pend_sv_isr
.thumb_set pend_sv_isr, Default_Handler

.weak sys_tick_isr
.thumb_set sys_tick_isr, Default_Handler


/* External interrupts */
.weak wwdg_isr
.thumb_set wwdg_isr, Default_Handler

.weak pvd_isr
.thumb_set pvd_isr, Default_Handler

.weak tamp_stamp_isr
.thumb_set tamp_stamp_isr, Default_Handler

.weak rtc_wkup_isr
.thumb_set rtc_wkup_isr, Default_Handler

.weak flash_isr
.thumb_set flash_isr, Default_Handler

.weak rcc_isr
.thumb_set rcc_isr, Default_Handler

.weak exti0_isr
.thumb_set exti0_isr, Default_Handler

.weak exti1_isr
.thumb_set exti1_isr, Default_Handler

.weak exti2_isr
.thumb_set exti2_isr, Default_Handler

.weak exti3_isr
.thumb_set exti3_isr, Default_Handler

.weak exti4_isr
.thumb_set exti4_isr, Default_Handler

.weak dma1_stream0_isr
.thumb_set dma1_stream0_isr, Default_Handler

.weak dma1_stream1_isr
.thumb_set dma1_stream1_isr, Default_Handler

.weak dma1_stream2_isr
.thumb_set dma1_stream2_isr, Default_Handler

.weak dma1_stream3_isr
.thumb_set dma1_stream3_isr, Default_Handler

.weak dma1_stream4_isr
.thumb_set dma1_stream4_isr, Default_Handler

.weak dma1_stream5_isr
.thumb_set dma1_stream5_isr, Default_Handler

.weak dma1_stream6_isr
.thumb_set dma1_stream6_isr, Default_Handler

.weak adc_isr
.thumb_set adc_isr, Default_Handler

.weak can1_tx_isr
.thumb_set can1_tx_isr, Default_Handler

.weak can1_rx0_isr
.thumb_set can1_rx0_isr, Default_Handler

.weak can1_rx1_isr
.thumb_set can1_rx1_isr, Default_Handler

.weak can1_sce_isr
.thumb_set can1_sce_isr, Default_Handler

.weak exti9_5_isr
.thumb_set exti9_5_isr, Default_Handler

.weak tim1_brk_tim9_isr
.thumb_set tim1_brk_tim9_isr, Default_Handler

.weak tim1_up_tim10_isr
.thumb_set tim1_up_tim10_isr, Default_Handler

.weak tim1_trg_com_tim11_isr
.thumb_set tim1_trg_com_tim11_isr, Default_Handler

.weak tim1_cc_isr
.thumb_set tim1_cc_isr, Default_Handler

.weak tim2_isr
.thumb_set tim2_isr, Default_Handler

.weak tim3_isr
.thumb_set tim3_isr, Default_Handler

.weak tim4_isr
.thumb_set tim4_isr, Default_Handler

.weak i2c1_ev_isr
.thumb_set i2c1_ev_isr, Default_Handler

.weak i2c1_er_isr
.thumb_set i2c1_er_isr, Default_Handler

.weak i2c2_ev_isr
.thumb_set i2c2_ev_isr, Default_Handler

.weak i2c2_er_isr
.thumb_set i2c2_er_isr, Default_Handler

.weak spi1_isr
.thumb_set spi1_isr, Default_Handler

.weak spi2_isr
.thumb_set spi2_isr, Default_Handler

.weak usart1_isr
.thumb_set usart1_isr, Default_Handler

.weak usart2_isr
.thumb_set usart2_isr, Default_Handler

.weak usart3_isr
.thumb_set usart3_isr, Default_Handler

.weak exti15_10_isr
.thumb_set exti15_10_isr, Default_Handler

.weak rtc_alarm_isr
.thumb_set rtc_alarm_isr, Default_Handler

.weak otg_fs_wkup_isr
.thumb_set otg_fs_wkup_isr, Default_Handler

.weak tim8_brk_tim12_isr
.thumb_set tim8_brk_tim12_isr, Default_Handler

.weak tim8_up_tim13_isr
.thumb_set tim8_up_tim13_isr, Default_Handler

.weak tim8_trg_com_tim14_isr
.thumb_set tim8_trg_com_tim14_isr, Default_Handler

.weak tim8_cc_isr
.thumb_set tim8_cc_isr, Default_Handler

.weak dma1_stream7_isr
.thumb_set dma1_stream7_isr, Default_Handler

.weak fmc_isr
.thumb_set fmc_isr, Default_Handler

.weak sdio_isr
.thumb_set sdio_isr, Default_Handler

.weak tim5_isr
.thumb_set tim5_isr, Default_Handler

.weak spi3_isr
.thumb_set spi3_isr, Default_Handler

.weak uart4_isr
.thumb_set uart4_isr, Default_Handler

.weak uart5_isr
.thumb_set uart5_isr, Default_Handler

.weak tim6_dac_isr
.thumb_set tim6_dac_isr, Default_Handler

.weak tim7_isr
.thumb_set tim7_isr, Default_Handler

.weak dma2_stream0_isr
.thumb_set dma2_stream0_isr, Default_Handler

.weak dma2_stream1_isr
.thumb_set dma2_stream1_isr, Default_Handler

.weak dma2_stream2_isr
.thumb_set dma2_stream2_isr, Default_Handler

.weak dma2_stream3_isr
.thumb_set dma2_stream3_isr, Default_Handler

.weak dma2_stream4_isr
.thumb_set dma2_stream4_isr, Default_Handler

.weak can2_tx_isr
.thumb_set can2_tx_isr, Default_Handler

.weak can2_rx0_isr
.thumb_set can2_rx0_isr, Default_Handler

.weak can2_rx1_isr
.thumb_set can2_rx1_isr, Default_Handler

.weak can2_sce_isr
.thumb_set can2_sce_isr, Default_Handler

.weak otg_fs_isr
.thumb_set otg_fs_isr, Default_Handler

.weak dma2_stream5_isr
.thumb_set dma2_stream5_isr, Default_Handler

.weak dma2_stream6_isr
.thumb_set dma2_stream6_isr, Default_Handler

.weak dma2_stream7_isr
.thumb_set dma2_stream7_isr, Default_Handler

.weak usart6_isr
.thumb_set usart6_isr, Default_Handler

.weak i2c3_ev_isr
.thumb_set i2c3_ev_isr, Default_Handler

.weak i2c3_er_isr
.thumb_set i2c3_er_isr, Default_Handler

.weak otg_hs_ep1_out_isr
.thumb_set otg_hs_ep1_out_isr, Default_Handler

.weak otg_hs_ep1_in_isr
.thumb_set otg_hs_ep1_in_isr, Default_Handler

.weak otg_hs_wkup_isr
.thumb_set otg_hs_wkup_isr, Default_Handler

.weak otg_hs_isr
.thumb_set otg_hs_isr, Default_Handler

.weak dcmi_isr
.thumb_set dcmi_isr, Default_Handler

.weak fpu_isr
.thumb_set fpu_isr, Default_Handler

.weak spi4_isr
.thumb_set spi4_isr, Default_Handler

.weak sai1_isr
.thumb_set sai1_isr, Default_Handler

.weak sai2_isr
.thumb_set sai2_isr, Default_Handler

.weak quadspi_isr
.thumb_set quadspi_isr, Default_Handler

.weak cec_isr
.thumb_set cec_isr, Default_Handler

.weak spdif_rx_isr
.thumb_set spdif_rx_isr, Default_Handler

.weak fmpi2c1_ev_isr
.thumb_set fmpi2c1_ev_isr, Default_Handler

.weak fmpi2c1_er_isr
.thumb_set fmpi2c1_er_isr, Default_Handler
