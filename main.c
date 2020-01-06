#include <led2_task.h>
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "led_task.h"
#include "task1.h"
#include "helpers.h"

/*******************************************************************************
 * Global constants
 ******************************************************************************/
/* Priorities of user tasks in this project. configMAX_PRIORITIES is defined in
 * the FreeRTOSConfig.h and higher priority numbers denote high priority tasks.
 */
#define TASK_CAPSENSE_PRIORITY      (configMAX_PRIORITIES - 2)
#define TASK_LED_PRIORITY           (configMAX_PRIORITIES - 2)
#define TASK_1_PRIORITY           (configMAX_PRIORITIES - 1)

/* Stack sizes of user tasks in this project */
#define TASK_CAPSENSE_STACK_SIZE    (configMINIMAL_STACK_SIZE)
#define TASK_LED_STACK_SIZE         (configMINIMAL_STACK_SIZE)
#define TASK_1_STACK_SIZE         (configMINIMAL_STACK_SIZE)

/* Queue lengths of message queues used in this project */
#define SINGLE_ELEMENT_QUEUE        (1u)



/*******************************************************************************
* Function Name: main()
********************************************************************************
* Summary:
*  System entrance point. This function sets up user tasks and then starts
*  the RTOS scheduler.
*
* Return:
*  int
*
*******************************************************************************/



int main(void)
{

    cy_rslt_t result;

    /* Initialize the device and board peripherals */
    init_cycfg_all();
	__enable_irq();

    if(result != CY_RSLT_SUCCESS)
    	{
    		handle_error();
    	}
    result = cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX, CY_RETARGET_IO_BAUDRATE);
    if(result != CY_RSLT_SUCCESS)
        {
        	handle_error();
        }
    printf("************************************************************\r\n");
    printf("PSoC 6 MCU UART Transmit and Receive\r\n");
    printf("************************************************************\r\n\n");




    /* Create the queues. See the respective data-types for details of queue
     * contents
     */
    led1_command_q  = xQueueCreate(SINGLE_ELEMENT_QUEUE,
                                       sizeof(led_command_data_t));
    led2_command_q  = xQueueCreate(SINGLE_ELEMENT_QUEUE,
                                       sizeof(capsense_command_t));
    task_1_q = xQueueCreate(120,sizeof(char));
    task_2_q = xQueueCreate(2,sizeof(int));
    /* Create the user tasks. See the respective task definition for more
     * details of these tasks.*/


//    xTaskCreate(task_capsense, "CapSense Task", TASK_CAPSENSE_STACK_SIZE,
//                NULL, TASK_CAPSENSE_PRIORITY, NULL);
    xTaskCreate(task_led, "Led Task", TASK_LED_STACK_SIZE*3,
                NULL, TASK_LED_PRIORITY, NULL);
    xTaskCreate(task_1, "Task1", TASK_1_STACK_SIZE*3,
                    NULL, TASK_1_PRIORITY, NULL);




    /* Start the RTOS scheduler. This function should never return */
    vTaskStartScheduler();
    printf("Very bad error\n\r");

    /*~~~~~~~~~~~~~~~~~~~~~ Should never get here! ~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    /* RTOS scheduler exited */
    /* Halt the CPU if scheduler exits */
    CY_ASSERT(0);
 
}

