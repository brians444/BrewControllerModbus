#include <stdint.h>
#include "Debug.h"

#define DEBUG 0
#define GRABAR_SD 0
#define CANT  8
#define CANT_FERM 8


struct conf{
    uint8_t frio;       // Utilizo los bits como booleanos
    uint8_t calor;      // Utilizo los bits como booleanos
    uint8_t salida_frio[CANT_FERM];
    uint8_t salida_calor[CANT_FERM];
    uint8_t habilitado; // Utilizo los bits como booleanos
    uint8_t fulltime;
    float temp[2];
    uint8_t cte1, cte2, cte3, cte4;
};

struct sensores{
    float temp[CANT_FERM];
};

struct target{
    float set_temp[CANT_FERM];
};



/**** COPIO LO QUE VOY ELIMINANDO **** / 

/*
void WriteSD()
{
  
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("data.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.print("T1,");
    dataFile.print(temp1);
    dataFile.print(",");
    dataFile.print("T2,");
    dataFile.print(temp2);
    dataFile.print(",");
    dataFile.print(now.hour());
    dataFile.print(":");dataFile.print(now.minute());dataFile.print(":");dataFile.print(now.second());dataFile.print(",");
    dataFile.print(now.year());dataFile.print("/");dataFile.print(now.month());dataFile.print("/");dataFile.print(now.day());
    dataFile.println(';');
    dataFile.close();
    // print to the serial port too:
    #if DEBUG
    Serial.println("Escribiendo la tarjeta");
    #endif
  }
  // if the file isn't open, pop up an error:
  else {
    #if DEBUG
    Serial.println("Error Abriendo datalog.txt");
    #endif
  }
}
*/

/*

void ControlTempFrio(int i)
{
  if(temp[i] > set[i] + 0.2 ) // TEMP ALTA - Abrir agua fria
  {
  
    digitalWrite(salidas_frio[i],LOW);
    if(i == 0) // Puente para solo el 0
      digitalWrite(S5,LOW);
    estado = i;
    //estado_salida_frio[i] = 1;
  }
  else if(temp[i] < set[i] - 0.2)     // TEMP BAJA - Cerrar agua fria
  {
    digitalWrite(salidas_frio[i],HIGH);
    //digitalWrite(S12,LOW);
    if(i==0) // Puente para solo el 0
      digitalWrite(S5,HIGH); //apago la bomba
    estado = 0;                   // Dejo libre
    //estado_salida_frio[i] = 0;
  }
}

void ControlTempCalor(int i)
{
    if(temp[i] < set[i] - 0.2 ) // TEMP BAJA - Abrir agua caliente *
    {
      #if DEBUG
      Serial.print("Enciendo agua caliente=");Serial.println(i);
      #endif
        digitalWrite(salidas_calor[i],LOW);
        //digitalWrite(S12,HIGH);
        estado = i;
        //estado_salida_calor[i] = 1;
    }
    else if(temp[i] > set[i] + 0.2)     // TEMP ALTA - Cerrar agua caliente
    {
      #if DEBUG
      Serial.print("Apago agua caliente=");Serial.println(i);
      #endif
        digitalWrite(salidas_calor[i],HIGH);
        //digitalWrite(S12,LOW);
        estado = 0;                   // Dejo libre
        //estado_salida_calor[i] = 0;
    }
}

*/
