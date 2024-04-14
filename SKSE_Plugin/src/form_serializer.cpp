#pragma once
#include "serializer.cpp"
#include "form_record.h"


template <typename T>
class FormSerializer {
public:
    virtual void Serialize(Serializer<T>*, RE::TESForm*) = 0;
    virtual void Deserialize(Serializer<T>*, RE::TESForm*) = 0;
    virtual bool Condition(RE::TESForm*) = false;
    virtual ~FormSerializer() = default;
};

template <typename T>
class NameSerializer : public FormSerializer<T> {
public:
    void Serialize(Serializer<T>* serializer, RE::TESForm* form) override {
        auto name = form->GetName();
        serializer->WriteString(name);
    }
    void Deserialize(Serializer<T>* serializer, RE::TESForm* form) override {
        auto* name = serializer->ReadString();
        auto named = form->As<RE::TESFullName>();
        if (named && name) {
            named->fullName = name;
        }
    }
    bool Condition(RE::TESForm* form) override { return form->As<RE::TESFullName>(); }
};


template <typename T>
class SpellSerializer : public FormSerializer<T> {
public:
    void Serialize(Serializer<T>* serializer, RE::TESForm* form) override {
        auto spell = form->As<RE::SpellItem>();
        if (spell) {
            serializer->Write<char>(((spell->data.flags & RE::SpellItem::SpellFlag::kCostOverride) == RE::SpellItem::SpellFlag::kCostOverride)?1:0);
            serializer->Write<int32_t>(spell->data.costOverride);
            serializer->Write<float>(spell->data.chargeTime);
            serializer->Write<float>(spell->data.castDuration);
            serializer->Write<float>(spell->data.range);
            serializer->WriteFormRef(spell->data.castingPerk);
        }
    }
    void Deserialize(Serializer<T>* serializer, RE::TESForm* form) override {
        auto spell = form->As<RE::SpellItem>();
        if (spell) {

            char costOverride = serializer->Read<char>();

            if (costOverride == 1) {
                spell->data.flags |= RE::SpellItem::SpellFlag::kCostOverride;
            } else {
                spell->data.flags &= static_cast<RE::SpellItem::SpellFlag>(
                    ~static_cast<std::uint32_t>(RE::SpellItem::SpellFlag::kCostOverride));
            }

            spell->data.costOverride = serializer->Read<int32_t>();
            spell->data.chargeTime = serializer->Read<float>();
            spell->data.castDuration = serializer->Read<float>();
            spell->data.range = serializer->Read<float>();
            spell->data.castingPerk = serializer->ReadFormRef<RE::BGSPerk>();
        }
    }
    bool Condition(RE::TESForm* form) override { return form->As<RE::SpellItem>(); }
};
template <typename T>
class AmmoSerializer : public FormSerializer<T> {
public:
    void Serialize(Serializer<T>* serializer, RE::TESForm* form) override {
        auto ammo = form->As<RE::TESAmmo>();
        if (ammo) {
            serializer->Write<float>(ammo->GetRuntimeData().data.damage);
            serializer->WriteFormRef(ammo->GetRuntimeData().data.projectile);
        }
    }
    void Deserialize(Serializer<T>* serializer, RE::TESForm* form) override {
        auto ammo = form->As<RE::TESAmmo>();
        if (ammo) {
            ammo->GetRuntimeData().data.damage = serializer->Read<float>();
            auto projectile = serializer->ReadFormRef<RE::BGSProjectile>();
            if (projectile) {
                ammo->GetRuntimeData().data.projectile = projectile;
            }
        }
    }
    bool Condition(RE::TESForm* form) override { return form->As<RE::TESAmmo>(); }
};

template <typename T>
class SoulGemSerializer : public FormSerializer<T> {
public:
    void Serialize(Serializer<T>* serializer, RE::TESForm* form) override {
        auto soulGem = form->As<RE::TESSoulGem>();
        if (soulGem) {
            serializer->Write<uint8_t>(static_cast<uint8_t>(soulGem->currentSoul.get()));
            serializer->Write<uint8_t>(static_cast<uint8_t>(soulGem->soulCapacity.get()));
            serializer->WriteFormRef(soulGem->linkedSoulGem);
        }
    }
    void Deserialize(Serializer<T>* serializer, RE::TESForm* form) override {
        auto soulGem = form->As<RE::TESSoulGem>();
        if (soulGem) {
            soulGem->currentSoul = static_cast<RE::SOUL_LEVEL>(serializer->Read<uint8_t>());
            soulGem->soulCapacity = static_cast<RE::SOUL_LEVEL>(serializer->Read<uint8_t>());
            auto target = serializer->ReadFormRef<RE::TESSoulGem>();
            if (target) {
                soulGem->linkedSoulGem = target;
            }
        }
    }
    bool Condition(RE::TESForm* form) override { return form->As<RE::TESSoulGem>(); }
};

template <typename T>
class EnchantmentSerializer : public FormSerializer<T> {
public:
    void Serialize(Serializer<T>* serializer, RE::TESForm* form) override {
        auto enchantment = form->As<RE::EnchantmentItem>();
        if (enchantment) {
            serializer->Write<char>(((enchantment->data.flags & RE::EnchantmentItem::EnchantmentFlag::kCostOverride) ==
                                     RE::EnchantmentItem::EnchantmentFlag::kCostOverride)
                                        ? 1
                                        : 0);

            serializer->Write<int32_t>(enchantment->data.costOverride);
            serializer->Write<int32_t>(enchantment->data.chargeOverride);
            serializer->Write<float>(enchantment->data.chargeTime);
        }
    }
    void Deserialize(Serializer<T>* serializer, RE::TESForm* form) override {
        auto enchantment = form->As<RE::EnchantmentItem>();
        if (enchantment) {
            char costOverride = serializer->Read<char>();

            if (costOverride == 1) {
                enchantment->data.flags |= RE::EnchantmentItem::EnchantmentFlag::kCostOverride;
            } else {
                enchantment->data.flags &= static_cast<RE::EnchantmentItem::EnchantmentFlag>(
                    ~static_cast<std::uint32_t>(RE::EnchantmentItem::EnchantmentFlag::kCostOverride));
            }

            enchantment->data.costOverride = serializer->Read<int32_t>();
            enchantment->data.chargeOverride = serializer->Read<int32_t>();
            enchantment->data.chargeTime = serializer->Read<float>();
        }
    }
    bool Condition(RE::TESForm* form) override { return form->As<RE::EnchantmentItem>(); }
};

template <typename T>
class ValueSerializer : public FormSerializer<T> {
public:
    void Serialize(Serializer<T>* serializer, RE::TESForm* form) override {
        auto name = form->GetGoldValue();
        serializer->Write<int32_t>(name);
    }
    void Deserialize(Serializer<T>* serializer, RE::TESForm* form) override {
        auto value = serializer->Read<int32_t>();
        auto named = form->As<RE::TESValueForm>();
        if (named && value) {
            named->value = value;
        }
    }
    bool Condition(RE::TESForm* form) override { return form->As<RE::TESValueForm>(); }
};

template <typename T>
class WeightSerializer : public FormSerializer<T> {
public:
    void Serialize(Serializer<T>* serializer, RE::TESForm* form) override {
        auto name = form->GetWeight();
        serializer->Write<float>(name);
    }
    void Deserialize(Serializer<T>* serializer, RE::TESForm* form) override {
        auto weight = serializer->Read<float>();
        auto named = form->As<RE::TESWeightForm>();
        if (named && weight) {
            named->weight = weight;
        }
    }
    bool Condition(RE::TESForm* form) override { return form->As<RE::TESWeightForm>(); }
};

template <typename T>
class MagicItemSerializer : public FormSerializer<T> {
public:
    void Serialize(Serializer<T>* serializer, RE::TESForm* form) override {
        auto potion = form->As<RE::MagicItem>();
        if (potion) {
            size_t length = potion->effects.size();
            serializer->Write<uint32_t>(static_cast<uint32_t>(length));
            for (uint32_t i = 0; i < length; i++) {
                auto effect = potion->effects[i];
                serializer->WriteFormRef(effect->baseEffect);
                serializer->Write<float>(effect->GetMagnitude());
                serializer->Write<uint32_t>(effect->GetArea());
                serializer->Write<uint32_t>(effect->GetDuration());
                serializer->Write<float>(effect->cost);
            }

        }
    }
    void Deserialize(Serializer<T>* serializer, RE::TESForm* form) override {
        auto potion = form->As<RE::MagicItem>();
        potion->effects.clear();
        size_t numEffects = serializer->Read<uint32_t>();

        for (size_t i = 0; i < numEffects; i++) {
            auto effectForm = serializer->ReadFormRef<RE::EffectSetting>();
            auto magnitude = serializer->Read<float>();
            auto area = serializer->Read<uint32_t>();
            auto duration = serializer->Read<uint32_t>();
            auto cost = serializer->Read<float>();

            if (effectForm) {
                bool found = false;
                for (auto effect : potion->effects) {
                    if (effect->baseEffect && effect->baseEffect->GetFormID() == effectForm->GetFormID()) {
                        effect->effectItem.magnitude = magnitude;
                        effect->effectItem.area = area;
                        effect->effectItem.duration = duration;
                        effect->cost = cost;
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    auto newEffect = new RE::Effect();
                    newEffect->baseEffect = effectForm;
                    newEffect->effectItem.magnitude = magnitude;
                    newEffect->effectItem.area = area;
                    newEffect->effectItem.duration = duration;
                    newEffect->cost = cost;
                    potion->effects.push_back(newEffect);
                }
            }
        }
    }
    bool Condition(RE::TESForm* form) override { return form->As<RE::MagicItem>(); }
};

template <typename T>
class EnchantableSerializer : public FormSerializer<T> {
public:
    void Serialize(Serializer<T>* serializer, RE::TESForm* form) override {
        auto enchanted = form->As<RE::TESEnchantableForm>();
        if (enchanted && enchanted->formEnchanting) {
            serializer->Write<char>(1);
            serializer->WriteFormRef(enchanted->formEnchanting);
            serializer->Write<uint16_t>(enchanted->amountofEnchantment);

        } else {
            serializer->Write<char>(0);
        }
    }
    void Deserialize(Serializer<T>* serializer, RE::TESForm* form) override {
        auto enchanted = form->As<RE::TESEnchantableForm>();
        int32_t hasEffects = serializer->Read<char>();
        if (hasEffects == 1) {
            auto enchantment = serializer->ReadFormRef<RE::EnchantmentItem>();
            auto ammount = serializer->Read<uint16_t>();
            if (enchantment) {
                enchanted->formEnchanting = enchantment;
                enchanted->amountofEnchantment = ammount;
            }
        }
    }
    bool Condition(RE::TESForm* form) override { return form->As<RE::TESEnchantableForm>(); }
};

template <typename T>
class ArmorSerializer : public FormSerializer<T> {
public:
    void Serialize(Serializer<T>* serializer, RE::TESForm* form) override {
        auto* amor = form->As<RE::TESObjectARMO>();
        serializer->Write<uint32_t>(amor->armorRating);
    }
    void Deserialize(Serializer<T>* serializer, RE::TESForm* form) override {
        auto armorRating = serializer->Read<uint32_t>();
        auto* target = form->As<RE::TESObjectARMO>();
        if (target) {
            target->armorRating = armorRating;
        }
    }
    bool Condition(RE::TESForm* form) override { return form->As<RE::TESObjectARMO>(); }
};

template <typename T>
class WeaponSerializer : public FormSerializer<T> {
public:
    void Serialize(Serializer<T>* serializer, RE::TESForm* form) override {
        auto sourceModelWeapon = form->As<RE::TESObjectWEAP>();
        if (sourceModelWeapon) {
            serializer->WriteFormRef(sourceModelWeapon->criticalData.effect);
            serializer->Write<uint16_t>(sourceModelWeapon->attackDamage);
            serializer->Write<uint16_t>(sourceModelWeapon->criticalData.damage);
            serializer->Write<float>(sourceModelWeapon->weaponData.speed);
            serializer->Write<float>(sourceModelWeapon->weaponData.reach);
            serializer->Write<float>(sourceModelWeapon->weaponData.minRange);
            serializer->Write<float>(sourceModelWeapon->weaponData.maxRange);
            serializer->Write<float>(sourceModelWeapon->weaponData.staggerValue);
        }
    }
    void Deserialize(Serializer<T>* serializer, RE::TESForm* form) override {
        auto target = form->As<RE::TESObjectWEAP>();
        if (target) {
            auto effect = serializer->ReadFormRef<RE::SpellItem>();
            if (effect) {
                target->criticalData.effect = effect;
            }
            target->attackDamage = serializer->Read<uint16_t>();
            target->criticalData.damage = serializer->Read<uint16_t>();;
            target->weaponData.speed = serializer->Read<float>();
            target->weaponData.reach = serializer->Read<float>();
            target->weaponData.minRange = serializer->Read<float>();
            target->weaponData.maxRange = serializer->Read<float>();
            target->weaponData.staggerValue = serializer->Read<float>();
        }
    }
    bool Condition(RE::TESForm* form) override { return form->As<RE::TESObjectWEAP>(); }
};

template <typename T>
class SpellTomeSerializer : public FormSerializer<T> {
public:
    void Serialize(Serializer<T>* serializer, RE::TESForm* form) override {
        auto book = form->As<RE::TESObjectBOOK>();
        serializer->WriteFormRef(book->data.teaches.spell);
    }
    void Deserialize(Serializer<T>* serializer, RE::TESForm* form) override {
        auto spell = serializer->ReadFormRef<RE::SpellItem>();
        auto book = form->As<RE::TESObjectBOOK>();

        if (spell && book) {
            book->data.flags = RE::OBJ_BOOK::Flag::kTeachesSpell;
            book->data.teaches.spell = spell;
        }
    }
    bool Condition(RE::TESForm* form) override { return form->As<RE::TESObjectBOOK>(); }
};

template <typename T>
class LeveledItemSerializer : public FormSerializer<T> {
public:
    void Serialize(Serializer<T>* serializer, RE::TESForm* form) override {
        auto leveledList = form->As<RE::TESLeveledList>(); 
        if (leveledList) {
            serializer->Write<int8_t>(leveledList->chanceNone);
            serializer->Write<uint8_t>(leveledList->numEntries);
            //serializer->Write<uint8_t>(static_cast<uint8_t>(leveledList->llFlags));
            serializer->WriteFormRef(leveledList->chanceGlobal);
            auto size = static_cast<uint32_t>(leveledList->entries.size());
            serializer->Write<uint32_t>(size);
            for (auto item : leveledList->entries) {
                serializer->Write<uint16_t>(item.count);
                serializer->Write<uint16_t>(item.level);
                serializer->WriteFormRef(item.form);
            }
        }
    }
    void Deserialize(Serializer<T>* serializer, RE::TESForm* form) override {
        auto leveledList = form->As<RE::TESLeveledList>();
        if (leveledList) {
            leveledList->chanceNone = serializer->Read<int8_t>();
            leveledList->numEntries = serializer->Read<uint8_t>();
            //leveledList->llFlags = static_cast<RE::TESLeveledList::Flag>(serializer->Read<uint8_t>());
            auto global = serializer->ReadFormRef<RE::TESGlobal>();
            if (global){
                leveledList->chanceGlobal = global;
            }
            auto size = serializer->Read<uint32_t>();
            leveledList->entries.resize(static_cast<size_t>(size));
            for (uint32_t i = 0; i < size; ++i) {
                leveledList->entries[i] = RE::LEVELED_OBJECT();
                auto count = serializer->Read<uint16_t>();
                auto level = serializer->Read<uint16_t>();
                auto newForm = serializer->ReadFormRef();
                if (form) {
                    leveledList->entries[i].count = count;
                    leveledList->entries[i].level = level;
                    leveledList->entries[i].form = newForm;
                }
            }
        }

    }
    bool Condition(RE::TESForm* form) override { return form->As<RE::TESLeveledList>(); }
};

template <typename T>
class ProduceFormSerializer : public FormSerializer<T> {
public:
    void Serialize(Serializer<T>* serializer, RE::TESForm* form) override {
        auto produceForm = form->As<RE::TESProduceForm>(); 
        if (produceForm) {
            serializer->WriteFormRef(produceForm->harvestSound);
            serializer->WriteFormRef(produceForm->produceItem);
        }
    }
    void Deserialize(Serializer<T>* serializer, RE::TESForm* form) override {
        auto produceForm = form->As<RE::TESProduceForm>();
        if (produceForm) {
            auto sound = serializer->ReadFormRef<RE::BGSSoundDescriptorForm>();
            auto item = serializer->ReadFormRef<RE::TESBoundObject>();
            if (sound) {
                produceForm->harvestSound = sound;
            }
            if (item) {
                produceForm->produceItem = item;
            }
        }
    }
    bool Condition(RE::TESForm* form) override { return form->As<RE::TESProduceForm>(); }
};

#define GetFilters() \
    filters.push_back(std::make_unique <NameSerializer<T>>());\
    filters.push_back(std::make_unique <ValueSerializer<T>>());\
    filters.push_back(std::make_unique <WeightSerializer<T>>());\
    filters.push_back(std::make_unique <MagicItemSerializer<T>>());\
    filters.push_back(std::make_unique <EnchantableSerializer<T>>());\
    filters.push_back(std::make_unique <WeaponSerializer<T>>());\
    filters.push_back(std::make_unique <ArmorSerializer<T>>());\
    filters.push_back(std::make_unique <SpellTomeSerializer<T>>());\
    filters.push_back(std::make_unique<SpellSerializer<T>>());\
    filters.push_back(std::make_unique<EnchantmentSerializer<T>>()); \
    filters.push_back(std::make_unique<AmmoSerializer<T>>());\
    filters.push_back(std::make_unique<SoulGemSerializer<T>>());\
    filters.push_back(std::make_unique<LeveledItemSerializer<T>>());\
    filters.push_back(std::make_unique<ProduceFormSerializer<T>>());


template <typename T>
void StoreEachFormData(Serializer<T>* serializer, FormRecord* elem) {

    if (!elem || !serializer) {
        return;
    }

    std::vector<std::unique_ptr<FormSerializer<T>>> filters;

    GetFilters();

    for (auto& filter : filters) {
        serializer->StartWritingSection();
        if (elem->actualForm && filter->Condition(elem->actualForm)) {
            serializer->Write<char>(1);
            filter->Serialize(serializer, elem->actualForm);
        } else {
            serializer->Write<char>(0);
        }
        serializer->finishWritingSection();
    }
}
template <typename T>
void RestoreEachFormData(Serializer<T>* serializer, FormRecord* elem) {

     if (!elem || !serializer) {
         return;
     }

     std::vector<std::unique_ptr<FormSerializer<T>>> filters;

     GetFilters();


     for (auto& filter : filters) {
         serializer->startReadingSection();
         auto kind = serializer->Read<char>();
         if (kind == 1 && elem->actualForm) {
            filter->Deserialize(serializer, elem->actualForm);
         }
         serializer->finishReadingSection();
     }
 }