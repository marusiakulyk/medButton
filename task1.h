#ifndef SOURCE_1_TASK_H_
#define SOURCE_1_TASK_H_

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

extern QueueHandle_t task_1_q;
extern QueueHandle_t task_2_q;

void task_1();


#endif

