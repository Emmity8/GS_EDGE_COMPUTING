#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// Configurações do DHT22
#define DHTPIN 4  
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Pinos dos componentes
const int ldrPin = 34;
const int ledVerde = 13;
const int ledAmarelo = 12;
const int ledVermelho = 14;
const int buzzer = 15;

// Configurações WiFi e MQTT
const char* SSID = "Wokwi-GUEST";
const char* PASSWORD = "";
const char* BROKER_MQTT = "44.223.43.74";
const int BROKER_PORT = 1883;
const char* TOPICO_PUBLISH = "/TEF/device007/attrs";
const char* ID_MQTT = "fiware_007";

WiFiClient espClient;
PubSubClient MQTT(espClient);

// Variáveis de controle
unsigned long lastSensorRead = 0;
const long sensorInterval = 5000;
unsigned long lastPauseAlert = 0;
const long pauseInterval = 1800000;

// Estado do sistema
bool sistemaAtivo = true;
int tempoTrabalho = 0;

void setup() {
  Serial.begin(115200);
  
  // Inicializar componentes
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAmarelo, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(ldrPin, INPUT);
  
  dht.begin();
  
  // Inicializar LEDs
  digitalWrite(ledVerde, LOW);
  digitalWrite(ledAmarelo, LOW);
  digitalWrite(ledVermelho, LOW);
  
  // Conectar WiFi e MQTT
  initWiFi();
  initMQTT();
  
  Serial.println("Sistema de Bem-estar no Trabalho Iniciado!");
  MQTT.publish(TOPICO_PUBLISH, "s|on");
}

void loop() {
  verificarConexoes();
  MQTT.loop();

  unsigned long agora = millis();
  
  // Leitura dos sensores a cada 5 segundos
  if (agora - lastSensorRead > sensorInterval) {
    lastSensorRead = agora;
    lerSensoresEAnalisar();
  }
  
  // Alerta de pausa a cada 30 minutos
  if (agora - lastPauseAlert > pauseInterval) {
    lastPauseAlert = agora;
    sugerirPausa();
  }
}

void initWiFi() {
  delay(10);
  Serial.println("Conectando WiFi...");
  WiFi.begin(SSID, PASSWORD);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  
  Serial.println("\nConectado! IP: " + WiFi.localIP().toString());
}

void initMQTT() {
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
}

void verificarConexoes() {
  if (WiFi.status() != WL_CONNECTED) {
    reconectarWiFi();
  }
  if (!MQTT.connected()) {
    reconectarMQTT();
  }
}

void reconectarWiFi() {
  Serial.println("Reconectando WiFi...");
  WiFi.begin(SSID, PASSWORD);
}

void reconectarMQTT() {
  Serial.println("Conectando MQTT...");
  while (!MQTT.connected()) {
    if (MQTT.connect(ID_MQTT)) {
      Serial.println("Conectado ao FIWARE!");
    } else {
      Serial.print("Falha MQTT, tentando novamente...");
      delay(2000);
    }
  }
}

void lerSensoresEAnalisar() {
  // Ler sensores
  float temperatura = dht.readTemperature();
  float umidade = dht.readHumidity();
  int valorLDR = analogRead(ldrPin);
  int luminosidade = map(valorLDR, 0, 4095, 0, 100);
  
  // Verificar leituras válidas
  if (isnan(temperatura) || isnan(umidade)) {
    Serial.println("Erro ao ler DHT22!");
    return;
  }
  
  // Exibir dados no serial
  Serial.println("=== DADOS AMBIENTAIS ===");
  Serial.print("Temperatura: "); Serial.print(temperatura); Serial.println("°C");
  Serial.print("Umidade: "); Serial.print(umidade); Serial.println("%");
  Serial.print("Luminosidade: "); Serial.print(luminosidade); Serial.println("%");
  
  // Analisar condições e acionar alertas
  analisarCondicoes(temperatura, umidade, luminosidade);
  
  // Publicar dados no FIWARE
  String payload = "t|" + String(temperatura, 1) + "|u|" + String(umidade, 1) + 
                   "|l|" + String(luminosidade) + "|s|" + (sistemaAtivo ? "on" : "off");
  
  MQTT.publish(TOPICO_PUBLISH, payload.c_str());
  Serial.println("Dados publicados: " + payload);
}

void analisarCondicoes(float temp, float umid, int lum) {
  // Resetar LEDs
  digitalWrite(ledVerde, LOW);
  digitalWrite(ledAmarelo, LOW);
  digitalWrite(ledVermelho, LOW);
  
  bool condicaoCritica = false;
  String alerta = "";
  
  // Analisar temperatura (faixa ideal: 20-26°C)
  if (temp < 18 || temp > 28) {
    condicaoCritica = true;
    alerta += "Temperatura extrema! ";
    tone(buzzer, 1000, 500);
  } else if (temp < 20 || temp > 26) {
    alerta += "Temperatura fora do ideal. ";
    digitalWrite(ledAmarelo, HIGH);
  }
  
  // Analisar umidade (faixa ideal: 40-60%)
  if (umid < 30 || umid > 70) {
    condicaoCritica = true;
    alerta += "Umidade extrema! ";
    tone(buzzer, 1200, 500);
  } else if (umid < 40 || umid > 60) {
    alerta += "Umidade fora do ideal. ";
    digitalWrite(ledAmarelo, HIGH);
  }
  
  // Analisar luminosidade (ideal: 40-80%)
  if (lum < 20) {
    condicaoCritica = true;
    alerta += "Ambiente muito escuro! ";
    tone(buzzer, 800, 500);
  } else if (lum > 90) {
    alerta += "Luz excessiva! ";
    digitalWrite(ledAmarelo, HIGH);
  }
  
  // Acionar LEDs conforme condições
  if (condicaoCritica) {
    digitalWrite(ledVermelho, HIGH);
    Serial.println("ALERTA: " + alerta);
  } else if (alerta == "") {
    digitalWrite(ledVerde, HIGH);
    Serial.println("Condições ideais! ✅");
  } else {
    Serial.println("Aviso: " + alerta);
  }
}

void sugerirPausa() {
  Serial.println("💡 SUGESTÃO: Hora de uma pausa! Levante, alongue-se e descanse os olhos.");
  tone(buzzer, 1500, 1000);
  
  // Piscar LED amarelo como lembrete
  for (int i = 0; i < 6; i++) {
    digitalWrite(ledAmarelo, HIGH);
    delay(500);
    digitalWrite(ledAmarelo, LOW);
    delay(500);
  }
}