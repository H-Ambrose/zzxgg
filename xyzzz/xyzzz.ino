#include<Wire.h>

#define address 0x0d
void setup() {
  Serial.begin(9600);
  delay(500);
  Wire.begin();
  Wire.beginTransmission(address);
  Wire.write(0x09);
  Wire.write(0x01);
  Wire.endTransmission();// put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  int x,y,z;
  Wire.beginTransmission(address);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.requestFrom(address,6);
  if(Wire.available()>=6){
    x=Wire.read() <<8;
    x|=Wire.read();
    y=Wire.read() <<8;
    y|=Wire.read();
    z=Wire.read() <<8;
    z|=Wire.read();
  }
  /*
  Serial.print("   x:  ");
  Serial.print(x);
  Serial.print("   y:  ");
  Serial.print(y);
  Serial.print("   z:  ");
  Serial.print(z);
  Serial.println();
  delay(250);
   */
   //######################
    float xHeading = atan2(x, y);
    while(xHeading<0)
    xHeading += 2*PI;
    while(xHeading>2*PI)
    xHeading -= 2*PI;
    float xDegrees = xHeading * 180/PI;//M_PI即为pi，这里计算的是角度值
    //float xDegrees = xHeading;//传输用弧度制
    Serial.print("angle：");
    Serial.print(xDegrees);
    Serial.println();
    delay(1000);
   //######################
  }
