#include <SPI.h>
#include <Ethernet.h>
#include <DHT.h>
#include <stdlib.h>

#define DHTPIN 2
#define DHTTYPE DHT11

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char server[] = "c2-systems.com";
static char tempstr[15];
static char humstr[15];

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192,168,1,11);
EthernetClient client;
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
}

void loop()
{

  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  
  if (client.connect(server, 80)) {
    
    dtostrf(dht.readTemperature(),5, 1, tempstr);
    dtostrf(dht.readHumidity(),5, 1, humstr);
    
    String PostData="macaddr=testaddr";
    PostData=PostData+"&temperature=";
    PostData=PostData+tempstr;
    PostData=PostData+"&humidity=";
    PostData=PostData+humstr;
    Serial.println(PostData);

    client.println("POST /dhtlog/postdata.php HTTP/1.1");
    client.println("Host:  c2-systems.com");
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
  
  delay(5000);
  client.flush();
  client.stop();
  
  delay(1800000); // 30 min = 1800000 ms
    
  // if the server's disconnected, stop the client:
  //if (!client.connected()) {
  //  client.stop();
  //  // do nothing forevermore:
  //  while(true);
  //}
}
