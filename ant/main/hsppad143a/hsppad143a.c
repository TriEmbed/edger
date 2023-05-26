#ifdef CONFIG_HSPPAD143A 
#ifdef ARDUINO
#include <Arduino.h>
#include <Wire.h>

#define DISPLAY_ERROR false
#define LOOP_DELAY    10000
#define USER_PIN      false

// Customize I2C bus pins for ESP8266 or ESP32
const int PIN_SCL = 22;
const int PIN_SDA = 21;

String I2Ctest() {
  byte error, address;
  int nDevices;
  String s;

  s = "Scanning...\n";

  nDevices = 0;
  for (address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      s += "I2C device found at 0x";
      if (address < 16)
        s += "0";
      s += String(address, HEX);
      s += "\n";

      nDevices++;
    } else if ( error > 0 ) {
      if ( DISPLAY_ERROR ) {
        s += "Unknow error at 0x";
        if (address < 16)
          s += "0";
        s += String(address, HEX);
        s += "\n";
      }
    }
  }
  if (nDevices == 0)
    s += "No I2C devices found\n";
  else
    s += "done\n";
  return s;
}

void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println("I2C scanner");
#if USER_PIN
  Wire.begin(PIN_SDA, PIN_SCL);
#else
  Wire.begin();
#endif
  Serial.println(I2Ctest());// device should be 0x48
  delay(10000);
}
static const int port = 0x48;



static const char readRegister(char value)
{
  unsigned char c;
  Wire.beginTransmission(port);
  // device address is specified in datasheet
  Wire.write(value);          // sends value byte
  Wire.endTransmission();     // stop transmitting

  Wire.requestFrom(port, 1);   

  unsigned char ret;
  while (Wire.available() )
  {
    c = Wire.read();    // receive a byte as character

  }
  return  c;    // receive a byte as character
}



static void Whoami()
{
  unsigned char c = readRegister(0);             // sends value byte
  Serial.printf("who am i[0]==0x49\t\t0x%x\n", c);
}

static void Information()
{
  unsigned char c = readRegister( 1);  
  Serial.printf("Information[0x1:1]==0x31\t0x%x\n", c);
}



static void FIFO_Status()
{
  typedef struct {
    unsigned int FIFI_Stored : 4;
    unsigned int : 3;
    unsigned int FIFO_Event : 1;
  } _status;

  union a {
    unsigned    char val;
    _status reg;
  } k;

  k.val =  readRegister(2);           // sends value byte
  Serial.printf("FIF0 status[0x2:1]\t\t0x%02x\tEvent=%x Stored=%x\n", k.val, k.reg.FIFO_Event, k.reg.FIFI_Stored );
}



static unsigned char Status()
{
  typedef struct {

    unsigned int Status_PRDY : 1;
    unsigned int : 1;
    unsigned int Status_PDOR : 1;
    unsigned int : 1;
    unsigned int Status_TRDY : 1;
    unsigned int : 2;
    unsigned int Status_Busy : 1;


  } _status;
 
  union a {
    unsigned char val;
    _status reg;
  } k;

  k.val =  readRegister(3);           // sends value byte
  Serial.printf("status[0x3:1]\t\t\t0x%02x\tBusy=%x\tTRDY=%x\tPDOR=%x\tPRDY=%x\n", k.val, k.reg.Status_Busy, k.reg.Status_TRDY,  k.reg.Status_PDOR, k.reg.Status_PRDY );
  return k.val;
}


static void readProduct() {
  unsigned char c = readRegister(0x1c);
  Serial.printf("Product Number[0x1c:1]==0xe0\t0x%02x\n", c);

}

static void readPressure()
{


  Serial.print("\nPressure[4:3]=");
  Wire.beginTransmission(port);
  // device address is specified in datasheet
  Wire.write(4);              // sends value byte
  Wire.endTransmission();     // stop transmitting

  Wire.requestFrom(port, 3);    

  while (Wire.available())   // slave may send less than requested
  {
    float val;
    val = Wire.read();
    val += Wire.read() * 256.0;
    val += (Wire.read() & 0x1) * 256.0 * 256.0;
    Serial.printf("\n\t%.0f Pa (%.3f psi)", val * 2, val * 2 * 0.00014503773);
    val = (101325.0 - val) / 9800.0;
    Serial.printf("\n\t%s %f (%f Feet) (101325.0-val)/9800.0)", val >= 0.0 ? "above" : "below", val, val * 39.37);
  }
}
static void   readTemperature()
{

  Serial.print("\nTemperature[9:2]=");
  Wire.beginTransmission(port);
  // device address is specified in datasheet
  Wire.write(9);              // sends value byte
  Wire.endTransmission();     // stop transmitting

  Wire.requestFrom(port, 2);   

  while (Wire.available())   // slave may send less than requested
  {
    float val = Wire.read();
    val +=  Wire.read() * 256.0;
    val /= 256.0;
    Serial.printf("\n\t%.3f Centigrade (%.2f Fahrenheit)", val  , 32.0 + 1.8 * (val));
  }
}

void loop() {

  Whoami();
  Information();
  FIFO_Status();
  Status();
  readProduct();

  static const unsigned char cmd = 0b1010;
  Serial.printf("WRITE Action Control[0x10]\t0x%02x\n", cmd);
  Wire.beginTransmission(port);
  // device address is specified in datasheet
  Wire.write(0x10);           // sends value byte
  Wire.write(cmd);         // sends value byte
  Wire.endTransmission();     // stop transmitting
  while (Status() != 0x11);
  

  readPressure();
  readTemperature();

  Serial.println("");
  delay(LOOP_DELAY);
}
#endif
#endif