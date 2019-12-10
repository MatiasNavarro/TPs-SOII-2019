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

/*	Dos programas
 *
 *	El primero con un productor y un consumidor simulando el problema productor-consumidor.
 *
 * 	El segundo programa con dos productores y un consumidor.
 *
 * 	El primer productor genera periodicamente valores de temperatura
 * 	y los envia a una cola.
 *
 * 	El otro productor genera aperiodicamente cadenas de caracteres
 * 	del usuario y las envia a la cola.
 *
 * 	El consumidor espera a que aparezcan valores en la cola y
 * 	procede a enviar esos datos por el puerto serie mediante UART.
*/

/* Kernel includes. */
#include <inc/freertos/basic_io.h>
#include <inc/freertos/FreeRTOS.h>
#include <inc/freertos/queue.h>
#include <inc/freertos/task.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* Si es 1 se ejecuta el primer programa, si es 0 se ejecuta el segundo programa  */

#define FLAG_PROG			0

/*
 * Define la cantidad de cadenas de texto del arreglo que simula la entrada de usuario.
 */

#define MAX_STRINGS			10

/* Bit del LED22 de la placa LPC1769.  */

#define mainLED_BIT 			( 22 )

#define TASK_TEMP_PRIORITY		( tskIDLE_PRIORITY + 1 ) //tskIDLE_PRIORITY Prioridad de la tarea inactiva
#define	TASK_STR_PRIORITY		( tskIDLE_PRIORITY + 3 )
#define	TASK_TX_PRIORITY		( tskIDLE_PRIORITY + 7)
#define TASK_PROD_PRIORITY		( tskIDLE_PRIORITY + 3)
#define TASK_CONS_PRIORITY		( tskIDLE_PRIORITY + 1)

/* Frecuencia de generacion de datos.
 * La tarea del sensor de temperatura es periodica.
 * La tarea del sensor de usuario es aperiodica y aleatoria.  */

#define SENSOR_TEMP_FREQ		( 1000 / portTICK_RATE_MS )		//tasa de tick en milisegundos
#define SENSOR_USR				( 2000 )
#define PROD_FREQ				( 300 / portTICK_RATE_MS )
#define CONS_FREQ				( 500 / portTICK_RATE_MS )

/* Numero de elementos maximos en la cola.  */
#define mainQUEUE_LENGTH		( 2 )

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

static void TaskProductor(void *pvParameters);
static void TaskConsumidor(void *pvParameters);
static void TaskTemperatura(void *pvParameters);
static void TaskString(void *pvParameters);
static void TaskTransmisor(void *pvParameters);

/*
 * Funcion para configurar e inicializar el hardware para UART.
 */
void configUART();

/* Tamanio maximo del buffer para productor-consumidor.  */
int buffer_size=5;

/* Cantidad producida por el productor*/
int itemCount=3;

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


	if(FLAG_PROG)
	{
		/* Crear las 2 tareas
		NULL no le paso parametros a la funcion y no se le asigna ningun identificar a la tarea.*/
		xTaskCreate( TaskConsumidor, (signed char *) "Consumidor", configMINIMAL_STACK_SIZE, NULL, TASK_CONS_PRIORITY, NULL);
		xTaskCreate( TaskProductor, (signed char *) "Productor", configMINIMAL_STACK_SIZE, NULL, TASK_PROD_PRIORITY, NULL);
	}
	else
	{
		/* Crear la cola */
		xQueue = xQueueCreate( mainQUEUE_LENGTH, sizeof( input *) );

		if( xQueue != NULL )
		{
			/*Crear las 3 tareas*/
			xTaskCreate( TaskTransmisor, (signed char * ) "Transmisor", configMINIMAL_STACK_SIZE, NULL, TASK_TX_PRIORITY, NULL );
			xTaskCreate( TaskTemperatura, (signed char * ) "Sensor Temperatura", configMINIMAL_STACK_SIZE, NULL, TASK_TEMP_PRIORITY, NULL );
			xTaskCreate( TaskString, (signed char * ) "Sensor UserIn", configMINIMAL_STACK_SIZE, NULL, TASK_STR_PRIORITY, NULL );
		}

		else
		{
			vPrintString("No se pudo crear la cola.\n");
		}
	}

	/* Inicio el scheduler */
	vTaskStartScheduler();

	/* No se llega a este punto si no hubo problemas al iniciar el scheduler. */
	for( ;; );
	return 0;
}

/*-----------------------------------------------------------*/

/*
 * CONSUMIDOR. Si el buffer no esta vacio, consume un item. Se ejecuta cada 700ms.
 * Apaga el LED 2.
 */
static void TaskConsumidor(void * pvParameters) {
	int productos;
	for (;;)
	{
		if(itemCount>0)
		{
			itemCount=itemCount-1;
		}
		LPC_GPIO0->FIOCLR = 1 << mainLED_BIT;
		vTaskDelay(CONS_FREQ);
	}
}

/*
 * PRODUCTOR. En caso de no llegar al tamanio maximo del buffer, produce un item. Se ejecuta cada 500ms.
 * Enciende el LED 2.
 */
static void TaskProductor(void * pvParameters) {
	for (;;)
	{
		if(itemCount<buffer_size)
		{
			itemCount=itemCount+1;
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
static void TaskTemperatura(void * pvParameters)
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
			vPrintString( "No se pudo enviar el dato a la cola.\r\n" );
		}

		vPortFree(in);
		vTaskDelay( SENSOR_TEMP_FREQ ); //Periodico cada 1000ms
	}
}

/*
 *  GENERADOR DE STRINGS. Genera strings de tamanio random.
 *  Se ejecuta aperiodicamente con un periodo aleatorio definido
 *  en SENS_USR_IN_FREQ.
 */
static void TaskString(void * pvParameters)
{
	portBASE_TYPE xStatus;
	char * array_input_string[MAX_STRINGS] = {"ACK\n", "Mate\n", "Termo\n", "Yerba\n", "Criollos\n", "Asado\n", "Pesca\n", "Folklore\n", "Tradicion\n", "Argentina\n"};
	size_t input_index = 0;
	for (;;)
	{
		input *in = pvPortMalloc(sizeof(input *));

		input_index = rand() % MAX_STRINGS;

		in->msg = (char*) array_input_string[input_index];

		xStatus = xQueueSendToBack( xQueue, &in, 100);
		if( xStatus != pdPASS )
		{
			vPrintString( "No se pudo enviar el dato a la cola.\r\n" );
		}

		vPortFree(in);
		vTaskDelay(( (rand() % SENSOR_USR) + SENSOR_USR) / portTICK_RATE_MS ); //Aleatorio entre 2000 y 3999 ms
	}
}

/*
 * CONSUMIDOR. Imprime/manda por UART el valor que hay en cola.
 * Se ejecuta cada vez que hay mensajes en cola.
 */
static void TaskTransmisor(void * pvParameters)
{
	unsigned int dec = 0;
	unsigned int un = 0;
	int ind = 0;
	portBASE_TYPE xStatus;
	for (;;)
	{
		input *in = pvPortMalloc(sizeof(input *));
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
				*U0THR |= '\r';
				*U0THR |= '\n'; /* ACTIVAR 'Implicit CR in every LF en PuTTY. */
				vPortFree(in);

			}
			else
			{
				*U0THR |= '\n';
				*U0THR |= 'M';
				*U0THR |= 's';
				*U0THR |= 'g';
				*U0THR |= ':';
				*U0THR |= ' ';
				ind = 0;
				do
				{
					*U0THR |= in->msg[ind++];
				}
				while(in->msg[ind] != '\n');
				*U0THR |= '\r';
				*U0THR |= '\n';
				vPortFree(in);
			}
		}
		else
		{
			vPrintString( "No se pudo recibir datos de la cola.\r \n" );
		}
	}
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

