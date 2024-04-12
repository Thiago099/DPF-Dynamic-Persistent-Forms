scriptName DynamicPersistentForms hidden

; Creates a new form that is a copy of given base form, changes to that form will be persisted in the save game.
Form function Craete(Form item) global native
; Dispose a form that was created using the previous function.
function Dispose(Form item) global native 

;Tracks a form that was not created using this mod, all changes made to it will be persisted on the save game
function Track(Form item) global native
;Un-tracks a form that was not created using this mod, all changes made to it will no longer be persisted on the save game
function UnTrack(Form item) global native


; Modify forms

function AddMagicEffect(Form item, MagicEffect effect, float magnitude, int area, int duration, float cost) global native
function ClearMagicEffects(Form item) global native
function CopyMagicEffects(Form from, Form to) global native
function CopyAppearance(Form source, Form target) global native

function SetSpellTomeSpell(Book target, Spell teaches) global native

function SetSpellAutoCalculate(Spell spell, bool value) global native
function SetSpellCostOverride(Spell spell, int value) global native
function SetSpellChargeTime(Spell spell, float value) global native
function SetSpellCastDuration(Spell spell, int value) global native
function SetSpellRange(Spell spell, int value) global native
function SetSpellCastingPerk(Spell spell, Perk value) global native

function SetEnchantmentAutoCalculate(Enchantment enchantment, bool value) global native
function SetEnchantmentChargeOverride(Enchantment enchantment, int value) global native
function SetEnchantmentCostOverride(Enchantment enchantment, int value) global native
function SeEnchantmentChargeTime(Enchantment enchantment, float value) global native


function SetAmmoDamage(Ammo ammo, float damage) global native
function SetAmmoProjectile(Ammo ammo, Projectile projectile) global native

; None = 0
; Petty = 1
; Lesser = 2
; Common = 3
; Greater = 4
; Grand = 5
function SetSoulGemCapacity(SoulGem gem, int capacity) global native
function SetSoulGemCurrentSoul(SoulGem gem, int capacity) global native
function LinkSoulGems(SoulGem empty, SoulGem filled) global native