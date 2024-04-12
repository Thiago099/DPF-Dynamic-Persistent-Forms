#pragma once
#include "form_record.h"
#include "form.cpp"

template <typename T>
void StoreAllFormRecords(Serializer<T>* serializer, bool incrementVersion = false) {

    size_t size = formData.size();

    serializer->Write<uint32_t>(static_cast<uint32_t>(size));
    print("write forms");

    EachFormData([&](FormRecord* instance) {
        if (incrementVersion  && !instance->deleted) {
            instance->NewVersion();
        }
        StoreFormRecord(serializer, instance);
        return true;
    });
    print("write data");

    EachFormData([&](FormRecord* elem) {
        StoreFormRecordData(serializer, elem);
        return true;
    });

    serializer->StartWritingSection();
    EachFormData([&](FormRecord* elem) {
        serializer->Write<GUID>(elem->version);
        return true;
    });
    serializer->finishWritingSection();
}
template <typename T>
bool RestoreAllFormRecords(Serializer<T>* serializer) {

    bool formRecordCreated = false;

    uint32_t size = serializer->Read<uint32_t>();
    printSize("number of items", size);
    for (uint32_t i = 0; i < size; i++) {
        print("form data", i);
        if (RestoreFormRecord(serializer, i)) {
            formRecordCreated = true;
        }
    }
    print("reading form data");

    for (uint32_t i = 0; i < size; ++i) {
        print("form data", i);
        auto instance = formData[i];
        RestoreFormRecordData(serializer, instance);
    }

    serializer->startReadingSection();
    for (uint32_t i = 0; i < size; ++i) {
        auto version = serializer->Read<GUID>();
        if (formData[i]) {
            formData[i]->version = version;
        }
    }
    serializer->finishReadingSection();

    return formRecordCreated;
}



template <typename T>
static void StoreFormRecord(Serializer<T>* serializer, FormRecord* instance) {

    serializer->StartWritingSection();

    serializer->Write<char>(instance->deleted ? 1 : 0);

    if (!instance->deleted) {
        serializer->Write<GUID>(instance->version);
        serializer->Write<char>(instance->reference ? 1 : 0);
        if (instance->reference) {
            serializer->WriteFormRef(instance->actualForm);
            serializer->WriteFormRef(instance->modelForm);

        } else {
            serializer->WriteFormRef(instance->baseForm);
            serializer->WriteFormRef(instance->modelForm);
            serializer->Write<uint32_t>(instance->formId);
        }
    } else {
        serializer->Write<uint32_t>(instance->formId);
    }

    serializer->finishWritingSection();
}

template <typename T>
static void StoreFormRecordData(Serializer<T>* serializer, FormRecord* instance) {
    serializer->StartWritingSection();
    serializer->Write<char>(instance->deleted ? 1 : 0);
    if (!instance->deleted) {
        serializer->Write<GUID>(instance->version);
        StoreEachFormData(serializer, instance);
    }
    serializer->finishWritingSection();
}



template <typename T>
static bool RestoreFormRecord(Serializer<T>* serializer, uint32_t i) {

    bool createdRecord = false;
    serializer->startReadingSection();
    auto deleted = serializer->Read<char>();

    if (deleted == 1) {
        auto formId = serializer->Read<uint32_t>();
        serializer->finishReadingSection();
        auto instance = FormRecord::CreateDeleted(formId);
        AddFormData(instance);
        return false;
    }

    auto version = serializer->Read<GUID>();
    auto reference = serializer->Read<char>();
    FormRecord* instance = nullptr;

    if (i < formData.size()) {
        instance = formData[i];
        if (instance->version == version) {
            serializer->finishReadingSection();
            print("skipped already up to date");
            return false;
        }
    }

    if (reference == 1) {
        createdRecord = RestoreModifiedItem(serializer, instance);
    } else if (reference == 0) {
        createdRecord = RestoreCreatedItem(serializer, instance);
    } else {
        serializer->finishReadingSection();
        print("this was an invalid key, something is broken");
        printInt("last reference", reference);
        return false;
    }

    return createdRecord;

}
template <typename T>
static bool RestoreModifiedItem(Serializer<T>* serializer, FormRecord* instance) {

    bool createdRecord = false;
    print("changed form");
    auto actualForm = serializer->ReadFormRef();
    auto modelForm = serializer->ReadFormRef();
    serializer->finishReadingSection();

    if (!actualForm) {
        print("missing actual form");
        return false;
    }


    if (!instance) {
        print("ref instance not found creating it");
        instance = FormRecord::CreateReference(actualForm);
        instance->modelForm = modelForm;
        AddFormData(instance);
        createdRecord = true;
    }

    if (instance->modelForm != modelForm) {
        instance->modelForm = modelForm;
        createdRecord = true;
    }

    instance->actualForm = actualForm;

    applyPattern(instance);


    return createdRecord;
}
template <typename T>
static bool RestoreCreatedItem(Serializer<T>* serializer, FormRecord* instance) {
    print("new form");
    bool createdRecord;
    auto baseForm = serializer->ReadFormRef();
    auto modelForm = serializer->ReadFormRef();
    auto id = serializer->Read<uint32_t>();
    serializer->finishReadingSection();

    if (!baseForm) {
        print("Missing base form");
        return false;
    }

    if (instance && instance->formType != baseForm->GetFormType()) {
        if (instance->actualForm) {
            instance->actualForm->SetDelete(true);
        }
        print("instance is of incompatible type");
        auto factory = RE::IFormFactory::GetFormFactoryByType(baseForm->GetFormType());
        RE::TESForm* current = factory->Create();
        current->SetFormID(id, false);
        instance->Undelete(current, baseForm->GetFormType());
        createdRecord = true;
    }
    else if (!instance) {
        print("instance not found creating it");
        auto factory = RE::IFormFactory::GetFormFactoryByType(baseForm->GetFormType());
        RE::TESForm* current = factory->Create();
        current->SetFormID(id, false);
        instance = FormRecord::CreateNew(current, baseForm->GetFormType(), id);
        createdRecord = true;
        AddFormData(instance);
    }

    instance->baseForm = baseForm;
    instance->modelForm = modelForm;

    applyPattern(instance);


    return createdRecord;
}

template <typename T>
static void RestoreFormRecordData(Serializer<T>* serializer, FormRecord* instance) {

    serializer->startReadingSection();

    auto deleted = serializer->Read<char>();

    if (deleted == 1) {
        print("delted");
        serializer->finishReadingSection();
        return;
    }

    auto version = serializer->Read<GUID>();

    if (version != instance->version) {
        print("restoring");
        RestoreEachFormData(serializer, instance);
    } else {
        print("skipped update, already up to date");
    }

    serializer->finishReadingSection();
}