#include <SoftwareSerial.h>

SoftwareSerial la66(10, 11); // RX, TX (Arduino)

bool networkJoined = false;
unsigned long lastSendTime = 0;
const unsigned long sendInterval = 60000; // 60s entre envois

void setup() {
  Serial.begin(9600);
  la66.begin(9600);
  delay(2000);

  Serial.println("🔄 Reset LA66...");
  la66.println("ATZ"); // Reset module
  delay(2000);

  // Rejoindre réseau LoRaWAN (si non automatique)
  Serial.println("➡️ Envoi AT+JOIN...");
  la66.println("AT+JOIN");
}

void loop() {
  // Lire réponse du module
  while (la66.available()) {
    String response = la66.readStringUntil('\n');
    Serial.print("← "); Serial.println(response);

    if (response.indexOf("JOINED") >= 0) {
      Serial.println("✅ Join réussi.");
      networkJoined = true;
    }

    if (response.indexOf("+SENDB: Done") >= 0) {
      Serial.println("✅ Payload envoyé avec succès.");
    }
  }

  // Si connecté au réseau, envoi périodique
  if (networkJoined && millis() - lastSendTime > sendInterval) {
    Serial.println("📡 Envoi Hello World vers Akenza...");
    // Hello World = 11 octets
    la66.println("AT+SENDB=0,1,11,48656C6C6F20576F726C64");
    lastSendTime = millis();
  }

  // Permet d’envoyer manuellement des commandes depuis le Moniteur Série
  while (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    la66.println(cmd);
  }
}
