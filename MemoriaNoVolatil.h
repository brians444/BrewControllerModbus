#include "Structs.h"
#include <EEPROM.h>



void cargar(uint8_t *destino, bool* origen, uint8_t nbits);
void descargar(uint8_t origen, bool* destino, uint8_t nbits);

void LeerEEPROM(float *sets, uint8_t *sal_f, uint8_t *sal_cal, bool *ftime, bool *hab_f, bool *hab_c, bool *hab,  uint8_t cant);

void CargarConfig(conf st, bool *var_frio, bool *var_calor, bool *hab, bool isFulltime, uint8_t *sal_f, uint8_t *sal_cal,  uint8_t cant);
void GuardarEnEEPROM();

void GuardarConfig(bool *hab_frio, bool *hab_cal, bool *hab, bool *f_time, uint8_t *sal_f, uint8_t *sal_cal, uint8_t cant);

void GuardarSetEEPROM(float *setp);
