#pragma once
#include "form.cpp"

std::mutex papyrusMutex;

RE::TESForm* Create(RE::StaticFunctionTag*, RE::TESForm* baseItem) {
    std::lock_guard<std::mutex> lock(papyrusMutex);
    try {
        if (!baseItem) {
            return nullptr;
        }

        // THIS WAS CAUSING ISSUES
        //if (baseItem->GetFormID() >= 0x7FF0800) {
        //    print("item in treshold", baseItem->GetFormID());
        //    bool found = false;
        //    EachFormData([&](FormRecord* item) {
        //        if (!item->Match(baseItem)) {
        //            if (item->baseForm) {
        //                found = true;
        //                print("item replaced", baseItem->GetFormID());
        //                baseItem = item->baseForm;
        //            }
        //            return false;
        //        }
        //        return true;
        //    });
        //    if (!found) {
        //        return nullptr;
        //    }
        //}

        auto* newForm = AddForm(baseItem);

        if (newForm) {
            printInt("new form id", newForm->GetFormID());
        }

        return newForm;

    } catch (const std::exception&) {
        return nullptr;
    }
}

void Track(RE::StaticFunctionTag*, RE::TESForm* baseItem) {
    std::lock_guard<std::mutex> lock(papyrusMutex);
    try {
        if (!baseItem) {
            return;
        }
        bool found = false;
        EachFormRef([&](FormRecord* item) {
            if (item->Match(baseItem)) {
                print("reference reused");
                if (item->deleted) {
                    item->UndeleteReference(baseItem);
                }
                found = true;
                return false;
            }
            if (item->Match(baseItem)) {
                found = true;
                return false;
            }
            return true;
        });
        if (!found) {
            AddFormRef(FormRecord::CreateReference(baseItem));
        }

    } catch (const std::exception&) {
    }
}

void UnTrack(RE::StaticFunctionTag*, RE::TESForm* form) {
    std::lock_guard<std::mutex> lock(papyrusMutex);
    try {
        if (!form) {
            return;
        }

        EachFormRef([&](FormRecord* item) {
            if (item->Match(form)) {
                item->deleted = true;
                return false;
            }
            return true;
        });

    } catch (const std::exception&) {
    }
}


void AddMagicEffect(RE::StaticFunctionTag*, RE::TESForm* item, RE::EffectSetting* effect, float magnitude,
                    uint32_t area, uint32_t duration, float cost) {
    std::lock_guard<std::mutex> lock(papyrusMutex);
    try {
        print("added");

        if (!item || !effect) {
            return;
        }

        auto magicItem = item->As<RE::MagicItem>();

        if (magicItem) {
            auto newEffect = new RE::Effect();
            newEffect->cost = cost;
            newEffect->baseEffect = effect;
            newEffect->effectItem.magnitude = magnitude;
            newEffect->effectItem.area = area;
            newEffect->effectItem.duration = duration;
            magicItem->effects.push_back(newEffect);
        }

    } 
    catch (const std::exception&) {
    }
}


void CopyMagicEffects(RE::StaticFunctionTag*, RE::TESForm* from, RE::TESForm* to) {
    std::lock_guard<std::mutex> lock(papyrusMutex);
    try {
        print("added");

        if (!from || !to) {
            return;
        }

        auto magicItemFrom = from->As<RE::MagicItem>();
        auto magicItemTo = to->As<RE::MagicItem>();

        if (magicItemFrom && magicItemTo) {
            for (auto item : magicItemFrom->effects) {
                auto newEffect = new RE::Effect();
                newEffect->cost = item->cost;
                newEffect->baseEffect = item->baseEffect;
                newEffect->effectItem.magnitude = item->effectItem.magnitude;
                newEffect->effectItem.area = item->effectItem.area;
                newEffect->effectItem.duration = item->effectItem.duration;
                magicItemTo->effects.push_back(newEffect);
            }

        }

    } catch (const std::exception&) {
    }
}


void Dispose(RE::StaticFunctionTag*, RE::TESForm* form) {
    std::lock_guard<std::mutex> lock(papyrusMutex);
    try {
        if (!form) {
            return;
        }

        EachFormData([&](FormRecord* item) {
            if (!item->deleted && item->Match(form)) {
                item->deleted = true;
                if (item->actualForm) {
                    item->actualForm->SetDelete(true);
                }
                return false;
            }
            return true;
        });


    }
    catch (const std::exception&) {
    }
}

void ClearMagicEffects(RE::StaticFunctionTag*, RE::TESForm* item) {
    std::lock_guard<std::mutex> lock(papyrusMutex);
    try {

        if (!item) {
            return;
        }

        print("added");

        auto magicItem = item->As<RE::MagicItem>();

        if (magicItem) {
            magicItem->effects.clear();
        }

    } 
    catch (const std::exception&) {
    }
}


static void CopyAppearance(RE::StaticFunctionTag*, RE::TESForm* source, RE::TESForm* target) {
    std::lock_guard<std::mutex> lock(papyrusMutex);
    try {

        if (!source || !target) {
            return;
        }

        EachFormData([&](FormRecord* item) {
            if (!item->deleted && item->Match(target)) {
                item->modelForm = source;
                return false;
            }
            return true;
        });

        EachFormRef([&](FormRecord* item) {
            if (!item->deleted && item->Match(target)) {
                item->modelForm = source;
                return false;
            }
            return true;
        });


        copyAppearence(source, target);

    } catch (const std::exception&) {
        print("error copying model");
    }
}
static void SetSpellTomeSpell(RE::StaticFunctionTag*, RE::TESObjectBOOK* spellTome, RE::SpellItem* spell) {
    std::lock_guard<std::mutex> lock(papyrusMutex);
    try {
        if (!spellTome || !spell) {
            return;
        }
        spellTome->data.flags = RE::OBJ_BOOK::Flag::kTeachesSpell;
        spellTome->data.teaches.spell = spell;
    } 
    catch (const std::exception&) {
        print("add spell");
    }
}
static void SetSpellAutoCalculate(RE::StaticFunctionTag*, RE::SpellItem* spell, bool autoCalculate) {
    std::lock_guard<std::mutex> lock(papyrusMutex);

    if (!spell) {
        return;
    }

    if (!autoCalculate) {
        spell->data.flags |= RE::SpellItem::SpellFlag::kCostOverride;
    } else {
        spell->data.flags &= static_cast<RE::SpellItem::SpellFlag>(~static_cast<uint32_t>(RE::SpellItem::SpellFlag::kCostOverride));
    }

}
static void SetSpellCostOverride(RE::StaticFunctionTag*, RE::SpellItem* spell, uint32_t costOverride) {

    std::lock_guard<std::mutex> lock(papyrusMutex);

    if (!spell) {
        return;
    }
    spell->data.flags |= RE::SpellItem::SpellFlag::kCostOverride;
    spell->data.costOverride = costOverride;
}
static void SetSpellChargeTime(RE::StaticFunctionTag*, RE::SpellItem* spell, float chargeTime) {
    std::lock_guard<std::mutex> lock(papyrusMutex);

    if (!spell) {
        return;
    }
    spell->data.flags |= RE::SpellItem::SpellFlag::kCostOverride;
    spell->data.chargeTime = chargeTime;
}
static void SetSpellCastDuration(RE::StaticFunctionTag*, RE::SpellItem* spell, float castDuration) {
    std::lock_guard<std::mutex> lock(papyrusMutex);

    if (!spell) {
        return;
    }
    spell->data.flags |= RE::SpellItem::SpellFlag::kCostOverride;
    spell->data.castDuration = castDuration;
}
static void SetSpellRange(RE::StaticFunctionTag*, RE::SpellItem* spell, float range) {
    std::lock_guard<std::mutex> lock(papyrusMutex);

    if (!spell) {
        return;
    }

    spell->data.range = range;
}
static void SetSpellCastingPerk(RE::StaticFunctionTag*, RE::SpellItem* spell, RE::BGSPerk* perk) {
    std::lock_guard<std::mutex> lock(papyrusMutex);

    if (!spell || !perk) {
        return;
    }

    spell->data.castingPerk = perk;
}


static void SetEnchantmentAutoCalculate(RE::StaticFunctionTag*, RE::EnchantmentItem* spell, bool autoCalculate) {
    std::lock_guard<std::mutex> lock(papyrusMutex);

    if (!spell) {
        return;
    }

    if (!autoCalculate) {
        spell->data.flags |= RE::EnchantmentItem::EnchantmentFlag::kCostOverride;
    } else {
        spell->data.flags &=
            static_cast<RE::EnchantmentItem::EnchantmentFlag>(~static_cast<uint32_t>(RE::EnchantmentItem::EnchantmentFlag::kCostOverride));
    }
}
static void SetEnchantmentChargeOverride(RE::StaticFunctionTag*, RE::EnchantmentItem* spell, uint32_t costOverride) {
    std::lock_guard<std::mutex> lock(papyrusMutex);

    if (!spell) {
        return;
    }
    spell->data.flags |= RE::EnchantmentItem::EnchantmentFlag::kCostOverride;
    spell->data.chargeOverride = costOverride;
}
static void SetEnchantmentCostOverride(RE::StaticFunctionTag*, RE::EnchantmentItem* spell, uint32_t costOverride) {
    std::lock_guard<std::mutex> lock(papyrusMutex);

    if (!spell) {
        return;
    }

    spell->data.flags |= RE::EnchantmentItem::EnchantmentFlag::kCostOverride;
    spell->data.costOverride = costOverride;
}
static void SetEnchantmentChargeTime(RE::StaticFunctionTag*, RE::EnchantmentItem* spell, float chargeTime) {
    std::lock_guard<std::mutex> lock(papyrusMutex);

    if (!spell) {
        return;
    }
    spell->data.chargeTime = chargeTime;
}


static void SetAmmoDamage(RE::StaticFunctionTag*, RE::TESAmmo* ammo, float damage) {
    std::lock_guard<std::mutex> lock(papyrusMutex);

    if (!ammo) {
        return;
    }
    ammo->GetRuntimeData().data.damage = damage;
}

static void SetAmmoProjectile(RE::StaticFunctionTag*, RE::TESAmmo* ammo, RE::BGSProjectile* projectile) {
    std::lock_guard<std::mutex> lock(papyrusMutex);

    if (!ammo || !projectile) {
        return;
    }
    ammo->GetRuntimeData().data.projectile = projectile;
}


static void SetSoulGemCapacity(RE::StaticFunctionTag*, RE::TESSoulGem* soulGem, uint32_t capacity) {
    std::lock_guard<std::mutex> lock(papyrusMutex);

    auto value = static_cast<RE::SOUL_LEVEL>(capacity);

    if (!soulGem || value > RE::SOUL_LEVEL::kGrand || value < RE::SOUL_LEVEL::kNone) {
        return;
    }

    soulGem->soulCapacity = value;
}
static void SetSoulGemCurrentSoul(RE::StaticFunctionTag*, RE::TESSoulGem* soulGem, uint32_t capacity) {
    std::lock_guard<std::mutex> lock(papyrusMutex);

    auto value = static_cast<RE::SOUL_LEVEL>(capacity);

    if (!soulGem || value > RE::SOUL_LEVEL::kGrand || value < RE::SOUL_LEVEL::kNone) {
        return;
    }

    soulGem->currentSoul = value;

}

static void LinkSoulGems(RE::StaticFunctionTag*, RE::TESSoulGem* empty, RE::TESSoulGem* filled) {
    std::lock_guard<std::mutex> lock(papyrusMutex);

    if (!empty || !filled) {
        return;
    }

    empty->linkedSoulGem = filled;
}

bool PapyrusFunctions(RE::BSScript::IVirtualMachine* vm) {
    vm->RegisterFunction("Create", "DynamicPersistentForms", Create);
    vm->RegisterFunction("Dispose", "DynamicPersistentForms", Dispose);
    vm->RegisterFunction("Track", "DynamicPersistentForms", Track);
    vm->RegisterFunction("UnTrack", "DynamicPersistentForms", UnTrack);

    vm->RegisterFunction("AddMagicEffect", "DynamicPersistentForms", AddMagicEffect);
    vm->RegisterFunction("ClearMagicEffects", "DynamicPersistentForms", ClearMagicEffects);
    vm->RegisterFunction("CopyAppearance", "DynamicPersistentForms", CopyAppearance);
    vm->RegisterFunction("CopyMagicEffects", "DynamicPersistentForms", CopyMagicEffects);
    vm->RegisterFunction("SetSpellTomeSpell", "DynamicPersistentForms", SetSpellTomeSpell);

    vm->RegisterFunction("SetSpellCostOverride", "DynamicPersistentForms", SetSpellCostOverride);
    vm->RegisterFunction("SetSpellChargeTime", "DynamicPersistentForms", SetSpellChargeTime);
    vm->RegisterFunction("SetSpellCastDuration", "DynamicPersistentForms", SetSpellCastDuration);
    vm->RegisterFunction("SetSpellRange", "DynamicPersistentForms", SetSpellRange);
    vm->RegisterFunction("SetSpellCastingPerk", "DynamicPersistentForms", SetSpellCastingPerk);
    vm->RegisterFunction("SetSpellAutoCalculate", "DynamicPersistentForms", SetSpellAutoCalculate);

    vm->RegisterFunction("SetEnchantmentAutoCalculate", "DynamicPersistentForms", SetEnchantmentAutoCalculate);
    vm->RegisterFunction("SetEnchantmentChargeOverride", "DynamicPersistentForms", SetEnchantmentChargeOverride);
    vm->RegisterFunction("SetEnchantmentCostOverride", "DynamicPersistentForms", SetEnchantmentCostOverride);
    vm->RegisterFunction("SetEnchantmentChargeTime", "DynamicPersistentForms", SetEnchantmentChargeTime);

    vm->RegisterFunction("SetAmmoDamage", "DynamicPersistentForms", SetAmmoDamage);
    vm->RegisterFunction("SetAmmoProjectile", "DynamicPersistentForms", SetAmmoProjectile);

    vm->RegisterFunction("SetSoulGemCapacity", "DynamicPersistentForms", SetSoulGemCapacity);
    vm->RegisterFunction("SetSoulGemCurrentSoul", "DynamicPersistentForms", SetSoulGemCurrentSoul);
    vm->RegisterFunction("LinkSoulGems", "DynamicPersistentForms", LinkSoulGems);

    return true;
}