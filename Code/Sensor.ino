#define TRIGGER_PIN  5
#define ECHO_PIN     4
void Line_Notify(String message) ;
#include <ESP8266WiFi.h>
#include <WiFiClientSecureAxTLS.h>
// Config connect WiFi
#define WIFI_SSID "Nun"
#define WIFI_PASSWORD "12345678"
// Line config
#define LINE_TOKEN "gN4SVAxdxyQ2Xy7vASDEhSj2uN3FrsvwBvxO1pEfNex"
String message = "ถังขยะเต็มเเล้วครับท่านกรุณาเอาขยะไปทิ้งด้วยครับ";
void setup() {
  Serial.begin (9600);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  WiFi.mode(WIFI_STA);
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // ตัวเซนเซอร์//
  digitalWrite(LED_BUILTIN, LOW); 
  long duration, distance;
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration/2) / 29.1;
  
  {if(distance <= 28 && distance >= 18)
    digitalWrite(D4, HIGH);
    else
   digitalWrite(D4, LOW);}
  {if(distance <= 17 && distance >= 6)
    digitalWrite(D5, HIGH);
   else
   digitalWrite(D5, LOW);}
  {if(distance <= 5){
    digitalWrite(D6, HIGH);
    Serial.println("Enter !");
    Line_Notify(message);}
   else
    digitalWrite(D6, LOW);}
  Serial.print(distance);
  Serial.println(" cm");
  delay(100);
}
void Line_Notify(String message) {
  axTLS::WiFiClientSecure client;

  if (!client.connect("notify-api.line.me", 443)) {
    Serial.println("connection failed");
    return;   
  }

  String req = "";
  req += "POST /api/notify HTTP/1.1\r\n";
  req += "Host: notify-api.line.me\r\n";
  req += "Authorization: Bearer " + String(LINE_TOKEN) + "\r\n";
  req += "Cache-Control: no-cache\r\n";
  req += "User-Agent: ESP8266\r\n";
  req += "Connection: close\r\n";
  req += "Content-Type: application/x-www-form-urlencoded\r\n";
  req += "Content-Length: " + String(String("message=" + message).length()) + "\r\n";
  req += "\r\n";
  req += "message=" + message;
  // Serial.println(req);
  client.print(req);
    
  delay(20);

  // Serial.println("-------------");
  while(client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
    //Serial.println(line);
  }
  // Serial.println("-------------");
}
