#include "cybsp.h"
#include "cyhal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "cycfg.h"
#include "timers.h"

#include "cy_retarget_io.h"
#include "led_task.h"
#include "task1.h"


/*******************************************************************************
* Global constants
*******************************************************************************/
#define PWM_LED_FREQ_HZ     (1000000u)  /* in Hz */
#define GET_DUTY_CYCLE(x)   (100 - x)   /* subtracting from 100 since the LED
                                         * is connected in active low
                                         * configuration
                                         */

#define LED_SCAN_INTERVAL_MS   (10u)

/*******************************************************************************
 * Global variable
 ******************************************************************************/
TimerHandle_t scan_timer_handle1;
/* Queue handle used for LED data */
QueueHandle_t led1_command_q;


static void led1_timer_callback(TimerHandle_t xTimer)
{
    led_command_t command = 0;
    BaseType_t xYieldRequired;

    (void)xTimer;

    xYieldRequired = xQueueSendToBackFromISR(led1_command_q, &command, 0u);
    portYIELD_FROM_ISR(xYieldRequired);
};

void task_led(void* param)
{
	cyhal_gpio_init((cyhal_gpio_t)CYBSP_USER_LED1, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, CYBSP_LED_STATE_OFF);
//    cyhal_gpio_write((cyhal_gpio_t)CYBSP_USER_LED1, CYBSP_LED_STATE_ON);

    scan_timer_handle1 = xTimerCreate ("Scan Timer", LED_SCAN_INTERVAL_MS,
                                      pdTRUE, NULL, led1_timer_callback);
    xTimerStart(scan_timer_handle1, 0u);

	__enable_irq();

	uint8_t read_data;
	char buffer[120] = "";
	cyhal_gpio_write((cyhal_gpio_t)CYBSP_USER_LED1, CYBSP_LED_STATE_ON);
	for(;;)
	{
		cyhal_gpio_write((cyhal_gpio_t)CYBSP_USER_LED1, CYBSP_LED_STATE_ON);
		if(CY_SCB_UART_SUCCESS == cyhal_uart_getc(&cy_retarget_io_uart_obj, &read_data, 0))
		{
			uint8_t counter = 0;
			while (counter < 119 && read_data != '\n') {
				if(CY_SCB_UART_SUCCESS == cyhal_uart_getc(&cy_retarget_io_uart_obj, &read_data, 0)) {
					buffer[counter] = read_data;
					counter++;
				}
			}
			buffer[counter] = 0;
//			printf("%s\n\r", buffer);
//			"GPGSV"
			if((strstr(buffer,"GNGLL") || strstr(buffer,"GPGGA")) &&
					(!strstr(buffer,"GNGLL,,,") || !strstr(buffer,"GPGGA,,,"))){
				xQueueReset(task_1_q);

				for(int j = 0; j < 120; j++){
					xQueueSendToBack(task_1_q,&buffer[j],0u);
					}
				}
			}
		else
		{
			handle_error();
			}
		}
	}
