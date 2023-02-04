#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiClient.h>
#include <TimeLib.h>//时间
#include <WiFiUdp.h>
const char *wifiName="CMCC-test";
const char* wifiPwd="internet123456";


int led=5;
bool led_status;
void setup() 
{
  Serial.begin(9600);
  pinMode(led,OUTPUT);
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifiName,wifiPwd);   //开始连接
  while(WiFi.status()!=WL_CONNECTED)
  {
  Serial.println("trying......");
    delay(500);
   }
}



void testWIFI()
{
   if(WiFi.status()!=WL_CONNECTED)
  {
      WiFi.disconnect();
      WiFi.begin(wifiName,wifiPwd);   //开始连接
      while(WiFi.status()!=WL_CONNECTED)
       {
           Serial.println("trying......");
           delay(500);
        }   
  }
  else if(WiFi.status()==WL_CONNECTED)
  {
       //Serial.print("WIFI_CONNECTED");
       //Serial.print("-->IP address");
      // Serial.println(WiFi.localIP());
       digitalWrite(led,1);
       Serial.println("@"+getTime());
       delay(500);
       digitalWrite(led,0);
       delay(500);
  }
  
}

String getTime()
{ 
  String now_time;
  if(WiFi.status()==WL_CONNECTED)
  {
    HTTPClient http;
    http.begin("http://quan.suning.com/getSysTime.do");
    int httpCode=http.GET();
    if(httpCode>0)
    {
        const size_t cap=JSON_OBJECT_SIZE(2)+60;
        DynamicJsonBuffer jsonBuffer(cap);
        JsonObject&root=jsonBuffer.parseObject(http.getString());
        const char* systime=root["sysTime1"];
       now_time=systime;
        //Serial.println(systime);
      }
    }  
  return now_time;
}

void loop() 
{
 testWIFI();
}
