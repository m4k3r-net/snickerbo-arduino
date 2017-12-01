#include <SPI.h>
#include <Ethernet.h>
#include <DHT.h>
#include <stdlib.h>

#define DHTPIN1 2
#define DHTPIN2 3
#define DHTTYPE DHT11

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char server[] = "xxxxx.herokuapp.com";
static char tempstr1[15];
static char humstr1[15];
//static char tempstr2[15];
//static char humstr2[15];

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192,168,1,11);
EthernetClient client;
unsigned long postDHTTimer;
int laserPin = 7;
boolean laserIsBlocked;
DHT dht1(DHTPIN1, DHTTYPE);
//DHT dht2(DHTPIN2, DHTTYPE);

void setup() {
  pinMode(laserPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  
  if(millis()-postDHTTimer >= 10000UL) {
    postDHTTimer = millis();
    postDHTData();
  }

  if( digitalRead(laserPin) == LOW) {
    if( !laserIsBlocked ) {
      laserIsBlocked = true;
      Serial.write('s');
      delay(100);
      Serial.write('25511898');
      delay(100);
      Serial.write('CATSCAN!');
      postLaserTriggered();
      delay(800);
    }    
  } else {
    laserIsBlocked = false;
  }
  
}

void postDHTData() {
    
    dtostrf(dht1.readTemperature(),5, 1, tempstr1);
    dtostrf(dht1.readHumidity(),5, 1, humstr1);
    String PostData1="macaddr=testaddr";
    PostData1=PostData1+"&sensor=DHT11_1";
    PostData1=PostData1+"&temperature=";
    PostData1=PostData1+tempstr1;
    PostData1=PostData1+"&humidity=";
    PostData1=PostData1+humstr1;
    httpPostClient(PostData1);

    //dtostrf(dht2.readTemperature(),5, 1, tempstr2);
    //dtostrf(dht2.readHumidity(),5, 1, humstr2);
    //String PostData2="macaddr=testaddr";
    //PostData2=PostData2+"&sensor=DHT11_2";
    //PostData2=PostData2+"&temperature=";
    //PostData2=PostData2+tempstr2;
    //PostData2=PostData2+"&humidity=";
    //PostData2=PostData2+humstr2;
    //httpPostClient(PostData2);
}

void postLaserTriggered() {
    String PostData="macaddr=testaddr";
    PostData=PostData+"&sensor=laser_1";
    httpPostClient(PostData);
}

void httpPostClient(String PostData) {
// start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
    // give the Ethernet shield a second to initialize:
    delay(500);
  }
  
  if (client.connect(server, 80)) {
    
    client.println("POST /postdata HTTP/1.1");
    client.println("Host:  xxxxx.herokuapp.com");
    client.println("User-Agent: Arduino/1.0");
    client.println("Connection: close");
    client.println("Content-Type: application/x-www-form-urlencoded;");
    client.print("Content-Length: ");
    client.println(PostData.length());
    client.println();
    client.println(PostData);
  } 
  else {
    Serial.println("Client not connected - could not send data.");
  }
  
  // if there are incoming bytes available 
  // from the server, read them and print them:
  //  if (client.available()) {
  //    char c = client.read();
  //    Serial.print(c);
  //  }
  
  client.flush();
  client.stop();
  
  //delay(1000); // 30 min = 1800000 ms
    
  // if the server's disconnected, stop the client:
  //if (!client.connected()) {
  //  client.stop();
  //  // do nothing forevermore:
  //  while(true);
  //}
}
