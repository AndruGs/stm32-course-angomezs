#include <stdint.h>
#include <limits.h>
#include "stm32f4xx.h"
#include "stm32f411xe.h"

// Andru Gomez
// Taller V - Week 00-week-03

int main(void)
{
    // Ejercicio 0.1
    // uint8_t my_variable = 42;

    // Ejercicio 0.2
    // uint8_t dec = 65;
    // uint8_t hex = 0x41;
    // uint8_t bin = 0b01000001;

    // Ejercicio 0.3
    // uint8_t  a = 255;
    // uint16_t b = 255;
    // uint32_t c = 255;
    // uint8_t  d = 256;
    // uint8_t  e = 257;

    // Ejercicio 0.4
    // uint8_t a = 200;
    // int8_t  b = 200;
    // int8_t  c = -1;

     // Ejercicio 0.5
     // int8_t x = 127;
     // x = x + 1;

     // uint8_t y = 255;
     // y = y + 1;

     // int8_t int8_max = INT8_MAX;
     // int8_t int8_min = INT8_MIN;
     // uint8_t uint8_max = UINT8_MAX;
     // uint16_t uint16_max = UINT16_MAX;

     // Ejercicio 0.6
     // uint8_t r1 = 0x0F + 0x01;
     // uint8_t r2 = 0xFF + 0x01;
     // uint8_t r3 = 0xA0 + 0x5F;
     // uint8_t r4 = 0xA0 + 0x60;

     // Ejercicio 0.7
     // uint8_t x1 = 0x01;
     // uint8_t x2 = 0x02;
     // uint8_t x3 = 0x04;
     // uint8_t x4 = 0x08;
     // uint8_t x5 = 0x10;
     // uint8_t x6 = 0x20;
     // uint8_t x7 = 0x40;
     // uint8_t x8 = 0x80;

     // Ejercicio 0.8
     // uint8_t a = 5;
     // uint8_t b = 0;
     // uint8_t c = 255;

     // uint8_t r1 = !a;
     // uint8_t r2 = !b;
     // uint8_t r3 = !c;
     // uint8_t r4 = ~c;
     // uint8_t r5 = ~a;

     // Ejercicio 1.1
     // uint8_t a = 17;
     // uint8_t b = 5;

     // uint8_t div_result = a / b;
     // uint8_t mod_result = a % b;
     // uint8_t mul_result = a * b;

     // Ejercicio 1.2
     // uint8_t x = 200;
     // uint8_t y = 100;

     // uint8_t sum = x + y;

     // Ejercicio 1.3
     // uint8_t val = 3;

     // uint8_t left1 = val << 1;
     // uint8_t left2 = val << 2;
     // uint8_t left3 = val << 3;
     // uint8_t right1 = val >> 1;

     // uint8_t overflow = 0b10000000;
     // uint8_t overflow_left = overflow << 1;

	/*
	 * Ejercicio 1.4
	 *
	uint8_t a = 5;
	uint8_t b = 0;
	uint8_t c = 10;

	uint8_t result = 0;

	// Condición distinta de cero
	if (a)
	{
	    result = 1;
	}

	// Condición igual a cero
	if (b)
	{
	    result = 2;
	}
	else
	{
	    result = 3;
	}

	// Comparación de igualdad
	if (c == 10)
	{
	    result = 4;
	}
	*/

	/*
	 * Ejercicio 1.5
	 *
	uint8_t counter = 0;
	uint8_t i = 0;

	counter = 0;

	for (i = 10; i > 0; i--)
	{
	    counter++;
	}
	*/

	/* Ejercicio 1.6

	uint8_t i = 1;
	uint8_t sum = 0;

	while (i <= 100)
	{
	    sum += i;
	    i++;
	}
	*/
	/* Ejercicio 1.7

	uint8_t result_while = 0;
	uint8_t result_do = 0;

	while (0)
	{
	    result_while = 42;
	}

	do
	{
	    result_do = 42;
	}
	while (0);
	*/
	// Ejercicio 1.8

	uint8_t input = 3;
	uint8_t output = 0;

	switch (input)
	{
	case 1:
	    output = 10;
	    break;

	case 2:
	    output = 20;
	    break;

	case 3:
	    output = 30;
	    break;

	case 4:
	    output = 40;
	    break;

	default:
	    output = 99;
	    break;
	}

	while (1)
	{

	}
	}
