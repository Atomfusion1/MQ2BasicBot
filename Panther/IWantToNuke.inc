

Sub Nuke
/if (${CheckDebug}) /echo \atCalling Nuke
	| This Line Checks for Open Windows if Merchant or Trade windows are Open Macro will Just Loop
	/call CheckWindows /if (${Macro.Return}==1) /return
	/if (${NukeTimer2}!=0 || !${Bool[${Spawn[${TargetID}]}]} || ${Math.Calc[${Macro.RunTime}-${MobTime}]}<=0 || ${Me.PctMana}<=${ManaNukeLevel}) /return
| This Determines the Best Nuke	
	/if (${NukeRate}==0 && ${Spawn[${TargetID}]].Distance3D}<=180) {
		/delay 30 ${Spawn[${TargetID}].PctHPs}<=99
		| Mobs Percent Loss of HPs per Second 
		/varset NRate ${Math.Calc[(${MobHPs}-${Spawn[${TargetID}].PctHPs})/(${Macro.RunTime}-${MobTime})]}
		| Changing these values will determine how the Macro will Nuke 
		/if (${NRate}>8.0) /return
		/if (${NRate}>3.0) /varset NukeRate 1
		/if (${NRate}>=2.0 && ${NRate}<=3.0 && ${Bool[${SpellNukes[2]}]}) /varset NukeRate 2
		/if (${NRate}<2.0 || !${Bool[${Me.Gem[${SpellNukes[2]}]}]} && !${Bool[${Me.Gem[${SpellNukes[3]}]}]}) /varset NukeRate 3
		|/echo NukeRate is ${NukeRate}
		|/varset NukeTimer2 5s
		| This Area is Self Adjusting for Nuking If your have alot of Mana it reduces Nuke Time if you are low on mana it raises Nuke Time
		/if (${OldTargetID} != ${TargetID}) {
			/if (${Me.PctMana}<45 && ${StaticNukeTimer2}<80) {
				/varset StaticNukeTimer2 ${Math.Calc[${StaticNukeTimer2}+1]}
				| /echo changing added 1 to StaticNukeTimer its at ${StaticNukeTimer2}
			} else {
				/if (${Me.PctMana}>60 && ${StaticNukeTimer2}>1) {
					/varset StaticNukeTimer2 ${Math.Calc[${StaticNukeTimer2}-1]}
				|	/echo changing subtracted 1 to StaticNukeTimer its at ${StaticNukeTimer2}
				}
			}
		/varset OldTargetID ${TargetID}
		}
	}
| This secion Is For Nuking the Mob	
	/if (${TargetID}!=0 && ${Spawn[${TargetID}].Type.Equal[NPC]} && !${Spawn[${TargetID}].Type.Equal[Corpse]} && ${Spawn[${TargetID}].PctHPs}<100 && ${Bool[Spawn[${TargetID}]}) {
		/if (${NukeRate}!=0) {
			/if (${NukeRate}==1 && ${Spawn[${TargetID}].PctHPs}>=30 || ${NukeRate}==2 && ${Spawn[${TargetID}].PctHPs}<=20 ) {
				/docommand /timed ${Math.Calc[(${Spell[${SpellNukes[3]}].MyCastTime.Float}*10)]} /if (!${Bool[${Spawn[${TargetID}].ID}]}) /stopcast
				/if (${Bool[${Spell[${SpellNukes[3]}]}]}) /call SpellCast ${TargetID} "${SpellNukes[3]}" "Sub Nuke:SpellNukes[3]"
				| /echo Quick Nuking a ${TargetID}
				/varset NukeRate 0
				/varset SitTimer ${StaticSitTimer}s
				/varset NukeTimer2 ${StaticNukeTimer2}s
			}

			/if (${NukeRate}==2 && ${Spawn[${TargetID}].PctHPs}>=20 || ${NukeRate}==3 && ${Spawn[${TargetID}].PctHPs}<5) {
				/docommand /timed ${Math.Calc[(${Spell[${SpellNukes[2]}].MyCastTime.Float}*10)]} /if (!${Bool[${Spawn[${TargetID}].ID}]}) /stopcast
				/if (${Bool[${Spell[${SpellNukes[2]}]}]}) /call SpellCast ${TargetID} "${SpellNukes[2]}" "Sub Nuke:SpellNukes[2]"
				| /echo Medium Nuking a ${TargetID}
				/varset NukeRate 0
				/varset SitTimer ${StaticSitTimer}s
				/varset NukeTimer2 ${StaticNukeTimer2}s
			}

			/if (${NukeRate}==3 && ${Spawn[${TargetID}].PctHPs}>1 && ${Spawn[${TargetID}].PctHPs}<=99) {
				/docommand /timed ${Math.Calc[(${Spell[${SpellNukes[1]}].MyCastTime.Float}*10)]} /if (!${Bool[${Spawn[${TargetID}].ID}]}) /stopcast
				/if (${Bool[${Spell[${SpellNukes[1]}]}]}) /call SpellCast ${TargetID} "${SpellNukes[1]}" "Sub Nuke:SpellNukes[1]"
				| /echo Large Nuke on ${TargetID}
				/varset NukeRate 0
				/varset SitTimer ${StaticSitTimer}s
				/varset NukeTimer2 ${StaticNukeTimer2}s
			}
		}
	}
	/varset NukeRate 0
/return