#ifndef SOURCE_HELPERS_H_
#define SOURCE_HELPERS_H_

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

extern QueueHandle_t task_1_q;
extern QueueHandle_t task_2_q;
//extern QueueHandle_t task_led_q;

void handle_error(void);
void send_message(const uint8_t* message, uint8_t number) ;


#endif
