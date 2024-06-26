scriptName FC_IngredientScript extends Quest

Ingredient Property StatsSource Auto
Ingredient Property ModelSource Auto
Ingredient Property EffectSource Auto
MagicEffect Property Effect Auto

Event OnInit()
    RegisterForSingleUpdate(2)
EndEvent

Event OnUpdate()
    Ingredient newIngredient = DynamicPersistentForms.Create(StatsSource) as Ingredient
    if(newIngredient)
        newIngredient.SetName("My New Dynamic Ingredient")
        DynamicPersistentForms.ClearMagicEffects(newIngredient)
        DynamicPersistentForms.CopyMagicEffects(EffectSource, newIngredient)
        DynamicPersistentForms.CopyAppearance(ModelSource, newIngredient)
        Game.GetPlayer().AddItem(newIngredient,5,true)
        Game.GetPlayer().AddItem(StatsSource,5,true)
        Game.GetPlayer().AddItem(ModelSource,5,true)
        Game.GetPlayer().AddItem(EffectSource,5,true)
    endif
EndEvent