/*******************************************************************************
* File Name: capsense_task.c
*
* Description: This file contains the task that handles touch sensing.
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


/******************************************************************************
* Header files includes
******************************************************************************/


#include "led2_task.h"

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

#include "led_task.h"
#include "task1.h"
#include "helpers.h"


/*******************************************************************************
* Global constants
*******************************************************************************/
#define CSD_COMM_HW                 (SCB3)
#define CSD_COMM_IRQ                (scb_3_interrupt_IRQn)
#define CSD_COMM_PCLK               (PCLK_SCB3_CLOCK)
#define CSD_COMM_CLK_DIV_HW         (CY_SYSCLK_DIV_8_BIT)
#define CSD_COMM_CLK_DIV_NUM        (1U)
#define CSD_COMM_CLK_DIV_VAL        (3U)
#define CSD_COMM_SCL_PORT           (GPIO_PRT6)
#define CSD_COMM_SCL_PIN            (0u)
#define CSD_COMM_SDA_PORT           (GPIO_PRT6)
#define CSD_COMM_SDA_PIN            (1u)
#define CSD_COMM_SCL_HSIOM_SEL      (P6_0_SCB3_I2C_SCL)
#define CSD_COMM_SDA_HSIOM_SEL      (P6_1_SCB3_I2C_SDA)
#define CAPSENSE_INTERRUPT_PRIORITY (7u)
#define EZI2C_INTERRUPT_PRIORITY    (6u)    /* EZI2C interrupt priority must be
                                             * higher than CapSense interrupt
                                             */
#define LED1_SCAN_INTERVAL_MS   (10u)   /* in milliseconds*/


/*******************************************************************************
* Function Prototypes
*******************************************************************************/


/******************************************************************************
* Global variables
******************************************************************************/
QueueHandle_t led2_command_q;
TimerHandle_t scan_timer_handle;
cy_stc_scb_ezi2c_context_t ezi2c_context;


/*******************************************************************************
* Function Name: task_capsense
********************************************************************************
* Summary:
*  Task that initializes the CapSense block and processes the touch input.
*
* Parameters:
*  void *param : Task parameter defined during task creation (unused)
*
*******************************************************************************/
static void led2_timer_callback(TimerHandle_t xTimer)
{
    led_command_t command = 0;
    BaseType_t xYieldRequired;

    (void)xTimer;

    xYieldRequired = xQueueSendToBackFromISR(led2_command_q, &command, 0u);
    portYIELD_FROM_ISR(xYieldRequired);
};


void task_capsense(void* param)
{
	cyhal_gpio_init((cyhal_gpio_t)CYBSP_USER_LED2, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, CYBSP_LED_STATE_OFF);
    cyhal_gpio_write((cyhal_gpio_t)CYBSP_USER_LED2, CYBSP_LED_STATE_ON);
    BaseType_t rtos_api_result;
    cy_status status;
    led_command_t capsense_cmd;

//     Remove warning for unused parameter
    (void)param;

  //   Initialize timer for periodic CapSense scan
    scan_timer_handle = xTimerCreate ("Scan Timer", LED1_SCAN_INTERVAL_MS,
                                      pdTRUE, NULL, led2_timer_callback);


     //Start the timer
    xTimerStart(scan_timer_handle, 0u);


    int number = 2000;
    /* Repeatedly running part of the task */
    for(;;)
    {
    	cyhal_gpio_write((cyhal_gpio_t)CYBSP_USER_LED2, CYBSP_LED_STATE_ON);
    	number--;
    	xQueueReset(task_2_q);
    	xQueueSendToBack(task_2_q,&number,0u);
    	number--;
    	xQueueSendToBack(task_2_q,&number,0u);

    }

}

/* END OF FILE [] */
