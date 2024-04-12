scriptName FC_BowScript extends Quest

MagicEffect Property Effect Auto
Weapon Property StatsSoruce Auto
Weapon Property ModelSource Auto
Enchantment Property MyEnchantment Auto

Event OnInit()
    RegisterForSingleUpdate(2)
EndEvent

Event OnUpdate()
    Weapon newWeapon = DynamicPersistentForms.Craete(StatsSoruce) as Weapon
    if(newWeapon)
        Enchantment newEnchantment = DynamicPersistentForms.Craete(MyEnchantment) as Enchantment
        newWeapon.SetName("My New Dynamic Bow")
        newWeapon.SetBaseDamage(100)
        newWeapon.SetSpeed(newWeapon.GetSpeed()*2.0)
        newWeapon.SetGoldValue(1003)
        newWeapon.SetReach(100)
        DynamicPersistentForms.ClearMagicEffects(newEnchantment)
        DynamicPersistentForms.AddMagicEffect(newEnchantment, Effect, 100, 100, 1, 0)
        newWeapon.SetEnchantment(newEnchantment)
        newWeapon.SetEnchantmentValue(100)
        DynamicPersistentForms.CopyAppearance(ModelSource, newWeapon)
        Game.GetPlayer().AddItem(newWeapon)
    endif
EndEvent