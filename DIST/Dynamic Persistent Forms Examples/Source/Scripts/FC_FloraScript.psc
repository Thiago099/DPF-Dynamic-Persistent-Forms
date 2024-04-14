scriptName FC_FloraScript extends Quest

Flora Property FloraSource Auto
LeveledItem Property IngredientSource Auto
Ingredient Property IngredientItem Auto

Event OnInit()
    RegisterForSingleUpdate(2)
EndEvent

Event OnUpdate()
    Flora newFlora = DynamicPersistentForms.Create(FloraSource) as Flora
    LeveledItem myLeveledItem = DynamicPersistentForms.Create(IngredientSource) as LeveledItem
    if(newFlora && myLeveledItem)
        myLeveledItem.AddForm(IngredientItem, 1, 10)
        newFlora.SetIngredient(myLeveledItem)
        newFlora.SetName("My New Dynamic Flora")
        Game.GetPlayer().PlaceAtMe(newFlora)
    endif
EndEvent