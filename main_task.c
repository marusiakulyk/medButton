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


uint32_t WaitforSwitchPressAndRelease(void)
{

    uint32_t helddown;
    uint32_t sw_status;

    sw_status = 0u; /* Switch is not active */
    helddown = 0u;  /* Reset debounce counter */

    /* Wait for debounce period before determining that the switch is pressed */
    while (0UL == cyhal_gpio_read((cyhal_gpio_t)P13_6))
    {
        /* Count debounce period */
        Cy_SysLib_Delay(1u);
        ++helddown;

        if (helddown > 4294967294u)
        {
            sw_status = 1u; /* Switch is pressed */
            break;
        }
    }
   return sw_status;
}

//void WaitforSwitchPressAndRelease(void)
//{
//    /* Wait for SW2 to be pressed */
//    while( CYBSP_BTN_PRESSED != cyhal_gpio_read((cyhal_gpio_t)CYBSP_SW2));
//    /* Wait for SW2 to be released */
//    while( CYBSP_BTN_PRESSED == cyhal_gpio_read((cyhal_gpio_t)CYBSP_SW2));
//}
void task_1(){
	char gps;
	char gps_ar[120] = "";
	cyhal_gpio_init((cyhal_gpio_t)P13_6, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_PULLUP, CYBSP_BTN_PRESSED);
	cyhal_gpio_init((cyhal_gpio_t)CYBSP_LED_RGB_GREEN, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, CYBSP_LED_STATE_OFF);
	cyhal_gpio_init((cyhal_gpio_t)CYBSP_USER_LED2, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, CYBSP_LED_STATE_OFF);

	for(;;){
	cyhal_gpio_write((cyhal_gpio_t)CYBSP_LED_RGB_GREEN, CYBSP_LED_STATE_ON);
	CyDelay(2000);

	while (0UL != WaitforSwitchPressAndRelease()){
		cyhal_gpio_write((cyhal_gpio_t)CYBSP_LED_RGB_GREEN, CYBSP_LED_STATE_OFF);
	};

	cyhal_gpio_write((cyhal_gpio_t)CYBSP_LED_RGB_GREEN, CYBSP_LED_STATE_ON);
	cyhal_gpio_write((cyhal_gpio_t)CYBSP_USER_LED2, CYBSP_LED_STATE_ON);

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
			if(strstr(gps_ar,"GPGGA")){
			while(i+j < ln && (isdigit(gps_ar[i+j]) || gps_ar[i+j] == '.')){ // for gpgga
				j++;                                                         // for gpgga
			}                                                                // for gpgga
			i += j;                                                          // for gpgga
			j = 1;                                                           // for gpgga
			}
			while(i+j < ln && (isdigit(gps_ar[i+j]) || gps_ar[i+j] == '.')){
				lat[j-1] = gps_ar[i+j];
				j++;
			}

			i += j + 2;                                                      // for gngll & gpgga
			j = 1;
			while(i+j < ln && (isdigit(gps_ar[i+j]) || gps_ar[i+j] == '.')){
				longt[j-1] = gps_ar[i+j];
				j++;
			}
			break;
		}
	}

	cy_rslt_t result;
	cy_stc_scb_uart_context_t uartContext;
	uint32_t read_data;
	cy_en_scb_uart_status_t initstatus;

  initstatus = Cy_SCB_UART_Init(SCB1, &scb_1_config, &uartContext);
  if(initstatus!=CY_SCB_UART_SUCCESS)
  {
	handle_error();
  }

	Cy_SCB_UART_Enable(SCB1);
	int i =0;
	char rxBuffer[150];
	char message[70] = "";
	strncpy(message, lat, sizeof(message));
//	printf("%s - lat\n ",lat);
	strncat(message, " ", (sizeof(message) - strlen(message)));
	strncat(message, longt, (sizeof(message) - strlen(message)));

	char unique_id[100];
	sprintf(unique_id, ", id - %llu", Cy_SysLib_GetUniqueId());
	strncat(message, unique_id, (sizeof(message) - strlen(message)));
	printf(" %llu ", Cy_SysLib_GetUniqueId());

	strncat(message,"\x1A\x0d", (sizeof(message) - strlen(message)));
	printf("%s\n",message);
	CyDelay(5000);
	Cy_SCB_UART_PutString(SCB1, "AT\x0d");
	CyDelay(1000);

	for(int i = 0; i<1 ; i++){
	Cy_SCB_UART_PutString(SCB1, "AT+CMGF=1\x0d");
	CyDelay(1000);
	}


	Cy_SCB_UART_PutString(SCB1, "AT+CMGS=\x22+380673101033\x22\x0d");
	CyDelay(1000);
	Cy_SCB_UART_PutString(SCB1, message);
	CyDelay(1000);
	Cy_SCB_UART_PutString(SCB1, "ATH0\x0d")  ;
	CyDelay(5000);
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
//			while (0UL == Cy_SCB_UART_Put(SCB5,read_data))
//			{}
		}
	}
	if(strstr(rxBuffer,"ERROR")){
		printf("switching to Lora ...\n");
		Cy_SCB_UART_Enable(SCB2);
		Cy_SCB_UART_PutString(SCB2, message);
	}
	printf("%s - gsm response\n\r",rxBuffer);
	cyhal_gpio_write((cyhal_gpio_t)CYBSP_LED_RGB_GREEN, CYBSP_LED_STATE_ON);
	}

}
