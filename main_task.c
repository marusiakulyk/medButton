#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"

#include "cycfg.h"
#include "cycfg_capsense.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "task1.h"
#include "helpers.h"

QueueHandle_t task_1_q;
QueueHandle_t task_2_q;


void WaitforSwitchPressAndRelease(void)
{
    /* Wait for SW2 to be pressed */
    while( CYBSP_BTN_PRESSED != cyhal_gpio_read((cyhal_gpio_t)CYBSP_SW2));

    /* Wait for SW2 to be released */
    while( CYBSP_BTN_PRESSED == cyhal_gpio_read((cyhal_gpio_t)CYBSP_SW2));
}

void task_1(){
	int number;
	cyhal_gpio_init((cyhal_gpio_t)CYBSP_LED_RGB_GREEN, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, CYBSP_LED_STATE_OFF);
	cyhal_gpio_init((cyhal_gpio_t)CYBSP_SW2, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_PULLUP, CYBSP_BTN_OFF);


	cy_rslt_t result = cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX, CY_RETARGET_IO_BAUDRATE);
	    if(result != CY_RSLT_SUCCESS)
	    {
	    	handle_error();
	    }
	for(;;){
	cyhal_gpio_write((cyhal_gpio_t)CYBSP_LED_RGB_GREEN, CYBSP_LED_STATE_OFF);
	WaitforSwitchPressAndRelease();
    cyhal_gpio_write((cyhal_gpio_t)CYBSP_USER_LED1, CYBSP_LED_STATE_OFF);
	cyhal_gpio_write((cyhal_gpio_t)CYBSP_USER_LED2, CYBSP_LED_STATE_OFF);

	cyhal_gpio_write((cyhal_gpio_t)CYBSP_LED_RGB_GREEN, CYBSP_LED_STATE_ON);
		if(xQueueReceive(task_1_q,&number,portMAX_DELAY)){
			printf("%d - led\r\n",number);
		};
		if(xQueueReceive(task_2_q,&number,portMAX_DELAY)){
			printf("%d - cap1\r\n",number);
				};
		if(xQueueReceive(task_2_q,&number,portMAX_DELAY)){
			printf("%d - cap2\r\n",number);
						};
	}

}
