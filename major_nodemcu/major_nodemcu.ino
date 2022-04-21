#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>   
#include <ArduinoJson.h>

const char* ssid = "____WIFI SSID_____";
const char* password = "_____PASSWORD______";

#define BOTtoken "________BOT_TOKEN__________" 
#define CHAT_ID "___TELEGRAM_CHAT_ID_______"

#ifdef ESP8266
  X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

#define echoPin 14 // attach pin D5 Arduino to pin Echo of HC-SR04
#define trigPin 12 //attach pin D6 Arduino to pin Trig of HC-SR04
unsigned long lastTimeBotRan;
int botRequestDelay = 1000;
long duration;
int distance;


void handleNewMessages(String text) {
  Serial.println("handleNewMessages");
  bot.sendMessage(CHAT_ID, text, "");
}

void setup() {
  Serial.begin(9600);

  #ifdef ESP8266
    configTime(0, 0, "pool.ntp.org");   
    client.setTrustAnchors(&cert);
  #endif

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  
  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  #ifdef ESP32
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  #endif
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  bot.sendMessage(CHAT_ID, "Bot Started", "");
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  if (distance > 10){
    bot.sendMessage(CHAT_ID, "door opened", "");
    }
}
