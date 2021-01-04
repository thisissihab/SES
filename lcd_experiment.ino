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
#define deep_green 0x0452
#define light_green 0x0680

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
bool new_card = false;
int count = 0;
bool register_card = false;
String temp = "";
String top_string = "RFID Entry Mode";
String bottom_string = "Hold ID Near the Reader";
String new_id = "";
String inf = "";

void setup()
{
    Serial.begin(115200);
    mySerial.begin(9600);
    if (!Serial) delay(5000);           //allow some time for Leonardo
    uint16_t ID = tft.readID(); //
    tft.begin(ID);
    tft.setRotation(1);

    tft.fillScreen(BLUE);
    tft.setTextSize(5);
    tft.setTextColor(WHITE);
    tft.setCursor(95, 75);
    tft.println("TARS");
    tft.setCursor(80, 135);
    tft.println("BINARY");
    delay(3000);
    

    
    
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
    else if(data == '%'){
      //new_card = true;
      //received_data = true;
      new_id = temp;
      temp = "";
      break;
      }
    else if(data == '$'){
      new_card = true;
      received_data = true;
      inf = temp;
      break;
      }
    else if(data == ')'){
      top_bottom(top_string, bottom_string);
      tft.setTextSize(4);
      tft.setTextColor(RED);
      tft.setCursor(95, 105);
      tft.println("ENTER");

      tft.setTextSize(4);
      tft.setTextColor(RED);
      tft.setCursor(60, 140);
      tft.println("PASSWORD");
      register_card = true;
      temp = "";
      break;
      }
    else temp += (String)data;
  }
  if(received_data){
      
      received_data = false;
      if(!new_card){
      /*tft.fillScreen(GRAY);
      tft.fillRect(0,0,320, 50,BLUE);
      tft.setTextSize(3);
      tft.setTextColor(WHITE);
      tft.setCursor(20, 15);
      tft.println("RFID Entry Mode");*/
      //tft.setCursor(70, 40);
      //tft.println("Mode");
      top_bottom(top_string, bottom_string);

      
      tft.setTextSize(2);
      tft.setTextColor(BLACK);
      tft.setCursor(15, 60);
      tft.println("User ID: ");
      tft.setCursor(120, 60);
      tft.println(identity);
      
     
      tft.setTextSize(2);
      tft.setTextColor(BLACK);
      tft.setCursor(15, 80);
      tft.println("Name: ");
      tft.setCursor(80, 80);
      tft.println(employee_name);
      
   
      access_granted();

      /*tft.fillRect(0,200,320, 40,BLUE);
      tft.setTextSize(2);
      tft.setTextColor(WHITE);
      tft.setCursor(15, 210);
      tft.println("Hold ID Near the Reader");*/
      
      if(identity.equals("rk"))manual_mode = true;
      
      
      }
      else{
        top_bottom(top_string, bottom_string);
        
        tft.setTextSize(2);
        tft.setTextColor(BLACK);
        tft.setCursor(15, 57);
        tft.println("New Card Detected !!");
      
        tft.setCursor(15, 75);
        tft.println("Do You Want to Register");
        tft.setCursor(15, 93);
        tft.println("It to the System?");
      
        tft.setTextSize(4);
        tft.setTextColor(RED);
        tft.setCursor(10, 135);
        tft.println("ACCESS");

        tft.setTextSize(4);
        tft.setTextColor(RED);
        tft.setCursor(170, 135);
        tft.println("DENIED");
        
        temp = "";
        new_card = false;
        }
  }

  
   
  if(mySerial.available() || manual_mode){
    manual_mode = false;
    char data = mySerial.read();
    

    code += (String) data;
    tft.fillScreen(GRAY);
    tft.fillRect(0,0,320, 50,BLUE);
    tft.setTextSize(3);
    tft.setTextColor(WHITE);
    tft.setCursor(10, 15);
    tft.println("Manual Entry Mode");
    
    tft.setTextSize(2);
    tft.setTextColor(BLACK);
    tft.setCursor(5, 75);
    tft.println("Entered Pass : ");
    tft.setCursor(180, 75);
    tft.println(code);

    tft.setTextSize(7);
    tft.setTextColor(GRAY);
    tft.setCursor(100, 145);
    tft.println(previous_data);

    tft.setTextSize(7);
    tft.setTextColor(BLACK);
    tft.setCursor(140, 120);
    tft.println(data);

    tft.fillRect(0,200,320, 40,BLUE);
    tft.setTextSize(2);
    tft.setTextColor(WHITE);
    tft.setCursor(10, 210);
    tft.println("Use Keypad to Enter Pass");

    previous_data = data;
    
    if( code.length() == 4){
      tft.setTextSize(7);
      tft.setTextColor(GRAY);
      tft.setCursor(140, 120);
      tft.println(previous_data);
    if(code.equals("1234")){
      /*tft.setTextSize(4);
      tft.setTextColor(RED);
      tft.setCursor(10, 135);
      tft.println("ACCESS");

      tft.setTextSize(4);
      tft.setTextColor(RED);
      tft.setCursor(170, 135);
      tft.println("GRANTED");

      delay(3000);

      tft.setTextSize(4);
      tft.setTextColor(GRAY);
      tft.setCursor(10, 135);
      tft.println("ACCESS");

      tft.setTextSize(4);
      tft.setTextColor(GRAY);
      tft.setCursor(170, 135);
      tft.println("GRANTED");*/
      if(!register_card){
      access_granted();
      delay(3000);
      access_granted_erase();
      }
      else{
        card_registered();
        Serial.print("Done");
        delay(3000);
        card_registered_erase();
        register_card = false;
        }
    }
    else{
      if(!register_card){
      tft.setTextSize(4);
      tft.setTextColor(RED);
      tft.setCursor(10, 135);
      tft.println("ACCESS");

      tft.setTextSize(4);
      tft.setTextColor(RED);
      tft.setCursor(170, 135);
      tft.println("DENIED");

      delay(3000);

      tft.setTextSize(4);
      tft.setTextColor(GRAY);
      tft.setCursor(10, 135);
      tft.println("ACCESS");

      tft.setTextSize(4);
      tft.setTextColor(GRAY);
      tft.setCursor(170, 135);
      tft.println("DENIED");
      }else{
        wrong_password();
        delay(3000);
        wrong_password_erase();
        }
    }
    tft.setTextSize(2);
    tft.setTextColor(GRAY);
    tft.setCursor(5, 75);
    tft.println("Entered Pass : "+ code);
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
    tft.setTextColor(RED);
    tft.setCursor(90, 115);
    tft.println("ACCESS");

    tft.setTextSize(4);
    tft.setTextColor(RED);
    tft.setCursor(83, 150);
    tft.println("GRANTED");
  }

void card_registered(){
    tft.setTextSize(4);
    tft.setTextColor(RED);
    tft.setCursor(90, 115);
    tft.println("CARD");

    tft.setTextSize(4);
    tft.setTextColor(RED);
    tft.setCursor(40, 150);
    tft.println("REGISTERED");
  }

void card_registered_erase(){
    tft.setTextSize(4);
    tft.setTextColor(GRAY);
    tft.setCursor(90, 115);
    tft.println("CARD");

    tft.setTextSize(4);
    tft.setTextColor(GRAY);
    tft.setCursor(40, 150);
    tft.println("REGISTERED");
  }

void wrong_password_erase(){
    tft.setTextSize(4);
    tft.setTextColor(GRAY);
    tft.setCursor(90, 115);
    tft.println("WRONG");

    tft.setTextSize(4);
    tft.setTextColor(GRAY);
    tft.setCursor(40, 150);
    tft.println("PASSWORD");
  }

void wrong_password(){
    tft.setTextSize(4);
    tft.setTextColor(RED);
    tft.setCursor(90, 115);
    tft.println("WRONG");

    tft.setTextSize(4);
    tft.setTextColor(RED);
    tft.setCursor(40, 150);
    tft.println("PASSWORD");
  }

void access_granted_erase(){
    tft.setTextSize(4);
    tft.setTextColor(GRAY);
    tft.setCursor(90, 115);
    tft.println("ACCESS");

    tft.setTextSize(4);
    tft.setTextColor(GRAY);
    tft.setCursor(83, 150);
    tft.println("GRANTED");
  }

void access_denied(){
    tft.setTextSize(4);
    tft.setTextColor(RED);
    tft.setCursor(90, 115);
    tft.println("ACCESS");

    tft.setTextSize(4);
    tft.setTextColor(RED);
    tft.setCursor(83, 150);
    tft.println("DENIED");
  }

void top_bottom(String str1, String str2){
    tft.fillScreen(GRAY);
      tft.fillRect(0,0,320, 50,BLUE);
      tft.setTextSize(3);
      tft.setTextColor(WHITE);
      tft.setCursor(20, 15);
      tft.println(str1);

    tft.fillRect(0,200,320, 40,BLUE);
      tft.setTextSize(2);
      tft.setTextColor(WHITE);
      tft.setCursor(15, 210);
      tft.println(str2);
  }
