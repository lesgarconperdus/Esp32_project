#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <PubSubClient.h>  

#define WIFI_AP "Wokwi-GUEST"  
#define WIFI_PASS ""

#define PIR_PIN 14
#define LED_PIN 13
#define DHT_PIN 15
#define DHT_TYPE DHT22

// Informations du serveur ThingsBoard
#define TB_SERVER "thingsboard.cloud"
#define TOKEN "PQvY9U0pxhytrDqUiXa9"  

// Initialisation des objets DHT, OLED et MQTT
DHT dht(DHT_PIN, DHT_TYPE);
Adafruit_SSD1306 display(128, 64, &Wire);
WiFiClient espClient;
PubSubClient client(espClient);

void connectToWiFi() {
  WiFi.begin(WIFI_AP, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnecté au WiFi");
}

void connectToThingsBoard() {
  client.setServer(TB_SERVER, 1883);
  while (!client.connected()) {
    Serial.print("Connexion à ThingsBoard...");
    if (client.connect("ESP32_Device", TOKEN, NULL)) {
      Serial.println("Connecté à ThingsBoard");
    } else {
      Serial.print("Échec de connexion, rc=");
      Serial.print(client.state());
      Serial.println(" nouvelle tentative dans 3 secondes");
      delay(3000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  dht.begin();

  // Ecran OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Échec de l'initialisation de l'écran OLED"));
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.display();

  // WiFi et ThingsBoard
  connectToWiFi();
  connectToThingsBoard();
}

void loop() {

  if (!client.connected()) {
    connectToThingsBoard();
  }
  client.loop();

  // Capteur DHT22
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Vérification capteur DHT22
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Erreur de lecture du capteur DHT22 !");
    displayMessage("Erreur capteur");
  } else {
    Serial.printf("Température: %.2f °C, Humidité: %.2f %%\n", temperature, humidity);
    if (temperature >= 40) {
      displayMessage("J'ai chaud !!");
    } else {
      displayMessage("Tout va bien");
    }

    // Envoi des données à ThingsBoard
    String payload = "{\"temperature\":" + String(temperature) + ", \"humidity\":" + String(humidity) + "}";
    client.publish("v1/devices/me/telemetry", payload.c_str());
    Serial.println("Données envoyées à ThingsBoard : " + payload);
  }

  // Détection de mouvement avec le capteur PIR
  if (digitalRead(PIR_PIN) == HIGH) {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("Mouvement détecté !");
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  delay(2000);
}

void displayMessage(const char *message) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print(message);
  display.display();
}
