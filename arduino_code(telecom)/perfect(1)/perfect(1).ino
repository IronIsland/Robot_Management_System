#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <Servo.h>
#include <Adafruit_MPU6050.h>

Adafruit_MPU6050 mpu;

const int MPU_addr = 0x68; // MPU6050 I2C address
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ; // Sensor data variables

float gyroAngleZ = 0.0; // Z轴的旋转角度
unsigned long prevTime = 0; // 上一次的时间戳

int pos1 = 0;//下舵机
int pos2 = 130;//上舵机
Servo myservo1;
Servo myservo2;

#define STOP      0
#define FORWARD   1
#define BACKWARD  2
#define LEFT  3
#define RIGHT 4
const int d_wall = 16 ;
const int lr_wall = 12;
const int VAL = 90;
const int VAL1 = 150;

int leftMotor1 = 4;
int leftMotor2 = 5;
int rightMotor1 = 6;
int rightMotor2 = 7;
float Zexia=0;
int n=0;


int inputPin1=8;   // 定义左侧超声波信号接收接口
int outputPin1=9;  // 定义左侧超声波信号发出接口

int inputPin2=10;   // 定义中间超声波信号接收接口
int outputPin2=11;  // 定义中间超声波信号发出接口

int inputPin3=12;   // 定义右侧超声波信号接收接口
int outputPin3=13;  // 定义右侧超声波信号发出接口

int dis[3];//记录三个方向超声波探测到的障碍物距离 行走优先级左、中、右

    

void setup() {
  // put your setup code here, to run once:
  //串口初始化
  Serial.begin(9600);
  //测速引脚初始化
  delay(2000);
  pinMode(leftMotor1, OUTPUT);
  pinMode(leftMotor2, OUTPUT);
  pinMode(rightMotor1, OUTPUT);
  pinMode(rightMotor2, OUTPUT);


  int pos1;// = 90;//下舵机
int pos2; //= 170;//上舵机
  myservo1.attach(3, 500, 2500);          //修正脉冲宽度
  myservo2.attach(2, 500, 2500);
myservo1.write(pos1);
myservo2.write(pos2);

  //超声波控制引脚初始化
  pinMode(inputPin1, INPUT);
  pinMode(outputPin1, OUTPUT);
  pinMode(inputPin2, INPUT);
  pinMode(outputPin2, OUTPUT);
  pinMode(inputPin3, INPUT);
  pinMode(outputPin3, OUTPUT);
  Wire.begin();
  Serial.begin(9600);
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
}


void loop() {
    static int loopCounter = 0; // 计数器用于跟踪循环次数
    myservo1.write(70);
myservo2.write(100);
    int dis1, dis2, dis3;
    float AcZ;
//    float Z=getZ();
//    Z=getmpu();
    
    dis1 = getLeftDistance();
    dis2 = getMidDistance();
    dis3 = getRightDistance();
/*
    Serial.println(n);
    Serial.print(dis1);
    Serial.print(",");
    Serial.print(dis2);
    Serial.print(",");
    Serial.println(dis3);

    dis1 = getLeftDistance();
    dis2 = getMidDistance();
    dis3 = getRightDistance();*/
    
    if (loopCounter % 3 == 0) {
        // 输出dis值，每隔3次循环执行
       //Serial.print("dis1: ");
        Serial.print(dis1);
        Serial.print(",");
        Serial.print(dis2);
        Serial.print(",");
        Serial.print(dis3);
        Serial.println(",");
        /*Serial.print(AcZ);
         Serial.print(",");
          Serial.print(Z);
        Serial.println(" ");*/
        
        
    }

    if (dis1 < 2 || dis3 < 2) {
        motorRun(STOP);
        delay(300);
        motorRun(BACKWARD);
        delay(300);
        motorRun(STOP);
        dis1 = getLeftDistance();
        dis2 = getMidDistance();
        dis3 = getRightDistance();
        if (dis1 < dis3) {
            motorRun(RIGHT);
            delay(300);
        } else {
            motorRun(LEFT);
            delay(300);
        }
    }

    if (dis2 < 14) {
        motorRun(STOP);
        delay(200);
        //printf("stop");
        if (dis1 > 30) {
            motorRun(LEFT);
            //Serial.print("turn left");
            delay(600);
            motorRun(STOP);
        } else if (dis3 > 30) {
            motorRun(RIGHT);
            //Serial.print("turn right");
            delay(600);
            motorRun(STOP);
        } else if (dis1 < dis3) {
            motorRun(BACKWARD);
            delay(350);
            motorRun(STOP);
            duoji();
            motorRun(RIGHT);
            //Serial.print("turn around");
            delay(1150);
            motorRun(STOP);
        } else {
          motorRun(BACKWARD);
            delay(350);
            motorRun(STOP);
            duoji();
            motorRun(LEFT);
            delay(1150);
            motorRun(STOP);
        }
    } else {
        if (dis1 < 3) {
            motorRun(BACKWARD);
            delay(300);
            motorRun(RIGHT);
            delay(100);
        }
        if (dis3 < 3) {
            motorRun(BACKWARD);
            delay(300);
            motorRun(LEFT);
            delay(100);
        }
        motorRun(FORWARD);
        delay(100);
    }

    loopCounter++; // 增加循环计数器
}


/*int getn(int n){
  n++;
    if(n>=8000){
    n=0;
  }
}*/
/*
float getZ(){
   float GyZ = getmpu();
   int gyroAngle;

   if(GyZ>500||GyZ<-500){
    GyZ = getmpu();
   }
else
{
  GyZ=0;
}
  // 获取当前时间
  unsigned long currentTime = millis();
  
  // 计算时间间隔
  float elapsedTime = (currentTime - prevTime) / 1000.0; // 时间间隔，单位：秒
  
  // 使用陀螺仪数据来更新Z轴的旋转角度
  gyroAngleZ += (GyZ / 131.0) * elapsedTime; // 131.0是陀螺仪的灵敏度因子，根据传感器规格表来调整

  // 打印Z轴的旋转角度
  Serial.println(GyZ);
  Serial.print("Z轴旋转角度: ");
  Serial.println(gyroAngleZ);

  // 更新时间戳
  prevTime = currentTime;

  return gyroAngleZ;
}*/
/*
float getmpu() {
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // 从寄存器0x3B（ACCEL_XOUT_H）开始读取数据
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14); // 请求总共14个寄存器的数
  
  // 读取传感器数据
  int16_t t = Wire.read();
  AcX = (t << 8) | Wire.read(); // ACCEL_XOUT_H & ACCEL_XOUT_L
  t = Wire.read();
  AcY = (t << 8) | Wire.read(); // ACCEL_YOUT_H & ACCEL_YOUT_L
  t = Wire.read();
  AcZ = (t << 8) | Wire.read(); // ACCEL_ZOUT_H & ACCEL_ZOUT_L
  t = Wire.read();
  Tmp = (t << 8) | Wire.read(); // TEMP_OUT_H & TEMP_OUT_L
  t = Wire.read();
  GyX = (t << 8) | Wire.read(); // GYRO_XOUT_H & GYRO_XOUT_L
  t = Wire.read();
  GyY = (t << 8) | Wire.read(); // GYRO_YOUT_H & GYRO_YOUT_L
  t = Wire.read();
  GyZ = (t << 8) | Wire.read(); // GYRO_ZOUT_H & GYRO_ZOUT_L
  t = Wire.read();
  //return t; 
  Serial.println(GyZ);
  return GyZ;
  //delay(500);
}*/

void motorRun(int cmd)
{
analogWrite(leftMotor1,5);//从0到255
    analogWrite(leftMotor2,5);//从0到255
    
  switch(cmd){
    case FORWARD:
      analogWrite(leftMotor1, 0);
      analogWrite(leftMotor2, VAL);
      analogWrite(rightMotor1, VAL);
      analogWrite(rightMotor2, 0);
      break;
    case BACKWARD:
      analogWrite(leftMotor1, VAL);
      analogWrite(leftMotor2, 0);
      analogWrite(rightMotor1, 0);
      analogWrite(rightMotor2, VAL);
      break;
    case LEFT:
      analogWrite(leftMotor1, 0);
      analogWrite(leftMotor2, VAL1);
      analogWrite(rightMotor1, 0);
      analogWrite(rightMotor2, VAL1);
      break;
    case RIGHT:
      analogWrite(leftMotor1, VAL1);
      analogWrite(leftMotor2, 0);
      analogWrite(rightMotor1, VAL1);
      analogWrite(rightMotor2, 0);
      break;

    default:
      analogWrite(leftMotor1, 0);
      analogWrite(leftMotor2, 0);
      analogWrite(rightMotor1, 0);
      analogWrite(rightMotor2, 0);
  }
}







//超声波测距
int getLeftDistance()
{
  digitalWrite(outputPin1, LOW); // 使发出发出超声波信号接口低电平2μs
  delayMicroseconds(2);
  digitalWrite(outputPin1, HIGH); // 使发出发出超声波信号接口高电平10μs，这里是至少10μs
  delayMicroseconds(10);
  digitalWrite(outputPin1, LOW); // 保持发出超声波信号接口低电平
  int distance = pulseIn(inputPin1, HIGH); // 读出脉冲时间
  distance= distance/58; // 将脉冲时间转化为距离（单位：厘米）
 
if (distance <=50 && distance>=0)
  {
    //如果距离小于50厘米返回左侧超声波测距数据
    return distance;
  }
  else
    return 50;
}

int getMidDistance()
{
  digitalWrite(outputPin2, LOW); // 使发出发出超声波信号接口低电平2μs
  delayMicroseconds(2);
  digitalWrite(outputPin2, HIGH); // 使发出发出超声波信号接口高电平10μs，这里是至少10μs
  delayMicroseconds(10);
  digitalWrite(outputPin2, LOW); // 保持发出超声波信号接口低电平
  int distance = pulseIn(inputPin2, HIGH); // 读出脉冲时间
  distance= distance/58; // 将脉冲时间转化为距离（单位：厘米）
 
if (distance <=50 && distance>=0)
  {
    //如果距离小于50厘米返回左侧超声波测距数据
    return distance;
  }
  else
    return 50;
}

int getRightDistance()
{
  digitalWrite(outputPin3, LOW); // 使发出发出超声波信号接口低电平2μs
  delayMicroseconds(2);
  digitalWrite(outputPin3, HIGH); // 使发出发出超声波信号接口高电平10μs，这里是至少10μs
  delayMicroseconds(10);
  digitalWrite(outputPin3, LOW); // 保持发出超声波信号接口低电平
  int distance = pulseIn(inputPin3, HIGH); // 读出脉冲时间
  distance= distance/58; // 将脉冲时间转化为距离（单位：厘米）
 
  if (distance <=50 && distance>=0)
  {
    //如果距离小于50厘米返回左侧超声波测距数据
    return distance;
  }
  else
    return 50;
}




//舵机转动
void duoji(){
 myservo2.write(100);
  for (pos1 = 0; pos1 <= 180; pos1 += 1) {       //pos+=1等价于pos=pos+1下舵机
    myservo1.write(pos1);
if (pos1==0||pos1==45||pos1==90||pos1==135||pos1==180){

    for (pos2 = 150; pos2 <= 180; pos2 += 1) {       //pos+=1等价于pos=pos+1上舵机
    delay(15);          
  }
  for (pos2 = 180; pos2 >= 150; pos2 -= 1) {
    delay(15);          
  }
    }
    delay(15);          
  }

  
  for (pos1 = 180; pos1 >= 0; pos1 -= 1) {
    myservo1.write(pos1);
    if (pos1==0||pos1==45||pos1==90||pos1==135||pos1==180){

    for (pos2 = 150; pos2 <= 180; pos2 += 1) {       //pos+=1等价于pos=pos+1上舵机
    delay(15);          
  }
  for (pos2 = 180; pos2 >= 150; pos2 -= 1) {
    delay(15);          
  }
    delay(15);          
  }
  }
}

void initializeMPU6050() {
  // Initialize MPU6050
  mpu.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0); // Set to 0 to wake up the MPU6050
  Wire.endTransmission(true);

  // Configure the gyro sensitivity (optional)
  // You may need to adjust this based on your MPU6050 module
  // See MPU6050 datasheet for sensitivity values
  /*Wire.beginTransmission(MPU_addr);
  Wire.write(0x1B); // Gyro configuration register (0x1B)
  Wire.write(0x08); // Set the gyro sensitivity to +/- 500 degrees per second
  Wire.endTransmission(true);*/
}
