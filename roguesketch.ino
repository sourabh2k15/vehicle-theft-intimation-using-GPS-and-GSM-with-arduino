#include<SoftwareSerial.h>

SoftwareSerial gps(2,3);

int counter =0;
char gps_msg[200] = "";
int index=0;
char latitude[11] = "";
char longitude[11]= "";


void setup(){
  Serial.begin(9600);
  gps.begin(9600);
  delay(1000);
}

void loop(){
 if(counter==1){
   if(gps.available()){
    char c = byte(gps.read());
    if(index<200){ gps_msg[index] = c;index++;}
    else{ 
      //Serial.println(gsm_msg);
      index=0;
      delay(100);
      
      for(int j=0;j<200;j++){
        if(gps_msg[j]=='$') Serial.println();
        Serial.write(gps_msg[j]);
      }
      Serial.println();
      Serial.println();
      Serial.println();
      Serial.println();
      Serial.println();
    }
   }
 }
 
 if(counter==0){
   if(Serial.available()){
    Serial.write("Start listening on GPS!!");
    counter++;
    delay(500);
   }
 }
}

