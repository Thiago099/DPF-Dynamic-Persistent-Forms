scriptName FC_DisposeScript extends Quest

Armor Property StatsSoruce Auto
Armor newArmor
Event OnInit()
    newArmor = DynamicPersistentForms.Craete(StatsSoruce) as Armor
    if(newArmor)
        newArmor.SetName("this item will be deleted")
        Game.GetPlayer().AddItem(newArmor)
        ; You must take care of removing the item from the inventory it is in, however it
        ; will no longer be tracked by this mod
        DynamicPersistentForms.Dispose(newArmor)
    endif
    RegisterForUpdate(0.1)
EndEvent
bool added = false
Event OnUpdate()
    if(!added && Game.GetPlayer().IsSneaking())
        Game.GetPlayer().RemoveItem(newArmor)
        added = true
        newArmor = DynamicPersistentForms.Craete(StatsSoruce) as Armor
        newArmor.SetName("This item will take its place")
        Game.GetPlayer().AddItem(newArmor)
    endif
EndEvent
