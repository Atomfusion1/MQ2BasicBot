Sub MoveTo
/if (${CheckDebug}) /echo \awCalling MoveTo
| Short and Sweet Follow as efficient as possible breaks on attack call or can be adjusted in events 
	/if (${Spawn[${MoveToT}].Distance3D}>120 && !${Spawn[${MoveToT}].LineOfSight} || ${Spawn[${MoveToT}].Distance3D}>320 ) /return
	/if (${Me.Sitting}) /stand
	/if (${Bool[${Me.Mount}]}) /dismount
	/echo \awFollow Start ${MoveToT}
	/squelch /target id ${MoveToT}
	/delay 10 ${Target.ID}==${MoveToT}
	/while (${IsBool} && !${StopFollow}) {
		/if (${Target.ID}!=${MoveToT}) /squelch /target id ${MoveToT}
			/while (${Target.Distance3D} > ${IDist}) {
				/if (${Target.ID}!=${MoveToT}) /squelch /target id ${MoveToT}
				/squelch /face fast nolook
				/keypress forward hold
				/if (${IWantToTwist}) 						/call Twist				| IWantToTwist.Inc
				/delay 2 (${Target.Distance3D} <= ${IDist})
				/keypress forward
			}
		/if (${IWantToTwist}) 						/call Twist				| IWantToTwist.Inc	
		/doevents
	}
/return 

Sub CampLoc(int MoveRange)
/if (${CheckDebug}) /echo \awCalling CampLoc
	/if (!${CampUp}) /return
	/if (${Math.Distance[${LocY},${LocX}]}<=25 || ${LocY}==0 && ${LocX}==0) /return
	/if ( ${Math.Distance[${LocY},${LocX}]} > 520 || ${Bool[${Me.XTarget[1].ID}]} && ${Math.Distance[${LocY},${LocX}]} < 150 || ${LocY}==0 && ${LocX}==0) /return
		/doevents 
			/varset MoveDistance ${Math.Distance[${LocY},${LocX}]}
			/squelch /face nolook loc ${LocY},${LocX}
			/keypress Up Hold
			/delay 30 ${Math.Distance[${LocY},${LocX}]}<${MoveRange}
			/keypress Up
			/keypress Up
			/squelch /face away ${MainCamp}
/return

Sub Pulling
/declare Starti int local 1
/if (${CheckDebug}) /echo \awCalling Pulling
	| This Line Checks for Open Windows if Merchant or Trade windows are Open Macro will Just Loop
	/call CheckWindows /if (${Macro.Return}==1) /return
	|/echo mobs in range = ${SpawnCount[NPC radius ${Math.Calc[${PullD}/1]}]} and PullTimer is ${PullTimer}

	/if (${Spawn[${MainCamp}].Distance3D}>60 && ${SpawnCount[NPC radius ${Math.Calc[${PullD}/1.4]}]}==0 ) /call RunBack ${MainCamp} 20
	/if (${Me.Dead} || ${PullTimer}!=0 || ${MobPulling}==0 || ${Bool[${Me.Casting}]} && !${Me.Class.ShortName.Equal[BRD]} || ${Bool[${Me.XTarget[2].ID}]} || ${Bool[${Me.Combat}]} && !${RapidPulling} || ${Me.PctHPs} < 60) /return
	/if (${SpawnCount[NPC radius ${Math.Calc[${PullD}/1]}]}==0) {
		|/echo No Mob Found This stops you from instantly running out when a mob spawns, this is more random 
		/varset PullTimer 30s
		/return
	}
	| if fastpull is on when should i not pull a mob 
	/if (${Me.XTarget[1].PctHPs} > ${MinPullHP} || ${Me.XTarget[1].PctAggro} > 90) {
		/return
	}
	| Check for Extra PCs
	/if (${SpawnCount[PC radius ${Math.Calc[${PullD}*1.3]}]} > ${PCNear} && ${PCNearTimer}==0) {
		/echo \arExtra PC Near Not Pulling	
		/varset PCNearTimer 60s
		/beep
		/beep
		/if (${Spawn[=${MainCamp}].Distance} > 30) /squelch /nav id ${Spawn[=${MainCamp}].ID} Distance=30
		/return
	}	
		
	/if (${PCNearTimer}!=0) /return
	
	| This Spot Checks the Group for the Cleric then Checks His Mana if its low Then He Meds up before he trys to Auto Pull 
	/for i 1 to ${Group} | Cleric = 2 , Shaman = 10 
		/if (${Group.Member[${i}].Class.ID}==2 || ${Group.Member[${i}].Class.ID}==10) {
			/if (${Group.Member[${i}].PctMana}<=30) {
				/echo \agSitting for 5 mins to let Cleric Regen Mana
				/sit
				/delay 10000 (${Group.Member[${i}].PctMana}>90 || ${Bool[${Me.XTarget[1].ID}]})
				/return			
			}
		}	
		/if (${Group.Member[${i}].Type.Equal[Corpse]}) {
			/echo \arGroup Member Dead 
			/varset PullTimer 195s
			/delay 100
			/return		
		}
	/next i
	/declare TotalMana int inner 0
	| watch group mana 
	/declare GroupHolder ${Group.GroupSize}
	/for i 0 to ${Group}
		/if (${Select[${Group.Member[${i}].Spawn.Class.ShortName},WAR,MNK,ROG,BER,BRD]}) {
		| remove group members who dont have mana from Group (or use it) 
			/varset GroupHolder ${Math.Calc[${GroupHolder}-1]}
		} else {
			/varset TotalMana  ${Math.Calc[${TotalMana}+${Group.Member[${i}].PctMana}]}
		}
	/next i
	/varset TotalMana ${Math.Calc[${TotalMana}/${GroupHolder}]} 
	/if (${TotalMana}<=25) {
		/echo \arTotal Group Mana is ${TotalMana}
		/sit
		/delay 400 (${Group.Member[${i}].PctMana}>90 || ${Bool[${Me.XTarget[1].ID}]})
		/if (!${Bool[${Me.XTarget[1].ID}]}) /return			
	}
	| /echo Pulling Nearest Spawn
	/call GetMob 1
	/squelch /face ${Target}
	/delay 80 (${Spawn[${Me.XTarget[1].ID}].Distance3D}<=100 )
/return

Sub GetMob (int Starti)
/doevents 
/squelch /attack off
/delay 10 (!${Bool[${Me.Combat}]})
/if (${Me.BardSongPlaying}) /stopsong
/if (${CheckDebug}) /echo \awCalling GetMob
/declare LoopTimeout int local 0 | Setup breakout for pulling loop 
/declare MobFound bool local FALSE 
/declare SkipMob int local 0
/declare NavMob string local NULL
/declare RunDelay int local 0
/declare MeLocYX ${Me.LocYX}
	/if (${Bool[${Spawn[${Me.XTarget[1].ID}]}]}) /varset SkipMob ${Me.XTarget[1].ID}
	/if (${PullTimer}!=0) /return
	/while (${MobPulling}==1 && ${PullTimer}==0) {
	/squelch /target clear
	/if (${CheckDebug}) /echo \arGetMob loop ${LoopTimeout}
		/varset LoopTimeout ${Math.Calc[${LoopTimeout}+1]}
		/if (${LoopTimeout}>20) { 
			/varset PullTimer 25s
			|/echo \a-wLoopTimeout Running Back 
			/call RunBack ${MainCamp} 20
			/break
		}
		/varset Temp FALSE
		| FOR LOOP Search for Mob
		|/echo searching for mob
		/for i 1 to 60
			/varset MobFound FALSE
			| /squelch /target ${NearestSpawn[${i}, NPC]}	
			/for Index 1 to ${RV_MobArray.Size} | this Checks for mobs on list 
				/if (${Bool[${Spawn[${NearestSpawn[${i}, NPC]}].CleanName.Find[${RV_MobArray[${Index}]}]}]} && ${Navigation.PathExists[spawn ${NearestSpawn[${i}, NPC]}]} ) {
					/varset MobFound TRUE
			}
			/next Index

				/if (${IgnoreMobs}) {
					/if (${MobFound} || ${Me.XTarget[1].ID} == ${NearestSpawn[${i}, NPC].ID}) {
					} else /if (${Spawn[${NearestSpawn[${i}, NPC]}].Level} < ${Math.Calc[${Me.Level}+${MobLevelOverMe}]} && ${Spawn[${NearestSpawn[${i}, NPC]}].Distance3D} < ${PullD}) {
						/varset NavMob ${NearestSpawn[${i}, NPC]}
						/varset Temp TRUE
						|/echo \a-wmy target is ${NavMob}
						/break | break out of For loop
					}
				} else {
					/if (${MobFound} && ${Me.XTarget[1].ID} != ${NearestSpawn[${i}, NPC].ID}) {
						/if (${Spawn[${NearestSpawn[${i}, NPC]}].Level} < ${Math.Calc[${Me.Level}+${MobLevelOverMe}]} && ${Spawn[${NearestSpawn[${i}, NPC]}].Distance3D} < ${PullD}) {
							/varset NavMob ${NearestSpawn[${i}, NPC]}
							|/echo \a-wMy xtarget is ${Me.XTarget[1].ID} but im targeting ${Target.ID}
							/varset Temp TRUE
							/break | break out of For loop
						}
					}
				}
			/if (${Temp}) {
			/break
			}
			/if (${i} == 60) {
				/echo \a-wNo Mob to Pull !
				/varset PullTimer 25s
				/break
			}
		/next i
		
		/if (${NoMezzer} && ${SpawnCount[npc radius 80 loc ${Spawn[${NavMob}].X} ${Spawn[${NavMob}].Y}]} > ${PullSize}) {
			/echo \argreater than ${PullSize} Mobs Change with Pull Size #  
			/varset PullTimer 25s
			/break
		}
		
		| Run To Mob Area 
		/varset MoveDistance ${Spawn[${NavMob}].Distance3D}
		/varset MeLocYX ${Me.LocYX}
		| found mob going to pull turning off Torpor like effects Quick and Dirty 
		/squelch /echo ${Me.Song[Torpor].Remove}
		/squelch /echo ${Me.Song[Stoicism].Remove}
		| /squelch /face ${Target}
		/if (${IWantToUseMQ2Nav} && !${Navigation.MeshLoaded}) {/echo \arNav Mesh Not Loaded please generate}
		/if (${IWantToUseMQ2Nav} && ${Navigation.MeshLoaded}) {
			/squelch /nav spawn ${NavMob}
			|/echo \a-wCan I Find a Path is ${Navigation.PathExists[spawn ${NavMob}]}
			/delay 20 ${Spawn[${NavMob}].Distance3D}<=60
			/if (${Spawn[${NavMob}].Distance3D}<=60) {
				/squelch /target ${NavMob}
				/delay 10 ${Bool[${Target.ID}]}
			}
			/doevents flush Stunned
			/doevents 
		} else {
			/squelch /face fast ${Spawn[${NavMob}]}
			/squelch /attack off
			/delay 10 (!${Bool[${Me.Combat}]})
			/keypress Up Hold
			/varset RunDelay ${Math.Calc[20+${Math.Rand[30]}]}
			/delay 10 (${Spawn[${NavMob}].Distance3D}<=40)
			/delay ${RunDelay} (${Spawn[${NavMob}].Distance3D}<=40 || ${IHaveArrows} && ${Spawn[${NavMob}].Distance3D}<=160 && ${Bool[${Spawn[${NavMob}].LineOfSight}]})
			/keypress Up
			/keypress Up
			/doevents 
		}
		
		/if (${Spawn[${NavMob}].Distance3D}<=60 && !${Spawn[${NavMob}].Dead} && ${Spawn[${NavMob}].Type.Equal[NPC]} && !${Spawn[${NavMob}].Aggressive} && ${Bool[${Spawn[${NavMob}].LineOfSight}]}) {
			|/echo \ar In Cycle 1 
			/squelch /target ${NavMob}
			/delay 20 ${Bool[${Target.ID}]}
			/doevents 
			/squelch /nav stop
			|/echo \a-wThrow Stone 
			/delay 10 (!${Me.Moving} || ${Bool[${Me.XTarget[1].ID}]})
			/delay 5 (${Spawn[${NavMob}].Aggressive})
			/squelch /doability "Throw Stone"
			/delay 5 (${Spawn[${NavMob}].Aggressive})
			/if (!${Spawn[${NavMob}].Aggressive} && !${Bool[${Spell[${PullSpell}].ID}]}) /squelch /ranged
			/delay 30 (${Spawn[${NavMob}].Aggressive} || ${Bool[${Spell[${PullSpell}].ID}]})
			/if (${Bool[${Spell[${PullSpell}].ID}]} && !${Spawn[${NavMob}].Aggressive}) /call SpellCast ${Target.ID} "${PullSpell}" "Sub GetMob:PullSpell"
			/delay 30 (${Spawn[${NavMob}].Aggressive})
			
		}
		/if (${Spawn[${NavMob}].Distance3D}<=140 && ${Bool[${Spawn[${NavMob}].LineOfSight}]} && !${Spawn[${NavMob}].Aggressive}) {
			|/echo \a-wIn Cycle 2
			/squelch /target ${NavMob}
			/delay 20 ${Bool[${Target.ID}]}
			/doevents 
			/squelch /nav stop
			/delay 10 (!${Me.Moving} || ${Bool[${Me.XTarget[1].ID}]})
			/delay 5 (${Target.Aggressive})
			/squelch /ranged
			/if (${Bool[${Spell[${PullSpell}].ID}]}) /call SpellCast ${Target.ID} "${PullSpell}" "Sub GetMob:PullSpell"
			/delay 30 (${Target.Aggressive})
		}
		/if (${SpawnCount[NPC radius 75]}>3 || ${Spawn[${MainCamp}].Distance3D}>${Math.Calc[${PullD}/1]} || ${Spawn[${NavMob}].Aggressive} || ${MobPulling}==0 || ${Me.PctHPs} < 60) {
			/doevents 
			/squelch /nav stop
			/delay 5 ${Spawn[${NavMob}].Aggressive}
			/squelch /target ${NavMob}
			/delay 20 ${Bool[${Target.ID}]}
			/call RunBack ${MainCamp} 20
			/return
		}
	 |/echo math distance 1 ${Spawn[${NavMob}].Distance3D}>=65 && ${Math.Distance[${MeLocYX}]}<5
	 | Math.Distance takes saved MeLocYX and current position and calculates a distance 
		/if (${Spawn[${NavMob}].Distance3D}>=65 && ${Math.Distance[${MeLocYX}]}<5) {
			|/echo \a-wMeStuck 1
			/call MeStuck
		}
	}
	| this is a catch all that should never hit but may save lives if it does 
	|/echo \a-wMobPulling or PullTimer Changed  (${MobPulling}==1 && ${PullTimer}==0)
	/if (${Spawn[${MainCamp}].Distance3D}>=55 ) /call RunBack ${MainCamp} 20
/return

Sub RunBack(string MoveToZ, int MoveRange)
/if (${CheckDebug}) /echo \apCalling RunBack
	/declare MoveToID int local 0
	/declare MeLocYX ${Me.LocYX}
	/varset MoveToID ${Spawn[=${MoveToZ}].ID}
	|/echo \awRunBack Called
	|/echo (${Spawn[${MoveToID}].Distance3D}<=${MoveRange} || !${Bool[${Spawn[${Me.XTarget[1].ID}]}]})
	/if (${Spawn[${MoveToID}].Distance3D}<=${MoveRange} ) /return
	| Quick Hack to stop tank from returning home when mob runs 
	/if (${Target.PctHPs} < 50 && ${Bool[${Target}]}) /return 
	| WHILE 1 
	/while (${MobPulling}==1) {
		/if (${CheckDebug}) /echo \apRunBack While 1
		/varset MeLocYX ${Me.LocYX}
		/doevents flush Stunned
		/doevents 
		/squelch /attack off
		/delay 10 (!${Bool[${Me.Combat}]})
		/varset MoveDistance ${Spawn[${MoveToID}].Distance3D}
		/squelch /face fast nolook id ${MoveToID}
		| /echo run to ${MoveToID} to ${Spawn[${MoveToID}].Distance3D}<=${MoveRange}
		/if (${IWantToUseMQ2Nav} && ${Navigation.MeshLoaded} && ${Navigation.PathExists[id ${MoveToID} Distance=${Math.Calc[${MoveRange}-2]}]}) {
			/squelch /nav id ${MoveToID} Distance=${Math.Calc[${MoveRange}-2]}
			/delay 10 (${Spawn[${MoveToID}].Distance3D}<=${MoveRange})
			/squelch /nav stop
		} else {
			|/echo \atManual Move 
			/keypress Up Hold
			/delay 10 ${Spawn[${MoveToID}].Distance3D}<${MoveRange}
			/keypress Up
			/keypress Up
		}
		/if (${Spawn[${Me.XTarget[1].ID}].Distance3D}>=250 && ${Bool[${Spawn[${Me.XTarget[1].ID}]}]} && ${Bool[${Me.XTarget[1].ID}]}) {
			/squelch /nav stop
			/keypress Up
			/keypress Up
			/delay 10 ${Spawn[${Me.XTarget[1].ID}].Distance3D}<=250
		}
		| /echo math distance 2 ${Spawn[${MoveToZ}].Distance3D}>=55 && ${Math.Distance[${MeLocYX}]}<5 && ${MobPulling}==1 && ${PullTimer}==0
		/if (${Math.Distance[${MeLocYX}]}<5 && ${MobPulling}==1 && ${PullTimer}==0) {
			|/echo \awMeStuck 2
			/call MeStuck
		}
		/if (${Spawn[${MoveToZ}].Distance3D}<=${MoveRange}) {
			/squelch /nav stop
			/squelch /face fast 
			| New Code This will pause while monitoring if the mob is moving if the mob is coming then wait longer 
			/varset MoveDistance ${Target.Distance3D}
			/declare MoveHomeTimeout int local 0
			/delay 10 ${Target.Distance3D}<=25
			|/echo \ag${Math.Calc[${MoveDistance}-${Target.Distance3D}]}<0 and ${Target.Distance3D}<=25
			| So Wait till mob is within 100 then start to check if the mob stops moving as it should be running in when it gets in range attack.. if it stops to attack another player timeout and charge 
			| WHILE 2
			/while (${Math.Calc[${MoveDistance}-${Target.Distance3D}]}>1 && ${Target.Distance3D}>=${Target.MaxRange} || ${Target.Distance3D}>=110) {
				/if (${CheckDebug}) /echo \apRunBack While 2
				/varset MoveDistance ${Target.Distance3D}
				/delay 10 ${Target.Distance3D}<=120
				/delay 5
				/if (${Bool[${Me.TargetOfTarget}]} && ${Me.TargetOfTarget.Name.NotEqual[${Me}]} && ${MoveRange}*5<=120) {
					/echo \apMob Changed Targets Going after it 
					/break
				}
				|/echo \ayWaiting for Mob ${Math.Calc[${MoveDistance}-${Target.Distance3D}]}>1 movehometimeout ${MoveHomeTimeout} and ${Target.Distance3D}<=25
				/if (${Target.Distance3D}<=${Target.MaxRange}*2) {
					/echo \apEngaging Mob
					/break 
				}
				/if (${MoveHomeTimeout} > 15) {
					/echo \apMob Stuck left Pull by break !
					/break 
				}
				/varset MoveHomeTimeout ${Math.Calc[${MoveHomeTimeout}+1]}
			}
			| /echo \agend for Mob ${Math.Calc[${MoveDistance}-${Target.Distance3D}]}>1 movehometimeout ${MoveHomeTimeout} > 15 and ${Target.Distance3D}>=${Target.MaxRange} or || Target Greater than ${Target.Distance3D}>=110
			| /delay 20 ${Target.Distance3D}<=${Target.MaxRange}
			/return
		}
	 |/echo RunBack Loop
	}
/return

Sub MovingTo(string MoveToZ, int MoveRange)
/if (${CheckDebug}) /echo \awCalling MovingTo
	/declare MoveToID int local ${Spawn[=${MoveToZ}].ID}
	/if (${Spawn[${MoveToID}].Distance3D}<=${MoveRange}) /return
	
	/while (${Spawn[${MoveToID}].Distance3D}>=${MoveRange}) {
		/doevents 
		/if (${Spawn[${MoveToID}].Distance3D}<=${MoveRange}) /return
			/varset MoveDistance ${Spawn[${MoveToID}].Distance3D}
			/squelch /face nolook id ${MoveToID}
			/keypress Up Hold
			/delay 10 ${Spawn[${MoveToID}].Distance3D}<${MoveRange}
			/keypress Up
			/keypress Up
		/if (${Spawn[${MoveToID}].Distance3D}<=${MoveRange}) /return
	}
/return

Sub MeStuck
/if (${CheckDebug}) /echo \awCalling MeStuck
	/nav stop
	/varset Random ${Math.Rand[2]}
		/if (${Random}==0) /varset Strafe Strafe_Left
		/if (${Random}!=0) /varset Strafe Strafe_Right
	/keypress back hold
	/keypress ${Strafe} hold
	/delay 15
	/keypress Up Hold
	/delay 18
	/keypress ${Strafe}
	/keypress Up 
	/keypress Up
/return

| This is a Test to see if im smart enough to make a macro to move behind a mob 
| This should strafe around the mob 
| Version 0.5 Working Well
| FTarget is the MOB or Person you Wish to Follow, MobDistance is the Distance you Will Follow From, MobFight 0=False 1=True Tells the Program to exit when you are Behind the mob

| Put these Values into your Startup

Sub MoveBehind(string FTarget,int MobDistance,int MobFight)
/if (${CheckDebug}) /echo \awCalling MoveBehind
/if (${Target.Distance3D}>=70 || !${Me.Combat} && ${TargetID}==0 || ${Spawn[${TargetID}].Type.Equal[Corpse]} && ${TargetID}!=0 || !${Bool[${Spawn[${TargetID}]}]} && ${TargetID}!=0 || ${Spawn[${TargetID}].Type.Equal[PC]} && ${TargetID}!=0 || ${MoveBehindTimer}!=0 || ${Bool[${Me.Casting}]} && !${Me.Class.Name.Equal[Bard]}  || ${IWantToRange}) /return
	/varset FTarget ${Target.ID}
	/varset MobDistance ${Target.MaxRangeTo}
	/varset MobFight 0
	/varset Stay 0
	/doevents flush Follow
	/doevents flush Stay
	/declare LoopTimeout int local 0
	:Loop
	/varset LoopTimeout ${Math.Calc[${LoopTimeout}+1]}
	/if (${LoopTimeout}>4) /return | loop Timeout 
	/if (${Target.PctHPs} < 30) /return
	/if (${Bool[${Spawn[${FTarget}]}]} || !${Bool[${Target}]}) {
			/if (!${Me.Combat} && ${TargetID}==0 || ${Spawn[${TargetID}].Type.Equal[Corpse]} && ${TargetID}!=0 || !${Bool[${Spawn[${TargetID}]}]} && ${TargetID}!=0 || ${Spawn[${TargetID}].Type.Equal[PC]} && ${TargetID}!=0 || ${MoveBehindTimer}!=0) /return
		/squelch /target ${Spawn[${FTarget}]}
		/squelch /face fast nolook
		/call MoveClose ${FTarget} ${MobDistance}
		/doevents
		/if (${Spawn[${FTarget}].Distance3D}>500 || !${Bool[${Spawn[${FTarget}]}]}) /varset Stay 1
		/varcalc CalcHeading ${Spawn[${FTarget}].Heading.Degrees}-${Me.Heading.Degrees}
		/if (${Stay}==1 || !${Bool[${Target}]} ) /return
		/if (${CalcHeading}>=-33 && ${CalcHeading}<=33 && ${MobFight}==1 || ${CalcHeading}<=-327 && ${CalcHeading}>=327 && ${MobFight}==1) 	/return
		/if (${CalcHeading}>=-33 && ${CalcHeading}<=33 || ${CalcHeading}<=-327 && ${CalcHeading}>=327) 	{
			/if (${IWantToMoveBehind}) /goto :Loop
			/varset MoveBehindTimer ${Math.Calc[${Math.Rand[10]} + 2]}s
			/return			
		} 
		/if (${Spawn[${Me.TargetOfTarget}].ID}==${Spawn[${MTank}].ID}) {
			/squelch /face fast
			/varcalc CalcHeading ${Spawn[${FTarget}].Heading.Degrees}-${Me.Heading.Degrees}
			/if (${Stay}==1 || !${Bool[${Target}]} ) /return
			/if (${CalcHeading}>=-33 && ${CalcHeading}<=33 && ${MobFight}==1 || ${CalcHeading}<=-327 && ${CalcHeading}>=327 && ${MobFight}==1) 	/return
			
			/if (${CalcHeading}>=-180 && ${CalcHeading}<-33 || ${CalcHeading}>180 && ${CalcHeading}<327) 		/call Strife Strafe_Right ${FTarget} ${MobDistance}
			/if (${CalcHeading}>-327 && ${CalcHeading}<-180 || ${CalcHeading}<=180 && ${CalcHeading}>33) 		/call Strife Strafe_Left ${FTarget} ${MobDistance}
		}
	}
	/if (${Stay}!=1) /goto :Loop
/return

Sub MoveClose(string FTarget,int MobDistance)
/if (${IWantToTwist}) /call Twist				| IWantToTwist.Inc	
/if (${CheckDebug}) /echo \awCalling MoveClose
	/if (!${Me.Combat} && ${TargetID}==0 || ${Spawn[${TargetID}].Type.Equal[Corpse]} && ${TargetID}!=0 || !${Bool[${Spawn[${TargetID}]}]} && ${TargetID}!=0 || ${Spawn[${TargetID}].Type.Equal[PC]} && ${TargetID}!=0) /return
	/if (${Spawn[${FTarget}].Distance3D}>=${MobDistance}) {
		/squelch /face fast nolook
		/keypress Up Hold
		/delay 15 ${Spawn[${FTarget}].Distance3D}<=${MobDistance}
		/keypress Up
		/keypress Up
	/varset MoveBehindTimer ${Math.Calc[${Math.Rand[6]} + 0]}s
	/delay 5
	}
/return

Sub Strife(string KPress,string FTarget,int MobDistance)
/if (${CheckDebug}) /echo \awCalling Strife
/if (${IWantToTwist}) /call Twist				| IWantToTwist.Inc	
	/if (!${Me.Combat} && ${TargetID}==0 || ${Spawn[${TargetID}].Type.Equal[Corpse]} && ${TargetID}!=0 || !${Bool[${Spawn[${TargetID}]}]} && ${TargetID}!=0 || ${Spawn[${TargetID}].Type.Equal[PC]} && ${TargetID}!=0 || ${MoveBehindTimer}!=0) /return
	| Used to be Strafe_Right / Strafe_Left now its ? 	
	/squelch /face fast nolook
	/keypress ${KPress} Hold
	/delay 2
	/squelch /face fast nolook
	/delay 2
	/squelch /face fast nolook
	/delay 2
	/squelch /face fast nolook	
	/if (${Spawn[${FTarget}].Distance3D}>=${MobDistance}) {
		/squelch /face fast nolook
		/keypress ${KPress} 
	}
|	/delay 7 ${Spawn[${FTarget}].Distance3D}<=${MobDistance}
|	/squelch /face fast nolook
	/keypress ${KPress}


	/varcalc CalcHeading ${Spawn[${FTarget}].Heading.Degrees}-${Me.Heading.Degrees}
		/if (${CalcHeading}>=-33 && ${CalcHeading}<=33 || ${CalcHeading}<=-327 && ${CalcHeading}>=327) 	{
			/varset MoveBehindTimer ${Math.Calc[${Math.Rand[10]} + 2]}s
			/call MoveClose
			/return 
		}
		/if (${CalcHeading}>=-180 && ${CalcHeading}<-36 || ${CalcHeading}>180 && ${CalcHeading}<324) 		/call Strife Strafe_Right ${FTarget} ${MobDistance}
		/if (${CalcHeading}>-324 && ${CalcHeading}<-180 || ${CalcHeading}<=180 && ${CalcHeading}>36) 		/call Strife Strafe_Left ${FTarget} ${MobDistance}
	/varset MoveBehindTimer ${Math.Calc[${Math.Rand[10]} + 2]}s
	/call MoveClose
/return