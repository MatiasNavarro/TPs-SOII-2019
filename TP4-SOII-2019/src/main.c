/*
 * FreeRTOS Kernel V10.0.1
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/*
 * Programa con 2 productores y un consumidor.
 * El primer productor genera periodicamente valores de temperatura
 * y los envia a una cola.
 *
 * El otro productor genera aperiodicamente cadenas de caracteres
 * del usuario y las envia a la cola.
 *
 * El consumidor espera a que aparezcan valores en la cola y
 * procede a enviar esos datos por el puerto serie mediante UART.
*/

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "basic_io.h"

/* Si es 1, se crean solo 2 tareas productor-consumidor que
 * solamente sacan y ponen unos en un buffer.
 *
 * Si es 0, se crean las 3 tareas del funcionamiento normal.  */
#define PROD_CONS				0

/*
 * Define la cantidad de cadenas de texto del arreglo que
 * simula la entrada de usuario.
 */
#define USR_MAX_STRINGS			7

/* Bit del LED2 de la placa LPC17xx.  */
#define mainLED_BIT 			( 22 )

#define TASK_TEMP_PRIORITY		( tskIDLE_PRIORITY + 1 )
#define	TASK_STR_PRIORITY		( tskIDLE_PRIORITY + 2 )
#define	TASK_TX_PRIORITY		( tskIDLE_PRIORITY + 5)
#define TASK_PROD_PRIORITY		( tskIDLE_PRIORITY + 2)
#define TASK_CONS_PRIORITY		( tskIDLE_PRIORITY + 1)

/* Frecuencia de generacion de datos.
 * La tarea del sensor de temperatura es periodica.
 * La tarea del sensor de usuario es aperiodica y aleatoria.  */
#define SENSOR_TEMP_FREQ		( 1000 / portTICK_RATE_MS )
#define SENSOR_USR_IN_MIN		( 2000 )
#define SENSOR_USR_IN_MAX		( 4000 )
#define PROD_FREQ				( 500 / portTICK_RATE_MS )
#define CONS_FREQ				( 750 / portTICK_RATE_MS )

/* Numero de elementos maximos en la cola.  */
#define mainQUEUE_LENGTH					( 2 )

/* ----------------------MODULO UART--------------------------- */
#define U0LCR				((unsigned int volatile*) 0x4000C00C)
#define U0IER 				((unsigned int volatile*) 0x4000C004)
#define U0RBR				((unsigned int volatile*) 0x4000C000)
#define U0TER				((unsigned int volatile*) 0x4000C030)
#define U0THR				((unsigned int volatile*) 0x4000C000)
#define U0FCR				((unsigned int volatile*) 0x4000C008)
#define U0DLL				((unsigned int volatile*) 0x4000C000)
#define U0DLM				((unsigned int volatile*) 0x4000C004)
/* ----------------------MODULO GPIO-------------------------- */
#define FIOSET0 			((unsigned int volatile*) 0x2009C018)
#define FIODIR0 			((unsigned int volatile*) 0x2009C000)
#define FIOCLR0 			((unsigned int volatile*) 0x2009C01C)
#define FIOPIN0 			((unsigned int volatile*) 0x2009C014)
#define PINMODE0			((unsigned int volatile*) 0x4002C040)
#define PINSEL0				((unsigned int volatile*) 0x4002C000)

/* Declaraciones de las tareas.  */
static void prvTaskProd(void *pvParameters);
static void prvTaskCons(void *pvParameters);
static void prvTaskTemp(void *pvParameters);
static void prvTaskStr(void *pvParameters);
static void prvTaskTx(void *pvParameters);

/*
 * Funcion para toglear el LED2 para testing.
 */
static void prvToggleLED( void );
/*
 * Funcion para configurar e inicializar el hardware para UART.
 */
void configUART();

/* Arreglo que simula buffer para productor-consumidor.  */
int arreglo[5] = {1,1,1,0,0};

/* La cola es usada por las 3 tareas.  */
static xQueueHandle xQueue = NULL;

/* Estructura con el mensaje a enviar.  */
typedef struct{
	char * msg;
	char temp;
}input;

/*-----------------------------------------------------------*/

int main(void)
{
	/* Inicializacion de Pin 22 para LED2.  */
	LPC_PINCON->PINSEL1	&= ( ~( 3 << 12 ) );
	LPC_GPIO0->FIODIR |= ( 1 << mainLED_BIT );

	configUART();

	/* Comienza a grabar datos de trace.  */
	vTraceEnable(TRC_START);

	/* Crear la cola. */
	xQueue = xQueueCreate( mainQUEUE_LENGTH, sizeof( input *) );

	/* Crear las 3 tareas e iniciar el scheduler.  */
	if( xQueue != NULL )
	{
		if(PROD_CONS)
		{
			xTaskCreate( prvTaskCons, (signed char *) "Consumidor", configMINIMAL_STACK_SIZE, NULL, TASK_CONS_PRIORITY, NULL);
			xTaskCreate( prvTaskProd, (signed char *) "Productor", configMINIMAL_STACK_SIZE, NULL, TASK_PROD_PRIORITY, NULL);
		}
		else
		{
			xTaskCreate( prvTaskTx, (signed char * ) "Transmisor", configMINIMAL_STACK_SIZE, NULL, TASK_TX_PRIORITY, NULL );
			xTaskCreate( prvTaskTemp, (signed char * ) "Sensor Temp", configMINIMAL_STACK_SIZE, NULL, TASK_TEMP_PRIORITY, NULL );
			xTaskCreate( prvTaskStr, (signed char * ) "Sensor UserIn", configMINIMAL_STACK_SIZE, NULL, TASK_STR_PRIORITY, NULL );
		}
		vTaskStartScheduler();
	}
	else
	{
		vPrintString("No se pudo crear la cola.\n");
	}
	/* No se llega a este punto si no hubo problemas al
	 * iniciar el scheduler. */
	for( ;; );
	return 0;
}

/*-----------------------------------------------------------*/

/*
 * CONSUMIDOR. Pone ceros en el arreglo. Se ejecuta cada 600ms.
 * Apaga el LED 2.
 */
static void prvTaskCons(void * pvParameters) {
	int productos;
	for (;;){

		productos = 0;
		for(int k = 0; k<5; k++)
		{
			if(arreglo[k] == 1)
			{
				productos++;
			}
		}
		if(productos>0)
		{
			arreglo[rand() % 5] = 0;
		}
		LPC_GPIO0->FIOCLR = 1 << mainLED_BIT;
		vTaskDelay(CONS_FREQ);
	}
}

/*
 * PRODUCTOR. Pone unos en el arreglo. Se ejecuta cada 500ms.
 * Enciende el LED 2.
 */
static void prvTaskProd(void * pvParameters) {
	int lugares;
	for (;;){

		lugares = 0;
		for(int k = 0; k<5; k++)
		{
			if(arreglo[k] == 0)
			{
				lugares++;
			}
		}
		if(lugares>0)
		{
			arreglo[rand() % 5] = 1;
		}
		LPC_GPIO0->FIOSET = 1 << mainLED_BIT;
		vTaskDelay(PROD_FREQ);
	}
}

/*-----------------------------------------------------------*/

/*
 *  GENERADOR DE TEMPERATURA. Genera valores random de 0-100
 *  que representan temperaturas, ejecutado periodicamente con
 *  el periodo definido en SENSOR_TEMP_FREQ.
 */
static void prvTaskTemp(void * pvParameters)
{
	portBASE_TYPE xStatus;
	for (;;)
	{
		input *in = pvPortMalloc(sizeof(input *));

		in->temp = rand() % 100;
		in->msg = NULL;

		xStatus = xQueueSendToBack( xQueue, &in, 100);
		if( xStatus != pdPASS )
		{
			vPrintString( "No se pudo enviar dato a la cola.\r\n" );
		}

		vTaskDelay( SENSOR_TEMP_FREQ );
	}
}

/*
 *  GENERADOR DE STRINGS. Genera strings de tamanio random.
 *  Se ejecuta aperiodicamente con un periodo aleatorio definido
 *  en SENS_USR_IN_FREQ.
 */
static void prvTaskStr(void * pvParameters)
{
	portBASE_TYPE xStatus;
	char * array_input_string[USR_MAX_STRINGS] = {"hola\n", "ok\n", "negativo\n", "usuario\n", "auth\n", "adios\n", "ACK\n"};
	size_t input_index = 0;
	for (;;)
	{
		input *in = pvPortMalloc(sizeof(input *));
		in->msg = pvPortMalloc(sizeof(char *));

		input_index = rand() % USR_MAX_STRINGS;

		in->msg = (char*) array_input_string[input_index];

		xStatus = xQueueSendToBack( xQueue, &in, 100);
		if( xStatus != pdPASS )
		{
			vPrintString( "No se pudo enviar dato a la cola.\r\n" );
		}

		vTaskDelay(( (rand() % (SENSOR_USR_IN_MAX - SENSOR_USR_IN_MIN )) + SENSOR_USR_IN_MIN ) / portTICK_RATE_MS );
	}
}

/*
 * CONSUMIDOR. Imprime/manda por UART el valor que hay en cola.
 * Se ejecuta cada vez que hay mensajes en cola.
 */
static void prvTaskTx(void * pvParameters)
{
	unsigned int dec = 0;
	unsigned int un = 0;
	int ind = 0;
	input * in;
	portBASE_TYPE xStatus;
	for (;;)
	{
		xStatus = xQueueReceive( xQueue, &in, portMAX_DELAY );
		if( xStatus == pdPASS )
		{
			if(in->msg == NULL)
			{
				dec = in->temp / 10;
				un = in->temp % 10;
				*U0THR |= '\n';
				*U0THR |= 'T';
				*U0THR |= 'e';
				*U0THR |= 'm';
				*U0THR |= 'p';
				*U0THR |= ':';
				*U0THR |= ' ';
				*U0THR |= 48+dec;
				*U0THR |= 48+un;
				*U0THR |= '\n'; /* ACTIVAR 'Implicit CR in every LF en PuTTY. */
				vPortFree(in);
			}
			else
			{
				*U0THR |= '\n';
				*U0THR |= 'M';
				*U0THR |= ':';
				*U0THR |= ' ';
				ind = 0;
				do
				{
					*U0THR |= in->msg[ind++];
				}
				while(in->msg[ind] != '\n');
				*U0THR |= '\n';
				//vPortFree(in->msg);
				vPortFree(in);
			}
		}
		else
		{
			vPrintString( "No se pudo recibir datos de la cola.\r\n" );
		}
	}
}

static void prvToggleLED( void )
{
	unsigned long ulLEDState;
	/* Obtiene el estado del LED. */
	ulLEDState = LPC_GPIO0->FIOPIN;

	/* Enciende el LED si estaba apagado.
	 * Appaga el LED si estaba prendido. */
	LPC_GPIO0->FIOCLR = ulLEDState & ( 1 << mainLED_BIT );
	LPC_GPIO0->FIOSET = ( ( ~ulLEDState ) & ( 1 << mainLED_BIT ) );
}
/*-----------------------------------------------------------*/

/* Configuracion de hardware UART.  */
void configUART()
{
	*U0LCR |= (1<<7);						/* Para usar DLL */
	*U0DLL &=~ (255<<0); 					/* Limpia los bits 7:0 del registro */
	*U0DLL |= 0xa1;							/* Pone DLL = 161 -> 9585 baudios */
	*U0DLM &=~ (255<<0); 					/* Limpia los bits 7:0 del registro y lo deja en 0 */
	*U0LCR &=~(1<<7); 						/* coloco DLAB=0 */

	/* Por defecto viene prendido en el PCONP.
	 *  Usamos la frecuencia por defecto (PCLK) */

	*U0FCR |= 1;							/* Habilita la cola FIFO para el receptor */
	*U0LCR |= (3<<0);						/* Se transmitira una palabra de 8 bits */
	*U0LCR &= ~(1<<2);						/* Define 1 bit de parada */
	*FIODIR0 |= (1<<2);						/* TX como salida */
	*FIODIR0 &=~ (1<<3);					/* RX como entrada */
	*PINSEL0 |= (1<<4);						/* Configura P0.2 como TXD0 */
	*PINSEL0 |= (1<<6);						/* Configura P0.3 como RXD0 */
}

