#include <EtherCard.h>
#include <Modbus.h>
#include <ModbusIP_ENC28J60.h>

#define REG_START_TEMPS 0x01              //Address 400001
#define REG_START_SETPOINTS 0x33          //Address 400051
#define REG_START_VALVE_STATE_COLD 0x46        //Address 400070
#define REG_START_FORCE_ON_VALVE_COLD 0x50     //Address 400080
#define REG_START_FORCE_OFF_VALVE_COLD 0x5A    //Address 400090
#define REG_START_VALVE_STATE_HOT 0x64        //Address 400100
#define REG_START_FORCE_ON_VALVE_HOT 0x6E     //Address 400110
#define REG_START_FORCE_OFF_VALVE_HOT 0x78    //Address 400120
#define REG_START_CHILLER 0x82            //Address 400130
#define REG_START_HABILITATIONS 0xA0      //Address 400160
#define REG_START_PUMPS_A 0xAA            //Address 400170
#define REG_START_PUMPS_B 0xB9            //Address 400185
#define REG_START_HABCOLD 0xC8            //Address 400200
#define REG_START_HABHOT 0xD2             //Address 400210
#define REG_SAVE_DATA 0xDC                //Address 400220
#define REG_COUNTER 0xDE                  //Address 400222
#define N_REG_FLOAT 2

//SPI: 10 (SS), 11 (MOSI), 12 (MISO), 13 (SCK). 
#define CE_PIN 40



void ModbusTask();
//void ModbusInit();

void ModbusInit(float *set);
uint8_t checkSetpoints(float *set);

void CopyFloatValuesToModbusRegisters(uint16_t direccion, float *values, uint8_t cantidad);

void transferWordStates(uint16_t direccion, uint8_t *values, uint8_t cantidad);
void getWordStates(uint16_t direccion, uint8_t *values, uint8_t cantidad);

void transferWordStates(uint16_t direccion, bool *values, uint8_t cantidad);
void getWordStates(uint16_t direccion, bool *values, uint8_t cantidad);
