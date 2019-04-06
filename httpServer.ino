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

void handleAddRecipe(WiFiClient &client) {
    Serial.println("Handling add recipe");

    char recipe_file_name[DB_FOLDER_LEN + KEY_LEN + 5];
    generateID(recipe_file_name);

    File db_file = SD.open(recipe_file_name, FILE_WRITE);

    while(client.available())
        if(client.readStringUntil('\r') == "\n")
            break;
    
    if(db_file) {
        Serial.println("File created");
        Serial.println(recipe_file_name);

        const int MAX_BUFFER_LEN = 200;
        char buffer[MAX_BUFFER_LEN+1];
        client.readBytesUntil('=', buffer, MAX_BUFFER_LEN);
        
        int end_pos = client.readBytesUntil('&', buffer, MAX_NAME_LEN);
        buffer[end_pos+1] = '\0';
        storeName(db_file, buffer);
        Serial.println(buffer);

        int people_atrib = client.parseInt();
        storePeople(db_file, people_atrib);
        Serial.println(people_atrib);

        //char ingredients[MAX_INGREDIENTS][MAX_INGREDIENT_LEN];
        
        
        //char steps[MAX_STEPS][MAX_STEP_LEN];
        
        db_file.close();
    }
}

int handleListRecipes(WiFiClient &client) {
    client.println("HTTP/1.1 200 Success");
    client.println("Content-Type: text/plain");
    client.println();

    File db = SD.open("db/");
    if(!db) return ERR_NOTFOUND;
    db = db.openNextFile();

    char buffer[MAX_NAME_LEN+1];
    while(db) {
        Serial.println(db.name());
        readName(db, buffer);
        Serial.println(buffer);
        //client.println(buffer);
        db = db.openNextFile();
    }

    return NO_ERR;
}

int handleGetRequest(WiFiClient &client, String &uri) {
    if(uri == "/list")
        return handleListRecipes(client);

    return sendFromSDCard(client, uri);
}

int handlePostRequest(WiFiClient &client, String &uri) {
    if(uri == "/add.htm") {
        handleAddRecipe(client);
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
        error = handleGetRequest(client, requestURI);
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
