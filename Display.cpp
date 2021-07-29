#include "Display.h"

// initialize the library by providing the nuber of pins to it
LiquidCrystal lcd(6,7,8,9,10,11);

uint8_t estado_display;

void DisplayInit()
{
	  // Inicio LCD
  lcd.begin(20,4);
  // set cursor position to start of first line on the LCD
  lcd.clear();
  estado_display=0;
}

void DisplayTask(float *t, float *s)
{
	Display(t, s);
}


void Display(float *t, float *s)
{
  
  if(estado_display == 0)
  {
    lcd.clear();
    for(int i = 0; i < 4; i++)
    {
      lcd.setCursor(0,i);
      lcd.print("TEMP "+String(i+1)+"= ");
      lcd.print(t[i]);
      lcd.print(" C");
    }
  }
  else if(estado_display == 2)
  {
    lcd.clear();
    for(int i = 0; i < 4; i++)
    {
      lcd.setCursor(0,i);
      lcd.print("TEMP "+String(i+4+1)+"= ");
      lcd.print(t[i+4]);
      lcd.print(" C");
    }
  }
  else if(estado_display == 4)
  {
    lcd.clear();
    for(int i = 0; i < 4; i++)
    {
      lcd.setCursor(0,i);
      lcd.print("SET "+String(i+1)+"= ");
      lcd.print(s[i]);
      lcd.print(" C");
    }
  }
  else if(estado_display == 6)
  {
    lcd.clear();
    for(int i = 0; i < 4; i++)
    {
      lcd.setCursor(0,i);
      lcd.print("SET "+String(i+1+4)+"= ");
      lcd.print(s[i+4]);
      lcd.print(" C");
    }
  }
  
  estado_display++;
  if(estado_display == 8)
  {
    estado_display = 0;
  }

}
