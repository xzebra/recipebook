#include <SD.h>

const int BUFFER_LEN = 1024;

const int NO_ERR = 0;
const int ERR_FORBIDDEN = 1;
const int ERR_NOTFOUND = 2;
const int ERR_UNEXPECTED = 3;

int sendFromSDCard(WiFiClient &client, String path) {
    path = appendIndexIfRoot(path);
    if (path.length() == 0) {
        return ERR_UNEXPECTED;
    }
    String dataType = parseDataType(path);
    if (path.indexOf("..") != -1) {
        return ERR_FORBIDDEN;
    }

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

void handleClient(WiFiClient &client) {
    String requestURI = getURI(client.readStringUntil('\r'), 1);
    Serial.println(requestURI);
    int err = sendFromSDCard(client, requestURI);
    client.flush();
    switch (err) {
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