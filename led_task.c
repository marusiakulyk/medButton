/******************************************************************************
* File Name: led_task.c
*
* Description: This file contains the task that handles led.
*
* Related Document: README.md
*
********************************************************************************
* Copyright (2019), Cypress Semiconductor Corporation.
********************************************************************************
* This software, including source code, documentation and related materials
* (“Software”), is owned by Cypress Semiconductor Corporation or one of its
* subsidiaries (“Cypress”) and is protected by and subject to worldwide patent
* protection (United States and foreign), United States copyright laws and
* international treaty provisions. Therefore, you may use this Software only
* as provided in the license agreement accompanying the software package from
* which you obtained this Software (“EULA”).
*
* If no EULA applies, Cypress hereby grants you a personal, nonexclusive,
* non-transferable license to copy, modify, and compile the Software source
* code solely for use in connection with Cypress’s integrated circuit products.
* Any reproduction, modification, translation, compilation, or representation
* of this Software except as specified above is prohibited without the express
* written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death (“High Risk Product”). By
* including Cypress’s product in a High Risk Product, the manufacturer of such
* system or application assumes all risk of such use and in doing so agrees to
* indemnify Cypress against all liability.
*****************************************​**************************************/


/*******************************************************************************
 * Header file includes
 ******************************************************************************/

#include "cybsp.h"
#include "cyhal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "cycfg.h"
#include "timers.h"

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


/*******************************************************************************
* Function Name: task_led
********************************************************************************
* Summary:
*  Task that controls the LED.
*
* Parameters:
*  void *param : Task parameter defined during task creation (unused)
*
*******************************************************************************/
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
    cyhal_gpio_write((cyhal_gpio_t)CYBSP_USER_LED1, CYBSP_LED_STATE_ON);

    scan_timer_handle1 = xTimerCreate ("Scan Timer", LED_SCAN_INTERVAL_MS,
                                      pdTRUE, NULL, led1_timer_callback);
    xTimerStart(scan_timer_handle1, 0u);

    int number1 = 0;
    for(;;)
    {
    	number1 = number1 + 10;
    	xQueueReset(task_1_q);
    	xQueueSendToBack(task_1_q,&number1,0u);

    }
}


/* END OF FILE [] */
