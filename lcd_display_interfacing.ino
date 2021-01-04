#include <MCUFRIEND_kbv.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11);
MCUFRIEND_kbv tft;

// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define GRAY    0xDCDC

uint16_t version = MCUFRIEND_KBV_H_;
const char *aspectname[] = {
        "PORTRAIT", "LANDSCAPE", "PORTRAIT_REV", "LANDSCAPE_REV"
    };
char data[] = {'H', 'E', 'L', 'L', 'O'};
char control;
int y_axis = 40;
int y_axis_pv;
unsigned long pv = 0, manual_pv = 0;
String identity = "";
String employee_name = "";
String str_pv = "";
String code = "";
char previous_data = ' ';
bool manual_mode = false;
bool received_data = false;
int count = 0;
String temp = "";

void setup()
{
    Serial.begin(115200);
    mySerial.begin(9600);
    if (!Serial) delay(5000);           //allow some time for Leonardo
    uint16_t ID = tft.readID(); //
    tft.begin(ID);
    tft.setRotation(0);

    tft.fillScreen(BLUE);
    tft.setTextSize(5);
    tft.setTextColor(WHITE);
    tft.setCursor(60, 110);
    tft.println("TARS");
    tft.setCursor(45, 180);
    tft.println("BINARY");
    delay(3000);
    pinMode(A5, OUTPUT);

    
    
}

void loop()
{
  
  while(Serial.available()){
    char data = Serial.read();
    if(data == '#'){
      identity = temp;
      identity += " BD0";
      temp = "";
      }
    else if(data == '@'){
      employee_name = temp;
      received_data = true;
      temp = "";
      }
    else temp += (String)data;
  }
  if(received_data){
      
      received_data = false;
      tft.fillScreen(GRAY);
      tft.fillRect(0,0,240, 70,BLUE);
      tft.setTextSize(3);
      tft.setTextColor(WHITE);
      tft.setCursor(20, 15);
      tft.println("RFID Entry");
      tft.setCursor(70, 40);
      tft.println("Mode");

      
      tft.setTextSize(1);
      tft.setTextColor(BLACK);
      tft.setCursor(5, 75);
      tft.println("User ID : ");
      tft.setCursor(115, 75);
      tft.println(identity);
      
     
      tft.setTextSize(1);
      tft.setTextColor(BLACK);
      tft.setCursor(5, 105);
      tft.println("Name: ");
      tft.setCursor(70, 105);
      tft.println(employee_name);
      
   
      access_granted();

      tft.fillRect(0,280,240, 40,BLUE);
      tft.setTextSize(2);
      tft.setTextColor(WHITE);
      tft.setCursor(30, 290);
      tft.println("Swing Your ID");
      
      if(identity.equals("rk"))manual_mode = true;
      count ++;
      
      }else if(data == '*'){
        
      tft.setTextSize(4);
      tft.setTextColor(RED);
      tft.setCursor(45, 150);
      tft.println("ACCESS");

      tft.setTextSize(4);
      tft.setTextColor(RED);
      tft.setCursor(45, 200);
      tft.println("DENIED");

      delay(3000);

      tft.setTextSize(4);
      tft.setTextColor(GRAY);
      tft.setCursor(45, 150);
      tft.println("ACCESS");

      tft.setTextSize(4);
      tft.setTextColor(GRAY);
      tft.setCursor(45, 200);
      tft.println("DENIED");
        
     }
     
   
  if(mySerial.available() || manual_mode){
    manual_mode = false;
    char data = mySerial.read();
    

    code += (String) data;
    tft.fillScreen(GRAY);
    tft.fillRect(0,0,240, 70,BLUE);
    tft.setTextSize(3);
    tft.setTextColor(WHITE);
    tft.setCursor(20, 15);
    tft.println("Manual Entry");
    tft.setCursor(70, 40);
    tft.println("Mode");

    
    tft.setTextSize(2);
    tft.setTextColor(BLACK);
    tft.setCursor(5, 75);
    tft.println("Entered Code : ");
    tft.setCursor(180, 75);
    tft.println(code);

    tft.setTextSize(7);
    tft.setTextColor(GRAY);
    tft.setCursor(100, 145);
    tft.println(previous_data);

    tft.setTextSize(7);
    tft.setTextColor(BLACK);
    tft.setCursor(100, 145);
    tft.println(data);

    tft.fillRect(0,280,240, 40,BLUE);
    tft.setTextSize(2);
    tft.setTextColor(WHITE);
    tft.setCursor(40, 290);
    tft.println("Reading Keypad");

    previous_data = data;
    
    if( code.length() == 4){
      tft.setTextSize(7);
      tft.setTextColor(GRAY);
      tft.setCursor(100, 145);
      tft.println(previous_data);
    if(code.equals("1234")){
      tft.setTextSize(4);
      tft.setTextColor(WHITE);
      tft.setCursor(45, 150);
      tft.println("ACCESS");

      tft.setTextSize(4);
      tft.setTextColor(WHITE);
      tft.setCursor(35, 200);
      tft.println("GRANTED");
      
      delay(3000);

      tft.setTextSize(4);
      tft.setTextColor(GRAY);
      tft.setCursor(45, 150);
      tft.println("ACCESS");

      tft.setTextSize(4);
      tft.setTextColor(GRAY);
      tft.setCursor(35, 200);
      tft.println("GRANTED");
    }
    else{
      tft.setTextSize(4);
      tft.setTextColor(RED);
      tft.setCursor(45, 150);
      tft.println("ACCESS");

      tft.setTextSize(4);
      tft.setTextColor(RED);
      tft.setCursor(45, 200);
      tft.println("DENIED");

      delay(3000);

      tft.setTextSize(4);
      tft.setTextColor(GRAY);
      tft.setCursor(45, 150);
      tft.println("ACCESS");

      tft.setTextSize(4);
      tft.setTextColor(GRAY);
      tft.setCursor(45, 200);
      tft.println("DENIED");
    }
    tft.setTextSize(2);
    tft.setTextColor(GRAY);
    tft.setCursor(5, 75);
    tft.println("Entered Code : "+ code);
    tft.setCursor(180, 75);
    tft.println(code);
    code = "";
    tft.setTextSize(2);
    tft.setTextColor(BLACK);
    tft.setCursor(5, 75);
    tft.println("Entered Code : ");
   }
      
  }
  /*if(Serial.available()) control = Serial.read();

  if(control == '1')
      print_on_screen(40, 40, "TARS Robotics");
  else if(control == '2')
      clear_screen(40, 40, "TARS Robotics");
      
  print_on_screen(40, 40, "TARS Robotics");
  digitalWrite(A5, HIGH);
  delay(1000);
  clear_screen(40, 40, "TARS Robotics");
  digitalWrite(A5, LOW);
  delay(1000);*/

  /*if(millis() - pv > 4000){
    tft.fillScreen(WHITE);
    y_axis = 40;
    }*/
  
}

void print_on_screen(int x, int y, String msg){
    tft.setTextSize(2);
    tft.setTextColor(BLACK);
    tft.setCursor(x, y);
    tft.println(msg);
  }



void clear_screen(int x, int y, String msg){
    tft.setTextSize(2);
    tft.setTextColor(BLACK);
    tft.setCursor(x, y);
    tft.println(msg);
  }

void revealing_character_print(int x, int y, String msg){
  int location[] = {15, 30, 45, 60, 75};
  int i;
  while(msg[i] != '\0'){
    tft.setTextSize(2);
    tft.setTextColor(WHITE);
    tft.setCursor(x+location[i], y);
    tft.println(data[i]);
    i++;
    delay(100);
  }
}

void unrevealing_character_print(int x, int y, String msg){
  int location[] = {15, 30, 45, 60, 75};
  int i;
  while(msg[i] != '\0'){
    tft.setTextSize(2);
    tft.setTextColor(BLACK);
    tft.setCursor(x+location[i], y);
    tft.println(data[i]);
    i++;
    delay(100);
  }
}

void access_granted(){
    tft.setTextSize(4);
    tft.setTextColor(WHITE);
    tft.setCursor(45, 150);
    tft.println("ACCESS");

    tft.setTextSize(4);
    tft.setTextColor(WHITE);
    tft.setCursor(35, 200);
    tft.println("GRANTED");
  }
