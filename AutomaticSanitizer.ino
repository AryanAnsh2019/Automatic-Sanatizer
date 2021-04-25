#include <WiFi.h>
#include<HTTPClient.h>
#include<Blynk.h>
#include<BlynkSimpleEsp32.h>
#define BLYNK_PRINT Serial
char ssid[] = "Ktayal";
char pass[] = "1357924680";
char auth[] = "6f5d3f0634f742c78606f4366bfb3c9e";
WidgetLCD lcd(V1);
int i = 0, start = 0;
boolean wifi = true;
String active, recovered, deaths;
int total;
//setup and declaration finishes here

void setup()
{
  Serial.begin(9600);
  pinMode(19, OUTPUT);//relay
  pinMode(34, INPUT);//sensor
  pinMode(4, INPUT); //switch
  WiFi.mode(WIFI_STA);
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to SSID: ");
    while (WiFi.status() != WL_CONNECTED)
    {
      WiFi.begin(ssid, pass);
      Serial.print(".");
      start = millis();
      if (start >= 15000)//check wifi
      {
        wifi = false;
        break;
      }
      else
      {
        wifi = true;
      }
      delay(5000);
    }
    Serial.println("\nConnected.");
  }
  if (wifi == true)
  {
    Blynk.begin(auth, ssid, pass);
  }
}


void getData()
{
  HTTPClient http, http1, http2;
  http1.begin("https://api.thingspeak.com/apps/thinghttp/send_request?api_key=HMETIK8VW44Z8D2T");
  int httpCode1 = http1.GET();
  http.begin("https://api.thingspeak.com/apps/thinghttp/send_request?api_key=YB83D288BX7OC3DW");
  int httpCode = http.GET();
  http2.begin("https://api.thingspeak.com/apps/thinghttp/send_request?api_key=K3WPTDGQ4S5EEXTO");
  int httpCode2 = http2.GET();
  if (httpCode > 0)
  {
    active = http.getString();
  }
  if (httpCode1 > 0)
  {
    recovered = http1.getString();
  }
  if (httpCode2 > 0)
  {
    deaths = http2.getString();
  }
  lcd.clear();
  total = active.toInt() + recovered.toInt() + deaths.toInt() + 1;
  lcd.print(0, 0, "Total  : ");
  lcd.print(10, 0, total);
  lcd.print(0, 1, "Active : ");
  lcd.print(10, 1, active);
  delay(1000);
  lcd.clear();
  lcd.print(0, 0, "Recover : ");
  lcd.print(10, 0, recovered);
  lcd.print(0, 1, "Deaths  : ");
  lcd.print(10, 1, deaths);
  delay(1000);
}

void getPrecautions()
{
  lcd.clear();
  lcd.print(0, 0, "Wash Hands");
  lcd.print(0, 1, "Avoid Contact");
  delay(1000);

}

void sensorCheck()
{
  int value = digitalRead(34);
  if (value == 0)
  {
    delay(100);   
    digitalWrite(19, HIGH);
    Serial.println("Pump on!!");
    delay(2000);
    digitalWrite(19, LOW);
    Serial.println("Pump off!!");
    delay(2000);
  }
}

void loop()
{
  if (wifi == true)
  {
    Blynk.run();
    sensorCheck();
    //    getData();
    //    getPrecautions();
  }
  else
  {
    sensorCheck();
  }
}
