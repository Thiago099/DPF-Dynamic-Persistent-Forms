scriptName FC_TrackScript extends Quest

MagicEffect Property Effect Auto
Enchantment Property MyEnchantment Auto
Armor Property ModifyArmor Auto
Armor Property ModelSource Auto

Event OnInit()
    RegisterForSingleUpdate(2)
EndEvent

Event OnUpdate()
    DynamicPersistentForms.Track(ModifyArmor)
    Enchantment newEnchantment = DynamicPersistentForms.Craete(MyEnchantment) as Enchantment
    ModifyArmor.SetName("this is a base skyrim item, its changes will persist")
    ModifyArmor.SetArmorRating(100)
    ModifyArmor.SetGoldValue(1003)
    DynamicPersistentForms.ClearMagicEffects(newEnchantment)
    DynamicPersistentForms.AddMagicEffect(newEnchantment, Effect, 100, 100, 1, 0)
    ModifyArmor.SetEnchantment(newEnchantment)
    DynamicPersistentForms.CopyAppearance(ModelSource, ModifyArmor)
    Game.GetPlayer().AddItem(ModifyArmor)
    Game.GetPlayer().AddItem(ModelSource)
EndEvent