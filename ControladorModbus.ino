#include "MemoriaNoVolatil.h"
#include "Display.h"
#include "ModbusTCP.h"

// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>
// Date and time functions uspiing a DS1307 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 rtc;


// Cable de datos conectado al pin 3
#define ONE_WIRE_BUS 3
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
DateTime now;

long STATE = 1;

void inputInit();
void outputInit();
void outputTransfer();


/******* VaRIABLES DEL MAIN *******/
bool habilitados[CANT];
bool fulltime;
bool frio[CANT];    // Guardo si hay que enfriar o calentar
bool calor[CANT];    // Guardo si hay que enfriar o calentar
uint8_t salidas_frio[CANT] ; // Aca pongo un 1 un cero para prender
uint8_t salidas_calor[CANT]; // Aca pongo un 1 un cero para prender

uint8_t forced_on_hot[CANT];		// Con un uno fuerzo la salida
uint8_t forced_off_hot[CANT];		// Con un uno fuerzo la salida

uint8_t forced_on_cold[CANT];		// Con un uno fuerzo la salida
uint8_t forced_off_cold[CANT];		// Con un uno fuerzo la salida

//const uint8_t pin_salidas_frio[CANT] = {31, 33, 35, 37,39, 41,43, 45};
// DE forma temporal uso la salida 33 para la bomba. 
const uint8_t pin_salidas_frio[CANT] = {31, 45, 35, 37,39, 41,43, 45};

const uint8_t pin_salidas_calor[CANT] = {29, 27, 25, 23, 26, 28, 30, 32};
const uint8_t array_pin_botones[4] = {4, 5, 12, 13};

uint8_t state_cold[CANT];
uint8_t state_hot[CANT];


long valores[CANT];
float values[CANT];
float temp[CANT+2];
float set[CANT];


int cont = 0;
int leidos = 0;
int serial_state = 0;
//long comando = 1;
uint16_t contador = 0; 
int estado = 0;

unsigned long actual_time, back_time;
uint8_t estado_main;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
const int chipSelect = 25;

unsigned long startTime;
unsigned long elapsedTime;
/*
 * The setup function. We only start the sensors here
 */
void setup(void)
{
  fulltime = 0;
  leidos = 0;
  // Inicializo los valores.
  for(int i = 0; i < CANT; i++)
  {
	  state_cold[i] = 0;
	  state_hot[i] = 0;
    temp[i] = 0.0;
    set[i] = 18.0;
    habilitados[i] = true;
    frio[i] = true;
    //salidas_frio[i] = 0;
    //salidas_calor[i] = 0;
	/**** SACO TODOS LOS FORZADOS ****/
	forced_off_hot[i]=0;
  forced_on_hot[i]=0;
	
	forced_off_cold[i]=0;
  forced_on_cold[i]=0;
  }
  
  inputInit();
  outputInit();
  
  debugInit(0);

  DisplayInit();

  set[0] = 21.0;
  ModbusInit(set);

  rtcInit();

  // Start up the library
  sensors.begin();

  // void LeerEEPROM(float *sets, uint8_t *sal_f, uint8_t *sal_cal, uint8_t *ftime, bool *hab_f, bool *hab_c, bool *hab,  uint8_t cant)
  LeerEEPROM(set, salidas_frio, salidas_calor, &fulltime, frio, calor, habilitados, CANT_FERM );
  // Los valores que se mantienen los cargo en la tabla modbus.
  transferWordStates(REG_START_HABILITATIONS, habilitados, CANT_FERM);
  transferWordStates(REG_START_HABCOLD, frio, CANT_FERM);
  transferWordStates(REG_START_HABHOT, calor, CANT_FERM);
  CopyFloatValuesToModbusRegisters(REG_START_SETPOINTS, set, CANT_FERM);
  debugPrintSetpoints(set, CANT);
  
  fulltime = 1;
  
  
 
  // botones de entrada
  /*
  pinMode(Button1,INPUT_PULLUP);
  pinMode(Button2,INPUT_PULLUP);
  pinMode(Button3,INPUT_PULLUP);
  pinMode(Button4,INPUT_PULLUP);
  */

  back_time = millis();
  estado_main = 0;
  
	//LeerEEPROM();
  fulltime = 1;

}
 
/*
 * Main function, get and show the temperature
 */
void loop(void)
{
  
  actual_time = millis();
  if( (actual_time - back_time) >3000)		// Ejecuto el control cada 3 segundos
  {
    back_time = actual_time;
    now = rtc.now();      // Reloj de tiempo real!!!!!!!
    mostrarHora();
    
    updateInternalFromTable();
    readTemperatures();
    ControlFunction();
    outputTransfer();
    updateTableFromInternal();
    
    DisplayTask(temp, set);
    estado_main = 1;
    if(checkSetpoints(set) > 0)
    {
      debugPrintln("Found diff save eeprom");
      GuardarSetEEPROM(set);  
    }
  }
  
  ModbusTask();
}


void ControlFunction()
{
  int minuto = now.minute();
  //Serial.print("Funcion de control - Minuto:"); Serial.print(minuto); Serial.print(" Estado = ");Serial.println(estado);
  if(minuto <=30 || fulltime)
  {
      for(int i =0; i < CANT; i++)
      {
        //Serial.print("Control temp (");Serial.print(i);Serial.println(")");

        /*************************+++ VER LA FUNCION HABILITADOS, SI SE DESHABILITA estando la salida activa QUEDA activa al deshabilitar *********/
        if(habilitados[i])
        {
          if(frio[i])
          {
            debugPrint(" Controlando frio = ");debugPrintln(i);
            //Serial.print(" Controlando frio = ");Serial.println(i);
            ControlTempFrio(i);
          }
          if(calor[i])
          {
            debugPrint(" Controlando calor = ");debugPrintln(i);
            ControlTempCalor(i);
          }
        }
        else
        {
          // Apago salidas
          debugPrint(" Apagando salida (desactivada) = ");debugPrintln(i);
          salidas_frio[i] = 0;
		      salidas_calor[i] = 0;
        }
      }
      //checkWaterPumps();
  }
    
}

void ControlBombas() 		/*************** REVISARRRRRR - DEFINIR SALIDA *****************/
{
  if(contador > 0 )
  {
    //enciendo la bomba
    
    //digitalWrite(S5,LOW);
  }
  else
  {
    //apago la bomba
    //digitalWrite(S5,HIGH);
  }
}


void ControlTempFrio(int i)
{
  if(temp[i] > set[i] + 0.2 ) /******************** TEMP ALTA - Abrir agua fria *****************/
  {
  	debugPrint("Enciendo agua fria=");debugPrintln(i);
  	salidas_frio[i] = 1;
  }
  else if(temp[i] < set[i] - 0.2)     /******************** TEMP BAJA - Cerrar agua fria *****************/
  {
    debugPrint("Apago agua fria=");debugPrintln(i);
    salidas_frio[i] = 0;
  }
}

void ControlTempCalor(int i)
{
    if(temp[i] < set[i] - 0.2 ) /******************** TEMP BAJA - Abrir agua caliente *****************/
    {
  		debugPrint("Enciendo agua caliente=");debugPrintln(i);
  		salidas_calor[i] = 1;
    }
    else if(temp[i] > set[i] + 0.2)     /******************** TEMP ALTA - Cerrar agua caliente *****************/
    {
  		debugPrint("Apago agua caliente=");debugPrintln(i);
  		salidas_calor[i] = 0;
    }
}

void readTemperatures() // Leo las temperaturas y las escribo en la tabla modbus
{
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  debugPrintln("Leyendo temperatura...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  // After we got the temperatures, we can print them here.
  // We use the function ByIndex, and as an example get the temperature from the first sensor only.
  for(int i = 0; i < CANT_FERM; i++)
  {
    temp[i] = sensors.getTempCByIndex(i);
  }

}

void mostrarHora()
{
  #if DEBUG
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
  #endif
}

void rtcInit()
{
  if (! rtc.begin()) {
    debugPrintln("Couldn't find RTC");
    while (1);
  }
  
  if (! rtc.isrunning()) {
    debugPrintln("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }  

}

void inputInit()
{
	for(uint8_t i = 0; i<4; i++)
	{
		pinMode(array_pin_botones[i], INPUT);
	}
}

void outputInit()
{
  // set the digital pin as output:
  for(uint8_t i = 0; i<CANT; i++)
  {
    pinMode(pin_salidas_frio[i], OUTPUT);
    digitalWrite(pin_salidas_frio[i], 1); // En el arranque apagamos las salidas 
  }
  for(uint8_t i = 0; i<CANT; i++)
  {
    pinMode(pin_salidas_calor[i], OUTPUT);
    digitalWrite(pin_salidas_calor[i], 1); // En el arranque apagamos las salidas 
  }
}

void outputTransfer()
{
  /****** Enciendo o apago salidas con y sin forzado -
  *** Tiene prioridad el forzado de apagado por si estan ambos activos ******/ 
  /*** Primero leo los valores de forzado desde el modbus **/

  
  /***********++ SALIDAS FRIO **********/
  for(uint8_t i = 0; i<CANT; i++)
  {
    /***************************************************************************** ELIMINAR UNA VEZ IMPLEMENTADO EL CONTROL DE LA BOMBA *****/
    if(i==0)
    {
      if((salidas_frio[i] == 1 || forced_on_cold[i] == 1)  && forced_off_cold[i] == 0)
      {
        //Prendo bomba
        pinMode(33, OUTPUT);
        digitalWrite(33, 0); //  Prendo la bomba
      }
      else
      {
        //Apago bomba
        pinMode(33, OUTPUT);
        digitalWrite(33, 1); //  Prendo la bomba
      }
    }
  	if((salidas_frio[i] == 1 || forced_on_cold[i] == 1)  && forced_off_cold[i] == 0)
  	{
  		digitalWrite(pin_salidas_frio[i], 0); // La salida prende por cero
  		state_cold[i] = 1;
  	}
  	else if(salidas_frio[i] == 0 || forced_off_cold[i] == 1)
  	{
  		digitalWrite(pin_salidas_frio[i], 1); // La salida se apaga por uno
  		state_cold[i] = 0;
  	}
  }
  /***********++ SALIDAS CALOR **********/
  for(uint8_t i = 0; i<CANT; i++)
  {
  	if((salidas_calor[i] == 1 || forced_on_hot[i] == 1) && forced_off_hot[i] == 0)
  	{
  		digitalWrite(pin_salidas_calor[i], 0); // La salida prende por cero
  		state_hot[i] = 1;
  	}
  	else if(salidas_calor[i] == 0 || forced_off_hot[i] == 1)
  	{
  		digitalWrite(pin_salidas_calor[i], 1); // La salida se apaga por uno
  		state_hot[i] = 0;
  	}
  }

}

void updateInternalFromTable()
{
  getWordStates(REG_START_FORCE_ON_VALVE_COLD, forced_on_cold, CANT);
  getWordStates(REG_START_FORCE_OFF_VALVE_COLD, forced_off_cold, CANT);
  getWordStates(REG_START_FORCE_ON_VALVE_HOT, forced_on_hot, CANT);
  getWordStates(REG_START_FORCE_OFF_VALVE_HOT, forced_off_hot, CANT);
  getWordStates(REG_START_HABILITATIONS, habilitados, CANT_FERM);
  getWordStates(REG_START_HABCOLD, frio, CANT_FERM);
  getWordStates(REG_START_HABHOT, calor, CANT_FERM);
}

void updateTableFromInternal()
{
  // Copio las temperaturas en la tabla modbus. 
  CopyFloatValuesToModbusRegisters(REG_START_TEMPS, temp, CANT_FERM);
    /*** Transfiero las salidas al mapa modbus **/
  transferWordStates(REG_START_VALVE_STATE_COLD, state_cold, CANT);
  transferWordStates(REG_START_VALVE_STATE_HOT, state_hot, CANT);
}



