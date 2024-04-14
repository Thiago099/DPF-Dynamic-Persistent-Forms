scriptName FC_ArmorScript extends Quest

MagicEffect Property Effect Auto
Enchantment Property MyEnchantment Auto
Armor Property StatsSoruce Auto
Armor Property ModelSource Auto

Event OnInit()
    RegisterForSingleUpdate(2)
EndEvent

Event OnUpdate()
    Armor newArmor = DynamicPersistentForms.Create(StatsSoruce) as Armor
    if(newArmor)
        Enchantment newEnchantment = DynamicPersistentForms.Create(MyEnchantment) as Enchantment
        newArmor.SetName("My New Dynamic Armor")
        newArmor.SetArmorRating(100)
        newArmor.SetGoldValue(1003)
        DynamicPersistentForms.ClearMagicEffects(newEnchantment)
        DynamicPersistentForms.AddMagicEffect(newEnchantment, Effect, 100, 100, 1, 0)
        newArmor.SetEnchantment(newEnchantment)
        DynamicPersistentForms.CopyAppearance(ModelSource, newArmor)
        Game.GetPlayer().AddItem(newArmor,1,true)
        Game.GetPlayer().AddItem(StatsSoruce,1,true)
        Game.GetPlayer().AddItem(ModelSource,1,true)
    endif
EndEvent