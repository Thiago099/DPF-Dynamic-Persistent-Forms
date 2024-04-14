#pragma once
#include <unordered_set>
std::vector<FormRecord*> formData;
std::vector<FormRecord*> formRef;
uint32_t lastFormId = 0;  // last mod


void AddFormData(FormRecord* item) {
    if (!item) {
        return;
    }
    formData.push_back(item);
}

void AddFormRef(FormRecord* item) {
    if (!item) {
        return;
    }
    formRef.push_back(item);
}

void EachFormData(std::function<bool(FormRecord*)> const& iteration) {
    for (auto item : formData) {
        if (!iteration(item)) {
            return;
        }
    }
    return;
}

void EachFormRef(std::function<bool(FormRecord*)> const& iteration) {
    for (auto item : formRef) {
        if (!iteration(item)) {
            return;
        }
    }
    return;
}


void incrementLastFormID() {
    ++lastFormId;
}


void UpdateId() {
    EachFormData([&](FormRecord* item) {
        if (item->formId > lastFormId) {
            lastFormId = item->formId;
        }
        return true;
    });
    incrementLastFormID();
}