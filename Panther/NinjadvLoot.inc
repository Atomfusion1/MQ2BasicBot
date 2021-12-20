| Ninjadvloot.inc v1.5
| This is the Free NinjAdvLoot.inc from http://www.macroquest2.com/phpBB3/viewtopic.php?f=45&t=12578&start=45
| Modified by Atomfusion for Panther.mac 

#Event ALCantLoot         "#*#may not loot this corpse#*#"

Sub SetupAdvLootVars
	/declare ALDontLoot      int    outer
	/declare CorpseRotTimer  timer  outer
	/call ALLoadVar Settings LootMobs      TRUE  bool
	/call ALLoadVar Settings CorpseRadius  100   int
	/call ALLoadVar Settings MobsTooClose  30    int
	/call ALLoadVar Settings CorpseRotTime 440s  string
	/call ALLoadVar Settings ReportLoot    TRUE bool
	/call ALLoadVar Settings LootChannel   g  string
	/if (!${Defined[AlertList]}) /declare AlertList int outer 1
	/squelch /alert clear 25
/return

Sub ALLoadVar(IniSection,IniVar,IniValue,VarType)
	/if (!${Defined[${IniVar}]} && ${Defined[VarType]}) /declare ${IniVar} ${VarType} outer
	/declare IniString string local ${Ini[Panther/Loot.ini,${IniSection},${IniVar},NOTFOUND]}
	/varset ${IniVar} ${IniString}
	/if (${IniString.Equal["NOTFOUND"]}) {
	  /if (${IniString.Equal["NOTFOUND"]}) /varset ${IniVar} ${IniValue}
	  /ini "Panther/Loot.ini" "${IniSection}" "${IniVar}" "${${IniVar}}"
	}
/return

Sub LootCorpse
/if (${CheckDebug}) /echo \atCalling LootCorpse
	| /declare i          int    local
	/declare LootList   string local
	/call CheckCursor
	/loot
	/delay 3s ${Corpse.Open}
	/doevents CantLoot
	/if (${Target.ID}==${ALDontLoot} && ${Spawn[${ALDontLoot}].ID}) /squelch /alert add 25 id ${ALDontLoot}
	/delay 3s ${Corpse.Items} || ${Target.ID}==${ALDontLoot}
	/if (${Corpse.Open} && ${Corpse.Items}) {
	 /declare loottotal  int    local
	  :ALlootlag
		  /varset loottotal ${Corpse.Items}
		  /delay 1s ${loottotal}!=${Corpse.Items}
	  /if (${loottotal}!=${Corpse.Items}) /goto :ALlootlag
	  /for i 1 to ${loottotal}
		  /if (${Corpse.Item[${i}].ID} && !${Select[${Ini[Panther/Loot.ini,"${Corpse.Item[${i}].Name.Left[1]}","${Corpse.Item[${i}]}"]},Ignore,Keep,Destroy,Sell]}) {
			/if (${Corpse.Item[${i}].NoDrop}) /ini "Panther/Loot.ini" "${Corpse.Item[${i}].Name.Left[1]}" "${Corpse.Item[${i}]}" Ignore
			/if (!${Corpse.Item[${i}].NoDrop}) /ini "Panther/Loot.ini" "${Corpse.Item[${i}].Name.Left[1]}" "${Corpse.Item[${i}]}" Keep
		  }
		  /if (${Ini[Panther/Loot.ini,"${Corpse.Item[${i}].Name.Left[1]}","${Corpse.Item[${i}]}"].NotEqual[Destroy]} && !${Me.FreeInventory} && (!${FindItemCount[=${Corpse.Item[${i}].Name}]} || (${FindItemCount[=${Corpse.Item[${i}].Name}]} && ${Corpse.Item[${i}].Stackable} && !${Corpse.Item[${i}].FreeStack})) || (${Corpse.Item[${i}].Lore} && ${FindItem[${Corpse.Item[${i}]}].ID}) || ${Ini[Panther/Loot.ini,"${Corpse.Item[${i}].Name.Left[1]}","${Corpse.Item[${i}]}"].Equal[Ignore]}) /varset LootList ${LootList}${Corpse.Item[${i}]},
		  /if ((${Ini[Panther/Loot.ini,"${Corpse.Item[${i}].Name.Left[1]}","${Corpse.Item[${i}]}"].Equal[Keep]} || ${Ini[Panther/Loot.ini,"${Corpse.Item[${i}].Name.Left[1]}","${Corpse.Item[${i}]}"].Equal[Sell]}) && (!${Corpse.Item[${i}].Lore} || !${FindItem[${Corpse.Item[${i}]}].ID}) && (${Me.FreeInventory} || (${FindItemCount[=${Corpse.Item[${i}].Name}]} && ${Corpse.Item[${i}].Stackable} && ${Corpse.Item[${i}].FreeStack}))) /call LootItem ${i} Keep Right
		  /if (${Ini[Panther/Loot.ini,"${Corpse.Item[${i}].Name.Left[1]}","${Corpse.Item[${i}]}"].Equal[Destroy]}) /call LootItem ${i} Destroy left
	  /next i | "
	  /if (${Corpse.Items}) {
		/if (${ReportLoot} && ${LootList.Length}) {
		  /notify LootWND LW_BroadcastButton leftmouseup
		  /keypress enter chat
		}
		/if (${Target.ID}) /squelch /alert add 25 id ${Target.ID}
		/varset CorpseRotTimer ${CorpseRotTime}
	  }
	}
	:clickdoneffs
	/nomodkey /notify LootWnd LW_DoneButton leftmouseup
	/delay 5s !${Corpse.Open}
	/if (${Corpse.Open}) /goto :clickdoneffs
/return

Sub LootItem(int i,DoWhat,WhichButton)
	/declare CorpseItemID int local ${Corpse.Item[${i}].ID}
	/nomodkey /shift /itemnotify loot${i} ${WhichButton}mouseup
	/delay 5s ${Window[ConfirmationDialogBox].Open} || !${Corpse.Item[${i}].NoDrop}
	/if (${Window[ConfirmationDialogBox].Open}) /nomodkey /notify ConfirmationDialogBox Yes_Button leftmouseup
	/delay 5s ${Cursor.ID} || ${WhichButton.NotEqual[left]}
	/if (${DoWhat.Equal[Destroy]} && ${Cursor.ID}==${CorpseItemID}) /destroy
	/delay 3s !${Corpse.Item[${i}].ID}
	/if (${Cursor.ID}) /call CheckCursor
/return

Sub LootMobs
/if (${CheckDebug}) /echo \atCalling LootMobs
	/if (!${LootMobs} || ${SpawnCount[npc radius ${MobsTooClose} zradius 100 noalert ${AlertList}]} || !${SpawnCount[corpse radius ${CorpseRadius} zradius 100 noalert 25]} || ${Me.Combat} || (${Cursor.NoDrop} && !${Me.FreeInventory})) /return
	/if (!${CorpseRotTimer}) /squelch /alert clear 25
	| /declare i int local
	/declare CorpseList string local |
	/for i 1 to ${SpawnCount[corpse radius ${CorpseRadius} zradius 100 noalert 25]}
	/varset CorpseList ${CorpseList}${NearestSpawn[${i},corpse radius ${CorpseRadius} noalert 25].ID}|	
	/next i
	/declare DeadCount int local ${SpawnCount[corpse radius ${CorpseRadius} zradius 100 noalert 25]}
	/if (${Me.Mount.ID}) /dismount
	/for i 1 to ${DeadCount}
		/if (${Spawn[${CorpseList.Arg[${i},|]}].Deity.ID} && ${Spawn[${CorpseList.Arg[${i},|]}].ID}) /squelch /alert add 25 id ${Spawn[${CorpseList.Arg[${i},|]}].Deity.ID}
		/if (${Spawn[${CorpseList.Arg[${i},|]}].ID} && ${Spawn[${CorpseList.Arg[${i},|]}].LineOfSight} && !${Spawn[${CorpseList.Arg[${i},|]}].Deity.ID}) {
		  /if (${Target.ID}!=${Spawn[${CorpseList.Arg[${i},|]}].ID}) /target id ${Spawn[${CorpseList.Arg[${i},|]}].ID}
		  /if (!${Me.Standing}) /stand
		  /delay 2s ${Target.ID}==${Spawn[${CorpseList.Arg[${i},|]}].ID} && ${Me.Standing}
		  /if (${Spawn[${CorpseList.Arg[${i},|]}].Distance}>5) /call MoveToLoot ${Spawn[${CorpseList.Arg[${i},|]}].Y} ${Spawn[${CorpseList.Arg[${i},|]}].X} 
		  /delay 10s ${Spawn[${CorpseList.Arg[${i},|]}].Distance}<10
		  /if (${Spawn[${CorpseList.Arg[${i},|]}].Distance}<10) /call LootCorpse
		}
	/next i
/return

Sub Event_ALCantLoot
	/echo \arCant Loot Called
	/varset ALDontLoot ${Target.ID}
/return

Sub CheckCursor
	:ALauto_inv
	/if (${Cursor.ID}) {
	  /if (${Cursor.NoDrop} && !${Me.FreeInventory}) /return
	  /timed 5 /autoinventory
	  /delay 2s !${Cursor.ID}
	  /goto :ALauto_inv
	}
/return

Sub MoveToLoot(int CorpseY, int CorpseX)
	/declare MoveToTimer timer local 30
	/squelch /face nolook loc ${CorpseY}, ${CorpseX}
		/while (${Math.Distance[${CorpseY},${CorpseX}]} > 10 && ${Bool[${MoveToTimer}]}) {
			/squelch /face nolook loc ${CorpseY}, ${CorpseX}
			/keypress forward hold
			/delay 2 (${Math.Distance[${CorpseY},${CorpseX}]} < 10)
			/keypress forward
			/doevents
		}
/return