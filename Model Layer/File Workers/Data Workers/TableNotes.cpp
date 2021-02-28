//
// Created by Mark on 26.02.2021.
//

#include "TableNotes.h"

TableNotes::TableNotes(string name, int numberOfRecords, int numberOfColumns,
                       vector<TypeOfNote> types, vector<List> names)
        : TableDataWorker(name, numberOfRecords, numberOfColumns, types, names) {}

vector<vector<string>> TableNotes::get(vector<Request> request) {
    vector<string> recordData;
    vector<vector<string>> result;

    for (int i = 0; i < numberOfRecords; ++i) {
        recordData = readNextRecord(getFp());
        if (isAppropriate(recordData, request)) {
            result.push_back(recordData);
        }
    }
}

void TableNotes::add(vector<string> recordData) {
    openForReading();
    writeNextRecord(recordData, getFp());
    numberOfRecords++;
}

void TableNotes::change(vector<string> recordData) {
    openForWriting();
    FILE *fpOfCopy = fopen(TEMP_FILE_NAME.c_str(), "w+b");
    vector<string> temporaryRecordData;

    for (int i = 0; i < numberOfRecords; ++i) {
        temporaryRecordData = readNextRecord(getFp());
        if (temporaryRecordData[0] == recordData[0]) {
            writeNextRecord(recordData, fpOfCopy);
        } else {
            writeNextRecord(temporaryRecordData, fpOfCopy);
        }
    }

    deleteTableFile();
    renameTempFile();
    openForReading();
}

void TableNotes::deleteItem(string key) {
    openForWriting();
    FILE *fpOfCopy = fopen(TEMP_FILE_NAME.c_str(), "w+b");
    vector<string> temporaryRecordData;

    for (int i = 0; i < numberOfRecords; ++i) {
        temporaryRecordData = readNextRecord(getFp());
        if (temporaryRecordData[0] != key) {
            writeNextRecord(temporaryRecordData, fpOfCopy);
        }
    }

    deleteTableFile();
    renameTempFile();
    openForReading();

    numberOfRecords--;
}

void TableNotes::deleteTableFile() {
    string oldFile = "Data/Tables/" + getName() + ".dat";
    remove(oldFile.c_str());
}

void TableNotes::renameTempFile() {
    string originalName = "Data/Tables/" + getName() + ".dat";
    rename(TEMP_FILE_NAME.c_str(), originalName.c_str());
}

vector<string> TableNotes::readNextRecord(FILE *fp) {
    List list;
    vector<string> recordData;

    for (int i = 0; i < numberOfColumns; ++i) {
        list.readFromFile(fp);
        recordData.push_back(list.get());
    }

    return recordData;
}

void TableNotes::writeNextRecord(vector<string> recordData, FILE *fp) {
    List list;

    for (int i = 0; i < numberOfColumns; ++i) {
        list.put(recordData[i]);
        list.writeInFile(fp);
    }
}

bool TableNotes::isAppropriate(vector<string> recordData, vector<Request> request) {
    bool validNote = true;

    for (int i = 0; i < numberOfColumns && validNote == true; ++i) {
        if (request[i].state != Request::IGNORE_IT) {
            if (types[i].type == TypeOfNote::DOUBLE) {
                double value = stod(recordData[i]), target = stod(request[i].value);
                switch (request[i].state) {
                    case Request::State::LESS:
                        if (value > target) {
                            validNote = false;
                        }
                        break;
                    case Request::State::MORE:
                        if (value < target) {
                            validNote = false;
                        }
                        break;
                    case Request::State::NOT_MORE:
                        if (value >= target) {
                            validNote = false;
                        }
                        break;
                    case Request::State::NOT_LESS:
                        if (value <= target) {
                            validNote = false;
                        }
                        break;
                    case Request::State::EQUAL:
                        if (value != target) {
                            validNote = false;
                        }
                        break;
                    default:
                        validNote = false;
                }
            }
            else if (types[i].type == TypeOfNote::STRING) {
                string value = recordData[i], target = request[i].value;
                switch (request[i].state) {
                    case Request::State::EQUAL:
                        if (value.compare(target) != 0) {
                            validNote = false;
                        }
                        break;
                    case Request::State::INCLUDED:
                        if (!value.find(target)) {
                            validNote = false;

                        }
                        break;
                    default:
                        validNote = false;
                }
            }
        }
    }

    return validNote;
}