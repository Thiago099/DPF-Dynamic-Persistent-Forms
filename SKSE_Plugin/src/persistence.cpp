#pragma once
#include "model.cpp"
#include "form.cpp"
#include "serializer.cpp"
#include "form_serializer.cpp"
#include "form_record_serializer.cpp";
std::mutex callbackMutext;

void LoadCache();
void SaveCache();


static void SaveCallback(SKSE::SerializationInterface* a_intfc) {
    std::lock_guard<std::mutex> lock(callbackMutext);
    try {
        print("SAVE CAllBACK");

        if (!a_intfc->OpenRecord('ARR_', 1)) {
            print("Failed to open record for arr!");
        } else {
            auto serializer = new SaveDataSerializer(a_intfc);
            StoreAllFormRecords(serializer);
        }
        SaveCache();
    } catch (const std::exception&) {
        print("error saving");
    }
}

static void LoadCallback(SKSE::SerializationInterface* a_intfc) {
    std::lock_guard<std::mutex> lock(callbackMutext);
    try {
        print("LOAD CAllBACK");

        uint32_t type;
        uint32_t version;
        uint32_t length;
        bool refreshGame = false;

        while (a_intfc->GetNextRecordInfo(type, version, length)) {
            switch (type) {
                case 'ARR_': {
                    auto serializer = new SaveDataSerializer(a_intfc);
                    refreshGame = RestoreAllFormRecords(serializer);
                    delete serializer;
                } break;
                default:
                    print("Unrecognized signature type!");
                    break;
            }
        }
        if (refreshGame) {
            SaveCache();
            UpdateId();
            RE::PlayerCharacter::GetSingleton()->KillImmediate();
        }


        print("CAllBACK LOADED");
    } catch (const std::exception&) {
        print("error loading");
    }
}
void SaveCache() {
    print("save cache");


    auto fileWriter = new FileWriter("DynamicPersistentFormsCache.bin", std::ios::out | std::ios::binary | std::ios::trunc);

    if (!fileWriter->IsOpen()) {
        print("File not found");
        return;
    }

    StoreAllFormRecords(fileWriter);

    delete fileWriter;

    print("Property data has been written to file successfully.");
}

void LoadCache() {
    print("LOAD CACHE");
    auto fileReader = new FileReader("DynamicPersistentFormsCache.bin", std::ios::in | std::ios::binary);

    if (!fileReader->IsOpen()) {
        print("File not found");
        return;
    }
    RestoreAllFormRecords(fileReader);

    UpdateId();

    delete fileReader;

    print("Property data has been loaded from file successfully.");
}