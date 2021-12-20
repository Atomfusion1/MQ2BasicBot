| You Must Send the Target ID and the Name of the Spell 

Sub SpellCast(int IDTarget, string SpellCastZ, string FromSub)
	/call CheckBook ${IDTarget} "${SpellCastZ}"
	/if (${Macro.Return}) {
		/echo \arIn CheckBook: Cant Cast ${FromSub}
		/return
	}
	/call CheckWindows 
	/if (${Macro.Return}) {
		/echo \arIn CheckWindows: Cant Cast ${SpellCastZ}
		/return
	}
/if (${CheckDebug}) /echo \arCalling SpellCast
	/if (${Window[SpellBookWnd].Open}) /squelch ${Window[SpellBookWnd].DoClose}
	/if (!${Me.SpellReady[${Spell[${SpellCastZ}]}]}) {
		/delay 20 ${Me.SpellReady[${Spell[${SpellCastZ}]}]}
	} else /if (!${Me.SpellReady[${Spell[${SpellCastZ}]}]}) {
		/return
	}
	/if (${Target.ID}!=${IDTarget}) {
		/squelch /target clear
		/delay 5
		/squelch /target id ${IDTarget}
		/delay 5s ${Bool[${Target}]}
	}
	/cast "${SpellCastZ}"
	/delay 8 
	/squelch /cast "${SpellCastZ}"
	/delay 8 
	/squelch /cast "${SpellCastZ}"
	/delay 16 !${Bool[${Me.Casting}]}
	| needed Check if it gets into this and its negative delay locks 
	/if (${Math.Calc[${Spell[${SpellCastZ}].MyCastTime.Float}*10-16]} > 0 && ${Bool[${Me.Casting}]}) /docommand /delay ${Math.Calc[${Spell[${SpellCastZ}].MyCastTime.Float}*10-16]} (!${Bool[${Me.Casting}]})
/if (${CheckDebug}) /echo \arLeaving SpellCast
/return

Sub CheckWindows
/if (${CheckDebug}) /echo \aoCalling CheckWindows
	| This Line Checks for Open Windows if Merchant or Trade windows are Open Macro will Just Loop
	/if (${Window[TradeWnd].Open} || ${Window[MerchantWnd].Open} || ${Window[BazaarWnd].Open} || ${Window[BigBankWnd].Open} || ${Window[GiveWnd].Open}) {
		/if (${Window[TradeWnd].Open} && ${Bool[${AuthList.Find[${Window[TradeWnd].Child[TRDW_HisName].Text}]}]}) {
		} else {
		| /cleanup
		/return true
		} 
	}
/return false

Sub CheckBook(int IDTarget, string SpellCastZ)
/if (${CheckDebug}) /echo \aoCalling CheckBook
	/if (${SpellCastZ.Left[1].Equal["#"]}) {
		/echo \auDefault Spell Not Changed ${SpellCastZ.Left[1].Equal["#"]}
	} else /if (${Bool[${Me.Book[${Spell[SpellCastZ]}]}]}) {
		/echo \auWARNING Do NOT have "${SpellCastZ}" in My Book
		/beep
	} else /if (!${Bool[${Spell[${SpellCastZ}]}]}) {
		/echo \auWARNING Do NOT have "${SpellCastZ}" As a Spell
		/beep
	} else /if (!${Bool[${Me.Gem[${SpellCastZ}]}]}) {
		/echo \auWARNING Do NOT have "${SpellCastZ}" Memorized 
		/beep
	} else /return false
/return true

| Mem Spell in the Slot ID 
Sub SpellMem(int IDSlot, string SpellMemZ)
/if (${CheckDebug}) /echo \aoCalling SpellMem
	/memspell ${IDSlot} "${SpellMemZ}"
	/if (${Math.Calc[${Spell[${SpellMemZ}].MyCastTime.Float}*10-16]}>0) /docommand /delay ${Math.Calc[${Spell[${SpellMemZ}].RecoveryTime.Float}*10]}
	/if (${Math.Calc[${Spell[${SpellMemZ}].MyCastTime.Float}*10-16]}>0) /docommand /delay ${Math.Calc[${Spell[${SpellMemZ}].MyCastTime.Float}*10]}
	/delay 5s ${Me.SpellReady[${Spell[${SpellMemZ}]}]}
/return

Sub Sitting
/if (${CheckDebug}) /echo \aoCalling Sitting
	| This Line Checks for Open Windows if Merchant or Trade windows are Open Macro will Just Loop
	/call CheckWindows /if (${Macro.Return}==1) /return
	/if (${Bool[${Spawn[NPC Targetable los radius 200]}]} && ${Bool[${Me.XTarget[1].ID}]}) {
	/squelch /stand
	/return
	}
	
	/if (!${Bool[${Spawn[${MTank}]}]} || ${Me.Dead} || ${IWantToMelee} && ${Bool[${Spawn[NPC Targetable los radius 50]}]}) /return
| This section is to Sit the cleric So mana regen is faster 
	/if (${Bool[${Me.Casting}]} || ${Me.Moving}) /varset SitTimer 10s
	/if (${SitTimer}==0 && !${Me.Moving} && ${Me.PctMana}<=100 && !${Me.Sitting} && !${Bool[${Me.Casting}]}) {
	| I only ever buy Drogmor so i never added horse or anything else 
		/if (${IHaveMount} && !${Bool[${Me.Buff["Summon Drogmor"]}]}) {
			/Stand
			/delay 5
			/squelch /cast item "Small White Drum"
			/squelch /cast item "Small Red Drum"
			/squelch /cast item "Small Green Drum"
			/squelch /cast item "Small Black Drum"
			/delay 35
		}
		/if (${Math.Distance[${LocY},${LocX}]}<=25) {
			/sit
			/varset SitTimer ${StaticSitTimer}s
		}
	}
/return

Sub FoodItems(int IDTarget, string SFood)
/declare FoodID int local 0
/echo \auGiving ${SFood} to ${Spawn[${IDTarget}]}
/delay 15
	/if (${Bool[${Cursor.ID}]}) /return
	/squelch /target id ${IDTarget}
	/delay 10 (${Bool[${Target}]})
	/while (${Spawn[${Target}].Distance3D}>20 && ${Spawn[${Target}].Distance3D}<100) {
			/echo \awcalling RunBack with ${Spawn[${IDTarget}]}
			/call MovingTo ${Spawn[${IDTarget}]} 19
	
	}
	| Lets give you time to stop 
	/delay 20 (!${Me.Moving})
	/delay 10
	/if (!${Bool[${Target}]} || ${Spawn[${Target}].Distance3D}>100) /return
	/if (${SFood.Find[Drink]}) /varset FoodID 1
	/if (${SFood.Find[Food]}) /varset FoodID 2
		/if (!${Bool[${Spell[${SpellFood[${FoodID}]}]}]}) /return
			/if (!${Bool[${Me.Gem[${SpellFood[${FoodID}]}]}]} && !${Me.Moving} ) {
				/call SpellMem 8 "${SpellFood[${FoodID}]}"
				/delay 20
			}
			| This Summons Items 
			/echo \au${FoodID} foodID
			/echo \auSpellFood ${SpellFood[${FoodID}]}
			/echo \auSpell ID ${Spell[${SpellFood[${FoodID}]}].ID}
			/delay 10
			/call SpellCast ${Me.ID} "${SpellFood[${FoodID}]}" "Sub FoodItems:SpellFood[${FoodID}]"
			/delay 10
			/if (${Cursor.NoRent}) {
				/echo \auItem in hand giving Person ${IDTarget}
				/squelch /target id ${Spawn[=${IDTarget}].ID}
				/delay 10 (${Target.ID}==${Spawn[=${IDTarget}].ID})
				/delay 10 (${Bool[${Target}]})
				/if (${Target.ID} == ${IDTarget} && ${Spawn[${Target}].Distance3D}<25) {
					/click left target
					/delay 10 ${InvSlot[trade1].Item.ID}
					/delay 5
					/if (${Window[GiveWnd].Open}) /nomodkey /notify GiveWnd GVW_Give_Button leftmouseup
					/delay 10
					/delay 50 !${Window[GiveWnd].Open}
				}
			}
			/autoinv
	}	
	/autoinv
/return

Sub CastAnything(int IDTarget, string SpellCastZ)
	/if (${Bool[${Me.Book[${Spell[SpellCastZ]}]}]}) { | detect if spell
		/echo Casting Spell 
		/call SpellCast ${Me.ID} "SpellCastZ" "Sub CastAnything:SpellCastZ"
	} else /if () {			| AA 
		
	} else /if () {			| Combat Ability
	
	} else /if () {			| Ability
	
	} else /if () {			| Item
	
	} else {| Nothing 
	
	}
/return 