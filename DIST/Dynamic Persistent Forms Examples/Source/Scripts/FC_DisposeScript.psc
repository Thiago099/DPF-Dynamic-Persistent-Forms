scriptName FC_DisposeScript extends Quest

Armor Property StatsSoruce Auto
Armor newArmor

Event OnInit()
    RegisterForSingleUpdate(2)
EndEvent

bool added = false
Auto State Start
    Event OnUpdate()
        newArmor = DynamicPersistentForms.Create(StatsSoruce) as Armor
        if(newArmor)
            newArmor.SetName("this item will be deleted")
            Game.GetPlayer().AddItem(newArmor)
            ; You must take care of removing the item from the inventory it is in, however it
            ; will no longer be tracked by this mod
            DynamicPersistentForms.Dispose(newArmor)
        endif
        GotoState("Step")
        RegisterForUpdate(0.1)
    EndEvent
EndState
State Step
    Event OnUpdate()
        if(!added && Game.GetPlayer().IsSneaking())
            Game.GetPlayer().RemoveItem(newArmor)
            added = true
            newArmor = DynamicPersistentForms.Create(StatsSoruce) as Armor
            newArmor.SetName("This item will take its place")
            Game.GetPlayer().AddItem(newArmor)
            UnregisterForUpdate()
        endif
    EndEvent
EndState