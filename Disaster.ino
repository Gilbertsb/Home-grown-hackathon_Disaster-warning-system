#include <DHT.h>  
#include <ESP8266WiFi.h>                   // Including libraries
#include <ThingSpeak.h>;


String apiKey = "J70BANOZRWHCX40L";     // Write API key from ThingSpeak 
const char* WapiKey = "J70BANOZRWHCX40L";     //  Write API key from ThingSpeak
const char* RapiKey = "OVZMZLHIEACIWED9";     //  Read API key from ThingSpeak
 
const char *ssid =  "Mr_Gilly";     // ssid and wpa2 key
const char *pass =  "Gilly@Huawei12";
const char* server = "api.thingspeak.com";


unsigned long myChannelNumber = 1726362; // Geting mu channel number

int SensA=A0;
int SensB=16;
int SensC=5;
int buzzel=14;


 
#define DHTPIN 4          //pin where the dht11 is connected
 
DHT dht(DHTPIN, DHT11);
WiFiClient client;
 
void setup() 
{      
       
       Serial.begin(9600);  //Initiating Serial moniator
       delay(10);
       dht.begin();         // Inititiaing DHT 
      
 
       Serial.println("Connecting to ");
       Serial.println(ssid);            //Displaying is we are connected to WIFI
 
 
       WiFi.begin(ssid, pass);     //Getting connected to WiFi
       ThingSpeak.begin(client);    // Initiating Thing speak
 
      while (WiFi.status() != WL_CONNECTED)  // Checking if we are connected
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected");  // If connection was successful 
      pinMode(SensA, INPUT);
      pinMode(SensB,INPUT);
      pinMode(SensC,INPUT);
      pinMode(buzzel,OUTPUT);
      
 
}
 
void loop() 
{
  
      float h = dht.readHumidity();     //Getiing Humididty and Temperature from DHT sensor
      float t = dht.readTemperature();
     int Sens1=analogRead(SensA);
     int Sens2=digitalRead(SensB);
     int Sens3=digitalRead(SensC);
     //tone(buzzel, 6000,250);
     
    
      
              if (isnan(h) || isnan(t))    // Checking if we Humidity and Temperature come well from DHT
                 {
                     Serial.println("Failed to read from DHT sensor!");
                      return;
                 }
              if(Sens3==0){        //Putting on the buzzel
                  
                   tone(buzzel, 1000,500);   // Opening relay that openes Fan
                 }
               else{
                     tone(buzzel, 0,0); //Closing relathat close the Fan
                   }

                 

               

               //Sending Data to cloud
                   
                if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
                      {  
                            
                             String postStr = apiKey;
                             postStr +="&field1=";              // connecting to Fieleds
                             postStr += String(t);
                             postStr +="&field2=";
                             postStr += String(h);
                             postStr +="&field3=";              // connecting to Fieleds
                             postStr += String(Sens1);
                             postStr +="&field4=";
                             postStr += String(Sens2);
                             postStr +="&field5=";
                             postStr += String(Sens3);
                             postStr += "\r\n\r\n";
                             ThingSpeak.setField(1, t);      // setting Values to fields
                             ThingSpeak.setField(2, h);      // setting Values to fields
                             ThingSpeak.setField(3, Sens1);      // setting Values to fields
                             ThingSpeak.setField(4, Sens2);      // setting Values to fields
                             ThingSpeak.setField(5, Sens3);      // setting Values to fields
                             int x = ThingSpeak.writeFields(myChannelNumber, WapiKey);
                             long Temperature = ThingSpeak.readLongField(myChannelNumber, 1, RapiKey);    //Update in ThingSpeak
                             long Humidity = ThingSpeak.readLongField(myChannelNumber, 2, RapiKey); 
                             long Sens1 = ThingSpeak.readLongField(myChannelNumber, 3, RapiKey);    //Update in ThingSpeak
                             long Sens2 = ThingSpeak.readLongField(myChannelNumber, 4, RapiKey);  
                             long Sens3 = ThingSpeak.readLongField(myChannelNumber, 5, RapiKey);      
                             
                             
                       
                             
 
                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");                      //Checking if connection to cloud(Thingspeak) was successful
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);

 
                             Serial.print("Temperature: ");
                             Serial.print(t);                          //pring on Serial monitor 
                             Serial.print("Humidity: ");
                             Serial.print(h);
                             Serial.print("Sensor1: ");
                             Serial.print(Sens1);                          //pring on Serial monitor 
                             Serial.print("Sensor 2: ");
                             Serial.print(Sens2);
                             Serial.print("Sensor 3: ");
                             Serial.print(Sens3);
                             Serial.println(" Send to Thingspeak.");
                        }
          client.stop();                                 //stopnt the connection to the cloud
 
          Serial.println("Waiting...");

                 
 
                        

  delay(1000);
  
}
