#include <SoftwareSerial.h>
SoftwareSerial BTSerial(2, 3); //RX/TX

const int Flex0 = A0; //pin connectd to voltage output
const int Flex1 = A1;
const int Flex2 = A2;
const int Flex3 = A3;
const int Flex4 = A4;
const float VCC =  4.48;//actual voltage of Arduino 5v line
const float Resist0 =  10000;// Measured resistance of resistor
const float Resist1 =  10000;
const float Resist2 =  10000;
const float Resist3 =  10000;
const float Resist4 =  10000;


const float Straight0 = 32000.0;
const float Bend0 = 70000.0;//at 90 deg

const float Straight1 = 42000.0;
const float Bend1 = 86000.0;//at 90 deg

const float Straight2 = 74000.0;
const float Bend2 = 180000.0;//at 90 deg

const float Straight3 = 35000.0;
const float Bend3 = 52000.0;//at 90 deg

const float Straight4 = 30000.0;
const float Bend4 = 80000.0;//at 90 deg


const float readAngle = 50.0; //angle used to detect input
const float maxAngle = 300.0; // Here incase abnormal values occur, common when sensors are not soldered


void setup() {
  pinMode(Flex0, INPUT);
  pinMode(Flex1, INPUT);
  pinMode(Flex2, INPUT);
  pinMode(Flex3, INPUT);
  pinMode(Flex4, INPUT);

  Serial.begin(9600);
  BTSerial.begin(38400);
}

void loop() {

  float angle0 = calculate(Flex0, Straight0, Bend0, Resist0);
  float angle1 = calculate(Flex1, Straight1, Bend1, Resist1);
  float angle2 = calculate(Flex2, Straight2, Bend2, Resist2);
  float angle3 = calculate(Flex3, Straight3, Bend3, Resist3);
  float angle4 = calculate(Flex4, Straight4, Bend4, Resist4);


  
//Trouble Shoot Angles

//  Serial.println("Bend of 0: " + String(angle0) + " degrees");
//  Serial.println("Bend of 1: " + String(angle1) + " degrees");
//  Serial.println("Bend of 2: " + String(angle2) + " degrees");
//  Serial.println("Bend of 3: " + String(angle3) + " degrees");
//  Serial.println("Bend of 4: " + String(angle4) + " degrees");
//  Serial.println(); 
// delay(1000);

  
  if (bent(angle0) || bent(angle1) || bent(angle2) || bent(angle3) || bent(angle4)){
      delay(1000);
      angle0 = calculate(Flex0, Straight0, Bend0, Resist0);
      angle1 = calculate(Flex1, Straight1, Bend1, Resist1);
      angle2 = calculate(Flex2, Straight2, Bend2, Resist2);
      angle3 = calculate(Flex3, Straight3, Bend3, Resist3);
      angle4 = calculate(Flex4, Straight4, Bend4, Resist4); 


    //Normal binary Letters (comment out if using ASL)
     //char printed = letter(bitvalue(angle0), bitvalue(angle1), bitvalue(angle2), bitvalue(angle3), bitvalue(angle4));


    //ASL Letters (comment out if using binary letters)
     char printed = ASLletter(bitvalue(angle0), bitvalue(angle1), bitvalue(angle2), bitvalue(angle3), bitvalue(angle4));

      Serial.print(printed);
      BTSerial.write(printed);

    //Wait for reset to straight
    while(bent(angle0) || bent(angle1) || bent(angle2) || bent(angle3) || bent(angle4)){
      angle0 = calculate(Flex0, Straight0, Bend0, Resist0);
      angle1 = calculate(Flex1, Straight1, Bend1, Resist1);
      angle2 = calculate(Flex2, Straight2, Bend2, Resist2);
      angle3 = calculate(Flex3, Straight3, Bend3, Resist3);
      angle4 = calculate(Flex4, Straight4, Bend4, Resist4);
      
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);

    }
    digitalWrite(LED_BUILTIN, HIGH);
  }
 
 
}

float calculate(int Flex, float Straight, float Bend, float R_DIV ) {
  int flexADC = analogRead(Flex);
  float flexV = flexADC * VCC / 1023.0;
  float flexR = R_DIV * (VCC / flexV - 1.0);
  float angle = map(flexR, Straight, Bend, 0, 90.0);
  return angle;
}

boolean bent(float angle){
  return angle > readAngle && angle < maxAngle;
}

int bitvalue(float angle) {
  if (bent(angle)) 
    return 1;
  else return 0;
}
char ASLletter(int finger0, int finger1, int finger2, int finger3,int finger4){
  int value = 10000 * finger0 + 1000 * finger1 + 100 * finger2 + 10 * finger3 + finger4;

  switch (value) {
    case 1111:
      return 'A';
    case 10000:
      return 'B';
    case 11111:
      return 'C';
     case 11110:
      return '!';
    default:
      return;
     
  }
}
char letter(int finger0, int finger1, int finger2, int finger3,int finger4){
   int value = 10000 * finger0 + 1000 * finger1 + 100 * finger2 + 10 * finger3 + finger4;

   switch (value) {
    case 1: 
      return 'A';
    case 10:
      return 'B';
    case 11:
      return 'C';
    case 100:
      return 'D';
    case 101:
      return 'E';
    case 110:
      return 'F';
    case 111:
      return 'G';
    case 1000:
      return 'H';
    case 1001:
      return 'I';
    case 1010:
      return 'J';
    case 1011:
      return 'K';
    case 1100:
      return 'L';
    case 1101:
      return 'M';
    case 1110:
      return 'N';
    case 1111:
      return 'O';
    case 10000:
      return 'P';
    case 10001:
      return 'Q';
    case 10010:
      return 'R';
    case 10011:
      return 'S';
    case 10100:
      return 'T';
    case 10101:
      return 'U';
    case 10110:
      return 'V';
    case 10111:
      return 'W';
    case 11000:
      return 'X';
    case 11001:
      return 'Y';
    case 11010:
      return 'Z';
    case 11110:  
      return '!'; //Next line
    case 11100:
      return ' ';   //Space
    default:
      return ;    
   }

}

