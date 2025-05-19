/* https://github.com/xzhang03/Pico-ADS131M04
*/
#include "ADS131M04.h"
#include <SPI.h>
#include <WiFi.h>
#include <WiFiClient.h>



float res;
ADS131M04 adc;
float result1=0;

#define LED_STATUS 25

/* Wi-Fi info */
char ssid[] = "YourSSID";  // your network SSID (name)
char pass[] = "YourSSIDPassword";   // your network password
WiFiMulti multi;


int status = WL_IDLE_STATUS;  // the Wifi radio's status

char server[] = "api.thingspeak.com";  // server address
String apiKey = "Your write API Key";    // apki key

// Thingspeak

unsigned long lastThingSpeakUpdate = 0;  // last time you connected to the server, in milliseconds
unsigned long lastConnectionTime = 0;    // last time you connected to the server, in milliseconds
WiFiClient client;



void setup() {

  Serial.begin(115200);
  adc.begin(14, 12, 13, 5, 19, 3); 

  delay(1000);
  Serial.println("");

  adc.setInputChannelSelection(0, INPUT_CHANNEL_MUX_AIN0P_AIN0N);
  adc.setInputChannelSelection(1, INPUT_CHANNEL_MUX_AIN0P_AIN0N);
  adc.setInputChannelSelection(2, INPUT_CHANNEL_MUX_AIN0P_AIN0N);
  adc.setInputChannelSelection(3, INPUT_CHANNEL_MUX_AIN0P_AIN0N);

  // We start by connecting to a WiFi network
  
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  multi.addAP(ssid, pass);

  if (multi.run() != WL_CONNECTED) {
    Serial.println("Unable to connect to network");
   // LED_delay = 300;
    //rp2040.reboot();
  }
  else{
  //  LED_delay = 2000;
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

}

void loop() {
  adcOutput res;
  delay(30000);

  while (1)
  {
    if (adc.isDataReady())
    {
      res = adc.readADC();

      Serial.print("Status = ");
      Serial.println(res.status, BIN);

      Serial.print("CH0 = ");
      Serial.println(res.ch0);
      Serial.print("CH1 = ");
      Serial.println(res.ch1);
      Serial.print("CH2 = ");
      Serial.println(res.ch2);
      Serial.print("CH3 = ");
      Serial.println(res.ch3);
      Serial.println("");
      delay(500);
      result1 = res.ch0;
    }
  }
SendThingSpeak();
}


void SendThingSpeak() {


  if (multi.run() != WL_CONNECTED) {
    multi.addAP(ssid, pass);
    delay(5000);
  }
  printWifiStatus();
  // close any connection before send a new request
  // this will free the socket on the WiFi shield
  client.stop();

  // if there's a successful connection
  if (client.connect(server, 80)) {
    Serial.println("Connecting...");

    // send the Get request
    client.print(F("GET /update?api_key="));
    client.print(apiKey);
    client.print(F("&field5="));
    client.print(String(result1));
    client.println();
    // note the time that the connection was made
    lastConnectionTime = millis();
   
  } else {
    // if you couldn't make a connection
   
    Serial.println("Connection failed");
  }
}


void printWifiStatus() {
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
