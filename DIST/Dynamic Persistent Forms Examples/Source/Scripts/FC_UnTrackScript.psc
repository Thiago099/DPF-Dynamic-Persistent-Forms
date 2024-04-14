scriptName FC_UnTrackScript extends Quest

MagicEffect Property Effect Auto
Enchantment Property MyEnchantment Auto
Armor Property ModifyArmor Auto
Armor Property ModelSource Auto

bool added = false

Event OnInit()
    RegisterForSingleUpdate(2)
EndEvent

Auto State Default
    Event OnUpdate()
        DynamicPersistentForms.Track(ModifyArmor)
        Enchantment newEnchantment = DynamicPersistentForms.Create(MyEnchantment) as Enchantment
        ModifyArmor.SetName("[this items changes will not persist]")
        ModifyArmor.SetArmorRating(100)
        ModifyArmor.SetGoldValue(1003)
        DynamicPersistentForms.ClearMagicEffects(newEnchantment)
        DynamicPersistentForms.AddMagicEffect(newEnchantment, Effect, 100, 100, 1, 0)
        ModifyArmor.SetEnchantment(newEnchantment)
        DynamicPersistentForms.CopyAppearance(ModelSource, ModifyArmor)
        Game.GetPlayer().AddItem(ModifyArmor,1,true)
        Game.GetPlayer().AddItem(ModelSource,1,true)
        DynamicPersistentForms.UnTrack(ModifyArmor)
        GotoState("Step")
        RegisterForUpdate(0.1)
    EndEvent
EndState
State Step
    Event OnUpdate()
        if(!added && Game.GetPlayer().IsSneaking())
            added = true
            DynamicPersistentForms.Track(ModifyArmor)
            ModifyArmor.SetArmorRating(100)
            ModifyArmor.SetGoldValue(1003)
            ModifyArmor.SetName("[Now it they will]")
            Game.GetPlayer().AddItem(ModifyArmor,1,true)
            UnregisterForUpdate()
        endif
    EndEvent
EndState