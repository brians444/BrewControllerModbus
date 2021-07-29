#include "Debug.h"

uint8_t debug_activado;

void debugInit(bool act)
{
	// start serial port
	Serial.begin(19200);
	if(act)
	{
		debug_activado = 1;
		Serial.println("Bienvenido Debug activo!");
	}
	else
	{
		debug_activado = 0;
		Serial.println("Bienvenido Debug desactivado!");
	}
}

void debugPrintSetpoints(float *s, uint8_t cant)
{
	if(debug_activado)
	{
		for(int i = 0; i < cant; i++)
		{
		Serial.print("Setpoints leidos de eeprom:" );Serial.println(s[i]);
		}
	}
}

void debugPrint(float v)
{
	if(debug_activado)
	{
		Serial.print(v);
	}
}

void debugPrint(int v)
{
	if(debug_activado)
	{
		Serial.print(v);
	}
}

void debugPrint(uint16_t v)
{
  if(debug_activado)
  {
    Serial.print(v);
  }
}

void debugPrint(char v)
{
	if(debug_activado)
	{
		Serial.print(v);
	}
}

void debugPrint(char *v)
{
	if(debug_activado)
	{
		Serial.print(*v);
	}
}

void debugPrintln(float v)
{
	if(debug_activado)
	{
		Serial.println(v);
	}
}

void debugPrintln(int v)
{
	if(debug_activado)
	{
		Serial.println(v);
	}
}

void debugPrintln(char v)
{
	if(debug_activado)
	{
		Serial.println(v);
	}
}

void debugPrintln(char *v)
{
	if(debug_activado)
	{
		Serial.println(v);
	}
}

void debugPrintln(uint16_t v)
{
  if(debug_activado)
  {
    Serial.println(v);
  }
}
