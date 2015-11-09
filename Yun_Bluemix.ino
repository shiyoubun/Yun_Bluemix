#include <IRremote.h>
#include <YunClient.h>
#include <PubSubClient.h>
#include <Console.h>
IRsend irsend;
YunClient yunClient;

const int irReceiver = 2;
const int irLed = 13;
const int ledPin = 3;

String MS_Proxy = "63cts6.messaging.internetofthings.ibmcloud.com";
String MQTT_Clientname = "d:63cts6:vm1:shawnvm1";
String MQTT_Topic = "iot-2/evt/status/fmt/json";
String AUTHMethod = "use-token-auth";
String AUTHToken = "+Y!cIDH2jYJ44(?@Uv";

PubSubClient client((char*)MS_Proxy.c_str(), 1883, callback, yunClient);

void shining() {
  // MQTT Publish
   String json = buildJson();
   char jsonStr[80];
   json.toCharArray(jsonStr,80);
   Serial.print("attempt to send ");
   Serial.print(jsonStr);
   Serial.print(" to ");
   Serial.println((char*)MQTT_Topic.c_str());
   boolean pubresult = client.publish((char*)MQTT_Topic.c_str(),jsonStr);
   if(pubresult) 
    Serial.println("Successfully sent");
   else
    Serial.println("Unsuccessfully sent");
  //IR
  int i;
  for (i = 0; i < 5; i++) {
    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(ledPin, LOW);
    delay(100);
  }
}
void setup() {
  // initialize serial communication:
  Bridge.begin();
  Console.begin(); 
  Serial.begin(9600);
  
  pinMode(irReceiver, INPUT);
  pinMode(irLed, OUTPUT);
  pinMode(ledPin, OUTPUT);
  //tone(irLed, frequency);
  digitalWrite(ledPin, LOW);
  // Internet
  if(client.connect((char*)MQTT_Clientname.c_str(),(char*)AUTHMethod.c_str(),(char*)AUTHToken.c_str())){
    Serial.println("Connected successfully");
  }else{
    Serial.println("Connect failed, reset and try again...");
    abort();
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  irsend.sendNEC(0xF740BF, 32);
  int ir_status = digitalRead(irReceiver);
  Serial.print("IR: ");
  Serial.println(ir_status);
  if (ir_status == 1) {
    shining();
  }
  client.loop();
}
String buildJson(){
  String data = String("{\"d\":{\"myName\":\"Arduino Yun\",\"myStatus\":\"Invaded!\"}}");
  return data;
}
// Put code inside calback if subscribe needed
void callback(char* topic, byte* payload, unsigned int length){}
