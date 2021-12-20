Sub Buffs
/if (${CheckDebug}) /echo \apCalling Buffs
	| This Line Checks for Open Windows if Merchant or Trade windows are Open Macro will Just Loop
	/call CheckWindows /if (${Macro.Return}==1) /return
|********************************
| Uses the Array to Buff 
|********************************|
| ${Me.Buff[${SpellBuffs[1]}].Duration.Ticks}<150
	/if (!${Me.Moving} && ${Me.PctMana}>=60 && ${BuffTimer}==0 && !${Bool[${Me.XTarget[1].ID}]} && ${Bool[${Spawn[${MTank} radius 80]}]}) {
		| This Bufftimer is set so that the macro only checks buffs every 20 mins this stops a buff from being recast over and over
		/varset BuffTimer 15m
			/for Index 1 to 10
				/if (${Bool[${SpellBuffs[${Index}]}]} && !${Bool[${Me.Buff[${SpellBuffs[${Index}]}]}]} || ${Bool[${SpellBuffs[${Index}]}]} && ${Me.Buff[${SpellBuffs[${Index}]}].Duration.Ticks}<${Math.Calc[${Me.Spell[${SpellBuffs[${Index}]}].Duration.Ticks}*.05]} || ${BuffsCalled}) {
					/if (${Bool[${Me.Book[${SpellBuffs[${Index}]}]}]}) {
						/if (${Spell[${SpellBuffs[${Index}]}].TargetType.Equal[Self]}) {
							/echo \auAbout to Buff Self
						} else {
							/g Guys Im About to Buff ${SpellBuffs[${Index}]}
							/delay 15
						}
						/if (!${Bool[${Me.Gem[${SpellBuffs[${Index}]}]}]} && !${Me.Moving}) {
							/call SpellMem 7 "${SpellBuffs[${Index}]}"
						}	
						/if (${Spell[${SpellBuffs[${Index}]}].TargetType.Equal[Single]}) {
							/for i 0 to ${Group}
							/echo \auBuffing ${Group.Member[${i}].Name} "${SpellBuffs[${Index}]}"
								/call SpellCast ${Group.Member[${i}].ID} "${SpellBuffs[${Index}]}" "Sub Buffs:SpellBuffs[${Index}]"
								/delay ${Math.Calc[${Spell[${SpellBuffs[${Index}]}].RecastTime}/100]} (${Bool[${Me.XTarget}]} || !${Bool[${Me.Casting}]})
							/next i
						} else {
							/call SpellCast ${Me.ID} "${SpellBuffs[${Index}]}" "Sub Buffs:SpellBuffs[${Index}]"
							/delay ${Math.Calc[${Spell[${SpellBuffs[${Index}]}].RecastTime}/100]} (${Bool[${Me.XTarget}]} || !${Bool[${Me.Casting}]})
						}
					}	
				}
			/next Index
	}
	/varset BuffsCalled FALSE 
/return

Sub DS
/if (${CheckDebug}) /echo \apCalling DS
	| This Line Checks for Open Windows if Merchant or Trade windows are Open Macro will Just Loop
	/call CheckWindows /if (${Macro.Return}==1) /return
|********************************
| Uses the Array to Buff 
|********************************|
	/if (!${Bool[${NearestSpawn[NPC radius 50]}]} && !${Me.Moving} && ${Me.PctMana}>=90 && ${DSTimer}==0 && ${Spawn[${MTank}].Distance3D}<=${Spell[${DSBuffs}].Range}) {
		| This Bufftimer is set so that the macro only checks buffs every 20 mins this stops a buff from being recast over and over
		/delay 25
		/varset DSTimer ${Math.Calc[${Spell[${DSBuffs}].Duration}*6]}s
			/if (${Bool[${DSBuffs}]}) {
						/echo \auAbout to DS Buff
						/if (!${Bool[${Me.Gem[${DSBuffs}]}]} && !${Me.Moving}) {
							/call SpellMem 8 "${DSBuffs}"
						}						
						/call SpellCast ${Spawn[${MTank}].ID} "${DSBuffs}" "Sub DS:DSBuffs"
			}
	}
/return