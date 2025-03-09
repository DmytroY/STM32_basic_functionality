## Starting RTOS on NUCLEO-F030R8

1) in Pinout&Configuration, Middleware and Software Packs choose FreeRTOS.
1) Create there two tasks: StartLED1 and StartLED2.
1) configure two pins in GPIO output mode and connect to them LEDs.
1) ATTENTION! FreeRTOS will use SysTick to generate periodic interrupts for task scheduling, If we try to use SysTick for HAL timebase, it can create conflicts and timing issues. So go to SYS Mode and Configuration and change Timebase source from SysTick to one of hardware timers.
1) generate code.
1) in main.c implement body of generated StartLED1 and StartLED2 functions.
1) ATTENTION! NUCLEO-F030R8 has only 8 KB of RAM which is not enought to run FreeRTOS in default configuration. During compilation you will see error "ld.exe: RTOS.elf section ._user_heap_stack' will not fit in region RAM". To work around this go to FreeRTOSConfig.h and reduce heap size:
`#define configTOTAL_HEAP_SIZE    ((size_t)2048)`. After this RTOS will work.