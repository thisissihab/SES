
#include <SPI.h>
#include <MFRC522.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <EEPROM.h>
#include <Wire.h>
 
#define RST_PIN  D3          
#define SS_PIN D4           // Configurable, see typical pin layout above
 
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance
MFRC522::MIFARE_Key key;
MFRC522::StatusCode status;

String id, employee_name;
int j = 0;

const char *ssid = "Tech Rajshahi North";//"Tech Rajshahi North"  //ENTER YOUR WIFI ssid
const char *password = "Bangladesh123";  //ENTER YOUR WIFI password

String str = "rk#";
String str2 = "data";
String server_data;
String response = "";
int count = 0;

const int buttonPin = D8;    
bool found = false;
bool new_card_detected = false;
bool button_controller = false;
bool new_card_stored = false;

int ledState = HIGH;         
int buttonState;             
int lastButtonState = LOW;   

unsigned long lastDebounceTime = 0;  
unsigned long debounceDelay = 50;

int addr = 0, i = 0;
String employee_reg[30];
String temp_string="";
char temp;
int number_of_employees = 0;

  
void setup() {
  Serial.begin(115200);        // Initialize serial communications with the PC
  SPI.begin();               // Init SPI bus
  EEPROM.begin(512);
  Wire.begin(8); 
  //connectWifi();
  mfrc522.PCD_Init();        // Init MFRC522 card
  //Serial.println(F("Write personal data on a MIFARE PICC "));
  pinMode(buttonPin, INPUT);
  //connectWifi();
  read_memory();
  //delay(100);
  
  //delay(5000);
  //Serial.println(number_of_employees);
}
 
void loop() {
  //connectWifi();
  read_button();
  while(Serial.available()){
    
    char data = Serial.read();
    response += (String)data;
    //Serial.print(data);
    }
  if(response.equals("Done")){
    //Serial.println("Before : " +(String)number_of_employees);
    response = "";
    
    number_of_employees ++;
    write_to_memory();
    delay(100);
    /*EEPROM.write(0, number_of_employees);
    
    delay(100);
    Serial.println(id);
    while(id[i] != '\0'){
      EEPROM.write(addr, id[i]);
      i++;
      addr++;
      EEPROM.commit();
      delay(10);
      }*/
    
    //addr = 0;
    read_memory();
    delay(100);
    ESP.restart();
    /*int i=0;
    while(id[i] != '\0'){
      EEPROM.write(addr, id[i]);
      i++;
      addr++;
      }
      read_memory();
      Serial.println("After : " +(String)number_of_employees);*/
    }

  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;
  byte block;
  byte len;

  if ( ! mfrc522.PICC_IsNewCardPresent()) {
      return;
  }

  if ( ! mfrc522.PICC_ReadCardSerial()) {
      return;
  }

  byte buffer1[18];
  block = 4;
  len = 18;
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 4, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    //Serial.print(F("Authentication failed: "));
    //Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  
  status = mfrc522.MIFARE_Read(block, buffer1, &len);
  if (status != MFRC522::STATUS_OK) {
    //Serial.print(F("Reading failed: "));
    //Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
   String dump = ""; 
   dump += (String)'+';
   id = "";
   for (uint8_t i = 0; i < 5; i++)
   {
      id += (char)buffer1[i];
   }
   id.trim();
   
   //Serial.print("ID : "+ id);

   int i=0;
    while(i<number_of_employees){
      if(id.equals(employee_reg[i]))found = true;
      else {
        //if((id.length() == 5) || (id.length() == 10))new_card_detected = true;
        
      }
      i++;
      }
    
    server_data = id;
    if(found){
      id += (String)'#';
      dump += id;
      Serial.print(id);

      
      
      block = 5;
      len = 18;
      
      status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 4, &key, &(mfrc522.uid));
      if (status != MFRC522::STATUS_OK) {
      //Serial.print(F("Authentication failed: "));
      //Serial.println(mfrc522.GetStatusCodeName(status));
      return;
      }
      status = mfrc522.MIFARE_Read(block, buffer1, &len);
      if (status != MFRC522::STATUS_OK) {
      //Serial.print(F("Reading failed: "));
      //Serial.println(mfrc522.GetStatusCodeName(status));
      return;
      }
      employee_name ="";
      
      for (uint8_t i = 0; i < 16; i++)
      {
      employee_name += (char)buffer1[i];
      }
      
      employee_name.trim();
      employee_name += (String)'@';
      Serial.print(employee_name);
      //connectWifi();
      //send_to_server(server_data);
      found = false;
      }
    else{
      Serial.println("New Card Detected%");
      Serial.println("Do you want to register this ID?$");
     
    }

    /*if(new_card_detected){
      Serial.print('$');
      new_card_detected = false;
      button_controller = true;
      }*/

   

   
  
  //delay(3000);
  
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

void send_to_server(String data) {
  HTTPClient http;    //Declare object of class HTTPClient
  String employee_id, postData;
  int a = 5;
  //Post Data
  postData = "employee_id=8t105";
 
  http.begin("http://shahadmahmud.com/lockapi/sls/api/record.php");              //change the ip to your computer ip address
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //Specify content-type header
 
  int httpCode = http.POST(postData);   //Send the request
  String payload = http.getString();    //Get the response payload
 
  //Serial.println(httpCode);   //Print HTTP return code
  //Serial.println(payload);    //Print request response payload
 
  http.end();  //Close connection
  
  //delay(5000);  //Post Data at every 5 seconds
}

void connectWifi(){
  if(WiFi.status() != WL_CONNECTED){
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  delay(5000);
  }else{
    //Serial.println("Connected to Wifi");
  }
  //Serial.println("");
  //Serial.print("Connecting");
  // Wait for connection
  /*while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  }*/
  //If connection successful show IP address in serial monitor 
  //Serial.print("Connected to ");
  //Serial.println(ssid);
  //Serial.print("IP address: ");
  //Serial.println(WiFi.localIP());  //IP address assigned to your ESP
}

void read_button(){
  int reading = digitalRead(buttonPin);
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == HIGH) {
        ledState = !ledState;
        if(button_controller){
          button_controller = false;
          //number_of_employees ++;
          
          /*EEPROM.write(0, number_of_employees);
          while(id[i] != '\0'){
            EEPROM.write(addr, id[i]);
            i++;
            addr++;
            }
          read_memory();*/
          
          }
          Serial.print("+Yes)");
        }

      }
      
    }
  

  lastButtonState = reading;
}

void read_memory(){
  addr = 0;
  number_of_employees = EEPROM.read(addr);
  //Serial.println("After : " +(String)number_of_employees);
  //Serial.println("Number of Employees : "+(String)number_of_employees);
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
    //Serial.println(addr);
    //for(int k=0; k<number_of_employees; k++)
    //Serial.println(employee_reg[k]);
}

void write_to_memory(){
  addr = 0;
  EEPROM.write(addr, number_of_employees);
  addr++;
  //Serial.println("Initializing the chip");
  delay(2000);
  employee_reg[number_of_employees-1] = id;
  //Serial.println(employee_reg[number_of_employees-1]);
  for(int i=0; i<number_of_employees; i++){
    temp_string = employee_reg[i];
    while(temp_string[j] != '\0'){
      //Serial.print(temp_string[j]);
      EEPROM.write(addr, temp_string[j]);
      if(EEPROM.commit());//Serial.println(" -> Write Successful");
      else ;//Serial.println("Write failed");
      addr++;
      j++;
      }
    EEPROM.write(addr, '#');
    j=0; addr++;
    
    //Serial.println();
    //Serial.println(addr);
    }
  EEPROM.commit();
  }
