scriptName FC_AmmoScript extends Quest

Ammo Property StatsSoruce Auto
Ammo Property ModelSource Auto
Projectile Property SetProjectile Auto

Event OnInit()
    Ammo newAmmo = DynamicPersistentForms.Craete(StatsSoruce) as Ammo
    if(newAmmo)
        newAmmo.SetName("My New Dynamic Ammo")
        DynamicPersistentForms.SetAmmoDamage(newAmmo, 100)
        DynamicPersistentForms.SetAmmoProjectile(newAmmo, SetProjectile)
        newAmmo.SetGoldValue(1003)
        DynamicPersistentForms.CopyAppearance(ModelSource, newAmmo)
        Game.GetPlayer().AddItem(newAmmo,100)
    endif
EndEvent

