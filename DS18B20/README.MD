## I2C wia UART

Connecting 2 DS18B20 temperature sensors to one I2B bus drived by UART1.
UART2 used to print data to serial monitor. Using serial aproach is different from Serial_via_UART project where we used HAL to send message. Here I redefined _write() function which allo redirect output of fprint() to UART2.
To comunicate I2C devices I use [this library](https://github.com/taburyak/STM32_OneWire_DallasTemperature_HAL_UART_DMA) in accordance with [this manual](https://stm32withoutfear.blogspot.com/2019/05/stm32-onewire-dallas-temperature-hal.html)
