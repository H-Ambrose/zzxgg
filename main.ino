//============================亚博科技========================================
//  智能小车超声波避障实验(有舵机)
//  程序中电脑打印数值部分都被屏蔽了，打印会影响小车遇到障碍物的反应速度
//  调试时可以打开屏蔽内容Serial.print，打印测到的距离
//  本实验控制速度的pwm值和延时均有调节，但还是配合实际情况，实际电量调节数值
//=============================================================================
#include<Servo.h> 
Servo m;
Servo w;
int Echo =A0;  // Echo回声脚(P2.0)
int Trig =A1;  //  Trig 触发脚(P2.1)
int ledPin = 13;            
int inputPin = 8;  
int Left_motor_back=3;     //左电机后退(IN1)
int Left_motor_go=5;     //左电机前进(IN2)
int Right_motor_go=6;    // 右电机前进(IN3)
int Right_motor_back=11;    // 右电机后退(IN4)
int BT_COM;
int guaji;
void setup()
{
  Serial.begin(9600);     // 初始化串口
  //初始化电机驱动IO为输出方式
  pinMode(Left_motor_go,OUTPUT); // PIN 8 (PWM)
  pinMode(Left_motor_back,OUTPUT); // PIN 9 (PWM)
  pinMode(Right_motor_go,OUTPUT);// PIN 10 (PWM) 
  pinMode(Right_motor_back,OUTPUT);// PIN 11 (PWM)
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
   pinMode(ledPin, OUTPUT);   
  pinMode(inputPin, INPUT);  
  m.attach(4);
  w.attach(9);
   m.write(90);
   guaji=0;
  for(int i=1;i<=30;i++){
    w.write(88+i);
    delay(50);
  }
}
//=======================智能小车的基本动作=========================
float Distance_test()   // 量出前方距离 
{
  digitalWrite(Trig, LOW);   // 给触发脚低电平2μs
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);  // 给触发脚高电平10μs，这里至少是10μs
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);    // 持续给触发脚低电
  float Fdistance = pulseIn(Echo, HIGH);  // 读取高电平时间(单位：微秒)
  Fdistance= Fdistance/58.00;       //为什么除以58等于厘米，  Y米=（X秒*344）/2 // X秒=（ 2*Y米）/344 ==》X秒=0.0058*Y米 ==》厘米=微秒/58
  Serial.print("Distance:");      //输出距离（单位：厘米）
  Serial.println(Fdistance);         //显示距离
  Serial.println();
  delay(1000);
   //Distance = Fdistance;
  return Fdistance;
}  
void zhua()
{
  m.write(90);
  for(int i=1;i<=30;i++){
    w.write(118-i);
    delay(50);
  }
 delay(5000);
  for(int i=1;i<60;i++)
  {
   m.write(90+i); 
   digitalWrite(ledPin, LOW);  
   int val = digitalRead(inputPin); 
  if (val == HIGH) {    
    m.write(90+i-4);
    guaji=90+i-4;
    digitalWrite(ledPin, HIGH);
    break;  
  } 
  delay(50);
  }
  delay(1000);
  for(int i=1;i<=30;i++){
    w.write(88+i);
    delay(50);
  }
  delay(1000);
}
void fang()
{
   for(int i=1;i<=30;i++){
    w.write(118-i);
    delay(50);
   }
   delay(1000);
   for(int i=guaji;i>=90;i--){
        m.write(i);
        delay(50);
      }
      delay(1000);
  for(int i=1;i<=30;i++){
    w.write(88+i);
    delay(50);
  }
  delay(1000);
 
}
void run()     // 前进
{
  digitalWrite(Right_motor_go,HIGH);  // 右电机前进
  digitalWrite(Right_motor_back,LOW);
  analogWrite(Right_motor_go,125);//PWM比例0~255调速，左右轮差异略增减
  analogWrite(Right_motor_back,0);
  digitalWrite(Left_motor_go,HIGH);  // 左电机前进
  digitalWrite(Left_motor_back,LOW);
  analogWrite(Left_motor_go,115);//PWM比例0~255调速，左右轮差异略增减
  analogWrite(Left_motor_back,0);
}
void runn(int t)     // 前进
{
  digitalWrite(Right_motor_go,HIGH);  // 右电机前进
  digitalWrite(Right_motor_back,LOW);
  analogWrite(Right_motor_go,125);//PWM比例0~255调速，左右轮差异略增减
  analogWrite(Right_motor_back,0);
  digitalWrite(Left_motor_go,HIGH);  // 左电机前进
  digitalWrite(Left_motor_back,LOW);
  analogWrite(Left_motor_go,115);//PWM比例0~255调速，左右轮差异略增减
  analogWrite(Left_motor_back,0);
  delay(t);
  digitalWrite(Right_motor_go,LOW);
  digitalWrite(Right_motor_back,LOW);
  digitalWrite(Left_motor_go,LOW);
  digitalWrite(Left_motor_back,LOW);
}
void get(){
    float d=Distance_test();
    while(d>4.1){
        runn(100);
        d=Distance_test();
    }
    zhua();
}
void brake()  //刹车，停车
{
  digitalWrite(Right_motor_go,LOW);
  digitalWrite(Right_motor_back,LOW);
  digitalWrite(Left_motor_go,LOW);
  digitalWrite(Left_motor_back,LOW);
}
void left()         //左转(左轮后退，右轮前进)
{
  digitalWrite(Right_motor_go,HIGH);  // 右电机前进
  digitalWrite(Right_motor_back,LOW);
  analogWrite(Right_motor_go,125); 
  analogWrite(Right_motor_back,0);//PWM比例0~255调速
  digitalWrite(Left_motor_go,LOW);   //左轮后退
 // digitalWrite(Left_motor_back,HIGH);
  digitalWrite(Left_motor_back,LOW);
  analogWrite(Left_motor_go,0); 
  analogWrite(Left_motor_back,0);
  //analogWrite(Left_motor_back,115);//PWM比例0~255调速
}
void right()        //右转(右轮后退，左轮前进)
{
  digitalWrite(Right_motor_go,LOW);   //右电机后退
 // digitalWrite(Right_motor_back,HIGH);
  digitalWrite(Right_motor_back,LOW);
  analogWrite(Right_motor_go,0); 
  analogWrite(Right_motor_back,0);
  //analogWrite(Right_motor_back,125);//PWM比例0~255调速
  digitalWrite(Left_motor_go,HIGH);//左电机前进
  digitalWrite(Left_motor_back,LOW);
  analogWrite(Left_motor_go,115); 
  analogWrite(Left_motor_back,0);//PWM比例0~255调速
}
void back()          //后退
{
  digitalWrite(Right_motor_go,LOW);  //右轮后退
  digitalWrite(Right_motor_back,HIGH);
  analogWrite(Right_motor_go,0);
  analogWrite(Right_motor_back,125);//PWM比例0~255调速
  digitalWrite(Left_motor_go,LOW);  //左轮后退
  digitalWrite(Left_motor_back,HIGH);
  analogWrite(Left_motor_go,0);
  analogWrite(Left_motor_back,115);//PWM比例0~255调速
}
//==========================================================
//==========================================================
void loop()
{
   if(Serial.available())
  {
    BT_COM=Serial.read();
    switch(BT_COM)
    {
     case '1':
      zhua();
      break;
      
    case '2':
    run();
    break;
    
    case '3':
      fang();
      break;
      
    case '4': 
    left();
    break;
    
    case '5':  
    brake();
    break;

        case '6':  
    right();
    break;
    
  case '7':
  Distance_test();
  break;

        case '8':  
   back();
    break;
    
      case'9':
      get();
      break;
    }
  }
}
