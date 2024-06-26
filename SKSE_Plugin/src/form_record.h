#pragma once
class FormRecord {
    private:
     FormRecord() {

    }
    FormRecord(RE::TESForm *_actualForm) {

        actualForm = _actualForm;
    }
    public:

    static FormRecord *CreateNew(RE::TESForm *_actualForm, RE::FormType formType, RE::FormID formId) {
        if (!_actualForm) {
            return nullptr;
        }
        auto result = new FormRecord(_actualForm);
        result->formType = formType;
        result->formId = formId;
        return result;
    }
    static FormRecord *CreateReference(RE::TESForm *_actualForm) {
        if (!_actualForm) {
            return nullptr;
        }
        auto result = new FormRecord(_actualForm);
        result->formId = _actualForm->GetFormID();
        return result;
    }
    static FormRecord *CreateDeleted(RE::FormID formId) {
        auto result = new FormRecord();
        result->formId = formId;
        result->deleted = true;
        return result;
    }
    void UndeleteReference(RE::TESForm *_actualForm) {
        if (!_actualForm) {
            return;
        }
        deleted = false;
        actualForm = _actualForm;
        formId = _actualForm->GetFormID();
        return;
    }
    void Undelete(RE::TESForm *_actualForm, RE::FormType _formType) {
        if (!_actualForm) {
            return;
        }
        actualForm = _actualForm;
        formType = _formType;
        deleted = false;
    }


    bool Match(RE::TESForm *matchForm) {
        if (!matchForm) {
            return false;
        }
        return matchForm->GetFormID() == formId;
    }

    bool Match(RE::FormID matchForm) {
        return matchForm == formId;
    }
    bool HasModel() {
        return modelForm != nullptr; 
    }
    RE::TESForm *baseForm = nullptr;
    RE::TESForm *actualForm = nullptr;
    RE::TESForm *modelForm = nullptr;
    bool deleted = false;
    RE::FormType formType;
    RE::FormID formId;

};



