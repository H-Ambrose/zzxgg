int leftCounter=0,  rightCounter=0;
unsigned long time = 0, old_time = 0; // 时间标记
unsigned long time1 = 0; // 时间标记
float lv,rv;//左、右轮速度

#define STOP      0  //停止
#define FORWARD   1  //向前
#define BACKWARD  2  //向后
#define TURNLEFT  3  //向左
#define TURNRIGHT 4  //向右

int leftMotor1 = 4;
int leftMotor2 = 5;
int rightMotor1 = 6;
int rightMotor2 = 7;

void setup() {
  // put your setup code here, to run once:
/*interrupt：要初始化的外部中断编号，由上表可知我们Arduino UNO只能使用外部中断0和外部中断1； 
function：中断服务函数的名字，即当外部中断被触发时，将会自动调用这个函数； 
mode：中断触发的方式，可选方式如下
  LOW  低电平触发
  CHANGE  电平变化，高电平变低电平、低电平变高电平
  RISING  上升沿触发
  FALLING  下降沿触发
  HIGH  高电平触发(该中断模式仅适用于Arduino due)*/
  Serial.begin(9600); 
  attachInterrupt(0,RightCount_CallBack, FALLING);
  attachInterrupt(1,LeftCount_CallBack, FALLING);

  pinMode(leftMotor1, OUTPUT);
  pinMode(leftMotor2, OUTPUT);
  pinMode(rightMotor1, OUTPUT);
  pinMode(rightMotor2, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  SpeedDetection();
  if(Serial.available()>0)
  {
    char cmd = Serial.read();

    Serial.print(cmd);
    motorRun(cmd);

  }  
}
/*
 * *速度计算
 */
bool SpeedDetection()
{
  time = millis();//以毫秒为单位，计算当前时间 
  if(abs(time - old_time) >= 1000) // 如果计时时间已达1秒
  {  
    detachInterrupt(0); // 关闭外部中断0
    detachInterrupt(1); // 关闭外部中断1
    //把每一秒钟编码器码盘计得的脉冲数，换算为当前转速值
    //转速单位是每分钟多少转，即r/min。这个编码器码盘为20个空洞。
    lv =(float)leftCounter*60/20;//小车车轮电机转速
    rv =(float)rightCounter*60/20;//小车车轮电机转速
    Serial.print("left:");
    Serial.print(lv);//向上位计算机上传左车轮电机当前转速的高、低字节
    Serial.print("     right:");
    Serial.println(rv);//向上位计算机上传左车轮电机当前转速的高、低字节
    //恢复到编码器测速的初始状态
    leftCounter = 0;   //把脉冲计数值清零，以便计算下一秒的脉冲计数
    rightCounter = 0;
    old_time=  millis();     // 记录每秒测速时的时间节点   
    attachInterrupt(0, RightCount_CallBack,FALLING); // 重新开放外部中断0
    attachInterrupt(1, LeftCount_CallBack,FALLING); // 重新开放外部中断0
    return 1;
  }
  else
    return 0;
}
/*
 * *右轮编码器中断服务函数
 */
void RightCount_CallBack()
{
  rightCounter++;
}
/*
 * *左轮编码器中断服务函数
 */
void LeftCount_CallBack()
{
  leftCounter++;
}
/*
 * *小车运动控制函数
 */
void motorRun(int cmd)
{
  switch(cmd){
    case FORWARD:
      Serial.println("FORWARD"); //输出状态
      digitalWrite(leftMotor1, HIGH);
      digitalWrite(leftMotor2, LOW);
      digitalWrite(rightMotor1, HIGH);
      digitalWrite(rightMotor2, LOW);
      break;
     case BACKWARD:
      Serial.println("BACKWARD"); //输出状态
      digitalWrite(leftMotor1, LOW);
      digitalWrite(leftMotor2, HIGH);
      digitalWrite(rightMotor1, LOW);
      digitalWrite(rightMotor2, HIGH);
      break;
     case TURNLEFT:
      Serial.println("TURN  LEFT"); //输出状态
      digitalWrite(leftMotor1, HIGH);
      digitalWrite(leftMotor2, LOW);
      digitalWrite(rightMotor1, LOW);
      digitalWrite(rightMotor2, HIGH);
      break;
     case TURNRIGHT:
      Serial.println("TURN  RIGHT"); //输出状态
      digitalWrite(leftMotor1, LOW);
      digitalWrite(leftMotor2, HIGH);
      digitalWrite(rightMotor1, HIGH);
      digitalWrite(rightMotor2, LOW);
      break;
     default:
      Serial.println("STOP"); //输出状态
      digitalWrite(leftMotor1, LOW);
      digitalWrite(leftMotor2, LOW);
      digitalWrite(rightMotor1, LOW);
      digitalWrite(rightMotor2, LOW);
  }
}

/*Arduino:1.8.8 (Windows 10), 开发板："Arduino Nano, ATmega328P"

项目使用了 4544 字节，占用了 (14%) 程序存储空间。最大为 30720 字节。
全局变量使用了282字节，(13%)的动态内存，余留1766字节局部变量。最大为2048字节。
avrdude: ser_open(): can't open device "\\.\COM3": 系统找不到指定的文件。

上传出错。查看http://www.arduino.cc/en/Guide/Troubleshooting#upload 获取建议。*/
