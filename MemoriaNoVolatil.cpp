
#include "MemoriaNoVolatil.h"


union mem_t{
   target dato;
   unsigned char b[sizeof(target)];
}memoria_target;

union mem_c{
   conf dato;
   unsigned char b[sizeof(conf)];
}memoria_conf;


conf st_conf;
sensores st_sens;
target st_target;



/***************
 * Transforma una array de booleanos en una variable uint8_t
 * Se utiliza cuando se desea enviar la estructura st_conf para las habilitaciones
 */

void cargar(uint8_t *destino, bool* origen, uint8_t nbits)
{
  uint8_t temp = 0;
  for(char i = 0; i < nbits; i++)
  {
    bool temp_val;
    temp_val = origen[i];
    temp += temp_val<<i;
  }
  *destino = temp;
}

/************************ 
 *  Transforma una variable de uint8_t en un array de booleanos
 *  Se utiliza cuando se recibe la estructura st_conf para las habilitaciones de salidas.
 */
void descargar(uint8_t origen, bool* destino, uint8_t nbits)
{
  uint8_t temp = 0;
  for(char cont = nbits-1; cont >= 0; cont--)
  {
    bool s;
    s = (bool)(( origen>>cont)&0x01);
    destino[cont] = s;
  }
}


/***************************
 * Se usa para cargar la configuracion en la estructura
 */

void CargarConfig(bool *var_frio, bool *var_calor, bool *hab, bool isFulltime, uint8_t *sal_f, uint8_t *sal_cal,  uint8_t cant)
{
  cargar(&st_conf.frio , var_frio, cant);
  cargar(&st_conf.calor , var_calor , cant);
  cargar(&st_conf.habilitado , hab , cant);
  st_conf.fulltime = isFulltime;
  for(int i = 0; i< CANT; i++)
  {
    st_conf.salida_frio[i] =  sal_f[i];
    st_conf.salida_calor[i] =  sal_cal[i];
  }
  st_conf.temp[0] = 0.0;
  st_conf.temp[1] = 0.0;
  st_conf.cte1 = 0xF0; st_conf.cte4 = 0x0F; st_conf.cte3 = 0x11; st_conf.cte2 = 0x99;
}

void GuardarConfig(bool *hab_frio, bool *hab_cal, bool *hab, bool *f_time, uint8_t *sal_f, uint8_t *sal_cal, uint8_t cant)
{
  descargar(st_conf.frio , hab_frio, cant);
  descargar(st_conf.calor , hab_cal , cant);
  descargar(st_conf.habilitado , hab , cant);
  *f_time = st_conf.fulltime;
  for(int i = 0; i< cant; i++)
  {
    sal_f[i] = st_conf.salida_frio[i];
    sal_cal[i] = st_conf.salida_calor[i];
  }
  st_conf.temp[0] = 0.0;
  st_conf.temp[1] = 0.0;
}


void GuardarEnEEPROM()
{
  // Estructuras a escribir: st_conf st_target
  int eeAddress = 0;   //Location we want the data to be put.

  //Data to store.
  //Serial.println("GRABANDO EEPROM");
  EEPROM.put(eeAddress, st_conf);
  eeAddress += sizeof(st_conf); //Move address to the next byte after float 'f'.
  //Serial.print("Grabado CONFIGURACION tamaño = ");Serial.println(eeAddress);
  EEPROM.put(eeAddress, st_target);
  eeAddress += sizeof(st_target); //Move address to the next byte after float 'f'.
  //Serial.print("Grabado TARGET tamaño = ");Serial.println(eeAddress);
}

void GuardarSetEEPROM(float *setp)
{
  int eeAddress = 0;   //Location we want the data to be put.
  eeAddress += sizeof(st_conf); //Move address to the next byte after float 'f'.
  for(int i = 0; i < CANT; i++)
  {
    st_target.set_temp[i] = setp[i];
  }
  EEPROM.put(eeAddress, st_target);
  
}

void LeerEEPROM(float *sets, uint8_t *sal_f, uint8_t *sal_cal, bool *ftime, bool *hab_f, bool *hab_c, bool *hab,  uint8_t cant)
{
  //Serial.println("Leyendo EEPROM");
  int eeAddress = 0;   //Location we want the data to be put.
  //Data to store.
  for(int i=0 ; i<sizeof(st_conf) ; i++, eeAddress++ )
      memoria_conf.b[i] = EEPROM.read( eeAddress );

  for(int i=0 ; i<sizeof(st_target) ; i++, eeAddress++)
      memoria_target.b[i] = EEPROM.read( eeAddress );

  st_conf = memoria_conf.dato;
  //  GuardarConfig(conf st, bool *hab_frio, bool *hab_cal, bool *hab, bool *f_time, uint8_t *sal_f, uint8_t *sal_cal uint8_t cant);
  GuardarConfig( hab_f, hab_c, hab, ftime, sal_f, sal_cal, cant);


  // Devuelvo los valores de setpoints 
  st_target = memoria_target.dato;
  for(int i = 0; i< cant; i++)
  {
    sets[i] = st_target.set_temp[i];
  }

  
      
  /*EEPROM.get(eeAddress, st_conf);
  eeAddress += sizeof(st_conf); //Move address to the next byte after float 'f'.
  Serial.print("Leida CONFIGURACION tamaño = ");Serial.println(eeAddress);
  EEPROM.get(eeAddress, st_target);
  eeAddress += sizeof(st_target); //Move address to the next byte after float 'f'.
  */
  //Serial.print("Leida TARGET tamaño = ");Serial.println(eeAddress);


}
