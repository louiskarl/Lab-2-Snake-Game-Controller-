#include <Wire.h>
//Lab 2 Arduino Code. References Elegoo open source examples and content from the arduino "wire.h" reference guide. 
//Initializes program variables
int VRx = A0; //Assign analog pins A0 and A1 as inputs from the joystick.
int VRy = A1;
int SW = 2;
int buzz = 3;
int xPosition = 0;
int yPosition = 0;
int SW_state = 0;
int mapX = 0;
int mapY = 0;
int counter = 0;
int incomingByte = 0;
int appleFlag = 0;

int16_t gyro[] = {0, 0, 0}; //Creates a 3 element array to assign gyro data to.

void setup() {
  Serial.begin(9600); //Establishes baud rate an opens serial port for communicating with COM3 port on laptop.
  Wire.begin(); //Join I2C bus
  //Establish pin assignments as inputs.
  pinMode(VRx, INPUT);
  pinMode(VRy, INPUT);
  pinMode(SW, INPUT_PULLUP); 
  pinMode(buzz, OUTPUT);
  digitalWrite(buzz, LOW);

  //Initialize the MPU6050
  Wire.begin(); //Begin I2C communicaiton
  Wire.beginTransmission(0x68); //Transmit to device at address 0x68 (address of MPU6050 found in reference manual)
  Wire.write(0x6B); //Write 0x00 to register 0x6B. This resets the MPU6050.
  Wire.write(0x00);
  Wire.endTransmission(); //End I2C communication.

}

void loop() {
  //Request sensor data from MPU6050
  Wire.beginTransmission(0x68); //Specify MPU6050 address
  Wire.write(0x43); //Specify register to pull data from (0x43 is the beginning register where gyroscoe data is stored.)
  Wire.endTransmission(); //End transmission
  Wire.requestFrom(0x68, 6); //Pull 6 bytes of data read from the MPU. (These will be six bytes starting at address 0x43, which yields data for all three axes).

  //Read joystick inputs
  xPosition = analogRead(VRx); //Read ADC value at pin A0
  yPosition = analogRead(VRy); //Read ADC value at pin A1
  SW_state = digitalRead(SW);  //Read digital value at pin 2
  mapX = map(xPosition, 0, 1023, -10, 10); //Convert the 0-1023 number into an integer between -10 and 10.
  mapY = map(yPosition, 0, 1023, -10, 10);

  //Checks if the python prgram set the flag indicating the apple was eaten, and if so, turns on the buzzer.
    // read the incoming byte:
    incomingByte = Serial.read();
    
    if(incomingByte != -1) {  //Serial.read() returns a -1 if it detects no data at the serial port. 
      digitalWrite(buzz, HIGH);
      appleFlag = 1;
    }
    else {
      digitalWrite(buzz, LOW);
      appleFlag = 0;
    }

 //Use Serial library to export data stream to PC. Data values are seperated by a /.
  //Joystick outputs
  Serial.print("/");
  Serial.print(mapX);
  Serial.print("/");
  Serial.print(mapY);
  Serial.print("/");
  Serial.print(SW_state);
  //MPU6050 outputs
  Serial.print("/");
  
    for(int i = 0 ; i < 3 ; ++i) { //For loop which fills the three values in gyro with the per-axis gyro data.
    gyro[i] = Wire.read() << 8 | Wire.read(); //Specifies that the value in gyro[i] will be written starting with the MSB and ending with the LSB, since each axis requires 16 bits of data.
    Serial.print(gyro[i]); //Prints value of gyro[i] to datastream to PC
    Serial.print("/");
    }
  Serial.print(incomingByte);
  Serial.println("/");

  delay(100);
  }  
