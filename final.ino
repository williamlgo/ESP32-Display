#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <PubSubClient.h>

// WiFi
const char *ssid = "CT94L"; // Enter your WiFi name
const char *password = "WZFCM95KBJRSZQQJ";  // Enter WiFi password

// MQTT Broker
const char *mqtt_broker = "192.168.1.8";
const char *topic = "esp32/test";
const char *mqtt_username = "emqx";
const char *mqtt_password = "123456789";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

//LED pin on GPIO2
int ledPin = 2;

//display stuff
int totalColumns = 16;
int totalRows = 2;

LiquidCrystal_I2C lcd(0x27, totalColumns, totalRows);

void setup() {
  lcd.init();
  lcd.backlight();

  pinMode(ledPin, OUTPUT);
  
  // Set software serial baud to 115200;
  Serial.begin(115200);
  // connecting to a WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  //connecting to a mqtt broker
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  while (!client.connected()) {
    String client_id = "esp32-client-";
    client_id += String(WiFi.macAddress());
    Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Public emqx mqtt broker connected");
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
  // publish and subscribe
  client.publish(topic, "Please send a message:");
  client.subscribe(topic);
  lcd.setCursor(0, 0);
  lcd.print("Send msg or 1/2");
  lcd.setCursor(0, 1);
  lcd.print("to on/off LED: ");
}

void callback(char *topic, byte *payload, unsigned int length) {
  lcd.setCursor(0, 0);
  lcd.print("New Message!    ");
  lcd.setCursor(0, 1);
  lcd.print("                ");
  delay(500);
  lcd.setCursor(0, 0);
  lcd.print("                ");
  delay(500);
  lcd.setCursor(0, 0);
  lcd.print("New Message!");
  delay(500);
  lcd.setCursor(0, 0);
  lcd.print("                ");
  delay(500);
  lcd.setCursor(0, 0);
  lcd.print("New Message!");
  delay(500);
  lcd.setCursor(0, 0);
  lcd.print("                ");
  delay(500);
  lcd.setCursor(0, 0);
  lcd.print("New Message!");
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char) payload[i]);
  }
  Serial.println();
  Serial.println("-----------------------");
  if (length <= 1) {
    if((char)payload[0] == '1'){
      digitalWrite(ledPin, HIGH);
      Serial.println("LED turned ON");
      lcd.setCursor(0, 0);
      lcd.print("LED turned ON   ");
      lcd.setCursor(0, 1);
      lcd.print("                ");
    }
    if((char)payload[0] == '2'){
      digitalWrite(ledPin, LOW);
      Serial.println("LED turned OFF");
      lcd.setCursor(0, 0);
      lcd.print("LED turned OFF  ");
      lcd.setCursor(0, 1);
      lcd.print("                ");
    }
  }
  for (int position = 0; position < length; position++) {
    lcd.setCursor(position, 1);
    lcd.print((char)payload[position]);
    delay(250);
  }
  delay(3000);
  lcd.setCursor(0, 0);
  lcd.print("Send msg or 1/2");
  lcd.setCursor(0, 1);
  lcd.print("to on/off LED: ");
}

void loop() {
  client.loop();
}