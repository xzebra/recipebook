#include <SD.h>

char* db_folder = "/db/";
const int DB_FOLDER_LEN = 4;
const int KEY_LEN = 8;

void databaseInit() {
    // Check dir for db files
    if (!SD.exists(db_folder)) {
        Serial.println("Dir for db files does not exist, creating...");
        SD.mkdir(db_folder);
    }
}

// Requires a <dest_key> array of length len(db_folder) + KEY_LEN + ".dat"
void generateID(char dest_key[]) {
    strcpy(dest_key, db_folder);
    strcat(dest_key, "aaaaaaaa");
    strcat(dest_key, ".dat");

    bool file_exists = true;
    while(file_exists) {
        for(int i = DB_FOLDER_LEN; i < KEY_LEN+DB_FOLDER_LEN; ++i) {
            char random_char = '0';
            switch(random(0,2)) { // choose char
                case 0: // numbers
                    random_char += random(0, 10);
                    break;
                case 1: // letters
                    random_char = 'A' + random(0, 26);
                    break;
            }
            dest_key[i] = random_char;
        }

        file_exists = SD.exists(dest_key);
    }
}

void storeName(File &db, const char name[]) {
    db.write(name, MAX_NAME_LEN);
}

void readName(File &db, char name[]) {
    db.readBytes(name, MAX_NAME_LEN);
}

void storePeople(File &db, const int people) {
    db.write((char*)&people, sizeof(people)/sizeof(char));
}

int readPeople(File &db) {
    int people = 0;
    db.readBytes((char*)&people, sizeof(people)/sizeof(char));
    return people;
}

void storeIngredient(File &db, const char ingredient[]) {
    db.write(ingredient, MAX_INGREDIENT_LEN);
}

void storeStep(File &db, char step[]) {
    db.write(step, MAX_STEP_LEN);
}
