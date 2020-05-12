#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <dht.h>

dht DHT_1;
dht DHT_2;
dht DHT_3;

#define DHT11_PIN_1 7
#define DHT11_PIN_2 8
#define DHT11_PIN_3 9

/*Temperature Variables*/
int x_box = 0;
int y_box = 0;
int z_box = 0;

/*Temperature Variables*/
float temp1;
float temp2;
float temp3;

/*Hall Effect Variables*/

int HE_PIN = 3;
unsigned long timeold = 0;
volatile byte rotations = 0;
unsigned int rpmilli = 0;
unsigned int rpm;
float speed;

char dataString[50] = {0}; //What we are serial-ly sending to the RasPi

Adafruit_BNO055 bno = Adafruit_BNO055(55);



void setup()
{
  Serial.begin(9600);
  delay(5000); // Let's the Pi catch up

  setup_imu();
  setup_sensors();
}

void loop()
{
  loop_imu();
  loop_sensor();
  countRotations();
  delay(250);
}

/******************************************************************************************/
void setup_imu() {
  //Serial.println("Setting Up IMU");
  //Serial.println("");

  /* Initialise the sensor */
  if (!bno.begin()) {
    Serial.print("Ooops, no IMU detected ... Check your wiring or I2C ADDR!");
    while (1);
  }
  delay(1000);
  bno.setExtCrystalUse(true);
}

/***************************************************************************************/
void setup_sensors() {
  //Serial.println("Setting Up Hall-Effect Sensor");
  //Serial.println("");

  /*HE Sensor Init*/
  pinMode(HE_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(HE_PIN), countRotations, FALLING);
}

/*********************************************************************************************/
void loop_sensor() {
  int chk0 = DHT_1.read11(DHT11_PIN_1);
  int chk1 = DHT_2.read11(DHT11_PIN_2);
  int chk2 = DHT_3.read11(DHT11_PIN_3);

  if (rotations >= 5) { //increase rpmCount for better RPM resolution, decrease for faster update
    rpm = 60.0 * 1000.0 * rotations / (millis() - timeold); //60*1000 is the factor to convert ms to minutes
    timeold = millis();
    rotations = 0;
    //Serial.println(rpm, DEC);
  }


sprintf(dataString, "%03d%03d%03d%04d%04d%04d%04d", (int)(x_box), (int)(y_box), (int)(z_box), (int)(DHT_1.temperature * 10), (int)(DHT_2.temperature * 10), (int)(DHT_3.temperature * 10), (int)(rpm)) ;
//When adding more variables, make sure to leave RPM at the end so as to make it "infinitely expand" incase RPM get to high
Serial.println(dataString);
}


/***************************************************************************************/
void countRotations() {
  rotations++;
}

/****************************************************************************************/
void loop_imu(void)
{
  /* Get a new sensor event */
  sensors_event_t event;
  bno.getEvent(&event);

  x_box = event.orientation.x;
  y_box = event.orientation.y;
  z_box = event.orientation.z;

  x_box += 180;
  y_box += 180;
  z_box += 180;

  delay(100);
}
