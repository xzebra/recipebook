#include <SD.h>

const int BUFFER_LEN = 1024;

const int NO_ERR = 0;
const int ERR_FORBIDDEN = 1;
const int ERR_NOTFOUND = 2;
const int ERR_UNEXPECTED = 3;

const int MAX_REQUEST_LEN = 4000;
const int MAX_GARBAGE_LEN = 5;

int sendFromSDCard(WiFiClient &client, String path) {
    path = appendIndexIfRoot(path);
    if (path.length() == 0) {
        return ERR_UNEXPECTED;
    }
    String dataType = parseDataType(path);
    

    File dataFile = SD.open(String("public") + path);
    // check if the file is available
    if (dataFile) {
        client.println("HTTP/1.1 200 Success");
        client.print("Content-Type: ");
        client.println(dataType);
        client.println("");
        long long int f_pos = 0;
        while (dataFile.available()) {
            // BUFFER_LEN+1 to have a constant null terminated
            char buffer[BUFFER_LEN + 1] = {0};
            int read = dataFile.read(buffer, BUFFER_LEN);
            client.print(buffer);
            f_pos += read;
            if (read <= 0 || !dataFile.seek(f_pos)) {
                // data seek failed
                break;
            }
        }
        dataFile.close();
    } else {  // if the file isn't open, return an error
        Serial.print("File ");
        Serial.print(path);
        Serial.println(" not found");
        return ERR_NOTFOUND;
    }

    return NO_ERR;
}

void handleForbidden(WiFiClient &client) {
    client.println("HTTP/1.1 403 Forbidden");
    client.println("Content-Type: text/html");
    client.println("");
}

void handleNotFound(WiFiClient &client) {
    client.println("HTTP/1.1 404 Not Found");
    client.println("Content-Type: text/html");
    client.println("");
}

int handlePostRequest(WiFiClient &client, String &uri) {
    // TODO: split the function for other POST utilities
    Serial.println("Handling add recipe");
    File db_file = SD.open(String(db_folder)+"0.dat", FILE_WRITE);

    while(client.available())
        if(client.readStringUntil('\r') == "\n")
            break;
    
    if(db_file) {
        Serial.println("File created");
        char garbage[MAX_GARBAGE_LEN];
        client.readBytesUntil('=', garbage, MAX_GARBAGE_LEN);
        
        char name_atrib[MAX_NAME_LEN];
        client.readBytesUntil('&', name_atrib, MAX_NAME_LEN);
        storeName(db_file, name_atrib);
        Serial.println(name_atrib);

        int people_atrib = client.parseInt();
        storePeople(db_file, people_atrib);
        Serial.println(people_atrib);

        char ingredients[MAX_INGREDIENTS][MAX_INGREDIENT_LEN];
        
        
        char steps[MAX_STEPS][MAX_STEP_LEN];
        
        db_file.close();
    }
    return sendFromSDCard(client, uri);
}

void handleRequest(WiFiClient &client) {
    String request = client.readStringUntil('\r');
    String requestType = split(request, 0, ' ');
    String requestURI = split(request, 1, ' ');

    int error = NO_ERR;
    if (requestURI.indexOf("..") != -1)
        error = ERR_FORBIDDEN;
    else if(requestType == "GET")
        error = sendFromSDCard(client, requestURI);
    else if(requestType == "POST")
        error = handlePostRequest(client, requestURI);
    
    switch (error) {
        case ERR_FORBIDDEN:
            handleForbidden(client);
            break;
        case ERR_NOTFOUND:
            handleNotFound(client);
            break;
    }
}

bool parseConfig(String &ssid, String &password) {
    bool open = false;
    File config = SD.open("config.ini");
    if (config) {
        open = true;
        config.readStringUntil('='); // Ignore SSID title
        ssid = removeLastChar(config.readStringUntil('\n'));
        config.readStringUntil('='); // Ignore password title
        password = removeLastChar(config.readStringUntil('\n'));
        config.close();
    } else {  // if the file isn't open, return an error
        Serial.println("config.ini not found");
    }
    return open;
}
