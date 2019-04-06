#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <SD.h>
#include <SPI.h>

const int chipSelect = 4;

const int MAX_NAME_LEN = 40;
const int MAX_INGREDIENTS = 25;
const int MAX_INGREDIENT_LEN = 40;
const int MAX_STEPS = 20;
const int MAX_STEP_LEN = 140;

/*
enum Section {arroz, ensalada, pasta, verdura, sopa, legumbres, carne, pescado, postre};

struct recipe_t {
    char name[MAX_NAME_LEN];
    int people;
    char ingredients[MAX_INGREDIENTS][MAX_INGREDIENT_LEN];
    char steps[MAX_STEPS][MAX_STEP_LEN];
};
*/

WiFiServer server(80);
const int led = LED_BUILTIN;

void setup(void) {
    pinMode(led, OUTPUT);
    digitalWrite(led, 0);
    delay(1);

    Serial.begin(9600);

    if (!SD.begin(chipSelect)) {
        Serial.println("Card failed, or not present");
        while(1);
    }

    // get wifi login details from config.ini in the SD card
    String ssid, password;
    if(!parseConfig(ssid, password)) {
        Serial.println("Failed to read config.ini");
        while(1);
    }
    WiFi.mode(WIFI_STA);
    Serial.println("");
    WiFi.begin(ssid.c_str(), password.c_str());

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    
    Serial.print("Connected to ");
    Serial.println(ssid);

    databaseInit();
    randomSeed(analogRead(A0));

    //server.on("/add", HTTP_POST, handleAddRecipe);
    //server.onNotFound(handleClient);
    server.begin();
    Serial.println("HTTP server started");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void loop(void) {
    //server.handleClient();
    WiFiClient client = server.available();
    if (!client) return;

    handleRequest(client);
}
