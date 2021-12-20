| Sub Control for Pets

Sub SummonPet
	/if (${CheckDebug}) /echo \atCalling SummonPet
	| This Line Checks for Open Windows if Merchant or Trade windows are Open Macro will Just Loop
	/call CheckWindows /if (${Macro.Return}==1) /return
	/if (${CampUp} && ${Math.Distance[${LocY},${LocX}]}>35) /return
	/if (!${Bool[${Me.Gem[${PetSpell}]}]} && !${Me.Moving} && ${Bool[${Spell[${PetSpell}]}]}!=FALSE && !${Bool[${Me.Pet.ID}]}) {
		/call SpellMem 8 "${PetSpell}"
	}
	/if (!${Bool[${NearestSpawn[NPC radius 50]}]} && ${Me.PctHPs}>90 && !${Me.Moving} && ${Me.PctMana}>=65 && !${Bool[${Me.Pet.ID}]}) {
		/call SpellCast ${Me.ID} "${PetSpell}" "Sub SummonPet:PetSpell"
		/if (${Bool[${Me.Pet.ID}]}) {
			/varset PetBuffTimer 0		
			/call SpellMem 7 "Tiny Companion"
			/call SpellCast ${Me.Pet.ID} "Tiny Companion" "Sub SummonPet:Tiny Companion"
			/delay 10
			/call SpellCast ${Me.Pet.ID} "Tiny Companion" "Sub SummonPet:Tiny Companion"
			/delay 10
		}
		/pet who leader
		/if (${Me.Class.ShortName.Equal[MAG]}) /call PetItems ${Me.Pet.ID} 
	}
/return

Sub HealPet
	/if (${CheckDebug}) /echo \atCalling HealPet
	| This Line Checks for Open Windows if Merchant or Trade windows are Open Macro will Just Loop
	/call CheckWindows /if (${Macro.Return}==1) /return
	/if (!${Bool[${Spawn[${Me.Pet.ID}]}]}) /return
	| This is for healing your pet Can Be Adjusted Defualt = 40%
	/if (${Spawn[${Me.Pet}].PctHPs}<=70 && ${Spawn[${Me.Pet}].Distance3D}<=${Spell[${PetHeal}].Range} && ${Me.Gem[1].Mana}<${Me.CurrentMana} && ${Bool[${Me.Gem[${PetHeal}]}]}) {
		/docommand /timed ${Math.Calc[(${Spell[${PetHeal}].MyCastTime.Float}*10)]} /if (${Spawn[${Me.Pet}].PctHPs}>97) /stopcast
		/call SpellCast ${Me.Pet.ID} "${PetHeal}" "Sub HealPet:PetHeal"
		/echo \ayCasting a Pet Heal on ${Me.Pet}
		/varset SitTimer ${StaticSitTimer}s
		/varset JustHealed TRUE
		/squelch /target clear
	}
/return

Sub PetBuffs
	/if (${CheckDebug}) /echo \atCalling PetBuffs
	| This Line Checks for Open Windows if Merchant or Trade windows are Open Macro will Just Loop
	/call CheckWindows /if (${Macro.Return}==1) /return
| This section checks for pet buffs
	/if (!${Bool[${Spawn[${Me.Pet.ID}]}]}) /return
	/if (!${Bool[${NearestSpawn[NPC radius 50]}]} && !${Me.Moving} && ${Me.PctMana}>=40 && ${PetBuffTimer}==0) {
	| This PetBufftimer is set so that the macro only checks buffs every 20 mins this stops a buff from being recast over and over
		/varset PetBuffTimer 10m
			/for Index 1 to 5
			/if (!${Bool[${Me.PetBuff[${PetBuffs[${Index}]}]}]} && ${Bool[${Me.Book[${PetBuffs[${Index}]}]}]} ) {
				/if (!${Bool[${Me.Gem[${PetBuffs[${Index}]}]}]} && !${Me.Moving} ) {
					/call SpellMem 7 "${PetBuffs[${Index}]}"
				}
				/call SpellCast ${Me.Pet.ID} "${PetBuffs[${Index}]}" "Sub PetBuffs:PetBuffs[${Index}]"
				/echo \auPet Buffing ${PetBuffs[${Index}]}
			}
		/next Index
		/delay 20
		/for Index 1 to 3
			/if (${Bool[${PetBuffItemSlot[${Index}]}]}!=FALSE && !${Bool[${Me.PetBuff[${PetBuffItemSpellName[${Index}]}]}]}) {
				/if (!${Me.Moving} ) {
					/echo \auUsing Item on Pet
					/cast item ${PetBuffItemSlot[${Index}]} 
					/delay 5
					/delay 30 !${Bool[${Me.Casting}]}
					/if (${Bool[${Me.Pet.ID}]}) {
						/call SpellMem 7 "Tiny Companion"
						/call SpellCast ${Me.Pet.ID} "Tiny Companion" "Sub PetBuffs:Tiny Companion"
						/delay 10
						/call SpellCast ${Me.Pet.ID} "Tiny Companion" "Sub PetBuffs:Tiny Companion"
						/delay 10
					}
				}
				/target ${Me.Pet}
			}
		/next Index
	}
/return

Sub PetItems(int IDTarget)
	/if (${CheckDebug}) /echo \atCalling PetItems
/echo \auEntering Pet Items My Target is ${IDTarget}
	/if (${Bool[${Cursor.ID}]}) /return
	|Lets Find Target and Run To
	/squelch /target id ${IDTarget}
	/delay 10 (${Bool[${Target}]})
	/while (${Spawn[${Target}].Distance3D}>20 && ${Spawn[${Target}].Distance3D}<100) {
			/call MovingTo ${Spawn[${IDTarget}]} 19
	
	}
	| Lets give you time to stop 
	/delay 20 (!${Me.Moving})
	/delay 10
	| This Summons Items	
	/if (!${Bool[${Target}]} || ${Spawn[${Target}].Distance3D}>100) /return
	/for i 1 to 5
	/if (!${Bool[${Spell[${PetItem[${i}]}]}]}) /continue
		/if (!${Bool[${Me.Gem[${PetItem[${i}]}]}]} && !${Me.Moving} ) {
			/call SpellMem 8 "${PetItem[${i}]}"
			/delay 20
		} 
		/call SpellCast ${Me.ID} "${PetItem[${i}]}" "Sub PetItems:PetItems[${i}]"
		/delay 10
		/if (${Cursor.NoRent}) {
			/echo \auItem in hand giving to pet ${IDTarget}
			/squelch /target id ${IDTarget}
			/delay 10 (${Bool[${Target}]})
			/if (${Target.ID} == ${IDTarget} && ${Spawn[${Target}].Distance3D}<25) {
				/click left target
				/delay 10 ${InvSlot[trade${i}].Item.ID}
				/delay 5
				/if (${Window[GiveWnd].Open}) /nomodkey /notify GiveWnd GVW_Give_Button leftmouseup
				/delay 10
				/delay 50 !${Window[GiveWnd].Open}
			}
		}
		/autoinv
	/next i	
	/autoinv
/return
		