#include <SoftwareSerial.h>
#include <TinyGPS.h>


TinyGPS gps_parser;

static void smartdelay(unsigned long ms);
static void print_float(float val, float invalid, int len, int prec);

int led = 13;

char RcvdMsg[200] = " ";
int  RcvdCheck = 0;
int  RcvdConf = 0;
int  index = 0;
int  RcvdEnd = 0;
char MsgMob[15];
char MsgTxt[50]=" ";
int  MsgLength = 0;
int counter = 0;

long l,m;
char clat[8];
char clon[8];

float flat, flon;
unsigned long age;

SoftwareSerial gsm(9,10);
SoftwareSerial gps(2,3);


void setup() {
   counter=0;
   pinMode(led,OUTPUT);
}

void loop() {
  if(counter==0){
    digitalWrite(led, LOW);
    gsm.begin(9600);   
    Serial.begin(9600);    
    delay(1000);
  
    gsm.println("AT+CMGF=1");  
    delay(2000);
    ShowSerialData();
   
    gsm.println("AT+CNMI=1,2,0,0,0");  
    delay(2000);
    ShowSerialData();
    counter++;
  }
  
  do{
    //counter=2;
    while(gsm.available()){
      if(RecSMS()){
        gsm.end();
        digitalWrite(led, HIGH);
        delay(1000);
        digitalWrite(led, LOW);
        counter++;
        Serial.println("message recieved!");
      }
    }
  }while(counter==1);
  
  if(counter==2){
    Serial.println("GPS listening!");
    gps.begin(9600);
    digitalWrite(led, HIGH);
    delay(1000);
    digitalWrite(led, LOW);
    counter++; 
  } 
  
  if(counter==3){
    gps_parser.f_get_position(&flat, &flon, &age);
    print_float(flat, TinyGPS::GPS_INVALID_F_ANGLE, 10, 6);
    print_float(flon, TinyGPS::GPS_INVALID_F_ANGLE, 11, 6);
    if (counter==4){
      Serial.print("flat: "); Serial.print(flat);
      Serial.print("flon: "); Serial.println(flon);
      l = flat*1000000;
      m = flon*1000000;
    }
    Serial.println();
    smartdelay(1000);
  }
  
  if(counter==4){
    gps.end();
    delay(1000);
    int index =0;
    
    Serial.print("l: ");Serial.print(l);
    Serial.print("m: ");Serial.println(m);
    
    while(l>0){
      switch(l%10){
        case 0:
         clat[7-index] = '0'; index++;
        break;
        case 1:
         clat[7-index] = '1'; index++;
        break;
        case 2:
         clat[7-index] = '2'; index++;
        break;
        case 3:
         clat[7-index] = '3'; index++;
        break;
        case 4:
         clat[7-index] = '4'; index++;
        break;
        case 5:
         clat[7-index] = '5'; index++;
        break;
        case 6:
         clat[7-index] = '6'; index++;
        break;
        case 7:
         clat[7-index] = '7'; index++;
        break;
        case 8:
         clat[7-index] = '8'; index++;
        break;
        case 9:
         clat[7-index] = '9'; index++;
        break;
        default:
        break;
      }
      
      l = l/10;
    }
    delay(100);
    int ind=0;
    
    while(m>0){
      switch(m%10){
        case 0:
         clon[7-ind] = '0'; ind++;
        break;
        case 1:
         clon[7-ind] = '1'; ind++;
        break;
        case 2:
         clon[7-ind] = '2'; ind++;
        break;
        case 3:
         clon[7-ind] = '3'; ind++;
        break;
        case 4:
         clon[7-ind] = '4'; ind++;
        break;
        case 5:
         clon[7-ind] = '5'; ind++;
        break;
        case 6:
         clon[7-ind] = '6'; ind++;
        break;
        case 7:
         clon[7-ind] = '7'; ind++;
        break;
        case 8:
         clon[7-ind] = '8'; ind++;
        break;
        case 9:
         clon[7-ind] = '9'; ind++;
        break;
        default:
        break;
      }
      
      m= m/10;
    }
    delay(1000);
    
 //   Serial.print(" clat: "); Serial.println(clat);
 //   Serial.print(" clon: "); Serial.println(clon);
    
    Serial.println("sending message!!");
    SendMessage();
    digitalWrite(led, LOW);
    delay(100);
    counter=0;
  }
}
   
void ShowSerialData(){
  while(gsm.available()!=0) Serial.write(gsm.read()); 
}

boolean RecSMS(){
   boolean flag=false;
     char data = gsm.read();
     
     if(data == '+'){RcvdCheck = 1;}
     if((data == 'C') && (RcvdCheck == 1)){RcvdCheck = 2;}
     if((data == 'M') && (RcvdCheck == 2)){RcvdCheck = 3;}
     if((data == 'T') && (RcvdCheck == 3)){RcvdCheck = 4;}
     if(RcvdCheck == 4){ index = 0; RcvdConf = 1; RcvdCheck = 0;flag=true;}
     
     if(RcvdConf == 1){
        if(data == '\n'){RcvdEnd++;}
        if(RcvdEnd == 3){ RcvdEnd = 0;}
        
        RcvdMsg[index] = data;
        index++;
        
       if(RcvdEnd == 2){RcvdConf = 0;MsgLength = index-2;}
       
       if(RcvdConf == 0){
         //Serial.print("Mobile Number is: ");
           for(int x = 4;x <17;x++){
             MsgMob[x-4] = RcvdMsg[x];
             //Serial.print(MsgMob[x-4]);
           }
         Serial.println();
         //Serial.print("Message Text: ");
         for(int x = 54;x <MsgLength;x++){
           MsgTxt[x-54] = RcvdMsg[x];
           //Serial.print(MsgTxt[x-54]);
         }
         Serial.println();
       }
     }
  return flag;
}

void SendMessage(){
    String msg = "Your car is at \n";
    for(int j=0;j<8;j++){
      if(j==2) msg+='.';
      msg+=clat[j];
    }
    msg+=" N, ";
    for(int j=0;j<8;j++){
      if(j==2) msg+='.';
      msg+=clon[j];
    }
    msg+=" E ";
    
    msg += "\n https://www.google.co.in/maps/place/";
    for(int j=0;j<8;j++){
      if(j==2) msg+='.';
      msg+=clat[j];
    }
    msg+="++";
    for(int j=0;j<8;j++){
      if(j==2) msg+='.';
      msg+=clon[j];
    }
    
    gsm.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
    delay(1000);  // Delay of 1000 milli seconds or 1 second
  
    gsm.println("AT+CMGS=\"+919424657134\"\r"); // Replace x with mobile number
    delay(1000);
  
    gsm.print(msg);
    gsm.println();
    delay(100);
  
    gsm.println((char)26);// ASCII code of CTRL+Z
    delay(1000);
    Serial.println("sent message!");
}

static void smartdelay(unsigned long ms){
    unsigned long start = millis();
    do 
    {
     while (gps.available())
       gps_parser.encode(gps.read());
    } while (millis() - start < ms);
 }
 

static void print_float(float val, float invalid, int len, int prec){
  if (val == invalid)
  {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
  }
  else
  {
    counter=4;
    
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i)
      Serial.print(' ');
  }
  smartdelay(0);
}



