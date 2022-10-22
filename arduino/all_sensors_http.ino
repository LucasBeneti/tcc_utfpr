#include <DFRobot_ENS160.h>
#include "DFRobot_AHT20.h"
#include <BH1750.h>
#include <WiFi.h>
#include <HTTPClient.h>
//#include <Wire.h>

#define SEA_LEVEL_PRESSURE    1015.0f

const char *ssid = "APT-1702-2.4G";
const char *password = "16047440";

const char *serverURL = "http://192.168.18.90:3000/save";

DFRobot_AHT20 aht20;
//AHT20 aht20(&Wire, 0x38);

DFRobot_ENS160_I2C ENS160(&Wire, 0x53);

BH1750 lightMeter;

void setup() {
  Serial.begin(115200);
  delay(100);
  // connect to wifi
  WiFi.begin(ssid, password);
  Serial.println("Connecting...");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  
  // esperando conexao do aht20
  uint8_t status;
  while((status = aht20.begin()) != 0){
    Serial.print("AHT20 sensor initialization failed. error status : ");
    Serial.println(status);
    delay(1000);
  }

  while( NO_ERR != ENS160.begin() ){
    Serial.println("Communication with device failed, please check connection");
    delay(3000);
  }
  // BH1750 sensor setup
  lightMeter.begin();
  Serial.println("BH1750 sensor setted up");
  
  float controlTemp = 0.0;
  float controlHum = 0.0;
  if(aht20.startMeasurementReady(/* crcEn = */true)) {
    controlTemp = aht20.getTemperature_C();
    controlHum = aht20.getHumidity_RH();
    
    Serial.print("Temp: ");
    Serial.print(controlTemp);
    Serial.print(" C\n");
    Serial.print("Hum: ");
    Serial.print(controlHum);
    Serial.print(" %\n");
  }
  /**
   * Set power mode
   * mode Configurable power mode:
   *   ENS160_SLEEP_MODE: DEEP SLEEP mode (low power standby)
   *   ENS160_IDLE_MODE: IDLE mode (low-power)
   *   ENS160_STANDARD_MODE: STANDARD Gas Sensing Modes
   */
  ENS160.setPWRMode(ENS160_STANDARD_MODE);

   /**
   * Users write ambient temperature and relative humidity into ENS160 for calibration and compensation of the 
measured gas data.
   * ambientTemp Compensate the current ambient temperature, float type, unit: C
   * relativeHumidity Compensate the current ambient temperature, float type, unit: %rH
   */
  ENS160.setTempAndHum(controlTemp, controlHum);
}

void loop() {
  delay(100);
  Serial.println("");               
  Serial.println("================ ENS160 + AHT20 SENSOR TEST ================");
    /**
   * Get the sensor operating status
   * Return value: 0-Normal operation, 
   *         1-Warm-Up phase, first 3 minutes after power-on.
   *         2-Initial Start-Up phase, first full hour of operation after initial power-on. Only once in the sensor’s 
lifetime.
   * note: Note that the status will only be stored in the non-volatile memory after an initial 24h of continuous
   *       operation. If unpowered before conclusion of said period, the ENS160 will resume "Initial Start-up" mode
   *       after re-powering.
   */
  uint8_t Status = ENS160.getENS160Status();
  Serial.print("Sensor operating status : ");
  Serial.println(Status);

  // medidas de umidade e temperatura
  float temp = 0.0;
  float umid = 0.0;

  if(aht20.startMeasurementReady(/* crcEn = */true)) {
    temp = aht20.getTemperature_C();
    umid = aht20.getHumidity_RH();
    
    Serial.print("Temp: ");
    Serial.print(temp);
    Serial.print(" C\n");
    Serial.print("Hum: ");
    Serial.print(umid);
    Serial.print(" %\n");
  }
  
  /**
   * Get TVOC concentration
   * Return value range: 0–65000, unit: ppb
   */
  uint16_t TVOC = ENS160.getTVOC();
  Serial.print("Concentration of total volatile organic compounds : ");
  Serial.print(TVOC);
  Serial.println(" ppb");

  /**
   * Get CO2 equivalent concentration calculated according to the detected data of VOCs and hydrogen (eCO2 – 
Equivalent CO2)
   * Return value range: 400–65000, unit: ppm
   * Five levels: Excellent(400 - 600), Good(600 - 800), Moderate(800 - 1000), 
   *               Poor(1000 - 1500), Unhealthy(> 1500)
   */
  uint16_t ECO2 = ENS160.getECO2();
  Serial.print("Carbon dioxide equivalent concentration : ");
  Serial.print(ECO2);
  Serial.println(" ppm");
  Serial.println();
  Serial.println("================ ENS160 + AHT20 SENSOR TEST END ================");

  Serial.println("================ BH1750 SENSOR TEST START ================");
  float lux = lightMeter.readLightLevel();

  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  Serial.println("================ BH1750 SENSOR TEST START ================");


  // send data to server
  sendSensorsData(lux, temp, umid, ECO2);
  delay(2000);
}

//int sendSensorsData(float lux, float temp, float hum, float co2) {
int sendSensorsData(float lux, float temp, float hum, float co2) {
  if(WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    http.begin(client, serverURL);

    http.addHeader("Content-Type","application/json");
    http.addHeader("Authorization","0");
    String sendingData = 
"{\"api_key\":\"tPmAT5Ab3j7F9\",\"l\":\"BME280\",\"value1\":\"24.25\",\"value2\":\"49.54\",\"value3\":\"1005.14\"}";
   
    int responseCode = http.POST("{\"d\":\"poc0\",\"l\":"+ String(lux) +",\"t\":"+ String(temp) +",\"c\":" + 
String(co2) +",\"h\":" + String(hum) +"}");

    Serial.print("Response code: ");
    Serial.println(responseCode);

    http.end();
    return responseCode;
  } else {
    Serial.println("Wifi disconnected");
    return -1;
  }
}

