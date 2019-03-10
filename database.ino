#include <SD.h>
typedef recipe_t Recipe;

char* db_folder = "/db/";

void databaseInit() {
    // Check dir for db files
    if (!SD.exists(db_folder)) {
        Serial.println("Dir for db files does not exist, creating...");
        SD.mkdir(db_folder);
    }
}

void storeName(File &db, const char name[]) {
    db.write((uint8_t*)name, MAX_NAME_LEN);
}

void storePeople(File &db, const int people) {
    db.write((uint8_t*)&people, sizeof(people)/sizeof(uint8_t));
}

void storeIngredient(File &db, char ingredient[MAX_INGREDIENT_LEN]) {
    db.write((uint8_t*)ingredient, MAX_INGREDIENT_LEN);
}

void storeSteps(File &db, char steps[][MAX_STEP_LEN]) {
    db.write((uint8_t*)steps, MAX_STEPS*MAX_STEP_LEN);
}

bool readRecipe(const String &file_name, recipe_t &recipe) {
    File db_file = SD.open(file_name+String(db_folder), FILE_WRITE);
    if(!db_file) {
        Serial.println("File doesn't exist");
        return false;
    }
    db_file.read((uint8_t*)&recipe, sizeof(Recipe)/sizeof(uint8_t));
    return true;
}
