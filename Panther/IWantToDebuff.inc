Sub DebuffMezz
/declare NamedFound bool inner FALSE
/if (${CheckDebug}) /echo \apCalling Mezzing
	/if (${Me.Class.Name.Equal[Bard]}) /return
	| This Line Checks for Open Windows if Merchant or Trade windows are Open Macro will Just Loop
	/call CheckWindows /if (${Macro.Return}==1) /return
	/if (!${MezzingMobs}) /return
	| check mobs near you with ${Me.XTarget[i].ID} this is auto aggro 
	| Lets Check For New Mobs 
	/if (${MezzedMobs}!=${Me.XTarget} && ${Me.XTarget}>1 && ${MezzedMobs}!=0 && ${SpawnCount[npc targetable los radius 100]}>1) {
		/varset SpellMezDuration 0
		/echo \ayMobs have Added
		|/g Mezzing New Mobs
	}
	/if (${SpawnCount[npc targetable los radius 100]}<2 || ${SpellMezDuration}!=0) /return
	/for Index 10 downto 1
		:MezResisted
		|/echo ${Index}==1 && !${NamedFound}
			/if (${Index}==1 && !${NamedFound}) {
				|/echo ${Index}==1 && !${NamedFound}
				/break
			}
			/if (${Spawn[${Me.XTarget[${Index}]}].Named}) {
				/varset NamedFound TRUE
			}
			
			/varset ResistDet FALSE
			/if (${Bool[${Spawn[${Me.XTarget[${Index}].ID}]}]} && ${Spawn[${Me.XTarget[${Index}]}].Distance3D}<=${Spell[${SpellMez}].Range} && ${Spawn[${Me.XTarget[${Index}].ID}].ID}!=${TargetID} && ) {
				/echo \ayMezzing Mob ${Me.XTarget[${Index}]} ${Me.XTarget[${Index}].ID} it should not be ${TargetID}
				/g Mezzing a ${Me.XTarget[${Index}]}
				/call SpellCast ${Me.XTarget[${Index}].ID} "${SpellMez}" "Sub DebuffMez:SpellMez"
				/varset SpellMezDuration ${Math.Calc[${Spell[${SpellMez}].Duration}*6*10*.4]}		
				/varset MezzedMobs ${Me.XTarget}
				/varset NukeTimer2 ${Math.Calc[${StaticNukeTimer2}*.5]}s
			}
			/doevents Resisted
			/if (${ResistDet}) {
				/if	(!${Spawn[${Me.XTarget[${Index}]}].Named}) {
					/echo \arMob RESISTED MEZ Recasting
					/goto :MezResisted
				}
			}
	/next Index	
/return

Sub Debuff
/if (${CheckDebug}) /echo \apCalling DeBuff
	| This Line Checks for Open Windows if Merchant or Trade windows are Open Macro will Just Loop
	/if (!${IDebuffNow}) /return
	/call CheckWindows /if (${Macro.Return}==1) /return
   | check mobs near you with ${Me.XTarget[i].ID} this is auto aggro 
	/if (!${Bool[${Spell[${TrackDebuff[2,1]}].ID}]}) {
	/echo \apSet DeBuff Spells Up
			/for Index 1 to 5 
				/varset TrackDebuff[2,${Index}]	${Spell[${SpellDebuffs[${Index}]}].ID}	
			/next Index 
	}
	|/if (!${Bool[${Spawn[${TargetID}]}]} || !${Bool[${TargetID}]}) /return	
	/if (${Spawn[${TrackDebuff[1,1]}].Type.Equal[NPC]}) {
	   | You Must Assist Tank For Debuffs to start This is Done in Events 
	   |${Math.Calc[${Spell[${TrackDebuff[2,${i}]}].Duration}*6*10*.9]}
		/for Index 1 to 10
			/if (${Bool[${Spawn[${Me.XTarget[${Index}].ID}]}]} && ${Spawn[${Me.XTarget[${Index}].ID}].ID}==${TrackDebuff[1,1]}) {
				/for i 1 to 5
				:DebuffResisted
				/varset ResistDet FALSE
					/if (${Spawn[${Me.XTarget[${i}]}].Distance3D}<=${Spell[${TrackDebuff[2,${i}]}].Range} && ${Bool[${Spell[${TrackDebuff[2,${i}]}]}]} && ${TrackDebuff[3,${i}]}<${Macro.RunTime}) {
						|/echo \apCasting ${Spell[${TrackDebuff[2,${i}]}]} Debuff on Mob ${Me.XTarget[${i}].ID} it should be ${TargetID}
						/call SpellCast ${Me.XTarget[${Index}].ID} "${Spell[${TrackDebuff[2,${i}]}]}" "Sub Debuff:${Spell[${TrackDebuff[2,${i}]}]}"
						/varset TrackDebuff[3,${i}] ${Math.Calc[${Spell[${TrackDebuff[2,${i}]}].Duration}*6*.9+${Macro.RunTime}]} 
						|/g DoT on %t
						|/echo \aptimer set on ${TrackDebuff[3,${i}]}
						/delay ${DebuffDelay}
						/varset NukeTimer2 ${Math.Calc[${StaticNukeTimer2}*.3]}s
					}
				/doevents Resisted
				/if (${ResistDet}) {
					/echo \arMob Resisted ${Spell[${TrackDebuff[2,${i}]}]} Recasting
					/goto :DebuffResisted
				}
				/next i
			}
		/next Index	
	}

/return

Sub DebuffDoT
/if (${CheckDebug}) /echo \apCalling DebuffDot
	/if (!${Bool[${Spell[${TrackDoT[2,1]}].ID}]}) {
			/for Index 1 to 5 
				/varset TrackDoT[2,${Index}]	${Spell[${SpellDoT[${Math.Calc[${Index}]}]}].ID}	
			/next Index 
			/echo \agDoT Spells Set Up ${Spell[${TrackDoT[2,1]}].Name} is first
	}
	| This Line Checks for Open Windows if Merchant or Trade windows are Open Macro will Just Loop
	/call CheckWindows /if (${Macro.Return}==1) /return
	/if (!${Bool[${Spawn[${TargetID}]}]}) /return
   | check mobs near you with ${Me.XTarget[i].ID} this is auto aggro 
   /if (${DoTDuration}!=0 || !${Bool[${TargetID}]}) /return
   | You Must Assist Tank For DoT to start This is Done in Events 
	/for Index 1 to 10
		/if (${Bool[${Spawn[${Me.XTarget[${Index}].ID}]}]} && ${Spawn[${Me.XTarget[${Index}].ID}].ID}==${TargetID}) {
			/for i 1 to 5
			:DOTResisted
			/varset ResistDet FALSE
				/if (${Spawn[${Me.XTarget[${i}]}].Distance3D}<=${Spell[${SpellDoT[${i}]}].Range} && ${Bool[${Spell[${SpellDoT[${i}]}]}]} && ${Bool[${Me.Book[${SpellDoT[${i}]}]}]} ) {
					| /echo A Dot is being Cast on Mob ${Me.XTarget[${Index}].ID} it should be ${TargetID}
					/call SpellCast ${Me.XTarget[${Index}].ID} "${SpellDoT[${i}]}" "Sub DebuffDot:SpellDot[${i}]"
					|/varset SpellDotDuration ${Math.Calc[${Spell[${SpellDoT[${i}]}].Duration}*6*10*.8]}
					|/g DoT on %t
					/delay 5
				}
			/doevents Resisted
			/if (${ResistDet}) {
				/echo \arMob Resisted ${SpellDoT[${i}]} Recasting
				/goto :DOTResisted
			}	
			/next i
		}
	/next Index	
/varset NukeTimer2 6s
/varset DoTDuration 60s
/return

Sub DebuffSnare 
/if (${Me.Class.Name.Equal[Bard]}) /return
/if (${CheckDebug}) /echo \apCalling Snare
| .Fleeing Always returns True until the mob Runs ... Who Knows ... 
	/if (!${Bool[${Target.Buff[${SpellSnare}]}]} && ${Target.PctHPs}<=${SnareHPs} && ${Spawn[${Target}].Type.Equal[NPC]} && ${Target.Moving}) {
	:SnareResisted
	/stopcast
	/varset ResistDet FALSE
		|/echo \ar Mob is Fleeing I am now Snaring or Rooting Spell ${Target.ID} "${SpellSnare}" "Sub DebuffSnare:SpellSnare"
		/call SpellCast ${Target.ID} "${SpellSnare}" "Sub DebuffSnare:SpellSnare"
		/varset SnareTimer 20s
		/if (${ResistDet}) {
			|/echo \arMob Resisted ${SpellSnare} Recasting
			/goto :SnareResisted
		}	
	}
/return