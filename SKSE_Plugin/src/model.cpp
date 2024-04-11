#pragma once
#include <unordered_set>
std::vector<FormRecord*> formData;
RE::FormID lastFormId = 0x7FF0800;  // last mod
RE::FormID deleteFormId = 0x7FF0500;  // last mod


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
    std::uint32_t loadOrder = lastFormId & 0xFF000000;
    std::uint32_t remainingDigits = lastFormId & 0x00FFFFFF;
    std::uint32_t modifiedRemainingDigits = (remainingDigits + 1) % 0x01000000;
    lastFormId = loadOrder | modifiedRemainingDigits;
}

void resetId() {
    lastFormId = 0x7FF0800;
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