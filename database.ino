#include <EDB.h>
#include <SD.h>

const int MAX_INGREDIENTS = 25;
const int MAX_INGREDIENT_LEN = 30;
const int MAX_STEPS = 20;
const int MAX_STEP_LEN = 140;

struct Recipe {
    String name;
    bool done;
    int people;
    char ingredients[MAX_INGREDIENTS][MAX_INGREDIENT_LEN];
    char steps[MAX_STEPS][MAX_STEP_LEN];
};

const int MAX_DATABASE_ELEMS = 200;
const unsigned long TABLE_SIZE = sizeof(Recipe)*MAX_DATABASE_ELEMS;

enum Section {arroz, ensalada, pasta, verdura, sopa, legumbres, carne, pescado, postre};

EDB db(&writer, &reader);
File database;
char* db_name = "/db/recipes.db";

void writer(unsigned long address, byte data) {
    database.seek(address);
    database.write(data);
    database.flush();
}

byte reader(unsigned long address) {
    database.seek(address);
    byte b = database.read();
    return b;
}

void createTable() {
    Serial.print("Creating new table... ");
    // create table with starting address 0
    db.create(0, TABLE_SIZE, (unsigned int)sizeof(Recipe));
}

void openDatabase() {
    database = SD.open(db_name, FILE_WRITE);
}

void databaseInit() {
    // Check dir for db files
    if (!SD.exists("/db")) {
        Serial.println("Dir for Db files does not exist, creating...");
        SD.mkdir("/db");
    }

    if (SD.exists(db_name)) {
        // Sometimes it wont open at first attempt, espessialy after cold start
        // Let's try one more time
        openDatabase();
        if (!database) {
            openDatabase();
        }

        if (database) {
            Serial.print("Openning current table... ");
            EDB_Status result = db.open(0);
            if (result != EDB_OK) {
                Serial.println("ERROR");
                Serial.println("Did not find database in the file " + String(db_name));
                createTable();
            }
            Serial.println("DONE");
        } else {
            Serial.println("Could not open file " + String(db_name));
        }
    } else {
        openDatabase();
        createTable();
        Serial.println("DONE");
    }
}