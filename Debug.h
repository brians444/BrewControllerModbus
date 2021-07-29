#include <String.h>
#include <stdint.h>
#include <hardwareSerial.h>

void debugInit(bool act);
void debugPrintSetpoints(float *s,  uint8_t cant);

void debugPrint(float v);
void debugPrint(int v);
void debugPrint(char v);
void debugPrint(char *v);
void debugPrint(uint16_t v);


void debugPrintln(float v);
void debugPrintln(int v);
void debugPrintln(char v);
void debugPrintln(char *v);
void debugPrintln(uint16_t v);
