// Cliente MQTT para ESP32 que publica hacia el broker de la Raspberry Pi.
// Libreria requerida (Gestor de bibliotecas de Arduino IDE): PubSubClient de Nick O'Leary.
// Placa: ESP32 Dev Module.

#include <WiFi.h>
#include <PubSubClient.h>

// ---- Configuracion (cambiar por los datos reales) ----
const char* WIFI_SSID     = "NOMBRE_DE_LA_RED";
const char* WIFI_PASSWORD = "CONTRASENA_DE_LA_RED";
const char* MQTT_BROKER   = "192.168.0.100";  // IP de la Raspberry Pi
const int   MQTT_PORT     = 1883;
const char* CLIENT_ID     = "esp32-cc3108";
const char* TOPIC_PUB     = "esp32/temperatura";
const char* TOPIC_SUB     = "esp32/comando";

WiFiClient espClient;
PubSubClient mqtt(espClient);
unsigned long ultimoEnvio = 0;

void alRecibirMensaje(char* topic, byte* payload, unsigned int length) {
  String mensaje;
  for (unsigned int i = 0; i < length; i++) mensaje += (char)payload[i];
  Serial.print("Mensaje en ");
  Serial.print(topic);
  Serial.print(": ");
  Serial.println(mensaje);
}

void conectarWifi() {
  Serial.print("Conectando a WiFi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\nWiFi conectado. IP: ");
  Serial.println(WiFi.localIP());
}

void conectarMqtt() {
  while (!mqtt.connected()) {
    Serial.print("Conectando al broker MQTT...");
    if (mqtt.connect(CLIENT_ID)) {
      Serial.println(" conectado");
      mqtt.subscribe(TOPIC_SUB);
    } else {
      Serial.print(" fallo, rc=");
      Serial.print(mqtt.state());
      Serial.println(". Reintentando en 3 s");
      delay(3000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  conectarWifi();
  mqtt.setServer(MQTT_BROKER, MQTT_PORT);
  mqtt.setCallback(alRecibirMensaje);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) conectarWifi();
  if (!mqtt.connected()) conectarMqtt();
  mqtt.loop();

  if (millis() - ultimoEnvio > 5000) {
    ultimoEnvio = millis();
    // Valor simulado; reemplazar por la lectura real del sensor.
    float temperatura = 20.0 + (esp_random() % 1000) / 100.0;
    char buffer[16];
    dtostrf(temperatura, 4, 2, buffer);
    mqtt.publish(TOPIC_PUB, buffer);
    Serial.print("Publicado ");
    Serial.print(TOPIC_PUB);
    Serial.print(": ");
    Serial.println(buffer);
  }
}
