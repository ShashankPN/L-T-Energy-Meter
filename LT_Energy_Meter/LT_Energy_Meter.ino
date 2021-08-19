#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>

#include <SoftwareSerial.h>

#define FIREBASE_HOST "lt-energy-meter-default-rtdb.firebaseio.com" //Without http:// or https:// schemes
#define FIREBASE_AUTH "auth"
#define WIFI_SSID "id"
#define WIFI_PASSWORD "pwd"

#define led D3
#define modbus D4


SoftwareSerial SSerial(D1, D2); //Rx, Tx

FirebaseData firebaseData;
FirebaseJson json;

int count;
byte byteReceived;
String value;
String path = "/Test";

void setup()
{
  pinMode(led, OUTPUT);
  pinMode(modbus, OUTPUT);
  SSerial.begin(9600);
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(led, !digitalRead(led));
    Serial.print(".");
    delay(150);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  digitalWrite(led, LOW);

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  //Set the size of WiFi rx/tx buffers in the case where we want to work with large data.
  firebaseData.setBSSLBufferSize(1024, 1024);

  //Set the size of HTTP response buffers in the case where we want to work with large data.
  firebaseData.setResponseSize(1024);

  //Set database read timeout to 1 minute (max 15 minutes)
  Firebase.setReadTimeout(firebaseData, 1000 * 60);

  //Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
  Firebase.setwriteSizeLimit(firebaseData, "tiny");

  Serial.println("------------------------------------");
  Serial.println("Started");
}


void loop()
{
  String f = (String)lineFreq();
  delay(100);

  String vr = (String)voltageRphase();
  delay(100);

  String vy = (String)voltageYphase();
  delay(100);

  String vb = (String)voltageBphase();
  delay(100);

  String value = "$" + f + "a" + vr + "a" + vy + "a" + vb + "a" + "*" ;

  json.set("String", value );
  if (Firebase.updateNode(firebaseData, path , json))
  {
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.println("TYPE: " + firebaseData.dataType());
    //No ETag available
    Serial.print("VALUE: ");
    printResult(firebaseData);
    Serial.println("------------------------------------");
    Serial.println();
    digitalWrite(led, HIGH);
    delay(200);
    digitalWrite(led, LOW);
    delay(1000);
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
    digitalWrite(led, HIGH);
    delay(1000);
    digitalWrite(led, LOW);
    delay(150);
  }
}
