#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h> // only for esp_wifi_set_channel()

#define CHANNEL 1
#define PRINTSCANRESULTS 0
#define DELETEBEFOREPAIR 0
#define Tiempo_Transmision 100
#define Analogo              34

typedef struct Mensaje
{
  int dato1; 
  int dato2;
  int dato3;
  int dato4;
} Mensaje;

esp_now_peer_info_t slave;
Mensaje myData;

//---------------------------------------estados del envio----------------------------------------------------------

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) 
{
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print("Last Packet Sent to: "); 
  Serial.println(macStr);
  Serial.print("Last Packet Send Status: "); 
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

//----------------------------------inicia el wifi--------------------------------------------------
void wifi_init (void)
{
  //Set device in STA mode to begin with
  WiFi.mode(WIFI_STA);
  esp_wifi_set_channel(CHANNEL, WIFI_SECOND_CHAN_NONE);
  Serial.println("ESPNow/Basic/Master Example");
  // This is the mac address of the Master in Station Mode
  Serial.print("STA MAC: "); 
  Serial.println(WiFi.macAddress());
  Serial.print("STA CHANNEL "); 
  Serial.println(WiFi.channel());
  // Init ESPNow with a fallback logic
  InitESPNow();
  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
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

//----------------------------------escanea si hay esclavos conectados en la comunicacion--------------------------------------------------

void ScanForSlave() 
{
  int16_t scanResults = WiFi.scanNetworks(false, false, false, 300, CHANNEL); // Scan only on one channel
  // reset on each scan
  bool slaveFound = 0;
  memset(&slave, 0, sizeof(slave));

  Serial.println("");
  if (scanResults == 0) 
    {
      Serial.println("No WiFi devices in AP Mode found");
    } 
  else 
      {
        Serial.print("Found "); 
        Serial.print(scanResults);
        Serial.println(" devices ");
        for (int i = 0; i < scanResults; ++i) 
         {
          // Print SSID and RSSI for each device found
          String SSID = WiFi.SSID(i);
          int32_t RSSI = WiFi.RSSI(i);
          String BSSIDstr = WiFi.BSSIDstr(i);
    
          if (PRINTSCANRESULTS) 
              {
                Serial.print(i + 1);
                Serial.print(": ");
                Serial.print(SSID);
                Serial.print(" (");
                Serial.print(RSSI);
                Serial.print(")");
                Serial.println("");
              }
          delay(10);
          // Check if the current device starts with `Slave`
          if (SSID.indexOf("Slave") == 0) 
           {
            // SSID of interest
            Serial.println("Found a Slave.");
            Serial.print(i + 1);
            Serial.print(": "); 
            Serial.print(SSID); 
            Serial.print(" ["); 
            Serial.print(BSSIDstr); 
            Serial.print("]"); 
            Serial.print(" ("); 
            Serial.print(RSSI); 
            Serial.print(")"); 
            Serial.println("");
            // Get BSSID => Mac Address of the Slave
            int mac[6];
            if ( 6 == sscanf(BSSIDstr.c_str(), "%x:%x:%x:%x:%x:%x",  &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5] ) ) 
              {
                for (int ii = 0; ii < 6; ++ii ) 
                  {
                    slave.peer_addr[ii] = (uint8_t) mac[ii];
                  }
              }
    
            slave.channel = CHANNEL; // pick a channel
            slave.encrypt = 0; // no encryption    
            slaveFound = 1;
            // we are planning to have only one slave in this example;
            // Hence, break after we find one, to be a bit efficient
            break;
          }
        }
      }

  if (slaveFound) 
    {
      Serial.println("Slave Found, processing..");
    } 
  else 
    {
      Serial.println("Slave Not Found, trying again.");
    }
  // clean up ram
  WiFi.scanDelete();
}

//-------------------manejador de esclavos---------------------------------------------------------

bool manageSlave() 
{
  if (slave.channel == CHANNEL) 
  {
    if (DELETEBEFOREPAIR) 
      {
        deletePeer();
      }

    Serial.print("Slave Status: ");
    // check if the peer exists
    bool exists = esp_now_is_peer_exist(slave.peer_addr);
    if ( exists) 
        {
          // Slave already paired.
          Serial.println("Already Paired");
          return true;
        } 
     else 
     {
      // Slave not paired, attempt pair
      esp_err_t addStatus = esp_now_add_peer(&slave);
      if (addStatus == ESP_OK) 
          {
            // Pair success
            Serial.println("Pair success");
            return true;
          } 
      else if (addStatus == ESP_ERR_ESPNOW_NOT_INIT) 
          {
            // How did we get so far!!
            Serial.println("ESPNOW Not Init");
            return false;
          } 
      else if (addStatus == ESP_ERR_ESPNOW_ARG) 
          {
            Serial.println("Invalid Argument");
            return false;
          } 
      else if (addStatus == ESP_ERR_ESPNOW_FULL) 
          {
            Serial.println("Peer list full");
            return false;
          } 
      else if (addStatus == ESP_ERR_ESPNOW_NO_MEM)
          {
            Serial.println("Out of memory");
            return false;
          }
      else if (addStatus == ESP_ERR_ESPNOW_EXIST) 
          {
            Serial.println("Peer Exists");
            return true;
          } 
      else 
          {
            Serial.println("Not sure what happened");
            return false;
          }
    }
  } 
  else 
  {
    // No slave found to process
    Serial.println("No Slave found to process");
    return false;
  }
}

//-----------------------------------elimina la conexion con el esclavo-----------------------------

void deletePeer() 
{
  esp_err_t delStatus = esp_now_del_peer(slave.peer_addr);
  Serial.print("Slave Delete Status: ");
  if (delStatus == ESP_OK) 
    {
      // Delete success
      Serial.println("Success");
    } 
  else if (delStatus == ESP_ERR_ESPNOW_NOT_INIT) 
    {
      // How did we get so far!!
      Serial.println("ESPNOW Not Init");
    } 
  else if (delStatus == ESP_ERR_ESPNOW_ARG) 
    {
      Serial.println("Invalid Argument");
    } 
  else if (delStatus == ESP_ERR_ESPNOW_NOT_FOUND) 
    {
      Serial.println("Peer not found.");
    } 
  else 
    {
      Serial.println("Not sure what happened");
    }
}


//---------------------envio de datos ------------------------------------------------

void sendData() 
{
  const uint8_t *peer_addr = slave.peer_addr;
  Serial.print("Sending: "); 
  Serial.print   (myData.dato1);
  Serial.print   (myData.dato2);
  Serial.print   (myData.dato3);
  Serial.println (myData.dato4);
  esp_err_t result = esp_now_send(peer_addr, (uint8_t *) &myData, sizeof(myData)); 
  Serial.print("Send Status: ");
  if (result == ESP_OK) 
    {
      Serial.println("Success");
    } 
  else if (result == ESP_ERR_ESPNOW_NOT_INIT) 
    {
      // How did we get so far!!
      Serial.println("ESPNOW not Init.");
    } 
  else if (result == ESP_ERR_ESPNOW_ARG) 
    {
      Serial.println("Invalid Argument");
    } 
  else if (result == ESP_ERR_ESPNOW_INTERNAL) 
    {
      Serial.println("Internal Error");
    } 
  else if (result == ESP_ERR_ESPNOW_NO_MEM) 
    {
      Serial.println("ESP_ERR_ESPNOW_NO_MEM");
    } 
  else if (result == ESP_ERR_ESPNOW_NOT_FOUND) 
    {
      Serial.println("Peer not found.");
    } 
  else 
    {
      Serial.println("Not sure what happened");
    }
}

//---------------------envio de datos al esclavo ------------------------------------------------

void Transmision (void)
{ 
  myData.dato1 = analogRead (Analogo);;
  myData.dato2 = 1;
  myData.dato3 = 1;
  myData.dato4 = 1;
  ScanForSlave();
  if (slave.channel == CHANNEL) 
    { 
      bool isPaired = manageSlave();
      if (isPaired) 
        {
          // pair success or already paired
          // Send data to device
          sendData();
        }
      else 
        {
          // slave pair failed
          Serial.println("Slave pair failed!");
        }
    }
  else 
    {
      // No slave found to process
    }  
}

//-------------------------------------------Setup-----------------------------------------------------------------

void setup ()
 {
  Serial.begin(115200);
  pinMode (Analogo, INPUT);
  analogSetWidth(12);                       // 12 bits de resolucion 
  analogSetAttenuation(ADC_11db);
  analogSetPinAttenuation (Analogo,ADC_11db); 
  wifi_init();
 }

//-------------------------------------------loop-----------------------------------------------------------------

 void loop ()
 {
  Serial.println(myData.dato1);
  Transmision();
  delay(Tiempo_Transmision);
 }
