#include <WiFi.h>
#include <WebSocketsServer.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "credentials.h"

//credentials.h
//#ifndef CREDENTIALS_H
//#define CREDENTIALS_H
//const char* ssid = "WIFI_SSID";   // Replace with your actual SSID
//const char* password = "*PASSWORD"; // Replace with your actual password
//#endif

#define PIN 9
#define NUMPIXELS 2
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void PixelSetup(){
  pixels.begin();
  Wire.begin(5, 6);
  
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE); 

  display.clearDisplay();
  display.display();
}

void DisplayMessage(String msg) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print(msg);
  display.display();
}

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
      // confirm message reception
      webSocket.sendTXT(num, "=====cmd_received=====");
      String message = (char *)payload;
      Serial.println("C> " + message);

      // Action on the message
      DisplayMessage(message);
    }
}

// Declaring the pins of the columns and rows of the button matrix
const int ROW_PINS[] = {7, 8, 1};  // CHANGE according to connections
const int COL_PINS[] = {20, 10, 0};  // CHANGE according to connections

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

void ButtonLoop(){
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
}

void setup() {
  Serial.begin(115200);
  WebSocketSetup();
  ButtonSetup();
  PixelSetup();
}

void loop() {
  webSocket.loop();
  ButtonLoop();
}