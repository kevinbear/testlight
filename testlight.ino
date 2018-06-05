//---Pin define---//
// Left
#define L0 1
#define L1 7
#define L2 5
#define L3 6
#define LOut 2
// Right
#define R0 A4
#define R1 A3   
#define R2 A2
#define R3 A1
#define ROut 3
// Motor
#define RE 10 
#define RM 12
#define LE 11
#define LM 13
//---------------//
int frequency = 0;
int Lrgb[3]={0};
int Rrgb[3]={0};
int fre = 0;
bool lwarning = false;
bool rwarning = false;
//set up tcs3200 sensor
void LightInit(int mode)
{
  pinMode(L0, OUTPUT);
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(L3, OUTPUT);
  pinMode(LOut, INPUT);

  pinMode(R0, OUTPUT);
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(R3, OUTPUT);
  pinMode(ROut, INPUT);

  pinMode(RM, OUTPUT);
  pinMode(LM, OUTPUT);
  digitalWrite(RE,LOW);
  digitalWrite(LE,LOW);
  if (mode == 1){  // Setting frequency-scaling to 2%
    digitalWrite(L0,LOW);
    digitalWrite(L1,HIGH);
    digitalWrite(R0,LOW);
    digitalWrite(R1,HIGH);
  }
  else if (mode == 2){ // Setting frequency-scaling to 20%
    digitalWrite(L0,HIGH);
    digitalWrite(L1,LOW);
    digitalWrite(R0,HIGH);
    digitalWrite(R1,LOW);
  }
  else if (mode == 3){ // Setting frequency-scaling to 100%
    digitalWrite(L0,HIGH);
    digitalWrite(L1,HIGH);
    digitalWrite(R0,HIGH);
    digitalWrite(R1,HIGH);
  }
  else { // Setting Power down
    digitalWrite(L0,LOW);
    digitalWrite(L1,LOW);
    digitalWrite(R0,LOW);
    digitalWrite(R1,LOW);
  }
}

void MotorDirection(int mode)
{
  switch (mode)
  {
    case 0: // stop
           digitalWrite(RE,LOW);
           digitalWrite(LE,LOW);
           break;
    case 1: // foward
           digitalWrite(RE,HIGH);
           digitalWrite(RM,LOW);
           digitalWrite(LE,HIGH);
           digitalWrite(LM,LOW);
           break;
    case 2: // backward
           digitalWrite(RE,HIGH);
           digitalWrite(RM,HIGH);
           digitalWrite(LE,HIGH);
           digitalWrite(LM,HIGH);
           break;
    case 3: // turn left
          digitalWrite(RE,HIGH);
          digitalWrite(RM,LOW);
          digitalWrite(LE,LOW);
          break;
    case 4: //turn right
          digitalWrite(RE,LOW);
          digitalWrite(LE,HIGH);
          digitalWrite(LM,LOW);
          break; 
  }
}

void ReadRGB(){ 
  lwarning = false;
  rwarning = false;
  // Setting red filtered photodiodes to be read
  digitalWrite(L2,LOW);
  digitalWrite(L3,LOW);
  digitalWrite(R2,LOW);
  digitalWrite(R3,LOW);
  // Reading the output frequency
  Lrgb[0] = pulseIn(LOut, LOW);
  Rrgb[0] = pulseIn(ROut,LOW);
  // Printing the value on the serial monitor
//  Serial.print("R= ");//printing name
//  Serial.print(Lrgb[0]);//printing RED color frequency
//  Serial.print("  ");
//  Serial.print(Rrgb[0]);
//  Serial.print("  ");
  delay(100);

  // Setting Green filtered photodiodes to be read
  digitalWrite(L2,HIGH);
  digitalWrite(L3,HIGH);
  digitalWrite(R2,HIGH);
  digitalWrite(R3,HIGH);
  // Reading the output frequency
  Lrgb[1] = pulseIn(LOut, LOW);
  Rrgb[1] = pulseIn(ROut,LOW);
  // Printing the value on the serial monitor
//  Serial.print("G= ");//printing name
//  Serial.print(Lrgb[1]);//printing RED color frequency
//  Serial.print("  ");
//  Serial.print(Rrgb[1]);
//  Serial.print("  ");
  delay(100);

  // Setting Blue filtered photodiodes to be read
  digitalWrite(L2,LOW);
  digitalWrite(L3,HIGH);
  digitalWrite(R2,LOW);
  digitalWrite(R3,HIGH);
  // Reading the output frequency
  Lrgb[2] = pulseIn(LOut, LOW);
  Rrgb[2] = pulseIn(ROut,LOW);
  // Printing the value on the serial monitor
//  Serial.print("B= ");//printing name
//  Serial.print(Lrgb[2]);//printing RED color frequency
//  Serial.print("  ");
//  Serial.print(Rrgb[2]);
//  Serial.println("  ");
  delay(100);

  //add condiation yellow and return 
  if ((Lrgb[0] > 20) && (Lrgb[0] <53) && (Lrgb[1] > 60) && (Lrgb[1] < 92) && (Lrgb[2] > 80) && (Lrgb[2] <147)) 
    lwarning = true;
  if ((Rrgb[0] > 20) && (Rrgb[0] <53) && (Rrgb[1] > 60) && (Rrgb[1] < 92) && (Rrgb[2] > 80) && (Rrgb[2] <147))
    rwarning = true;
}

//--------main----------//
void setup() 
{
  LightInit(3);
  Serial.begin(9600);
}

void loop() 
{
  ReadRGB();
//  Serial.print(Lrgb[0]);
//  Serial.print("  ");
//  Serial.print(Lrgb[1]);
//  Serial.print("  ");
//  Serial.print(Lrgb[2]);
//  Serial.print("  ");
//  Serial.println(lwarning);
//  Serial.println(rwarning);
  if ((!rwarning) && (!lwarning)) // car foward
  {
    MotorDirection(1);
  }
  else if ((rwarning) || (lwarning)) // don't care left or right out of lens
  {
    MotorDirection(0);
    delay(500);
    if (lwarning) {
      MotorDirection(4); // right wheel move
    }
    if (rwarning) {
      MotorDirection(3); // right wheel move
    }
  }
}
//---------------------//


