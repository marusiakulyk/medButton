#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"
#include <string.h>
#include "FreeRTOS.h"
#include "timers.h"
#include "led_task.h"

void handle_error(void){
     /* Disable all interrupts. */
    __disable_irq();

    CY_ASSERT(0);
};

void send_message(const uint8_t* message, uint8_t number) {
    for (uint8_t i = 0; i < number; i++) {
    	cyhal_uart_putc(&cy_retarget_io_uart_obj, message[i]);
    }
};



