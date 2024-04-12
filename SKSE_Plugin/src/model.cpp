#pragma once
#include <unordered_set>
std::vector<FormRecord*> formData;
uint32_t lastFormId = 0x7FF0800;  // last mod


void AddFormData(FormRecord* item) {
    if (!item) {
        return;
    }
    formData.push_back(item);
}



void EachFormData(std::function<bool(FormRecord*)> const& iteration) {
    for (auto item : formData) {
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
        if (item->reference) {
            return true;
        }
        if (item->formId > lastFormId) {
            lastFormId = item->formId;
        }
        return true;
    });
    incrementLastFormID();
}