/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#include <ESP8266WiFi.h>

const char* ssid     = "no internet";
const char* password = "4953167854";

const char* host = "api.openweathermap.org";

void setup() {
  Serial.begin(115200);
  delay(10);
  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  delay(3000);

  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request
  String url = "/data/2.5/weather?id=703448&APPID=e58526915e4f4def278d4f30b638b7c5";
  delay(500);
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  delay(500);
  // Read all the lines of the reply from server and print them to Serial
  int i=0;
  while(client.available()){
    String line = client.readStringUntil('\r');
    if (i == 11) {
        if (line[1] == '{') {
          char* weather_line = strstr(line.c_str(),"main");
          weather_line = &weather_line[7];
       //   Serial.println(weather_line);
          char main_weather[20];
          int j = 0;
          delay(100);
          while (j<20 && weather_line[j] != '\"') {
            main_weather[j] = weather_line[j];
            j++;
          }
          delay(100);
          Serial.println(main_weather);
          weather_line = strstr(weather_line,"temp");
          weather_line = &weather_line[6];
        //  Serial.println(weather_line);
          char tmp[10];
          j = 0;
          delay(100);
          while (j<10 && weather_line[j] != ',') {
            tmp[j] = weather_line[j];
            j++;
          }
          int temp = atoi(tmp);
          Serial.println(temp);
          delay(100);
      }
      else i--;
    }
    i++;
  /*
    if (i == 11) {
      Serial.println(i);
      Serial.println("I come");
      const char* new_line = line.c_str();
      Serial.println(new_line);
      Serial.println(new_line[0]);
      Serial.println(new_line[1]);
      if (new_line[0] != '{') {
        Serial.println("no come");
        continue; 
      }
      Serial.println(new_line);
      delay(500);
    //  char* weather_line = strstr(new_line,"main");
    //  Serial.println(weather_line);
    }
    i++;
*/
  }
  
  Serial.println();
  Serial.println("closing connection");
}

