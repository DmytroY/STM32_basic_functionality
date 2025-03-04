## Using timers

 In Pinout & Configuration:
 1) Activate timer
 2) Use prescaler which = incoming clock frequensy(MHz) - 1, so timer will tick every 1 mks.
 3) Set maximum Counter Period (for 16 bit timer is 65 535), so timer will count from 0 to 65535 then repeat.
 
 Now in your code you can start timer then get it's value.
 