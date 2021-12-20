| Events that happen In EQ 
#event Zoned "LOADING, PLEASE WAIT..."
#event IWasKilled "You have been knocked unconscious!"
#event Bash "you need to equip a shield in order to BASH"
#event Stunned "You are stunned!"
#event Move "Your target is too far away, get closer!"
#event Move "#*#cannot see your target#*#"
#event Move "You Can't hit them from here."
#event Exp "You gain#*#"
#event HasAggro "#*#YOU for #*#"
#event Invite   "#1# invites you to join a group."
|#event MakeFood "You are low on #1#."
#event MakeFood "You are out of #1#."
#event OutOfAmmo "You have run out of ammo!"
#event CampingOut "It will take about 10 more seconds to prepare your camp."
#event Resisted "#*#resisted your #1#!"
#event Resisted "Your spell is Interrupted."
#event Immune "Your Target is Immune to #1# spells."
#event ImmuneMez "Your target cannot be mesmerized."
#event ImmuneMez "Your target cannot be mesmerized #*#"
#event SetMez "#*#'Mob Is IMMUNE to MES HELP'"
#event WornOff "Your #1# spell has worn off of #2*."
#event CantLoot "#*#may not loot this corpse#*#"
#event Missing "You are missing some required components."
#event RangedClose "Your target is too close to use a ranged weapon!"



| Commands you send the MQ2 Window 
#event Info "[MQ2] info"
#event Help "[MQ2] Help"
#event QuickHelp "[MQ2] Quick Help"
#event IPaused "[MQ2] Lets Pause"
#event LazyMode "[MQ2] LazyMode"
#event MobPulling "[MQ2] Mob Pulling"
#event Camp "[MQ2] camp out in #1# Minutes"
#event Mod1 "[MQ2] Set Mob Pulling Distance to #1#" 
#event Mod2 "[MQ2] Set Mob Pull Rate to #1# Seconds"
#event Mod3 "[MQ2] Set Mob Pulling Pause to #1# Minutes"
#event Mod4 "[MQ2] #*#mana is less than 15#*#"
#event LetsDebuff "[MQ2] Lets Debuff"
#event MobPulling "#*#Mob Pulling on#*#"
#event StopTwisting "[MQ2] Stop Twisting"
#event AddPantherMob "[MQ2] Add My Target"
#event ReloadINI "[MQ2] ReloadINI"

| I Recommend using MQ2 IRC 
| Use Lets Camp On or Off
#event MATarget "#1# tells the group, 'Help Me Kill #2#'"
#event BuffUp "#1#'Ok Lets Buff Up'"
#event LetsZone "#1# tells the group, 'Ok Lets Zone'"
#event Follow "#1# tells the group, 'OTM'"
#event Stay "#1# tells the group, 'Ok Stay'"
#event HoldPull "#*#Guys Im About to Buff#1#"
#event GivePetWeapons "#*#Give#1#Pet Items#*#"
#event GiveMakeFood "#1# tells the group, 'Can I have some #2#'"
#event SummonItem "#1# tells the group, 'Summon me #2#'"
|#event Follow "#1#OTM#*#"
|#event Stay "#1#Stay#*#"
#event ComeClose "#*# 'Ok Come Closer'"
#event BackUp "#*# 'Ok Back Up'"
#event ToMe "#1# tells the group, 'Ok To Me'"
#event LogDesk "#*#'Ok Time for Bed'"
#event CampOn "#1# tells the group, 'Ok Lets Camp Here'"
#event CampOff "#1# tells the group, 'Ok Break Camp'"
#event MezUp "#1# tells the group, 'Ok Lets Mez'"
#event LetsDebuff "#1# tells the group, 'Ok Lets Debuff'"
#event BuffMe "#1# tells the group, 'Can I have #2#'"
#event BurnThis "#1# tells the group, 'Ok Burn This Mob'"
#event LetsRetreat "#1# tells the group, 'Ok Lets Retreat'"
#event LetsRetreat "You tell your party, 'Ok Lets Retreat'"

|****************************************************************************
| Main Events 
|****************************************************************************|	
Sub Event_QuickHelp
	/echo \ap/g Help Me Kill %T
	/echo \ap/echo LazyMode
	/echo \ap/echo MobPulling
	/echo \ap/g Ok Lets Buff Up
	/echo \ap/g Ok Lets Camp Here 
	/echo \ap/g Ok Break Camp
	/echo \ap/g OTM
	/echo \ap/g Ok To Me
	/echo \ap/g Ok Come Closer
	/echo \ap/g Ok Back Up
	/echo \ap/g Ok Stay
	/echo \ap/g Ok Lets Zone
	/echo \ap/g Ok Lets Retreat
	/echo \ap/g Ok Burn This Mob
	/echo \ap/g Ok Lets Mez
	/echo \ap/g Ok Lets Debuff 
/return

Sub Event_LogDesk 
	/dismount
	/delay 5
	/sit 
	/delay 5
	/echo \arOk Time for Bed Loggout 
	/camp 
	/delay 5
	/echo \arGood Night !
	/endm
/return 

Sub Event_Follow(string junk, string FTarget2)
	/call Event_CampOff
	/if (${Spawn[${FTarget2}].ID}==${Me.ID}) /return
	/varset IsBool TRUE
	/varset StopFollow FALSE
	| This part follows the one who calls OTM 
	/echo \awMy target is ${FTarget2}
	/squelch /target id ${Spawn[=${FTarget2}].ID}
	/delay 10 ${Target.ID}==${Spawn[=${FTarget2}].ID}
	/varset MoveToT ${Spawn[=${FTarget2}].ID}
	/call MoveTo 
/return

Sub Event_LetsRetreat(string junk, string FTarget2)
	/echo Retreating 
	/pet back 
	/pet back
	/if (${Me.Name.Equal[${MTank}]}) {
		/call MovingTo ${MainCamp} 5
	} else {
		/call MovingTo ${MainCamp} 15
	}
	/if (${Spawn[${MainCamp}].Distance3D} < 15) /delay 20
	/pet back 
	/pet back
	/if (${Me.Name.Equal[${MTank}]}) {
		/call MovingTo ${MainCamp} 5
	} else {
		/call MovingTo ${MainCamp} 15
	}
	/if (${Spawn[${MainCamp} PC].Distance3D} < 35) /delay 20
	/delay 50
	/pet attack
/return 

Sub Event_ComeClose
	/if (${IDist} > 4) {
	/varset IDist ${Math.Calc[${IDist}-5]}
		/if (${IDist}<4) /varset IDist 4
	
	}
	/echo \awDistance at ${IDist}
/return

Sub Event_BackUp
	/if (${IDist} < 100) {
	/varset IDist ${Math.Calc[${IDist}+5]}
		/if (${IDist}>100) /varset IDist 2
	
	}
	/echo \awDistance at ${IDist}
/return

Sub Event_ToMe(string junk, string Test)
	/call Event_CampOff
	/if (${Spawn[${MoveToT}].Distance3D}>320) /return
	/if (${Spawn[${Test}].ID}==${Me.ID}) /return
	/echo \awGoing To ${Test}
	/if (${Me.Sitting}) /stand
	/squelch /target id ${Spawn[=${Test}].ID}
	/delay 10 ${Target.ID}==${Spawn[=${Test}].ID}
	/while (${Target.Distance3D} > ${IDist}/2) {
		/squelch /face fast
		/keypress forward hold
		/delay 2 (${Target.Distance3D} <= ${IDist}/2)
		/keypress forward
		/doevents
	}
/return

Sub Event_Stay
	/varset IsBool FALSE
/return

Sub Event_ReloadINI
	/echo \ar Reloading INI 
	/beep
	| This Calls on the MySetupFile.inc and loads all your variables
	/call LoadIni "Panther/Char/${Me.CleanName}_Setup.ini" LOAD
	/echo \ar INI has been Reloaded 
/return

Sub Event_Missing
	/echo Missing Component for Pet
	/beep
	/varset ICheckPet FALSE
/return

Sub Event_RangedClose
	/varset TurnOffRanged TRUE
	/varset IWantToRange FALSE
	/squelch /attack on
/return

Sub Event_AddPantherMob
	/declare ArraySize int inner 0
	/varcalc ArraySize ${Math.Calc[${RV_MobArray.Size}-4+1]}
	/ini "Panther/PantherMob.ini" "${Zone.Name}" "mob${ArraySize}" "${Target.CleanName}"
	/call ReadINI "Panther/PantherMob.ini" "${Zone.Name}" Mob
/return

Sub Event_LetsDebuff
/if (!${IWantToDeBuff}) /return
	/if (${IDebuffNow}) {
		/varset IDebuffNow FALSE
		/echo \apI'm Not Debuffing Now
		/g I'm Not Debuffing Now
	} else {
		/varset IDebuffNow TRUE
		/echo \apI'm Now Debuffing 
		/g I'm Now Debuffing 
	}
/return

Sub Event_LazyMode
	/if (${LazyMode}) {
	/varset LazyMode FALSE
	/echo \ayLazyMode OFF
	} else {
	/varset LazyMode TRUE
	/echo \ayLazyMode ON
	/echo \ayJust Find some Aggro
	/echo \arMob Pulling Turned Off
	/nav stop
	/varset MobPulling 0
	/varset CampOut 0
	}
	
/return

Sub Event_Bash
	/echo \arNo Shield Detected 
	/varset Bash 0
/return

Sub Event_CampOn
	/call Event_Stay
	/varset LocY ${Me.Y}
	/varset LocX ${Me.X}
	/varset LocZ ${Me.Z}
	/varset CampUp TRUE
	/delay 10
	/if (${Me.Name.Equal[${MainCamp}]} || ${Me.Class.ShortName.Equal[BRD]}) /return
	/delay ${Math.Rand[100]}+1
	/declare TestHolder ${Math.Rand[2]}+1
	/if (${TestHolder} == 1) {
	/keypress Strafe_Left Hold
	/delay 3
	/delay ${Math.Rand[5]}+1
	/keypress Strafe_Left
	} else /if (${TestHolder} == 2) {
	/keypress Strafe_Right Hold
	/delay 3
	/delay ${Math.Rand[5]}+1
	/keypress Strafe_Right
	} else {
	/keypress back Hold
	/delay 3
	/delay ${Math.Rand[5]}+1
	/keypress back
	}
	/echo \agnow camping at ${LocY}, ${LocX}, ${LocZ}
/return

Sub Event_CampOff
	/varset LocY 0
	/varset LocX 0
	/varset LocZ 0
	/varset CampUp FALSE
	/echo \agnow Breaking Camp
/return


Sub Event_Stunned
	|/echo \awStunned pausing for 2s
	/varset TauntTimer 2s
/return

Sub Event_HasAggro
	/if (${Group.Member[0].Name.Equal[${MTank}]} || ${MobPulling}) /return
	/echo \arI Have Aggro Best Wait 
	/varset NukeTimer2 ${Math.Calc[${StaticNukeTimer2}*2]}s
	/varset DoTDuration ${Math.Calc[30*3]}s
	/varset SitTimer ${Math.Calc[${StaticSitTimer}*2]}s
	/varset SpellMezDuration 0
	/if (${HitTargetID} != ${TargetID}) {
		/varset StaticNukeTimer2 ${Math.Calc[${StaticNukeTimer2}+2]}
		/varset HitTargetID ${TargetID}
	}
/return

Sub Event_Zoned
/doevents flush
| End Macro Due to Zone
	/echo \agZone Detected Macro Paused for 10 seconds
	/nav reload
	/call ReadINI "Panther/PantherMob.ini" "${Zone.Name}" Mob
	/If (${Me.Combat}) /squelch /attack off
	/delay 10 (!${Bool[${Me.Combat}]})
	/varset MobPulling 0
	/varset MeAggro FALSE
	/varset CampUp FALSE
  /delay 50
  /doevents flush
/return

Sub Event_Move
| If Target is Locked and Target is not Fleeing You will move closer

	/if (${Bool[${Me.Casting}]} && !${Me.Class.ShortName.Equal[BRD]} || !${StickToMob} && ${MobPulling}==0 || ${LazyMode} && ${Target.PctHPs}>90) {
		/return
	}
	/squelch /face fast 
  	/if (${Spawn[${TargetID}].Distance3D}>${Math.Calc[${Spawn[${TargetID}].MaxRangeTo}-2]}) /keypress Up hold
	|/echo ${Target.Distance3D}<=${Math.Calc[${Target.MaxRangeTo}-2]} Event_Move
  	/delay 5 ${Target.Distance3D}<=${Math.Calc[${Target.MaxRangeTo}-2]}
	/doevents flush Move
	/keypress Up
	/keypress Up
/return

Sub Event_MobPulling
	/if (!${Bool[${Spawn[=${MainCamp}]}]}) {
		/echo \arCAN NOT PULL Check Macro Parameters No MainCamp
		/return
	}
	/if (${MobPulling}==0) {
		/beep
		/echo \ayMob Pulling Turned ON 
		/echo \ayPulling Distance is ${PullD}
		/echo \aySet /echo Set Mob Pulling Distance to ***
		/echo \ay/echo Add My Target (to add to list of approved mobs)
		/varset LazyMode FALSE
		/varset MobPulling 1
		/varset PCNear ${SpawnCount[PC radius ${Math.Calc[${PullD}*1.3]}]}
		/varset CampOut 1
		/varset CampOutTimer 180m
		/if (${CampOut}==1 && ${CampOutTimer}>0) /echo \ayCampOutTimer is Set Macro will Camp out in ${Math.Calc[${CampOutTimer}/600].Int} Minutes
		
		/if (${Group.GroupSize} < 6) {
			/beep
			/beep
			/beep
			/beep 
			/echo \arGroup only has ${Group.GroupSize} Members 
			/delay 100
		}
	} else {
		/echo \arMob Pulling Turned Off
		/varset MobPulling 0
		/varset CampOut 0
	}
/return

Sub Event_Mod1(string junk,int value)
	/varset PullD ${value}
	/echo \ayPull Distance now Changed to ${PullD}
	/echo \ayHave a good day
/return

Sub Event_Mod2(string junk,int value)
	/varset PullRate ${value}
	/echo \ayPull rate now Changed to ${PullRate} Seconds
	/echo \ayHave a good day
/return

Sub Event_Mod3(string junk,int value)
	/varset PullTimer ${value}m
	/echo \ayPulling paused for ${Math.Calc[${PullTimer}/600]} Minutes
	/echo \ayHave a good day
/return

Sub Event_IWasKilled
	/echo \arYou Have Been Killed
	/varset IPaused TRUE
	/varset IDied TRUE
	/varset IDiedTimer 25m
	/doevents flush
	| You Have Died now what 
	/echo \arDeath Detected Macro Paused for 10 seconds
	/If (${Me.Combat}) /squelch /attack off
	/delay 10 (!${Bool[${Me.Combat}]})
	| /varset MobPulling 0
	/varset MeAggro FALSE
	/varset LazyMode FALSE
	/doevents flush
	| Lets Wait for Rez 
	/call Wait4Rez
	/sit
	/delay 100s (${Bool[${Me.XTarget}]})
	/g Lets Buff Up
/return

Sub Event_IPaused
	/if (${IPaused}) {
		/varset IPaused FALSE
		/echo \ayI Just Unpaused
	} else {
		/varset IPaused TRUE
		/echo \ayI Just Paused Macro
	}
	
/return

Sub Event_GivePetWeapons(string junk, string PTarget)
	/if (${Me.Class.ShortName.Equal[MAG]}) { 
	/echo \auGive Weapons Event Target is ${PTarget}
	/call PetItems ${Spawn[${PTarget}].ID}
	}
/return

Sub Event_MakeFood(string junk, string Line)
	/if (${FoodTimer} !=0 || ${Bool[${Me.XTarget[1].ID}]}) /return
	/if (${IMakeFood}) {
		/if (${Line.Find[Drink]}) /call FoodItems ${Me.ID} Drink
		/if (${Line.Find[Food]}) /call FoodItems ${Me.ID} Food
		/echo \auMade ${Line} for Me 
	}
	/if (!${IMakeFood} && ${IAskForFood}) {
		/if (${Line.Find[Drink]}) {
			/g Can I Have some Drink
			/varset PullTimer 1m
		}
		/if (${Line.Find[Food]}) {
			/g Can I Have some Food
			/varset PullTimer 1m
		}
	}
	/varset FoodTimer 20m
/return

Sub Event_GiveMakeFood(string junk, string ETarget, string Line)
	/if (${Bool[${Me.XTarget[1].ID}]} || !${IGiveFood} || !${IMakeFood}) /return
		/declare NameID int local ${Spawn[=${ETarget}].ID}
		/squelch /target id ${NameID}
		/squelch /face ${NameID}
		| Summon Item
		/if (${Line.Find[Drink]}) /call FoodItems ${NameID} Drink
		/if (!${Window[TradeWnd].HisTradeReady} && ${AuthList.Find[${Window[TradeWnd].Child[TRDW_HisName].Text}]}) /notify TradeWnd TRDW_Trade_Button leftmouseup
		/delay 10
		/delay 50 !${Window[TradeWnd].Open}
		/if (${Line.Find[Food]}) /call FoodItems ${NameID} Food
		/if (!${Window[TradeWnd].HisTradeReady} && ${AuthList.Find[${Window[TradeWnd].Child[TRDW_HisName].Text}]}) /notify TradeWnd TRDW_Trade_Button leftmouseup
		/delay 10
		/delay 50 !${Window[TradeWnd].Open}
		/echo \auGave ${Line} to ${Spawn[${NameID}]}
/return

Sub Event_SummonItem(string junk, string ETarget, string SummonSpell)
/if (${Bool[${Me.XTarget[1].ID}]} || !${ISummonItems}) /return
	/echo \auSummoning ${SummonSpell} for ${ETarget}
	/declare NameID int local ${Spawn[=${ETarget}].ID}
	/squelch /target id ${NameID}
	/squelch /face ${NameID}
	/while (${Spawn[${Target}].Distance3D}>20 && ${Spawn[${Target}].Distance3D}<100) {
		/call MovingTo ${ETarget} 19

	}
	| Lets give you time to stop 
	/delay 20 (!${Me.Moving})
	/delay 10
	| Summon Item 
	/if (!${Bool[${Spell[${SummonSpell}]}]}) {
	/g Sorry I dont have ${SummonSpell}
	/return 
	}
	/echo /if (!${Bool[${Me.Gem[${SummonSpell}]}]} && !${Me.Moving} ) 
		/if (!${Bool[${Me.Gem[${SummonSpell}]}]} && !${Me.Moving} ) {
		/echo Mem Spell Called
			/call SpellMem 8 "${SummonSpell}"
			/delay 20
		} 
		/call SpellCast ${Me.ID} "${SummonSpell}" "Event_SummonItem:SummonSpell"
		/delay 10
		/if (${Cursor.NoRent}) {
			/echo \auItem in hand giving ${SummonSpell} to ${ETarget}
			/target id ${NameID}
			/delay 10 (${Bool[${Target}]})
			/if (${Target.ID} == ${NameID} && ${Spawn[${Target}].Distance3D}<25) {
				/click left target
				/delay 10 ${InvSlot[trade${i}].Item.ID}
				/delay 5
				/if (${Window[TradeWnd].Open}) /notify TradeWnd TRDW_Trade_Button leftmouseup
				/delay 10
				/delay 50 !${Window[TradeWnd].Open}
			}
		}
		/autoinv
/return

Sub Event_Info
   /if (${Math.Calc[${Macro.RunTime}/3600]}>0) {
   	/echo \ayYour Exp: ${Int[${MyExp}]}%
   	/echo \ayYour AAExp: ${Int[${MyAAExp}]}%
   	/echo \ayTotal Exp Gained: ${Int[${TotalExp}]}% 
   	/echo \ayExp Per Hour: ${Math.Calc[${TotalExp}/${Math.Calc[${Macro.RunTime}/3600]}]}%
   	/echo \amTotal Play Time: ${Int[${Math.Calc[${Macro.RunTime}/60]}]} Minutes 
   	/echo \amPulling Rate is ${PullRate} seconds and Pull Distance set to ${PullD} and max pull size ${PullSize}
	} else {
		/echo \arThe macro must run longer ... Please Try Again Later.
	}
	/if (${CampOut}==1 && ${CampOutTimer}>0) /echo \ayCampOutTimer is Set Macro will Camp out in ${Math.Calc[${CampOutTimer}/600].Int} Minutes
/return

Sub Event_Invite(junk,Inviter)
	/echo \abtrying to find ${Inviter} ${AuthList.Find[${Inviter}]}
	/if (${AuthList.Find[${Inviter}]}) /invite
/return

Sub Event_Exp
	/varset MyAAExp ${Math.Calc[${Me.PctAAExp}-${MyAAExp}]}
	/varset MyExp ${Math.Calc[${Me.PctExp}-${MyExp}]}
	
	| if you ding or die this will return without changing Total Exp Stops Negative Numbers
	/if (${MyExp}<0 || ${MyExp}>40) {
		/varset MyExp 0
		/return
	}
	/varset TotalExp ${Math.Calc[${MyExp}+${TotalExp}]}
	|Check if turned off Ranged as you where to close 
	/if (${TurnOffRanged}) {
		/varset TurnOffRanged	FALSE
		/varset IWantToRange	TRUE
	}
	
	/if (${Math.Calc[${Macro.RunTime}/3600]}>0) {
		/echo \a-gXP Gained \ag${MyExp} \a-gSession \ag${TotalExp} \a-gAA Gained \ag${MyAAExp} \a-gXP Per Hour \ag${Math.Calc[${TotalExp}/${Math.Calc[${Macro.RunTime}/3600]}]} \a-gRun Time: \ag${Math.Calc[${Macro.RunTime}/60]}m
		|/echo \a-gTotal XP This Session \ag${TotalExp} \a-gXP Per Hour \ag${Math.Calc[${TotalExp}/${Math.Calc[${Macro.RunTime}/3600]}]} \a-gTotal Run Time: \ag${Math.Calc[${Macro.RunTime}/60]} minutes 
	}
	/varset MyExp ${Me.PctExp}
	/varset MyAAExp ${Me.PctAAExp}

	| Timer till Next Mob Pulled
	/if (!${RapidPulling}) /varset PullTimer ${PullRate}s
	/varset MezzedMobs ${Math.Calc[${MezzedMobs}-1]}
	/varset TargetID 0  | Reset Call for if you kill mob not First 
	/varset SnareImmune false 
	| /varset LootTimer 20s
/return

Sub Event_Camp(string junk,int CampTimer)
	/varset CampOut 1
	/varset CampOutTimer ${CampTimer}m
	/if (${CampOut}==1 && ${CampOutTimer}>0) /echo \ayCampOutTimer is Set Macro will Camp out in ${Math.Calc[${CampOutTimer}/600].Int} Minutes
/return

Sub Event_Mod4
	/echo \arCleric Mana is Low Pausing for 4 Minutes
	| /varset PullTimer 4m
/return

Sub Event_MezUp
/if (!${IWantToMez}) /return	
	/if (!${MezzingMobs}) {
		/g Trying to Mez Everything in Sight
		/echo \ayMezing Turned On 
		/varset SpellMezDuration 0 
		/if (!${Bool[${TargetID}]}) /varset TargetID 1
		/call DeBuffMezz
		/varset MezzingMobs TRUE
	} else {
		/echo \ayMezing turned off 
		/varset MezzingMobs FALSE
		/g I Wont Mez For Now
	}
/return

Sub Event_HoldPull
	/squelch /nav stop
	/if (!${LazyMode} && ${Spawn[${MainCamp}].Distance}<300) /call MovingTo ${MainCamp} 45
	/if (${MobPulling}) /echo \arBuffs Called Holding Pulls for 45s
	/if (!${Bool[${Bool[${Me.XTarget[1]}]}]}) /varset PullTimer 45s
/return

Sub Event_Help
	/echo \arThis Macro Has the Following Commands
	/echo \ay/echo Quick Help = commands to setup
	/echo \ay/echo info = shows exp gained and played time 
	/echo \ay/echo Camp out in * minutes ... Camps out The Group after * minutes
	/echo \ay/echo Set Mob Pulling Distance to * ... Changes Mob Pulling Distance to *
	/echo \ay/echo Set Mob Pulling Rate to * Seconds ... Change the Mob Pulling Rate to * Seconds
	/echo \ay/echo Set Mob Pulling Pause to * Minutes ... Pauses Pulling for * Minutes	
	/echo \ay/echo Mob Pulling ... This Engages Mob Pulling or Disengages Mob Pulling 
	/echo \ay/g Lets Buff Up
	/echo \ay/g Ok Time for Bed
	/echo 
	/echo \ay/endm Ends Macro
/return

Sub Event_LetsZone(string junk)
/declare ZoneImIn string local NULL
/declare TimeOutZone int local 3
/varset ZoneImIn ${Zone}
	| lets random time move to the 
	/dismount
	/echo \agGoing to Zone
	/delay ${Math.Rand[60]} 
	/delay ${Math.Rand[30]}
	| Start loop to zone 
	/while (${Zone.Name.Equal[${ZoneImIn}]}) {
		/echo \agTrying to Zone
		/if (${TimeOutZone}<=0) {
			/echo \agZone Timeout 
			/break
		} else /varset TimeOutZone ${Math.Calc[${TimeOutZone}-1]}
		/delay ${Math.Rand[30]}
		/doortarget
		/timed 2 
		/squelch /face door
		/timed 2 
		/if (${Switch.Distance}>8) /keypress Up hold
		/delay 15 ${Switch.Distance}<8
		/keypress Up 
		/keypress Up
		
		/if (!${Switch.Open} && ${Zone.Name.Equal[${ZoneImIn}]}) {
			/click left door
			/delay 2
			/click left door
		}	
		/delay 60 ${Zone.Name.Equal[${ZoneImIn}]}
	}
	/delay 10
	/doevents flush
	
/return


Sub Event_BuffUp
	/varset BuffTimer 0
	/varset DSTimer 0
	/varset BuffsCalled TRUE
/return

Sub Event_MATarget(string junk, string FTarget2)
	| This Joins the Group in Attack
	/squelch /target clear
	/if (${IWantToMelee} && ${Me.Sitting}) /stand
	/squelch /assist ${FTarget2}
	/delay 30 ${Bool[${Target.Name}]}
	/echo \amAssisting ${FTarget2} on ${Target.CleanName}
	/squelch /pet attack
	/varset TargetID ${Target.ID}
	/varset TrackDebuff[1,1] ${Target.ID}
	/for i 1 to 5
		/varset TrackDebuff[3,${i}] 0
	/next i
	| Dot Delay after Engaged Called 
	/varset DoTDuration 1s
	| Nuke Engage after Engaged Called
	/varset NukeTimer2 2s
	| /echo DoTDuration= ${DoTDuration} and NukeTimer2= ${NukeTimer2} and StaticNukeTimer2=${StaticNukeTimer2}
	/varset MobTime ${Macro.RunTime}
	/varset MTHPs ${Spawn[${MTank}].PctHPs}
	/varset MobHPs ${Target.PctHPs}	
	/varset StopFollow TRUE
	/varset SitTimer 15s
	/if (${IWantToMelee} && !${IWantToRange}) {
		/squelch /stand
		/squelch /attack on
		/delay 10 ${Me.Combat}
		} else /if (${IWantToRange}) {
		/squelch /stand
			/range
			/face fast 
			/autofire
			/delay 5
		}
/return

Sub CampOut
| /if (${CheckDebug}) /echo \arCampOut
	/if (${CampOutTimer}>4800 && ${CampOutTimer}<5000 && ${CampOut}==1 && ${Spawn[${MainCamp}].Distance3D}<=55) {
		/echo \arFirst Warning Camping Out in 5000s!!
		/beep
		/beep
		/beep
		/delay 300
		/echo \arFirst Warning Camping Out in 5000s!!
		/beep
		/beep
		/beep
	}
	
	/if (${CampOutTimer}==0 && ${CampOut}==1 && ${Spawn[${MainCamp}].Distance3D}<=55 && ${SpawnCount[NPC radius ${Math.Calc[${PullD}/4.5]}]}==0 && !${Me.CombatState[].Equal[COMBAT]} || ${CampOutTimer}==0 && ${CampOut}==1 && ${IPaused}) {
		/dismount
		/stand
		/sit
		/echo \arNow Camping Out Good Night 4!!
		/beep
		/beep
		/beep
		/delay 300
		/echo \arNow Camping Out Good Night 3!!
		/beep
		/beep
		/beep
		/delay 300
		/echo \arNow Camping Out Good Night 2!!
		/beep
		/beep
		/beep
		/delay 300
		/echo \arNow Camping Out Good Night 1!!
		/beep
		/beep
		/beep
		/delay 300
		| PRIVATE 
		/tell Naiody Ok time for bed
		/g ok time for bed
		/camp 
		/endm
		/varset CampOut 0
	}
/return

Sub Event_OutOfAmmo
	|/echo OUT of Ammo
	/varset IHaveArrows FALSE
/return

Sub Event_CampingOut
	/echo \arYou are camping out ending macro 
	/end
/return

Sub Event_StopTwisting
	/if (${ImTwisting}) {
		/varset ImTwisting FALSE
		/echo \ayTwisting Turned OFF
	} else {
		/varset ImTwisting TRUE
		/echo \ayTwisting Turned ON	
	}
/return

Sub Event_BuffMe(string junk, string ETarget, string SummonSpell)
/if (!${Bool[${Me.Book[${SummonSpell}]}]}) /echo I dont have ${SummonSpell}
/if (${Bool[${Me.XTarget[1].ID}]} || !${Bool[${Me.Book[${SummonSpell}]}]}) /return
	/echo \auCasting ${SummonSpell} for ${ETarget}
	/declare NameID int local ${Spawn[=${ETarget}].ID}
	/squelch /target id ${NameID}
	/squelch /face ${NameID}
	| Lets give you time to stop 
	/delay 20 (!${Me.Moving})
	/delay 10
	| Summon Item 
		/if (!${Bool[${Me.Gem[${SummonSpell}]}]} && !${Me.Moving} ) {
			/call SpellMem 8 "${SummonSpell}"
			/delay 20
		} 
	/call SpellCast ${NameID} "${SummonSpell}" "Event_BuffMe:SummonSpell"
	/delay 5
	/delay ${Math.Calc[${Spell[${SummonSpell}].MyCastTime}/1000]} 
/return

Sub Event_Resisted( junk, ResistSpell)
	|/echo RDetected ${ResistSpell}
	/varset ResistDet TRUE
	/doevents Immune
/return

Sub Event_Immune(junk, ResistedSpell)
	/echo \ar Mob Is IMMUNE to ${ResistedSpell}
	/varset ResistDet FALSE
	/if (${ResistedSpell.Equal["snare"]}) {
		/echo \arSnare Spell Resisted 
		/varset SnareImmune true
	}
/return

Sub Event_SetMez(junk)
	/echo \ar Changing Mode to MezImmune 
	/varset MezImmune TRUE
/return

Sub Event_ImmuneMez(junk, ResistedSpell)
	/g Mob Is IMMUNE to MES HELP 
	/varset ResistDet FALSE
/return

Sub Event_WornOff(junk, ETarget)
	/varset BuffTimer 10s
/return 

Sub Event_CantLoot(junk)
	/varset LootTimer 10m
/return

Sub Event_BurnThis(string junk, string ETarget)
	/if ( ${Bool[${Me.Gem[${SpellNukes[1]}]}]} && !${IWantToHealMT} ) {
		/squelch /assist ${ETarget}
		/delay 30 ${Bool[${Target.Name}]}
		/g Ok Burning %t
		/echo \ayBurning a ${Target.CleanName}
		/while (${Target.Type.Equal[NPC]} && ${Target.PctHPs} > 0 && ${Bool[${Me.XTarget[1].ID}]} ) {
			/if (!${SpellNukes[1].Left[1].Equal["#"]}) /call SpellCast ${Target.ID} "${SpellNukes[1]}" "Event_BurnThis:SpellNukes[1]"
			/if (!${SpellNukes[2].Left[1].Equal["#"]}) /call SpellCast ${Target.ID} "${SpellNukes[2]}" "Event_BurnThis:SpellNukes[2]"
			/if (!${SpellNukes[3].Left[1].Equal["#"]}) /call SpellCast ${Target.ID} "${SpellNukes[3]}" "Event_BurnThis:SpellNukes[3]"
		}
	}

/return 

Sub Checks
/if (${CheckDebug}) /echo \atCalling Checks
| Checks if Character is at Character Select Screen
	/if (${EverQuest.GameState.Equal[CHARSELECT]}) /end 
	/if (${Me.Feigning} && !${Me.Class.ShortName.Equal[MNK]}) /stand
	/if (!${Bool[${Me.XTarget}]}) /varset MezImmune FALSE
	/declare SomeoneDead int inner 0
	/for i 0 to ${Group}
		/if (!${Bool[${Spawn[=${Group.Member[${i}]}]}]} && ${Bool[${Spawn[${Group.Member[${i}]} CORPSE]}]}) {
			/if (!${DeadMember}) {
				/echo \ar${Group.Member[${i}]} is Dead
				/echo \ar${Group.Member[${i}]} is Dead
			}
			/varset SomeoneDead ${Math.Calc[${SomeoneDead}+1]}
			/varset DeadMember TRUE
		}
	/next i
	/if (${SomeoneDead}!=0) {
		/if (${DeadMemberTimer}==0) {
			/varset DeadMemberTimer 20m
			/echo \arMEMBER DEAD CAMPING IN 19m
			/beep
			/beep
			/beep
		} else /if (${DeadMemberTimer} < 1200) {
			/disband
			/echo \arCamping NOW
			/beep
			/beep
			/beep
			/camp desktop
			/delay 400
		} else /if (${DeadMemberTimer} < 1800) {
			/echo \arCAMPING IN 1m
			/beep
			/beep
			/beep
			/delay 400
		}
	} else {
		/varset DeadMember FALSE
	
	}
	/if (${Spawn[gm].ID}) { 
      /beep 
      /beep 
      /beep 
 
      /echo \arGM has entered the zone! 
      /echo \arFUCK HIM but ending the macro... 
      /keypress forward 
      /keypress back 
      /endmacro 
   } 
/return