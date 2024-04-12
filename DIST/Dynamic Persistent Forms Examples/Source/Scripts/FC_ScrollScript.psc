scriptName FC_ScrollScript extends Quest

MagicEffect Property EffectModel Auto
MagicEffect Property EffectBase Auto
Scroll Property StatsSource Auto
Scroll Property ModelSource Auto

Event OnInit()
    Scroll newScroll = DynamicPersistentForms.Craete(StatsSource) as Scroll
    MagicEffect newEffect = DynamicPersistentForms.Craete(EffectBase) as MagicEffect
    if(newScroll && newEffect)
        newScroll.SetName("My New Dynamic Scroll")
        DynamicPersistentForms.ClearMagicEffects(newScroll)
        DynamicPersistentForms.AddMagicEffect(newScroll, newEffect, 100, 100, 1, 0)
        DynamicPersistentForms.CopyAppearance(ModelSource, newScroll)
        DynamicPersistentForms.CopyAppearance(EffectModel, newEffect)
        Game.GetPlayer().AddItem(newScroll,5)
        Game.GetPlayer().AddItem(StatsSource,5)
        Game.GetPlayer().AddItem(ModelSource,5)
    endif
EndEvent
