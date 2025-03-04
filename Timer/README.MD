## Using timers

 In Pinout & Configuration:
 1) Activate timer
 2) You can ajust timer period by using apropriate prescaler. For example, if you use prescaler = (incoming clock frequensy(MHz) - 1), then timer will tick every 1 mks. 
 3) Set Counter Period (for 16 bit timer maximum possible value is 65 535), so timer will count from 0 to Counter Period value then repeat.
 
 Now in your int main(void) function, before while(1) loop you should start timer with `HAL_TIM_Base_Start(&htim16);`
 You can get curent timer ticks count either with 'timer_val = htim16.Instance->CNT;` or `timer_val = __HAL_TIM_GET_COUNTER(&htim16);`
 This allows put some periodic activity under `if(htim16.Instance->CNT - timer_val > my_target_period)` and this will not block other code runing.
 
 Alternatively you can enable interrupt in NVIC Settings so timer will cause interrupt every end of cycle.
 In this case start timer in interrupt mode: `HAL_TIM_Base_Start_IT(&htim16);`
 Implement function which will be called by every timer interruption: `void HAL_TIM_PeriodElapsedCallback(htim) {......}`