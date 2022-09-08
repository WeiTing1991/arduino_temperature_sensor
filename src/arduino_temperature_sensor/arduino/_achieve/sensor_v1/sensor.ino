// ***********************************************************************************
// 
// PROJECT     :  temperature sensor V1 for arduino version
// 
// FUNCTION    :  sensor
// 
// AUTHOR      :  Wei-Ting Chen
//
// Reference   :  https://arduinojson.org/v6/api/jsondocument/
//                https://www.arduino.cc/reference/en/
//
//
//
// ***********************************************************************************

#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>


// *****************************************************************
// define the display type
LiquidCrystal_I2C lcd (0x27, 20,  4);


// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
int index = 0 ;

// To store in the heap
// that implicitly becomes an object
DynamicJsonDocument doc(1024);

// Useful :

// *****************************************************************



//******************************************************************
// Main setup
void setup(void)
{
  // start serial port
  Serial.begin(9600);
  Serial.println("Temperature Sensor");

  // Start up sensor
  sensors.begin();
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");
  
  // report parasite power requirements
  Serial.print("Parasite power is: "); 
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");

  // Start up LCD
  lcd.init();
  lcd.backlight();
  
}
//******************************************************************



//******************************************************************
// Function

// print the temperature on screen
void printTemperature(int index)
{
  
  //float tempC = sensors.getTempC(deviceAddress);
  float tempC = sensors.getTempCByIndex(index);
  int counter  = 0 ;
  if(tempC == DEVICE_DISCONNECTED_C) 
  {
    Serial.println("Error: Could not read temperature data");
    return;
  }
  
  // print temperature
  Serial.print("Temp C");
  Serial.print(",");
  Serial.println(tempC);
  
  // To json file 
  doc["Time"] = counter;
  doc["temperature"] = tempC;
  serializeJson(doc, Serial);

  //Serial.print(" Temp F: ");
  //Serial.println(DallasTemperature::toFahrenheit(tempC));

  //
  lcd.clear();
  lcd.print(" Temp C: ");
  lcd.setCursor(10,0);
  lcd.print(tempC);
  
}

// device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

//******************************************************************
//

//******************************************************************
// Main loop
void loop(void)
{ 
  
  // request to all devices on the bus and to issue a global temperature
  Serial.println(" ");
  Serial.println("Requesting temperatures...");
  sensors.requestTemperatures();
  Serial.println("DONE");

  // print the current temperature
  printTemperature(index);
  

}
//******************************************************************
// Main loop
