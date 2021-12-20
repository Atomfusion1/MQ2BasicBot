
Sub Looting
	/declare LootSlot 	int inner 0
	/declare LootTotal  int inner  0 
	/declare LoopTimeout int local 0 | Setup breakout for pulling loop 
/if (${CheckDebug}) /echo \atCalling Looting
	/if (${MobPulling}==1 && ${Me.Name.Equal[${MTank}]} || ${LazyMode} && ${Me.Name.Equal[${MTank}]} ) {
	} else /return
	| This Line Checks for Open Windows if Merchant or Trade windows are Open Macro will Just Loop
	/call CheckWindows /if (${Macro.Return}==1) /return
	/if (${SpawnCount[NPC CORPSE radius 170]}<3 || ${LootTimer}!=0 || ${Bool[${Me.XTarget}]} || ${Spawn[=${MainCamp}].Distance} > 75) /return 
	/while (${Me.XTarget}==0 &&  ${SpawnCount[NPC CORPSE radius 170]}!=0) {
	/if (${CheckDebug}) /echo \arLooting Loop ${LoopTimeout}
		/varset LoopTimeout ${Math.Calc[${LoopTimeout}+1]}
		/if (${LoopTimeout}>10) { 
			/varset ${LootTimer} 45s
			/call RunBack ${MainCamp} 20
			/return
		}
		/if (${LootTimer}!=0) /break
		/target corpse
		/if (${Spawn[${Target}].Distance3D}>=5) {
			/squelch /face fast nolook
			/keypress Up Hold
			/delay 15 ${Spawn[${Target}].Distance3D}<=5
			/keypress Up
			/keypress Up
		}
		|/delay 2s 
		/loot 
		| Don't make this too low, or ${Corpse.Items} will be wrong, since win has not populated
		/delay 2s
		/if (!${Corpse.Items}) { 
		  |/echo NO LOOT! Cheap Bastard! 
		} else {
			/varset LootTotal ${Corpse.Items} 
			/for LootSlot 1 to ${LootTotal} 
				/if (${Bool[${Corpse.Item[${LootSlot}].Name.Find[Defiant]}]} || ${Bool[${Corpse.Item[${LootSlot}].Name.Find[Adept]}]} || ${Bool[${Corpse.Item[${LootSlot}].Name.Find[Combatant]}]} || ${Bool[${Corpse.Item[${LootSlot}].Name.Find[Metallic]}]}) { 
					/timed 1 /shift /itemnotify Loot${LootSlot} leftmouseup
					/delay 3s ${Cursor.ID}
					/echo \amKeeping a ${Cursor.Name}... WOOT! 
					/timed 1 /autoinv
					/delay 3s !${Cursor.ID}
				}
			/next LootSlot
		}
		/notify LootWnd DoneButton leftmouseup 
		/delay 2s !${Window[LootWnd].Open}
		/doevents
		|/call LootMobs
	}
	/call MovingTo ${MainCamp} 25
/return

