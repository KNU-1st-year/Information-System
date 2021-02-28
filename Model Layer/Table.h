//
// Created by mark on 07.02.21.
//

#ifndef INFORMATION_SYSTEM_TABLE_H
#define INFORMATION_SYSTEM_TABLE_H

#include "../Header.h"

static const string valueOfTypeOfNote[2] = {"STRING", "DOUBLE"};

/**
 * This class represent (a part of) the model layer.
 * This class works with <sheetName>.dat and <sheetName>-prop.dat - files,
 * that have tables data and table properties.
 *
 * Template of <sheetName>-prop.dat :
 *  numberOfNotes (int)
 *  numberOfProperties (int)
 *  values (vector<TypeOfNote>)
 *
 * Template of <sheetName>.dat :
 *  note1 (TableProperties)
 *  note2 (TableProperties)
 *  note3 (TableProperties)
 *  ...
 *  note_i (TableProperties)
 */
class Table {
public:
    enum TypeOfNote {
        STRING,
        DOUBLE,
        DATE
    };

    struct Field {
        TypeOfNote type;
        char name[NAME_SIZE];
    };

    struct TableProperties {
        int numberOfNotes;
        int numberOfProperties;
        vector<Field> values;
    };

    struct Request {
        string value;
        enum State{
            LESS,
            MORE,
            NOT_MORE,
            NOT_LESS,
            EQUAL,
            INCLUDED,
            IGNORE_IT
        } state;
    };

    struct NoteValue {
        char value[DESCRIPTION_SIZE];
    };

private:
    FILE *fp;
    FILE *fpProp;

    void getProperties();

    void setProperties();

    void printHeader();

    void printNote(vector<NoteValue> noteProperties, int id);

    void openFpForReading();

    void openFpForWriting();

    void openFpPropForReading();

    void openFpPropForWriting();

    void writeNoteInFile(vector<NoteValue> noteProperties);

    void writeNoteInFile(vector<NoteValue> noteProperties, FILE *p);

    vector<NoteValue> readNextNote();

    vector<NoteValue> readNextNote(FILE *p);

    void deleteTableFile();

    void renameTempFile();

public:
    char name[NAME_SIZE];
    TableProperties properties;

    Table();

    Table(char name[]);

    void setTable(char name[]);

    void printNotes(vector<Request> requestDouble);

    void addNote(vector<NoteValue> noteProperties);

    void deleteNote(int id);

    void changeNote(vector<NoteValue> noteProperties, int id);

    static string getTypeOfNote(int index){
        return valueOfTypeOfNote[index];
    }

    /**
     * This method change table properties according to parameters.
     *
     * @param tableProperties Properties to apply
     * @param name Name of changing file
     */
    static void changeTableProperties(TableProperties tableProperties, char *name){
        string n1(name);
        string n = "tables/";
        n += n1;
        n += "-prop.dat";
        char fileName[n.size() + 1];
        strcpy(fileName, n.c_str());
        fileName[n.size()] = '\0';
        FILE *fp1 = fopen(fileName, "wb");

        fwrite(&tableProperties.numberOfNotes, INT_SIZE, 1, fp1);
        fwrite(&tableProperties.numberOfProperties, INT_SIZE, 1, fp1);
        for (int i = 0; i < tableProperties.numberOfProperties; ++i) {
            fwrite(&tableProperties.values[i], sizeof(Table::Field), 1, fp1);
        }

        fflush(fp1);
    }
};


#endif //INFORMATION_SYSTEM_TABLE_H