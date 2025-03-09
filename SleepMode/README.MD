## Using Low Power Mode

This program use EXTI for send system to low power mode and for wake if up.
IMPORTANT! Trying to send system to low power mode inside of interupt handler is bad idea bacause handler procedure will not finish correctly. To send system to low power mode after interruption is beter only mark some flag in handler, out of it, and later initiate low power mode in main function based on that flag.