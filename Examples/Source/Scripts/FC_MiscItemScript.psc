scriptName FC_MiscItemScript extends Quest

MiscObject Property StatsSoruce Auto
MiscObject Property ModelSource Auto

Event OnInit()
    RegisterForSingleUpdate(2)
EndEvent

Event OnUpdate()
    MiscObject newMiscObject = DynamicPersistentForms.Create(StatsSoruce) as MiscObject
    if(newMiscObject)
        newMiscObject.SetName("My New Dynamic Misc Item")
        newMiscObject.SetGoldValue(1003)
        DynamicPersistentForms.CopyAppearance(ModelSource, newMiscObject)
        Game.GetPlayer().AddItem(newMiscObject,1,true)
        Game.GetPlayer().AddItem(StatsSoruce,1,true)
        Game.GetPlayer().AddItem(ModelSource,1,true)
    endif
EndEvent