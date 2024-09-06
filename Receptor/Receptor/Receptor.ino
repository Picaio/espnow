#include <Arduino.h>
#include <esp_now.h>
#include <ESP32Servo.h>
#include <WiFi.h>

#define CHANNEL   1
#define PinLed   18

typedef struct Mensaje
{
  int dato1; 
  int dato2;
  int dato3;
  int dato4;
} Mensaje;



Mensaje myData;


//----------------------------------recepcion de datos del maestro--------------------------------------
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *Rxdata, int data_len) 
{
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print("Last Packet Recv from: "); 
  Serial.println(macStr);
  memcpy(&myData, Rxdata, sizeof(myData));
  Serial.print("Last Packet Recv Data: "); 
  Serial.print   (myData.dato1);
  Serial.print   (myData.dato2);
  Serial.print   (myData.dato3);
  Serial.print   (myData.dato4);
  Serial.println("");
  int Intensidad = map(myData.dato1, 0, 4095, 0, 255);
  analogWrite (PinLed, Intensidad );  
}

//----------------------------------inicia el wifi--------------------------------------------------

void wifi_init (void)
{
  Serial.println("ESPNow/Basic/Slave Example");
  //Set device in AP mode to begin with
  WiFi.mode(WIFI_AP);
  // configure device AP mode
  configDeviceAP();
  // This is the mac address of the Slave in AP Mode
  Serial.print("AP MAC: "); Serial.println(WiFi.softAPmacAddress());
  // Init ESPNow with a fallback logic
  InitESPNow();
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info.
  esp_now_register_recv_cb(OnDataRecv);
}


//----------------------------------inicia la comunicacion espnow--------------------------------------------------
void InitESPNow() 
{
  WiFi.disconnect();
  if (esp_now_init() == ESP_OK) 
    {
      Serial.println("ESPNow Init Success");
    }
  else 
    {
      Serial.println("ESPNow Init Failed");
        ESP.restart();
    }
}

//----------------------------------configura la red del esclavo--------------------------------------------------

void configDeviceAP() 
{
  const char *SSID = "Slave_1";
  bool result = WiFi.softAP(SSID, "Slave_1_Password", CHANNEL, 0);
  if (!result) 
    {
      Serial.println("AP Config failed.");
    } 
  else 
  {
    Serial.println("AP Config Success. Broadcasting with AP: " + String(SSID));
    Serial.print("AP CHANNEL "); 
    Serial.println(WiFi.channel());
  }
}

void setup() 
{
  Serial.begin(115200);
  wifi_init ();  
}

//---------------------------------------------------------------------------------------------------------------------------

void loop() 
{
  

}

//---------------------------------------------------------------------------------------------------------------------------
