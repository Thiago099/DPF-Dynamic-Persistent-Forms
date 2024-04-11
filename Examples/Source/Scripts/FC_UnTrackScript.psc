scriptName FC_UnTrackScript extends Quest

MagicEffect Property Effect Auto
Enchantment Property MyEnchantment Auto
Armor Property ModifyArmor Auto
Armor Property ModelSource Auto

Event OnInit()
    DynamicPersistentForms.Track(ModifyArmor)
    Enchantment newEnchantment = DynamicPersistentForms.Craete(MyEnchantment) as Enchantment
    ModifyArmor.SetName("[this items changes will not persist]")
    ModifyArmor.SetArmorRating(100)
    ModifyArmor.SetGoldValue(1003)
    DynamicPersistentForms.ClearMagicEffects(newEnchantment)
    DynamicPersistentForms.AddMagicEffect(newEnchantment, Effect, 100, 100, 1, 0)
    ModifyArmor.SetEnchantment(newEnchantment)
    DynamicPersistentForms.CopyAppearance(ModelSource, ModifyArmor)
    Game.GetPlayer().AddItem(ModifyArmor)
    Game.GetPlayer().AddItem(ModelSource)
    DynamicPersistentForms.UnTrack(ModifyArmor)
    RegisterForUpdate(0.1)
EndEvent
; Sneak to create a new form, it will take the place of the original form
bool added = false
Event OnUpdate()
    if(!added && Game.GetPlayer().IsSneaking())
        added = true
        DynamicPersistentForms.Track(ModifyArmor)
        ModifyArmor.SetArmorRating(100)
        ModifyArmor.SetGoldValue(1003)
        ModifyArmor.SetName("[Now it they will]")
        Game.GetPlayer().AddItem(ModifyArmor)
    endif
EndEvent
