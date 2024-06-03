#define BLYNK_TEMPLATE_ID "xxxxxx"
#define BLYNK_TEMPLATE_NAME "xxxxx"
#define BLYNK_AUTH_TOKEN "xxxxx"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char ssid[] = "xxxxxx";
char pass[] = "xxxxxx";

BlynkTimer timer;

int counter = 0;

void myTimerEvent() {
  String data = "";

  while (Serial.available() > 0) {
    data += char(Serial.read());
  }

  data.trim();

  if (data.length() > 0) {
    int temperatureIndex = data.indexOf("suhu:");
    int tdsIndex = data.indexOf("tds:");
    int phIndex = data.indexOf("ph:");

    if (temperatureIndex != -1 && tdsIndex != -1 && phIndex != -1) {
      // Extract temperature value
      String temperatureString = data.substring(temperatureIndex + 5, tdsIndex - 1);
      float temperature = temperatureString.toFloat();

      // Extract TDS value
      String tdsString = data.substring(tdsIndex + 4, phIndex - 1);
      int tds = tdsString.toInt();

      // Extract pH value
      String phString = data.substring(phIndex + 3);
      float ph = phString.toFloat();

      // Send each value to Blynk
      Blynk.virtualWrite(V0, temperature);  
      Blynk.virtualWrite(V1, tds);          
      Blynk.virtualWrite(V2, ph);           
    }
  }
}

void setup() {
  Serial.begin(9600);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(1000L, myTimerEvent);
}

void loop() {
  Blynk.run();
  timer.run();  
}
