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
QueueHandle_t task_led_q;


void WaitforSwitchPressAndRelease(void)
{
    /* Wait for SW2 to be pressed */
    while( CYBSP_BTN_PRESSED != cyhal_gpio_read((cyhal_gpio_t)CYBSP_SW2));

    /* Wait for SW2 to be released */
    while( CYBSP_BTN_PRESSED == cyhal_gpio_read((cyhal_gpio_t)CYBSP_SW2));
}

void task_1(){
	char gps;
	char gps_ar[120] = "";
	cyhal_gpio_init((cyhal_gpio_t)CYBSP_LED_RGB_GREEN, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, CYBSP_LED_STATE_OFF);
	cyhal_gpio_init((cyhal_gpio_t)CYBSP_SW2, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_PULLUP, CYBSP_BTN_OFF);


	for(;;){
	cyhal_gpio_write((cyhal_gpio_t)CYBSP_LED_RGB_GREEN, CYBSP_LED_STATE_ON);

	WaitforSwitchPressAndRelease();

//    cyhal_gpio_write((cyhal_gpio_t)CYBSP_USER_LED1, CYBSP_LED_STATE_OFF);
	cyhal_gpio_write((cyhal_gpio_t)CYBSP_USER_LED2, CYBSP_LED_STATE_OFF);

	cyhal_gpio_write((cyhal_gpio_t)CYBSP_LED_RGB_GREEN, CYBSP_LED_STATE_OFF);
    //printf("loser\n\r");
	for(int i = 0; i < 120; i++){
		if(xQueueReceive(task_1_q,&gps,portMAX_DELAY)){
				gps_ar[i] = gps;
			};
		}
	printf("%s - gps str\n\r",gps_ar);
	char lat[15] = "";
	char longt[15] = "";
	int ln = strlen(gps_ar);
	for(int i = 0; i < ln; i++){
		if(gps_ar[i] == ','){
			int j = 1;

//			while(i+j < ln && (isdigit(gps_ar[i+j]) || gps_ar[i+j] == '.')){ // for gpgga
//				j++;                                                         // for gpgga
//			}                                                                // for gpgga
//			i += j;                                                          // for gpgga
//			j = 1;                                                           // for gpgga

			while(i+j < ln && (isdigit(gps_ar[i+j]) || gps_ar[i+j] == '.')){
				lat[j-1] = gps_ar[i+j];
				j++;
			}

//			i += j + 2;                                                      // for gngll & gpgga
			j = 1;
			while(i+j < ln && (isdigit(gps_ar[i+j]) || gps_ar[i+j] == '.')){
				longt[j-1] = gps_ar[i+j];
				j++;
			}
			break;
		}
	}
//		printf("%s - gps\r\n", gps_ar);

	cy_rslt_t result;
	cy_stc_scb_uart_context_t uartContext;
	uint32_t read_data;
	cy_en_scb_uart_status_t initstatus;

//	printf("hello hippo\n\r");

  initstatus = Cy_SCB_UART_Init(SCB1, &scb_1_config, &uartContext);
  if(initstatus!=CY_SCB_UART_SUCCESS)
  {
	handle_error();
  }

	Cy_SCB_UART_Enable(SCB1);
	int i =0;
	char rxBuffer[150];
	char message[30];
	strncpy(message, lat, sizeof(message));
	strncat(message, " ", (sizeof(message) - strlen(message)));
	strncat(message, longt, (sizeof(message) - strlen(message)));
	strncat(message,"\x1A\x0d", (sizeof(message) - strlen(message) - 8));
	printf("%s\n",message);
	CyDelay(5000);
	Cy_SCB_UART_PutString(SCB1, "AT\x0d");
	CyDelay(1000);

	for(int i = 0; i<1 ; i++){
	Cy_SCB_UART_PutString(SCB1, "AT+CMGF=1\x0d");
	CyDelay(1000);
	}

	Cy_SCB_UART_PutString(SCB1, "AT+CMGS=\x22+380673101033\x22\x0d");
	//Cy_SCB_UART_PutString(SCB1, "AT+CMGS=\x22+380634465742\x22\x0d");
	CyDelay(1000);
	Cy_SCB_UART_PutString(SCB1, message);
	CyDelay(1000);
	Cy_SCB_UART_PutString(SCB1, "ATH0\x0d")  ;
	CyDelay(10000);
	i = 0;
	for(int s = 0;s < 150; s++)
	{
		/* Check if there is a received character from user console */
		if (0UL != Cy_SCB_UART_GetNumInRxFifo(SCB1))
		{
			/* Re-transmit whatever the user types on the console */
			read_data = Cy_SCB_UART_Get(SCB1);
			rxBuffer[i] =  read_data;
			i++;
//			printf("%c\n",read_data);
//			while (0UL == Cy_SCB_UART_Put(SCB5,read_data))
//			{
//
//			}
		}
	}
	printf("%s - gsm response\n\r",rxBuffer);
	cyhal_gpio_write((cyhal_gpio_t)CYBSP_LED_RGB_GREEN, CYBSP_LED_STATE_ON);
//	cyhal_gpio_init((cyhal_gpio_t)CYBSP_USER_LED1, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, CYBSP_LED_STATE_ON);
	}

}
