#pragma once
#include "log.cpp"
#include "model.cpp"

static void copyFormArmorModel(RE::TESForm* source, RE::TESForm* target);
static void copyFormObjectWeaponModel(RE::TESForm* source, RE::TESForm* target);
void copyMagicEffect(RE::TESForm* source, RE::TESForm* target);
static void copyBookAppearence(RE::TESForm* source, RE::TESForm* target);
static void applyPattern(FormRecord* instance);
template <class T>
void copyComponent(RE::TESForm* from, RE::TESForm* to);
void copyAppearence(RE::TESForm* source, RE::TESForm* target);
static void applyPattern(FormRecord* instance);


RE::TESForm* AddForm(RE::TESForm* baseItem) {
    RE::TESForm* result = nullptr;
    EachFormData([&](FormRecord* item) {
        if (!item->reference && item->deleted) {
            print("item undeleted",item->formId);
            auto factory = RE::IFormFactory::GetFormFactoryByType(baseItem->GetFormType());
            result = factory->Create();
            result->SetFormID(item->formId, false);
            item->Undelete(result, baseItem->GetFormType());
            item->baseForm = baseItem;
            applyPattern(item);
            return false;
        }
        return true;
    });
    if (result) {
        return result;
    }
    print("item created");
    auto factory = RE::IFormFactory::GetFormFactoryByType(baseItem->GetFormType());
    auto newForm = factory->Create();
    newForm->SetFormID(lastFormId, false);
    auto slot = FormRecord::CreateNew(newForm, baseItem->GetFormType(), lastFormId);
    incrementLastFormID();
    slot->baseForm = baseItem;
    applyPattern(slot);
    AddFormData(slot);
    return newForm;
}

static void applyPattern(FormRecord* instance) {
    if (!instance) {
        return;
    }

    auto baseForm = instance->baseForm;
    auto newForm = instance->actualForm;

    if (baseForm && newForm) {
        auto weaponBaseForm = baseForm->As<RE::TESObjectWEAP>();
        auto weaponNewForm = newForm->As<RE::TESObjectWEAP>();

        auto bookBaseForm = baseForm->As<RE::TESObjectBOOK>();
        auto bookNewForm = newForm->As<RE::TESObjectBOOK>();

        
        auto ammoBaseForm = baseForm->As<RE::TESAmmo>();
        auto ammoNewForm = newForm->As<RE::TESAmmo>();


        if (weaponNewForm && weaponBaseForm) {
            weaponNewForm->firstPersonModelObject = weaponBaseForm->firstPersonModelObject;

            weaponNewForm->weaponData = weaponBaseForm->weaponData;
            weaponNewForm->criticalData = weaponBaseForm->criticalData;

            weaponNewForm->attackSound = weaponBaseForm->attackSound;
            weaponNewForm->attackSound2D = weaponBaseForm->attackSound2D;
            weaponNewForm->attackSound = weaponBaseForm->attackSound;
            weaponNewForm->attackFailSound = weaponBaseForm->attackFailSound;
            weaponNewForm->idleSound = weaponBaseForm->idleSound;
            weaponNewForm->equipSound = weaponBaseForm->equipSound;
            weaponNewForm->unequipSound = weaponBaseForm->unequipSound;
            weaponNewForm->soundLevel = weaponBaseForm->soundLevel;

            weaponNewForm->impactDataSet = weaponBaseForm->impactDataSet;
            weaponNewForm->templateWeapon = weaponBaseForm->templateWeapon;
            weaponNewForm->embeddedNode = weaponBaseForm->embeddedNode;

        } else if (bookBaseForm && bookNewForm) {
            bookNewForm->data.flags = bookBaseForm->data.flags;
            bookNewForm->data.teaches.spell = bookBaseForm->data.teaches.spell;
            bookNewForm->data.teaches.actorValueToAdvance = bookBaseForm->data.teaches.actorValueToAdvance;
            bookNewForm->data.type = bookBaseForm->data.type;
            bookNewForm->inventoryModel = bookBaseForm->inventoryModel;
            bookNewForm->itemCardDescription = bookBaseForm->itemCardDescription;
        } else if (ammoBaseForm && ammoNewForm) {
            ammoNewForm->GetRuntimeData().data.damage = ammoBaseForm->GetRuntimeData().data.damage;
            ammoNewForm->GetRuntimeData().data.flags = ammoBaseForm->GetRuntimeData().data.flags;
            ammoNewForm->GetRuntimeData().data.projectile = ammoBaseForm->GetRuntimeData().data.projectile;
        }
        else {
            newForm->Copy(baseForm);
        }

        copyComponent<RE::TESDescription>(baseForm, newForm);
        copyComponent<RE::BGSKeywordForm>(baseForm, newForm);
        copyComponent<RE::BGSPickupPutdownSounds>(baseForm, newForm);
        copyComponent<RE::TESModelTextureSwap>(baseForm, newForm);
        copyComponent<RE::TESModel>(baseForm, newForm);
        copyComponent<RE::BGSMessageIcon>(baseForm, newForm);
        copyComponent<RE::TESIcon>(baseForm, newForm);
        copyComponent<RE::TESFullName>(baseForm, newForm);
        copyComponent<RE::TESValueForm>(baseForm, newForm);
        copyComponent<RE::TESWeightForm>(baseForm, newForm);
        copyComponent<RE::BGSDestructibleObjectForm>(baseForm, newForm);
        copyComponent<RE::TESEnchantableForm>(baseForm, newForm);
        copyComponent<RE::BGSBlockBashData>(baseForm, newForm);
        copyComponent<RE::BGSEquipType>(baseForm, newForm);
        copyComponent<RE::TESAttackDamageForm>(baseForm, newForm);
        copyComponent<RE::TESBipedModelForm>(baseForm, newForm);
    }
    if (newForm && instance->modelForm) {
        copyAppearence(instance->modelForm, newForm);
    }
}

void copyAppearence(RE::TESForm* source, RE::TESForm* target) {
    copyFormArmorModel(source, target);
    copyFormObjectWeaponModel(source, target);
    copyMagicEffect(source, target);
    copyBookAppearence(source, target);
    copyComponent<RE::BGSPickupPutdownSounds>(source, target);
    copyComponent<RE::BGSMenuDisplayObject>(source, target);
    copyComponent<RE::TESModel>(source, target);
    copyComponent<RE::TESBipedModelForm>(source, target);
}



template <class T>
void copyComponent(RE::TESForm* from, RE::TESForm* to) {
    auto fromT = from->As<T>();
    auto toT = to->As<T>();
    if (fromT && toT) {
        toT->CopyComponent(fromT);
    }
}


static void copyFormArmorModel(RE::TESForm* source, RE::TESForm* target) {
    auto* sourceModelBipedForm = source->As<RE::TESObjectARMO>();
    auto* targeteModelBipedForm = target->As<RE::TESObjectARMO>();
    if (sourceModelBipedForm && targeteModelBipedForm) {
        print("armor");
        targeteModelBipedForm->armorAddons = sourceModelBipedForm->armorAddons;
    }
}

static void copyFormObjectWeaponModel(RE::TESForm* source, RE::TESForm* target) {
    auto* sourceModelWeapon = source->As<RE::TESObjectWEAP>();
    auto* targeteModelWeapon = target->As<RE::TESObjectWEAP>();
    if (sourceModelWeapon && targeteModelWeapon) {
        print("weapon");
        targeteModelWeapon->firstPersonModelObject = sourceModelWeapon->firstPersonModelObject;
        targeteModelWeapon->attackSound = sourceModelWeapon->attackSound;
        targeteModelWeapon->attackSound2D = sourceModelWeapon->attackSound2D;
        targeteModelWeapon->attackSound = sourceModelWeapon->attackSound;
        targeteModelWeapon->attackFailSound = sourceModelWeapon->attackFailSound;
        targeteModelWeapon->idleSound = sourceModelWeapon->idleSound;
        targeteModelWeapon->equipSound = sourceModelWeapon->equipSound;
        targeteModelWeapon->unequipSound = sourceModelWeapon->unequipSound;
        targeteModelWeapon->soundLevel = sourceModelWeapon->soundLevel;
    }
}


static void copyMagicEffect(RE::TESForm* source, RE::TESForm* target) {
    auto* sourceEffect = source->As<RE::EffectSetting>();
    auto* targetEffect = target->As<RE::EffectSetting>();
    if (sourceEffect && targetEffect) {
        targetEffect->effectSounds = sourceEffect->effectSounds;
        targetEffect->data.castingArt = sourceEffect->data.castingArt;
        targetEffect->data.light = sourceEffect->data.light;
        targetEffect->data.hitEffectArt = sourceEffect->data.hitEffectArt;
        targetEffect->data.effectShader = sourceEffect->data.effectShader;
        targetEffect->data.hitVisuals = sourceEffect->data.hitVisuals;
        targetEffect->data.enchantShader = sourceEffect->data.enchantShader;
        targetEffect->data.enchantEffectArt = sourceEffect->data.enchantEffectArt;
        targetEffect->data.enchantVisuals = sourceEffect->data.enchantVisuals;
        targetEffect->data.projectileBase = sourceEffect->data.projectileBase;
        targetEffect->data.explosion = sourceEffect->data.explosion;
        targetEffect->data.impactDataSet = sourceEffect->data.impactDataSet;
        targetEffect->data.imageSpaceMod = sourceEffect->data.imageSpaceMod;
    }
}


static void copyBookAppearence(RE::TESForm* source, RE::TESForm* target) {
    auto* sourceBook = source->As<RE::TESObjectBOOK>();
    auto* targetBook = target->As<RE::TESObjectBOOK>();
    if (sourceBook && targetBook) {
        targetBook->inventoryModel = sourceBook->inventoryModel;
    }
}