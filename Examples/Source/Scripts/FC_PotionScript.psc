scriptName FC_PotionScript extends Quest

Potion Property StatsSource Auto
Potion Property ModelSource Auto
MagicEffect Property Effect Auto

Event OnInit()
    RegisterForSingleUpdate(2)
EndEvent

Event OnUpdate()
    Potion newPotion = DynamicPersistentForms.Create(StatsSource) as Potion
    if(newPotion)
        newPotion.SetName("My New Dynamic Potion")
        DynamicPersistentForms.ClearMagicEffects(newPotion)
        DynamicPersistentForms.AddMagicEffect(newPotion, Effect, 100, 100, 1, 0)
        DynamicPersistentForms.CopyAppearance(ModelSource, newPotion)
        Game.GetPlayer().AddItem(newPotion,5)
        Game.GetPlayer().AddItem(StatsSource,5)
        Game.GetPlayer().AddItem(ModelSource,5)
    endif
EndEvent