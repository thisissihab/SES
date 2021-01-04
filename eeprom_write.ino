#include <EEPROM.h>
 
int addr = 0;
String temp_string;
int j = 0;
int number_of_employees=8;
String employee_reg[] = {"00011010DA", "00011011DA", "00011012DA", "00011013DA", "00011014DA","8t103", "8t104", "8t105"};
void setup()
{
  Serial.begin(115200);
  EEPROM.begin(512);  //Initialize EEPROM
  EEPROM.write(addr, number_of_employees);
  addr++;
  Serial.println("Initializing the chip");
  delay(2000);
  for(int i=0; i<8; i++){
    temp_string = employee_reg[i];
    while(temp_string[j] != '\0'){
      Serial.print(temp_string[j]);
      EEPROM.write(addr, temp_string[j]);
      if(EEPROM.commit())Serial.println("Write Successful");
      else Serial.println("Write failed");
      addr++;
      j++;
      }
    EEPROM.write(addr, '#');
    j=0; addr++;
    
    Serial.println();
    Serial.println(addr);
    }
  EEPROM.commit();
  
  // write to EEPROM.
  number_of_employees = EEPROM.read(addr);
  
}
 
void loop()
{   
  
}
