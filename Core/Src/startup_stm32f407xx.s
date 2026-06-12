/**
  ******************************************************************************
  * @file      startup_stm32f407xx.s
  * @brief     STM32F407xx vector table for GCC toolchain
  *
  * Stack size: 1 KB
  * Heap size:  512 bytes
  ******************************************************************************
  */

    .syntax unified
    .cpu cortex-m4
    .fpu softvfp
    .thumb

.global g_pfnVectors
.global Default_Handler

/* Import external symbols */
.word _sidata
.word _sdata
.word _edata
.word _sbss
.word _ebss

/**
 * @brief  Stack and heap definitions
 */
    .section .stack
    .align 3
    .equ    Stack_Size, 0x400

    .globl  __StackTop
    .globl  __StackLimit
__StackLimit:
    .space  Stack_Size
    .size   __StackLimit, . - __StackLimit
__StackTop:
    .size   __StackTop, . - __StackTop

    .section .heap
    .align 3
    .equ    Heap_Size, 0x200

    .globl  __HeapBase
    .globl  __HeapLimit
__HeapBase:
    .if Heap_Size
    .space  Heap_Size
    .endif
    .size   __HeapBase, . - __HeapBase
__HeapLimit:
    .size   __HeapLimit, . - __HeapLimit

/**
 * @brief  Reset handler
 *         Copies .data section from FLASH to RAM, zeros .bss,
 *         then calls SystemInit() and main().
 */
    .section .isr_vector, "a", %progbits
    .type g_pfnVectors, %object
    .size g_pfnVectors, .-g_pfnVectors

g_pfnVectors:
    .word  _estack                     /* Initial stack pointer */
    .word  Reset_Handler               /* Reset Handler */
    .word  NMI_Handler                 /* NMI Handler */
    .word  HardFault_Handler           /* Hard Fault Handler */
    .word  MemManage_Handler           /* MPU Fault Handler */
    .word  BusFault_Handler            /* Bus Fault Handler */
    .word  UsageFault_Handler          /* Usage Fault Handler */
    .word  0                           /* Reserved */
    .word  0                           /* Reserved */
    .word  0                           /* Reserved */
    .word  0                           /* Reserved */
    .word  SVC_Handler                 /* SVCall Handler */
    .word  DebugMon_Handler            /* Debug Monitor Handler */
    .word  0                           /* Reserved */
    .word  PendSV_Handler              /* PendSV Handler */
    .word  SysTick_Handler             /* SysTick Handler */

    /* External Interrupts */
    .word  WWDG_IRQHandler             /* Window WatchDog */
    .word  PVD_IRQHandler              /* PVD through EXTI Line detection */
    .word  TAMP_STAMP_IRQHandler       /* Tamper and TimeStamps */
    .word  RTC_WKUP_IRQHandler         /* RTC Wakeup */
    .word  FLASH_IRQHandler            /* FLASH */
    .word  RCC_IRQHandler              /* RCC */
    .word  EXTI0_IRQHandler            /* EXTI Line0 */
    .word  EXTI1_IRQHandler            /* EXTI Line1 */
    .word  EXTI2_IRQHandler            /* EXTI Line2 */
    .word  EXTI3_IRQHandler            /* EXTI Line3 */
    .word  EXTI4_IRQHandler            /* EXTI Line4 */
    .word  DMA1_Stream0_IRQHandler     /* DMA1 Stream 0 */
    .word  DMA1_Stream1_IRQHandler     /* DMA1 Stream 1 */
    .word  DMA1_Stream2_IRQHandler     /* DMA1 Stream 2 */
    .word  DMA1_Stream3_IRQHandler     /* DMA1 Stream 3 */
    .word  DMA1_Stream4_IRQHandler     /* DMA1 Stream 4 */
    .word  DMA1_Stream5_IRQHandler     /* DMA1 Stream 5 */
    .word  DMA1_Stream6_IRQHandler     /* DMA1 Stream 6 */
    .word  ADC_IRQHandler              /* ADC1,2,3 */
    .word  CAN1_TX_IRQHandler          /* CAN1 TX */
    .word  CAN1_RX0_IRQHandler         /* CAN1 RX0 */
    .word  CAN1_RX1_IRQHandler         /* CAN1 RX1 */
    .word  CAN1_SCE_IRQHandler         /* CAN1 SCE */
    .word  EXTI9_5_IRQHandler          /* EXTI Line[9:5] */
    .word  TIM1_BRK_TIM9_IRQHandler    /* TIM1 Break and TIM9 */
    .word  TIM1_UP_TIM10_IRQHandler    /* TIM1 Update and TIM10 */
    .word  TIM1_TRG_COM_TIM11_IRQHandler /* TIM1 Trigger/Commutation and TIM11 */
    .word  TIM1_CC_IRQHandler          /* TIM1 Capture Compare */
    .word  TIM2_IRQHandler             /* TIM2 */
    .word  TIM3_IRQHandler             /* TIM3 */
    .word  TIM4_IRQHandler             /* TIM4 */
    .word  I2C1_EV_IRQHandler          /* I2C1 Event */
    .word  I2C1_ER_IRQHandler          /* I2C1 Error */
    .word  I2C2_EV_IRQHandler          /* I2C2 Event */
    .word  I2C2_ER_IRQHandler          /* I2C2 Error */
    .word  SPI1_IRQHandler             /* SPI1 */
    .word  SPI2_IRQHandler             /* SPI2 */
    .word  USART1_IRQHandler           /* USART1 */
    .word  USART2_IRQHandler           /* USART2 */
    .word  USART3_IRQHandler           /* USART3 */
    .word  EXTI15_10_IRQHandler        /* EXTI Line[15:10] */
    .word  RTC_Alarm_IRQHandler        /* RTC Alarm */
    .word  OTG_FS_WKUP_IRQHandler      /* USB OTG FS Wakeup */
    .word  TIM8_BRK_TIM12_IRQHandler   /* TIM8 Break and TIM12 */
    .word  TIM8_UP_TIM13_IRQHandler    /* TIM8 Update and TIM13 */
    .word  TIM8_TRG_COM_TIM14_IRQHandler /* TIM8 Trigger/Commutation and TIM14 */
    .word  TIM8_CC_IRQHandler          /* TIM8 Capture Compare */
    .word  DMA1_Stream7_IRQHandler     /* DMA1 Stream7 */
    .word  FSMC_IRQHandler             /* FSMC */
    .word  SDIO_IRQHandler             /* SDIO */
    .word  TIM5_IRQHandler             /* TIM5 */
    .word  SPI3_IRQHandler             /* SPI3 */
    .word  UART4_IRQHandler            /* UART4 */
    .word  UART5_IRQHandler            /* UART5 */
    .word  TIM6_DAC_IRQHandler         /* TIM6 and DAC1&2 underrun */
    .word  TIM7_IRQHandler             /* TIM7 */
    .word  DMA2_Stream0_IRQHandler     /* DMA2 Stream 0 */
    .word  DMA2_Stream1_IRQHandler     /* DMA2 Stream 1 */
    .word  DMA2_Stream2_IRQHandler     /* DMA2 Stream 2 */
    .word  DMA2_Stream3_IRQHandler     /* DMA2 Stream 3 */
    .word  DMA2_Stream4_IRQHandler     /* DMA2 Stream 4 */
    .word  0                           /* Reserved */
    .word  0                           /* Reserved */
    .word  CAN2_TX_IRQHandler          /* CAN2 TX */
    .word  CAN2_RX0_IRQHandler         /* CAN2 RX0 */
    .word  CAN2_RX1_IRQHandler         /* CAN2 RX1 */
    .word  CAN2_SCE_IRQHandler         /* CAN2 SCE */
    .word  OTG_FS_IRQHandler           /* USB OTG FS */
    .word  DMA2_Stream5_IRQHandler     /* DMA2 Stream 5 */
    .word  DMA2_Stream6_IRQHandler     /* DMA2 Stream 6 */
    .word  DMA2_Stream7_IRQHandler     /* DMA2 Stream 7 */
    .word  USART6_IRQHandler           /* USART6 */
    .word  I2C3_EV_IRQHandler          /* I2C3 Event */
    .word  I2C3_ER_IRQHandler          /* I2C3 Error */
    .word  OTG_HS_EP1_OUT_IRQHandler   /* USB OTG HS End Point 1 Out */
    .word  OTG_HS_EP1_IN_IRQHandler    /* USB OTG HS End Point 1 In */
    .word  OTG_HS_WKUP_IRQHandler      /* USB OTG HS Wakeup */
    .word  OTG_HS_IRQHandler           /* USB OTG HS */
    .word  DCMI_IRQHandler             /* DCMI */
    .word  0                           /* Reserved */
    .word  HASH_RNG_IRQHandler         /* Hash and Rng */
    .word  FPU_IRQHandler              /* FPU */

/**
 * @brief  Reset Handler
 */
    .section .text.Reset_Handler
    .weak Reset_Handler
    .type Reset_Handler, %function
Reset_Handler:
    /* Copy .data section from FLASH to RAM */
    ldr r0, =_sdata
    ldr r1, =_edata
    ldr r2, =_sidata
    b   LoopCopyDataInit

CopyDataInit:
    ldr r3, [r2], #4
    str r3, [r0], #4

LoopCopyDataInit:
    cmp r0, r1
    bcc CopyDataInit

    /* Zero fill .bss section */
    ldr r2, =_sbss
    ldr r4, =_ebss
    movs r3, #0
    b LoopFillZerobss

FillZerobss:
    str r3, [r2], #4

LoopFillZerobss:
    cmp r2, r4
    bcc FillZerobss

    /* Call system init and main */
    bl  SystemInit
    bl  main
    bx  lr
    .size Reset_Handler, .-Reset_Handler

/**
 * @brief  Default handler for all other interrupts
 */
    .section .text.Default_Handler,"ax",%progbits
Default_Handler:
Infinite_Loop:
    b Infinite_Loop
    .size Default_Handler, .-Default_Handler

/* Weak aliases for all handlers */
    .macro def_irq_handler handler_name
    .weak \handler_name
    .thumb_set \handler_name, Default_Handler
    .endm

    def_irq_handler NMI_Handler
    def_irq_handler HardFault_Handler
    def_irq_handler MemManage_Handler
    def_irq_handler BusFault_Handler
    def_irq_handler UsageFault_Handler
    def_irq_handler SVC_Handler
    def_irq_handler DebugMon_Handler
    def_irq_handler PendSV_Handler
    def_irq_handler SysTick_Handler

    def_irq_handler WWDG_IRQHandler
    def_irq_handler PVD_IRQHandler
    def_irq_handler TAMP_STAMP_IRQHandler
    def_irq_handler RTC_WKUP_IRQHandler
    def_irq_handler FLASH_IRQHandler
    def_irq_handler RCC_IRQHandler
    def_irq_handler EXTI0_IRQHandler
    def_irq_handler EXTI1_IRQHandler
    def_irq_handler EXTI2_IRQHandler
    def_irq_handler EXTI3_IRQHandler
    def_irq_handler EXTI4_IRQHandler
    def_irq_handler DMA1_Stream0_IRQHandler
    def_irq_handler DMA1_Stream1_IRQHandler
    def_irq_handler DMA1_Stream2_IRQHandler
    def_irq_handler DMA1_Stream3_IRQHandler
    def_irq_handler DMA1_Stream4_IRQHandler
    def_irq_handler DMA1_Stream5_IRQHandler
    def_irq_handler DMA1_Stream6_IRQHandler
    def_irq_handler ADC_IRQHandler
    def_irq_handler CAN1_TX_IRQHandler
    def_irq_handler CAN1_RX0_IRQHandler
    def_irq_handler CAN1_RX1_IRQHandler
    def_irq_handler CAN1_SCE_IRQHandler
    def_irq_handler EXTI9_5_IRQHandler
    def_irq_handler TIM1_BRK_TIM9_IRQHandler
    def_irq_handler TIM1_UP_TIM10_IRQHandler
    def_irq_handler TIM1_TRG_COM_TIM11_IRQHandler
    def_irq_handler TIM1_CC_IRQHandler
    def_irq_handler TIM2_IRQHandler
    def_irq_handler TIM3_IRQHandler
    def_irq_handler TIM4_IRQHandler
    def_irq_handler I2C1_EV_IRQHandler
    def_irq_handler I2C1_ER_IRQHandler
    def_irq_handler I2C2_EV_IRQHandler
    def_irq_handler I2C2_ER_IRQHandler
    def_irq_handler SPI1_IRQHandler
    def_irq_handler SPI2_IRQHandler
    def_irq_handler USART1_IRQHandler
    def_irq_handler USART2_IRQHandler
    def_irq_handler USART3_IRQHandler
    def_irq_handler EXTI15_10_IRQHandler
    def_irq_handler RTC_Alarm_IRQHandler
    def_irq_handler OTG_FS_WKUP_IRQHandler
    def_irq_handler TIM8_BRK_TIM12_IRQHandler
    def_irq_handler TIM8_UP_TIM13_IRQHandler
    def_irq_handler TIM8_TRG_COM_TIM14_IRQHandler
    def_irq_handler TIM8_CC_IRQHandler
    def_irq_handler DMA1_Stream7_IRQHandler
    def_irq_handler FSMC_IRQHandler
    def_irq_handler SDIO_IRQHandler
    def_irq_handler TIM5_IRQHandler
    def_irq_handler SPI3_IRQHandler
    def_irq_handler UART4_IRQHandler
    def_irq_handler UART5_IRQHandler
    def_irq_handler TIM6_DAC_IRQHandler
    def_irq_handler TIM7_IRQHandler
    def_irq_handler DMA2_Stream0_IRQHandler
    def_irq_handler DMA2_Stream1_IRQHandler
    def_irq_handler DMA2_Stream2_IRQHandler
    def_irq_handler DMA2_Stream3_IRQHandler
    def_irq_handler DMA2_Stream4_IRQHandler
    def_irq_handler CAN2_TX_IRQHandler
    def_irq_handler CAN2_RX0_IRQHandler
    def_irq_handler CAN2_RX1_IRQHandler
    def_irq_handler CAN2_SCE_IRQHandler
    def_irq_handler OTG_FS_IRQHandler
    def_irq_handler DMA2_Stream5_IRQHandler
    def_irq_handler DMA2_Stream6_IRQHandler
    def_irq_handler DMA2_Stream7_IRQHandler
    def_irq_handler USART6_IRQHandler
    def_irq_handler I2C3_EV_IRQHandler
    def_irq_handler I2C3_ER_IRQHandler
    def_irq_handler OTG_HS_EP1_OUT_IRQHandler
    def_irq_handler OTG_HS_EP1_IN_IRQHandler
    def_irq_handler OTG_HS_WKUP_IRQHandler
    def_irq_handler OTG_HS_IRQHandler
    def_irq_handler DCMI_IRQHandler
    def_irq_handler HASH_RNG_IRQHandler
    def_irq_handler FPU_IRQHandler
