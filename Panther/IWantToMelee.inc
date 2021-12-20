Sub TargetMob 
/if (${CheckDebug}) /echo \atCalling TargetMob
	/if (${Bool[${Me.XTarget[1].ID}]} && ${Spawn[${Me.XTarget[1].ID}].Distance3D} < 200 && ${Spawn[=${MainCamp}].Distance3D} < 200) {
		/squelch /target id ${Me.XTarget[1].ID}
		/squelch /attack on
		/if (${LazyMode} && ${Target.PctHPs}>90) /return
		/if (${Spawn[${Me.XTarget[1].ID}].Distance3D}>=${Spawn[${Me.XTarget[1].ID}].MaxRange}) {
			/squelch /face fast
			/delay 5
			/keypress Up Hold
			/delay 10 (${Spawn[${Me.XTarget[1].ID}].Distance3D}<=${Spawn[${Me.XTarget[1].ID}].MaxRange})
			/keypress Up
			/keypress Up
		
		}
	}
/return

Sub DoIHaveAggro
/if (${CheckDebug}) /echo \atCalling DoIHaveAggro
	/varset MeAggro FALSE
	/if (${Bool[${Me.XTarget[1].ID}]} && ${Bool[${Spawn[NPC Targetable los radius 150]}]}) {
		| /echo Mob ${Me.XTarget[1].ID} is upset and close
		/varset MeAggro TRUE
	} 
	/if (!${Bool[${Me.XTarget[1].ID}]} && ${Me.Combat} && ${MeCombatTimer}==0) {
		| Lets turn off attack as no one is mad at us
		/squelch /attack off
		/delay 10 (!${Bool[${Me.Combat}]})	
	} else {
		| This Lets you Pull a Mob without auto turnoff for 3 seconds (Good for Low levels and slow swings)
		/if (!${Me.Combat}) /varset MeCombatTimer 3s
	}
	/if (!${Bool[${Me.XTarget[1].ID}]}) {
		| This Resets TargetID and Resets Ability to Follow
		/varset StopFollow FALSE
	}
/return

Sub ImMelee
/if (${CheckDebug}) /echo \atCalling ImMelee
| Check Combat Ability's Must be hotlink on Bar in EQ
	/if (${Me.Combat} && ${Target.Distance}<=${Math.Calc[${Spawn[${Target}].MaxRangeTo}*4]}) {
		/for i 1 to 10 
			/if (${Me.AbilityReady[${MAbility[${i}]}]}) { 
			| Checking For Tuant (When Aggro is lost) and Bash (Wont Spam Bash without a shield)
				/if (${Me.Ability[${Me.Ability[${MAbility[${i}]}]}].Equal["Bash"]} && ${Bash}!=1) /continue
				/if (${Me.Ability[${Me.Ability[${MAbility[${i}]}]}].Equal["Taunt"]} && ${Target.PctAggro}>=100 && ${IWantToTaunt}) /continue
				/squelch /doability "${MAbility[${i}]}"
				/delay 5
				/break
			}	
		/next i 
		/for i 1 to 10 
			/if (${Me.CombatAbilityReady[${CAbility[${i}]}]} || ${Me.AbilityReady[${CAbility[${i}]}]}) { 
			| Looks to use CA tuants only when Aggro is lost
				/if (${Me.CombatAbility[${Me.CombatAbility[${CAbility[${i}]}]}].Name.Equal["Berate"]} && ${Target.PctAggro}>=100 && ${IWantToTaunt}) 		/continue
				/if (${Me.CombatAbility[${Me.CombatAbility[${CAbility[${i}]}]}].Name.Equal["Incite"]} && ${Target.PctAggro}>=100 && ${IWantToTaunt}) 		/continue
				/if (${Me.CombatAbility[${Me.CombatAbility[${CAbility[${i}]}]}].Name.Equal["Bellow"]} && ${Target.PctAggro}>=100 && ${IWantToTaunt}) 		/continue
				/if (${Me.CombatAbility[${Me.CombatAbility[${CAbility[${i}]}]}].Name.Equal["Provoke"]} && ${Target.PctAggro}>=100 && ${IWantToTaunt}) 		/continue
				/if (${Me.CombatAbility[${Me.CombatAbility[${CAbility[${i}]}]}].Name.Equal["Mock"]} && ${Target.PctAggro}>=100 && ${IWantToTaunt}) 			/continue
				/if (${Me.CombatAbility[${Me.CombatAbility[${CAbility[${i}]}]}].Name.Equal["Bazu Bellow"]} && ${Target.PctAggro}>=100 && ${IWantToTaunt}) 	/continue
				/squelch /doability "${CAbility[${i}]}" 
				/delay 10
				/break
			}	
		/next i 
		
		/if (${CastTauntTimer}==0 && ${IWantToTauntCast} && !${Me.SpellInCooldown} || ${Target.PctAggro} < 100 && ${IWantToTauntCast} && !${Me.SpellInCooldown}) {
			/if (${Bool[${Me.Book[${SpellTaunts[${TauntSpellNumber}]}]}]}) {
				/call SpellCast ${Target.ID} "${SpellTaunts[${TauntSpellNumber}]}" "Sub ImMelee:SpellTaunts[${TauntSpellNumber}]"
				/varset TauntSpellNumber ${Math.Calc[${TauntSpellNumber}+1]}
				/if (${TauntSpellNumber}>4) /varset TauntSpellNumber 1
				/varset CastTauntTimer 20s
			} else /varset TauntSpellNumber 1
		}
		
		/if (${NoMezzer} || ${MezImmune}) {
			/for i 2 to 6
				/if (${Bool[${Me.XTarget[${i}].ID}]} && !${Me.SpellInCooldown} && ${Me.XTarget[${i}].PctAggro} != 100 ) {
				| /echo Second Mob has  pct aggro ${Me.XTarget[${i}].PctAggro} casting ${SpellTaunts[${TauntSpellNumber}]} on ${Me.XTarget[${i}].Name}
					/target id ${Me.XTarget[${i}].ID}
					/delay 5
					/squelch /doability "taunt"
					| /echo trying to cast ${SpellTaunts[${TauntSpellNumber}]} which is number ${TauntSpellNumber}
					/if (${IWantToTauntCast}) /call SpellCast ${Target.ID} "${SpellTaunts[${TauntSpellNumber}]}" "Sub ImMelee:SpellTaunts[${TauntSpellNumber}]"
					/varset TauntSpellNumber ${Math.Calc[${TauntSpellNumber}+1]}
					/if (${TauntSpellNumber}>4) /varset TauntSpellNumber 1
					/target id ${TargetID}
					/delay 5
				}		
			/next i 
		}
	}
	| "
/return


Sub CallTarget	
/if (${CheckDebug}) /echo \atCalling CallTarget
	/if (${Bool[${Spawn[${Me.XTarget[1].ID}]}]} && ${MobPulling}==1 || ${Bool[${Spawn[${Me.XTarget[1].ID}]}]} && ${LazyMode}) {
	| This is my Auto Target I will Target XTarget 1 
	/declare TestHold bool inner FALSE
		/if (${Me.XTarget}>1) {
			/for i 1 to ${Me.XTarget}
				/if (${Spawn[${Me.XTarget[${i}]}].Named}) {
					/varset TestHold TRUE
					/if (${Target.ID}!=${Me.XTarget[${i}].ID}) {
						/g Named ${Target.CleanName} Lets Kill It 
						/echo \apWARNING NAMED MOB ${Target}
					}
					/squelch /target id ${Me.XTarget[${i}].ID}
					/delay 10 (${Target.ID}==${Me.XTarget[1].ID})
					/if (${MobPulling}==1) /squelch /face nolook 
					/break
				}
			
			/next i
			/if (!${TestHold}) {
				/if (${Target.ID}!=${Me.XTarget[1].ID}) { 
					/squelch /target id ${Me.XTarget[1].ID}
					/delay 10 (${Target.ID}==${Me.XTarget[1].ID})
					/if (${MobPulling}==1) /squelch /face nolook 
				}
			}
		} 
		/while (${Spawn[${MainCamp}].Distance3D}>120 && ${MobPulling}==1 && ${Target.PctHPs}>=32) {
			/if (${Me.Combat}) /squelch /attack off
			/delay 10 (!${Bool[${Me.Combat}]})
			/if (${Spawn[${MainCamp}].Distance3D}>25 ) /call RunBack ${MainCamp} 20
		}
		/if (${Spawn[${MainCamp}].Distance}<100 && !${Bool[${Me.Combat}]}) {
			/squelch /attack on
			/delay 10 (${Bool[${Me.Combat}]})
		}
		/if (${Target.ID}!=${TargetID} && ${Spawn[${Target.ID}].Type.Equal[NPC]}) {
			|/echo My Target is ${Target.ID} ${Spawn[${Target.ID}]} vs ${TargetID} ${Spawn[${TargetID}]}		
			/varset CallTargetOn TRUE
			/varset TargetID ${Target.ID}

		} 
		/if (${Target.ID}==${TargetID} && ${Target.PctHPs}<=${PctCallTarget} && ${CallTargetOn} && ${Bool[${Spawn[${TargetID}].Type.Equal[NPC]}]} && ${Spawn[${TargetID}].LineOfSight} && ${Spawn[${MainCamp}].Distance3D} < 100 && ${Spawn[${TargetID}].Distance3D} < 105) {
			/g Help Me Kill %t
			/squelch /pet Attack
			/varset CallTargetOn FALSE
		}		
	}
/return

