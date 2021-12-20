Sub Twist
/if (${CheckDebug}) /echo \amCalling Twist
	/if (${Window[TradeWnd].Open} || ${Window[MerchantWnd].Open} || ${Window[BazaarWnd].Open} || ${Window[BigBankWnd].Open} || ${Window[GiveWnd].Open} || ${Window[SpellBookWnd].Open}) /return
	/if (!${ImTwisting}) /return
	/if (${IWantToMez} && ${Me.XTarget} > 1 && ${Me.XTarget[2].Distance3D} < 100 && ${MezzingMobs} && !${MezImmune} && ${MezTwistTimer} == 0) {
		/call TwistMez
	} else /if (${IWantToSnare} && ${Me.XTarget}>0 && ${Target.PctHPs}<${SnareHPs} && ${Bool[${Target}]} && !${SnareImmune} && ${Target.Moving}) {
		/call TwistSnare	
	} else {
		/if (${Bool[${Me.Casting}]} && ${TwistTimer} == 0) {
		| This Starts the next spell Cast first stopping current Song being Sustained 
			/stopcast
			/delay 10 !${Bool[${Me.Casting}]}
		}
		/if (${Bool[${Me.Gem[Selo's Accelerating Chorus]}]} && !${Bool[${Me.Casting}]} && ${SelosTimer} == 0) {
			| Selos Accelerating Chorus Twisting if on spell gem 
			/cast "Selo's Accelerating Chorus"
			/varset TwistTimer ${Math.Calc[${Me.Spell[Selo's Accelerating Chorus].MyCastTime.TotalSeconds}*10+8]}
			/varset SelosTimer ${Math.Calc[${Me.Spell[Selo's Accelerating Chorus].MyDuration.TotalSeconds}*10-150]}
		}

		| Non Combat Twisting 
		/if (${Bool[${Me.Book[${SongTwist[${SongCounter}]}]}]} && !${Me.Combat} && !${Bool[${Me.Casting}]}) {
			/cast "${SongTwist[${SongCounter}]}"
			/varset TwistTimer ${Math.Calc[${Me.Spell[${SongTwist[${SongCounter}]}].MyCastTime.TotalSeconds}*10+8]}
			/varset SongCounter ${Math.Calc[${SongCounter}+1]}
		}
		| Combat Twisting 
		/if (${Bool[${Me.Book[${CSongTwist[${SongCounter}]}]}]} && ${Me.Combat} && !${Bool[${Me.Casting}]}) {
			/cast "${CSongTwist[${SongCounter}]}"
			/varset TwistTimer ${Math.Calc[${Me.Spell[${CSongTwist[${SongCounter}]}].MyCastTime.TotalSeconds}*10+8]}
			/varset SongCounter ${Math.Calc[${SongCounter}+1]}
		}
		/if (${SongCounter} > 4) {
			/varset SongCounter 1
		}
		
		/call TwistBook "${CSongTwist[${SongCounter}]}"
		/if (${Macro.Return}) {
			/varset SongCounter ${Math.Calc[${SongCounter}+1]}
			/if (${SongCounter} > 4) {
			/varset SongCounter 1
			}
		} 		
	}
/return

Sub TwistMez
/if (${CheckDebug}) /echo \amCalling TwistMez
		| This Line Checks for Open Windows if Merchant or Trade windows are Open Macro will Just Loop
	/call CheckWindows 
	/if (${Macro.Return}==1) /return
	| Bard Song Max of 3 Hold 3 Mez
	/declare varname int local
	/declare MezFail bool local FALSE
	/for varname 4 downto 2
		/if (!${Bool[${Me.XTarget[${varname}].ID}]} || ${MezTwistTimer} != 0) /continue
		/if (${Bool[${Me.Casting}]}) {
			/stopcast
			/delay 10 !${Bool[${Me.Casting}]}
		}
		/delay 10 (!${Bool[${Me.Casting}]})

		/if (!${Bool[${Me.Casting}]}) {
			/squelch /attack off
			/delay 10 (!${Bool[${Me.Combat}]})
			/squelch /target clear
			/delay 5
			/squelch /target id ${Me.XTarget[${varname}].ID}
			/delay 2s ${Bool[${Target}]}		
			/if (${Bool[${Me.Book[${SpellMez}]}]} && !${Bool[${Me.Casting}]}) {
				/echo \amTrying to mez ${Target} with ${SpellMez}
				/cast "${SpellMez}"
				/delay 10 ${Bool[${Me.Casting}]}
				/if (!${Bool[${Me.Casting}]}) {
					/cast "${SpellMez}"
				}
				/delay 30 (!${Bool[${Me.Casting}]})
				
			}	
			/delay 10 (${Bool[${Target.Mezzed}]})
			/if (${Bool[${Target.Mezzed}]}) {
				/echo \apMezzed ${Target.Name}
				/g Mezzed a ${Target.CleanName}
			} else {
				/echo \arMEZ FAIL
				/varset MezFail TRUE
			}
		}
	/if (${varname} == 2 && !${MezFail}) /varset MezTwistTimer 150	
	/cast 1
	/next varname
	/squelch /target clear
	/delay 5
	/squelch /target id ${Me.XTarget[1].ID}
	/squelch /attack on
	/delay 2s ${Bool[${Target}]}	
/return

Sub TwistSnare
/if (${CheckDebug}) /echo \amCalling TwistSnare
		/if (${Me.Casting.Name.Equal[${SpellSnare}]}) /return
		/stopcast
		/delay 10 !${Bool[${Me.Casting}]}
		/cast "${SpellSnare}"
		/varset TwistTimer ${Math.Calc[${Me.Spell[${SpellSnare}].MyCastTime.TotalSeconds}*10+8]}
		|/echo \ar Mob is Fleeing I am now Snaring or Rooting
/return

Sub TwistBook(string SpellCastZ)
/if (${CheckDebug}) /echo \amCalling TwistBook
	/if (${SpellCastZ.Left[1].Equal["#"]}) {
	} else /if (${Bool[${Me.Book[${Spell[SpellCastZ]}]}]} ) {
		/echo \arWARNING Do NOT have "${SpellCastZ}" in My Book
		/beep
	} else /if (!${Bool[${Spell[${SpellCastZ}]}]}) {
		/echo \arWARNING Do NOT have "${SpellCastZ}" As a Spell
		/beep
	} else /if (!${Bool[${Me.Gem[${SpellCastZ}]}]}) {
		/echo \arWARNING Do NOT have "${SpellCastZ}" Memorized 
		/beep
	} else /return false
/return true