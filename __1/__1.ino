//罗盘部分*************************
#include <Wire.h>
#define address 0x0d 
void setup()
{
  Serial.begin(9600);
  delay(500);
  Wire.begin();
  Wire.beginTransmission(address);
  Wire.write(0x09);
  Wire.write(0x01);
  Wire.endTransmission();
}
void loop()
{
//罗盘部分*************************
   int x,y,z; 
   Wire.beginTransmission(address); 
   Wire.write(0x00); 
   Wire.endTransmission();  
   Wire.requestFrom(address,6);
   if(Wire.available() >= 6){  
    x= Wire.read() << 8;   
    x |= Wire.read();  
    y = Wire.read() << 8; 
    y |= Wire.read();  
    z= Wire.read() << 8;
    z |= Wire.read();   
    }   
    float xHeading = atan2(x, y);
    if(xHeading < 0)    xHeading += 2*PI;
    if(xHeading > 2*PI)    xHeading -= 2*PI;
    //float xDegrees = xHeading * 180/M_PI;//M_PI即为pi，这里计算的是角度值
    float xDegrees = xHeading;//传输用弧度制
    Serial.print("angle：");
    Serial.print(xDegrees);
}
