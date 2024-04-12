scriptName FC_SoulGemScript extends Quest

SoulGem Property BaseEmptySoulGem Auto
SoulGem Property BaseFilledSoulGem Auto
MiscObject Property AppearenceSource Auto

Event OnInit()
    RegisterForSingleUpdate(2)
EndEvent

Event OnUpdate()
    SoulGem newEmptySoulGem = DynamicPersistentForms.Create(BaseEmptySoulGem) as SoulGem
    SoulGem newFilledSoulGem = DynamicPersistentForms.Create(BaseEmptySoulGem) as SoulGem
    if(newEmptySoulGem&&newFilledSoulGem)
        newEmptySoulGem.SetName("My New Dynamic Soul Gem")
        newEmptySoulGem.SetGoldValue(1003)

        newFilledSoulGem.SetName("My New Dynamic Soul Gem")
        newFilledSoulGem.SetGoldValue(1003)

        DynamicPersistentForms.SetSoulGemCapacity(newEmptySoulGem, 2)
        DynamicPersistentForms.SetSoulGemCapacity(newFilledSoulGem, 2)
        DynamicPersistentForms.SetSoulGemCurrentSoul(newFilledSoulGem, 2)
        DynamicPersistentForms.LinkSoulGems(newEmptySoulGem, newFilledSoulGem)

        DynamicPersistentForms.CopyAppearance(AppearenceSource, newEmptySoulGem)
        DynamicPersistentForms.CopyAppearance(AppearenceSource, newFilledSoulGem)
        Game.GetPlayer().AddItem(newEmptySoulGem)
        Game.GetPlayer().AddItem(newFilledSoulGem)
    endif
EndEvent