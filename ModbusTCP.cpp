#include "ModbusTCP.h"
#include "Structs.h"

//ModbusIP object
ModbusIP mb;

// The media access control (ethernet hardware) address for the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// The IP address for the shield
byte ip[] = { 10, 152, 130, 120 };
byte dgw[] = { 0, 0, 0, 0 };

void ModbusInit(float *set)
{
    debugPrintln("Estado iniciacion ENC28J60: ");
    //Config Modbus IP
    mb.config(mac, ip);

    debugPrint("Temps start address = ");debugPrintln(REG_START_TEMPS); 
    debugPrint("Setpoints start address = ");debugPrintln(REG_START_SETPOINTS);
    debugPrint("Valve state start address = ");debugPrintln(REG_START_VALVE_STATE_COLD);
    debugPrint("Force ON Valve start address = ");debugPrintln(REG_START_FORCE_ON_VALVE_COLD);
    debugPrint("Force OFF Valve start address = ");debugPrintln(REG_START_FORCE_OFF_VALVE_COLD);
    
    // Add registers - Use addHreg() for Holding Registers
    uint16_t temp_dir = 0, set_dir = 0;
    for(int i = 0; i<(CANT_FERM*N_REG_FLOAT); i++)  // Un flotante ocupa dos registros Holding de 16bits por lo q multiplico por dos la cantidad de registros a reservar
    {
      temp_dir = REG_START_TEMPS+i;
      set_dir = REG_START_SETPOINTS+i;
      mb.addHreg(temp_dir); //Temperatura i
      mb.addHreg(set_dir); //Setpoints i
    }
	
    for(int i = 0; i < CANT_FERM; i++)
    {
      mb.addHreg(REG_START_VALVE_STATE_COLD+i); //Salida Electrovalvula i
      mb.addHreg(REG_START_FORCE_ON_VALVE_COLD+i); //Salida forzada ON valvula i
      mb.addHreg(REG_START_FORCE_OFF_VALVE_COLD+i); //Salida Electrovalvula i
      mb.addHreg(REG_START_VALVE_STATE_HOT+i); //Salida Electrovalvula i
      mb.addHreg(REG_START_FORCE_ON_VALVE_HOT+i); //Salida forzada ON valvula i
      mb.addHreg(REG_START_FORCE_OFF_VALVE_HOT+i); //Salida Electrovalvula i
      mb.addHreg(REG_START_HABILITATIONS+i);  // Habilitaciones
      mb.addHreg(REG_START_HABCOLD+i);  // Habilitaciones Frio
      mb.addHreg(REG_START_HABHOT+i);  // Habilitaciones Calor
      
    }

    mb.addHreg(REG_START_CHILLER);mb.addHreg(REG_START_CHILLER+1); //Temperatura Tanque FLOAT
    mb.addHreg(REG_START_CHILLER+2);mb.addHreg(REG_START_CHILLER+3);//Setpoint temperatura FLOAT
    mb.addHreg(REG_START_CHILLER+4);mb.addHreg(REG_START_CHILLER+5);//Corriente equipo 1
    mb.addHreg(REG_START_CHILLER+6);mb.addHreg(REG_START_CHILLER+7);//Setpoint Maxima Corriente 1
    mb.addHreg(REG_START_CHILLER+8);mb.addHreg(REG_START_CHILLER+9);//Setpoint Minima Corriente 1
    mb.addHreg(REG_START_CHILLER+10);//Forzado ON equipo 1
    mb.addHreg(REG_START_CHILLER+11);//Forzado OFF equipo 1
    mb.addHreg(REG_START_CHILLER+12);//Estado Equipo 1
    mb.addHreg(REG_START_CHILLER+13);//Alarma Equipo 1
    mb.addHreg(REG_START_CHILLER+14);//Timer1 Alarma
    mb.addHreg(REG_START_CHILLER+15);mb.addHreg(REG_START_CHILLER+16);//Corriente equipo 2
    mb.addHreg(REG_START_CHILLER+17);mb.addHreg(REG_START_CHILLER+18);//Setpoint Maxima Corriente 2
    mb.addHreg(REG_START_CHILLER+19);mb.addHreg(REG_START_CHILLER+20);//Setpoint Minima Corriente 2
    mb.addHreg(REG_START_CHILLER+21);//Forzado ON equipo 2
    mb.addHreg(REG_START_CHILLER+22);//Forzado OFF equipo 2
    mb.addHreg(REG_START_CHILLER+23);//Estado Equipo 2
    mb.addHreg(REG_START_CHILLER+24);//Alarma Equipo 2
    mb.addHreg(REG_START_CHILLER+25);//Timer2 Alarma

    mb.addHreg(REG_START_PUMPS_A+0);mb.addHreg(REG_START_PUMPS_A+1);//Corriente bomba 1                   //Address 400171
    mb.addHreg(REG_START_PUMPS_A+2);mb.addHreg(REG_START_PUMPS_A+2);//Setpoint Maxima Corriente bomba 1   //Address 400173
    mb.addHreg(REG_START_PUMPS_A+4);mb.addHreg(REG_START_PUMPS_A+3);//Setpoint Minima Corriente bomba 1   //Address 400175
    mb.addHreg(REG_START_PUMPS_A+6);//Forzado ON bomba 1                                                //Address 400177
    mb.addHreg(REG_START_PUMPS_A+7);//Forzado OFF bomba 1                                               //Address 400178
    mb.addHreg(REG_START_PUMPS_A+8);//Estado bomba 1                                                    //Address 400179
    mb.addHreg(REG_START_PUMPS_A+9);//Alarma bomba 1                                                    //Address 400180
    mb.addHreg(REG_START_PUMPS_A+10);//Timer1 bomba Alarma                                               //Address 400181
        
    mb.addHreg(REG_START_PUMPS_B+0);mb.addHreg(REG_START_PUMPS_B+1);//Corriente bomba 1                   //Address 400171
    mb.addHreg(REG_START_PUMPS_B+2);mb.addHreg(REG_START_PUMPS_B+2);//Setpoint Maxima Corriente bomba 1   //Address 400173
    mb.addHreg(REG_START_PUMPS_B+4);mb.addHreg(REG_START_PUMPS_B+3);//Setpoint Minima Corriente bomba 1   //Address 400175
    mb.addHreg(REG_START_PUMPS_B+6);//Forzado ON bomba 1                                                //Address 400177
    mb.addHreg(REG_START_PUMPS_B+7);//Forzado OFF bomba 1                                               //Address 400178
    mb.addHreg(REG_START_PUMPS_B+8);//Estado bomba 1                                                    //Address 400179
    mb.addHreg(REG_START_PUMPS_B+9);//Alarma bomba 1                                                    //Address 400180
    mb.addHreg(REG_START_PUMPS_B+10);//Timer1 bomba Alarma                                               //Address 400181
    
    

    mb.addHreg(REG_SAVE_DATA); // 1 Para guardar datos, el programa la pone a cero luego de haber guardado los datos
    mb.addHreg(REG_COUNTER); // Contador interno para corroborar que funciona la conexion.

    CopyFloatValuesToModbusRegisters(REG_START_SETPOINTS, set, CANT_FERM);
    //checkSetpoints(set);
}

void ModbusTask()
{  
	//Call once inside loop() - all magic here
   mb.task();
   delay(10);
	
}


uint8_t checkSetpoints(float *set)
{
  uint8_t diff = 0;
  union{
      uint16_t u[2];
      float valor;
    } uval;
  // Busco las diferencias - Considerar hacer todo en un solo bucle... Solo para pruebas
  for(int i = 0; i < CANT_FERM; i++)
  {
    uval.u[0] = mb.Hreg(REG_START_SETPOINTS+(i*2)+1);
    uval.u[1] = mb.Hreg(REG_START_SETPOINTS+(i*2));
	
	debugPrint("Setpoint (");debugPrint(i); debugPrint(")=");debugPrintln(uval.valor);
    
    if(uval.valor != set[i])
    {
      diff++;
      set[i] = uval.valor;
    }
  }

  return diff;
}


void CopyFloatValuesToModbusRegisters(uint16_t direccion, float *f_val, uint8_t cantidad)
{
  uint16_t *seteos;
  seteos = (uint16_t*)f_val;
  for(int i = 0; i< cantidad; i++)
  {   
    mb.Hreg(direccion+(i*2), seteos[(i*2)+1]);
    mb.Hreg(direccion+(i*2)+1, seteos[(i*2)]);
    debugPrintln(seteos[(i*2)+1]);seteos[(i*2)];
  }
}

void transferWordStates(uint16_t direccion, uint8_t *values, uint8_t cantidad)
{
	for(int i = 0; i< cantidad; i++)
	{   
		mb.Hreg(direccion+i, values[i]);
	}
}

void getWordStates(uint16_t direccion, uint8_t *values, uint8_t cantidad)
{
	for(int i = 0; i< cantidad; i++)
	{   
		values[i] = mb.Hreg(direccion+i);
	}
}

void transferWordStates(uint16_t direccion, bool *values, uint8_t cantidad)
{
  for(int i = 0; i< cantidad; i++)
  {   
    mb.Hreg(direccion+i, values[i]);
  }
}

void getWordStates(uint16_t direccion, bool *values, uint8_t cantidad)
{
  for(int i = 0; i< cantidad; i++)
  {   
    values[i] = mb.Hreg(direccion+i);
  }
}

