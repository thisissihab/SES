#include <EEPROM.h>
int addr = 0, i = 0;
String employee_reg[30];
String temp_string="";
char temp;
int number_of_employees = 0;
void setup() {
  Serial.begin(115200);
  EEPROM.begin(512);  //Initialize EEPROM
  delay(2000);
  number_of_employees = EEPROM.read(addr);
  Serial.println("Number of Employees : "+(String)number_of_employees);
  addr++;
  while(i<number_of_employees){
    temp = (char) EEPROM.read(addr);
    //Serial.println(temp);
    if(temp == '#'){
      //Serial.print(temp);
      //Serial.print(i);
      //Serial.println(temp_string);
      employee_reg[i] = temp_string;
      //Serial.println(employee_reg[i]);
      temp_string="";
      i++;
      addr++;
      }else {
        temp_string += (String)temp;
        addr++;
      }
    }

  for(int k=0; k<number_of_employees; k++)
    Serial.println(employee_reg[k]);

}

void loop() {
  // put your main code here, to run repeatedly:

}
