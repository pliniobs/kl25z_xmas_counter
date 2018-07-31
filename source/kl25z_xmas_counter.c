#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "fsl_i2c.h"
#include "fsl_i2c_freertos.h"

void vTask_LED_Blink(void *pvParameters);

gpio_pin_config_t led_config = {
		kGPIO_DigitalOutput, 0
};

int main(void) {
	BaseType_t Task_Create_Result;

	/* Init board hardware. */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();
	/* Init FSL debug console. */
	BOARD_InitDebugConsole();

	PRINTF("Hello World\n");


	/* create task */
	Task_Create_Result = xTaskCreate(vTask_LED_Blink,						/* Pointer to the task entry function */
			"vTask_LED_Blink",						/* A descriptive name for the task */
			configMINIMAL_STACK_SIZE,				/* The number of words (not bytes!) to allocate for use as the task's stack */
			(void *)NULL,							/* A value that will passed into the created task as the task's parameter */
			1,										/* The priority at which the created task will execute */
			NULL);									/* Used to pass a handle to the created task out of the xTaskCreate() function.*/

	if( Task_Create_Result != pdPASS )
	{
		while(1); // Falha ao criar a tarefa
	}

	/* start the scheduler */
	vTaskStartScheduler();

	/* should never reach here! */
	for(;;);
	return 0 ;
}

void vTask_LED_Blink(void *pvParameters)
{
	CLOCK_EnableClock(kCLOCK_PortB);
	PORT_SetPinMux(PORTB, 18u, kPORT_MuxAsGpio);
	GPIO_PinInit(GPIOB, 18u, &led_config);


	while(1)
	{
		GPIO_WritePinOutput(GPIOB, 18u, 1);
		vTaskDelay(25/ portTICK_PERIOD_MS);
		GPIO_WritePinOutput(GPIOB, 18u, 0);
		vTaskDelay(25/ portTICK_PERIOD_MS);
	}
}

