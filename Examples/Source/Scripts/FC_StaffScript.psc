scriptName FC_StaffScript extends Quest

MagicEffect Property Effect Auto
Weapon Property StatsSoruce Auto
Weapon Property ModelSource Auto
Enchantment Property MyEnchantment Auto

Event OnInit()
    Weapon newStaff = DynamicPersistentForms.Craete(StatsSoruce) as Weapon
    if(newStaff)
        Enchantment newEnchantment = DynamicPersistentForms.Craete(MyEnchantment) as Enchantment
        newStaff.SetName("My New Dynamic Staff")
        newStaff.SetGoldValue(1003)
        DynamicPersistentForms.ClearMagicEffects(newEnchantment)
        DynamicPersistentForms.AddMagicEffect(newEnchantment, Effect, 100, 100, 1, 0)
        newStaff.SetEnchantmentValue(1000)
        newStaff.SetEnchantment(newEnchantment)
        DynamicPersistentForms.CopyAppearance(ModelSource, newStaff)
        Game.GetPlayer().AddItem(newStaff)
        Game.GetPlayer().AddItem(StatsSoruce)
        Game.GetPlayer().AddItem(ModelSource)
    endif
EndEvent

