#include <WiFi.h>
#include <WebSocketsServer.h>

const char* ssid = "PoleDeVinci_IFT";   //Wifi id and pswd TO CHANGE
const char* password = "*c.r4UV@VfPn_0";
WebSocketsServer webSocket = WebSocketsServer(80);

void WebSocketSetup(){
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

// Declaring the pins of the columns and rows of the button matrix
const int ROW_PINS[] = {7, 8, 1};  // CHANGE accroding to connections
const int COL_PINS[] = {20, 10, 0};  // CHANGE accroding to connections

// Declaring the button actions in a 2D array
const char* buttonMatrix[3][3] = {
  {"Up",    "Right", "A"},   // Row 0: Up, , Right, A
  {"Left",  "Select", "B"},  // Row 1: Left, Select, B
  {"Down",  nullptr, nullptr}  // Row 2: Down,
};

void ButtonSetup(){
  // Setup Rows as output and Columns as input
  for (int row = 0; row < 3; row++) {
    pinMode(ROW_PINS[row], OUTPUT);
    digitalWrite(ROW_PINS[row], HIGH); // Initialise rows at HIGH
  }
  for (int col = 0; col < 3; col++) {
    pinMode(COL_PINS[col], INPUT_PULLUP); // Use internal pull up resistors for columns
  }
}

String ButtonAction(int row, int col) {
  // Check if the row and column are within the valid range
  if (row >= 0 && row < 3 && col >= 0 && col < 3) {
    const char* action = buttonMatrix[row][col];
    if (action != nullptr) {
      return String(action);
    } else {
      return "No Action";  // In case the button doesn't map to an action
    }
  } else {
    return "Invalid";  // Return error if row/col are out of bounds
  }
}


void setup() {
  Serial.begin(115200);
  WebSocketSetup();
  ButtonSetup();
}

void loop() {
  webSocket.loop();

  // Scann button matrix
  for (int row = 0; row < 3; row++) {
    // Seting the current line to LOW 
    digitalWrite(ROW_PINS[row], LOW);
    
    for (int col = 0; col < 3; col++) {
      if (digitalRead(COL_PINS[col]) == LOW) {  //Button Press
        String btnAction = ButtonAction(row, col);
        webSocket.broadcastTXT(btnAction);
        delay(200);  // slight delay to avoid rebounce
      }
    }
    // Setting current line back to HIGH
    digitalWrite(ROW_PINS[row], HIGH);
  }
  
  // Send a message to all connected clients every 5 seconds
  //static unsigned long lastSend = 0;
  //if (millis() - lastSend >= 5000) {
  //  lastSend = millis();
    //Serial.println("Sending Ping");
  //  webSocket.broadcastTXT("Ping");
  //}
}