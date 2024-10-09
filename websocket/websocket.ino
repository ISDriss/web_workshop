#include <WiFi.h>
#include <WebSocketsServer.h>

const char* ssid = "PoleDeVinci_IFT";
const char* password = "*c.r4UV@VfPn_0";

WebSocketsServer webSocket = WebSocketsServer(80);

void onWebSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  // Handle WebSocket events here
  if (type == WStype_TEXT) {
      // Message reçu
      String message = (char *)payload;
      // Traitement du message
      Serial.println("Message reçu : " + message);

      // Send a response back to the client
      webSocket.sendTXT(num, "Message reçu et traité !");
    }
  
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println("Starting WebSocket server...");
  webSocket.begin();
  webSocket.onEvent(onWebSocketEvent);
  Serial.println("Working on IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  webSocket.loop();

  
  // Send a message to all connected clients every 5 seconds
  static unsigned long lastSend = 0;
  if (millis() - lastSend >= 5000) {
    lastSend = millis();
    Serial.println("Sending Ping");
    webSocket.broadcastTXT("Ping");
  }
}