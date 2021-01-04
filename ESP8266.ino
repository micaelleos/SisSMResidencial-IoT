/*
This example shows how to connect to Cayenne using an ESP8266 and send/receive sample data.

The CayenneMQTT Library is required to run this sketch. If you have not already done so you can install it from the Arduino IDE Library Manager.

Steps:
1. If you have not already installed the ESP8266 Board Package install it using the instructions here: https://github.com/esp8266/Arduino#installing-with-boards-manager.
2. Select your ESP8266 board from the Tools menu.
3. Set the Cayenne authentication info to match the authentication info from the Dashboard.
4. Set the network name and password.
5. Compile and upload the sketch.
6. A temporary widget will be automatically generated in the Cayenne Dashboard. To make the widget permanent click the plus sign on the widget.
*/

//#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP8266.h>
#include <SoftwareSerial.h>
#define VIRTUAL_CHANNEL1 1 //
#define VIRTUAL_CHANNEL2 2 //
#define VIRTUAL_CHANNEL3 3 // 
#define VIRTUAL_CHANNEL4 4 //
#define VIRTUAL_CHANNEL5 5 
#define VIRTUAL_CHANNEL6 6
#define VIRTUAL_CHANNEL7 7
#define VIRTUAL_CHANNEL8 8
#define VIRTUAL_CHANNEL9 9 //botão desliga alarme
// comunicação serial
char string_r[100];
char string_a0[100];
char string_a1[100];
char string_a2[100];
char string_a3[100];
char string_a4[100];
int i=0;
int j=0;
bool stringComplete = false; 
String testestring="";



//Cayenne input
int ACTUATOR_PIN=0;
int alarme=1;
int teste=0;

// WiFi network info.
char ssid[] = "Micaelle";
char wifiPassword[] = "123456789";

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "f35e9900-420a-11e9-a98b-69b394a1794b";
char password[] = "f733e1b811e5ec665ca39cbda4e326ba6f67eb8a";
char clientID[] = "da9af980-516f-11e9-ba40-5d168a516101";


void receber() {
  
  if(Serial.available()>0) {
    
    char inChar = (char)Serial.read();
    if(inChar !='\n'){
    string_r[i]= inChar;
    testestring=testestring+inChar;
    //Serial.print(inChar);
    i++;}
    else{
     i=0; 
    switch(j){
      case 0:
            memcpy(string_a0,string_r,sizeof(string_r));
            break;
      case 1:
            memcpy(string_a1,string_r,sizeof(string_r));
            break;
      case 2:
            memcpy(string_a2,string_r,sizeof(string_r));
            break;
      case 3:
            memcpy(string_a3,string_r,sizeof(string_r));
            break;
      case 4:
            memcpy(string_a4,string_r,sizeof(string_r));
            break;
      }
     memset(string_r,0,sizeof(string_r));
     j=j++;
     if(j>4){j=0;}  
    stringComplete = true;
    } 
  }
}


void enviar(char s[] ){
  int tam;
  int i1;
  tam = strlen(s);
  for(i1=0; i1<tam;i1++){
  Serial.write(s[i1]);
  }
  Serial.write('\n');
  }

void apagarbuff(char var[3]){
  
  if(strcmp(string_a0,var)==0){
    memset(string_a0,0,sizeof(string_a0));
    }
  if(strcmp(string_a1,var)==0){
    memset(string_a1,0,sizeof(string_a0));
    }
  if(strcmp(string_a2,var)==0){
    memset(string_a2,0,sizeof(string_a0));
    }
  if(strcmp(string_a3,var)==0){
    memset(string_a3,0,sizeof(string_a0));
    }
  if(strcmp(string_a4,var)==0){
    memset(string_a4,0,sizeof(string_a0));
    }
  
  }

void setup() {
	Serial.begin(19200);
  Serial.swap(); 
	Cayenne.begin(username, password, clientID, ssid, wifiPassword);
}

void loop() {
  receber();
	Cayenne.loop();

}


void checkAcessos()
{
    
    if(stringComplete == true){

      //Acesso 1
    if (strcmp(string_a0,"rx1")==0 ||strcmp(string_a1,"rx1")==0||strcmp(string_a2,"rx1")==0||strcmp(string_a3,"rx1")==0||strcmp(string_a4,"rx1")==0){
      teste=1;
      Cayenne.virtualWrite(VIRTUAL_CHANNEL1, teste, "digital_sensor", "d");
      alarme=1;
      apagarbuff("rx1");
      stringComplete=false;
      }
     //Acesso 2
    if (strcmp(string_a0,"rx2")==0||strcmp(string_a1,"rx2")==0||strcmp(string_a2,"rx2")==0||strcmp(string_a3,"rx2")==0||strcmp(string_a4,"rx2")==0){
      teste=1;
      Cayenne.virtualWrite(VIRTUAL_CHANNEL2, teste, "digital_sensor", "d");
      alarme=1;
      apagarbuff("rx2");
      stringComplete=false;
      }
      //Acesso 3
  if (strcmp(string_a0,"rx3")==0||strcmp(string_a1,"rx3")==0||strcmp(string_a2,"rx3")==0||strcmp(string_a3,"rx3")==0||strcmp(string_a4,"rx3")==0){
      teste=1;
      Cayenne.virtualWrite(VIRTUAL_CHANNEL3, teste, "digital_sensor", "d");
      alarme=1;
      apagarbuff("rx3");
      stringComplete=false;
      }
      //Acesso 4
   if (strcmp(string_a0,"rx4")==0||strcmp(string_a1,"rx4")==0||strcmp(string_a2,"rx4")==0||strcmp(string_a3,"rx4")==0||strcmp(string_a4,"rx4")==0){
      teste=1;
      Cayenne.virtualWrite(VIRTUAL_CHANNEL4, teste, "digital_sensor", "d");
      alarme=1;
      apagarbuff("rx4");
      stringComplete=false;
      }

     }
     //desliga alarme
     
}

CAYENNE_OUT_DEFAULT()
{
  checkAcessos();

  Cayenne.virtualWrite(0, millis());

}

CAYENNE_IN(VIRTUAL_CHANNEL5)
{
  int value = getValue.asInt();
  //CAYENNE_LOG("Channel %d, pin %d, value %d", VIRTUAL_CHANNEL5, ACTUATOR_PIN, value);
  // Write the value received to the digital pin.
  if(value==1){
  enviar("tx1l");
  //Serial.println("tx1l");
  }
  if(value==0){
  enviar("tx1d");
  //Serial.println("tx1d");
  }
}
CAYENNE_IN(VIRTUAL_CHANNEL6)
{
  int value = getValue.asInt();
  //CAYENNE_LOG("Channel %d, pin %d, value %d", VIRTUAL_CHANNEL6, ACTUATOR_PIN, value);
  // Write the value received to the digital pin.
  if(value==1){
  enviar("tx2l");
  //Serial.println("tx2l");
  }
  if(value==0){
  enviar("tx2d");
  //Serial.println("tx2d");
  }
}
CAYENNE_IN(VIRTUAL_CHANNEL7)
{
  int value = getValue.asInt();
  //CAYENNE_LOG("Channel %d, pin %d, value %d", VIRTUAL_CHANNEL7, ACTUATOR_PIN, value);
  // Write the value received to the digital pin.
  if(value==1){
  enviar("tx3l");
  //Serial.println("tx3l");
  }
  if(value==0){
  enviar("tx3d");
  //Serial.println("tx3d");
  }
}

CAYENNE_IN(VIRTUAL_CHANNEL8)
{
  int value = getValue.asInt();
  //CAYENNE_LOG("Channel %d, pin %d, value %d", VIRTUAL_CHANNEL8, ACTUATOR_PIN, value);
  // Write the value received to the digital pin.
  if(value==1){
  enviar("tx4l");
  //Serial.println("tx4l");
  }
  if(value==0){
  enviar("tx4d");
  //Serial.println("tx4d");
  }
}

CAYENNE_IN(VIRTUAL_CHANNEL9)
{
  int value = getValue.asInt();
  //CAYENNE_LOG("Channel %d, pin %d, value %d", VIRTUAL_CHANNEL9, ACTUATOR_PIN, value);
  // Write the value received to the digital pin.
  enviar("off");
  //Serial.println("off");
  alarme=0;

  if (alarme==0){
      teste=0;
      alarme=1;
      Cayenne.virtualWrite(VIRTUAL_CHANNEL1, teste, "digital_sensor", "d");
      Cayenne.virtualWrite(VIRTUAL_CHANNEL2, teste, "digital_sensor", "d");
      Cayenne.virtualWrite(VIRTUAL_CHANNEL3, teste, "digital_sensor", "d");
      Cayenne.virtualWrite(VIRTUAL_CHANNEL4, teste, "digital_sensor", "d");
      }
}

//CAYENNE_IN_DEFAULT()
//{
//  CAYENNE_LOG("Channel %u, value %s", request.channel, getValue.asString());
//}
