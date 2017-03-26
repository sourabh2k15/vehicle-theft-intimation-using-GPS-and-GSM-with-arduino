#include <SoftwareSerial.h>
#include <TinyGPS.h>

TinyGPS gps;
SoftwareSerial ss(2, 3);

static void smartdelay(unsigned long ms);
static void print_float(float val, float invalid, int len, int prec);

int start;
int counter =0;

void setup(){
  Serial.begin(9600);
  
  start = millis();
  Serial.println("code started executing at ");
  Serial.print(start);
  
  Serial.println();
  Serial.println(" Latitude  Longitude ");
  Serial.println(" (deg)      (deg)    ");
  Serial.println("---------------------");
  ss.begin(9600);
}

void loop(){
  if(counter==0){
    float flat, flon;
    unsigned long age;
  
    gps.f_get_position(&flat, &flon, &age);
    print_float(flat, TinyGPS::GPS_INVALID_F_ANGLE, 10, 6);
    print_float(flon, TinyGPS::GPS_INVALID_F_ANGLE, 11, 6);
    Serial.println();
  
    smartdelay(1000);
  }
  else if(counter==1){ 
    Serial.println("got co-ordinates yaay!");
    Serial.print(millis()-start);
    counter++;
  }
}

static void smartdelay(unsigned long ms){
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
  }

static void print_float(float val, float invalid, int len, int prec)
{
  if (val == invalid)
  {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
  }
  else
  {
    counter=1;
    Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i)
      Serial.print(' ');
  }
  smartdelay(0);
}
