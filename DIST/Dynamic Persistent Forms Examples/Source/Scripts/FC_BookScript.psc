scriptName FC_BookScript extends Quest


Book Property StatsSource Auto
Book Property ModelSource Auto
Spell Property TeachesSpell Auto

Event OnInit()
    RegisterForSingleUpdate(2)
EndEvent

Event OnUpdate()
    Book newBook = DynamicPersistentForms.Create(StatsSource) as Book
    if(newBook)
        newBook.SetName("My New Dynamic Tome")
        DynamicPersistentForms.SetSpellTomeSpell(newBook, TeachesSpell)
        DynamicPersistentForms.CopyAppearance(ModelSource, newBook)
        Game.GetPlayer().AddItem(newBook,1,true)
        Game.GetPlayer().AddItem(StatsSource,1,true)
        Game.GetPlayer().AddItem(ModelSource,1,true)
    endif
EndEvent