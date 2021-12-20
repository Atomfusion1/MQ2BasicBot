Sub HealMT
/if (${CheckDebug}) /echo \agCalling Main Tank Heal
	| This Line Checks for Open Windows if Merchant or Trade windows are Open Macro will Just Loop
	/call CheckWindows /if (${Macro.Return}==1) /return
	/if (!${Bool[${Spawn[${MTank}]}]} || ${Me.Dead} || ${Math.Calc[${Macro.RunTime}-${MobTime}]}<=0) /return
| This Spam Heals Quick Heal if the Tanks HPs are below 20%
	/if (${Spawn[${MTank}].Type.Equal[PC]} && ${Spell[${SpellHeals[2]}].Mana}<${Me.CurrentMana} && ${Spawn[${MTank}].PctHPs}<${Heal[6]} && ${Spawn[${MTank}].Distance3D}<=${Spell[${SpellHeals[2]}].Range}) {
		/docommand /timed ${Math.Calc[(${Spell[${SpellHeals[2]}].MyCastTime.Float}*10)]} /if (${Spawn[${MTank}].PctHPs}>95) /stopcast
		/call SpellCast ${Spawn[${MTank}].ID} "${SpellHeals[2]}" "Sub HealMT:SpellHeals[2]"
		/echo \agHEALING: Quick Heal Spam on ${MTank}
		/g Quick Heal on ${MTank}
	    /varset SitTimer ${Math.Calc[${StaticSitTimer}]}s
		/varset NukeTimer ${Math.Calc[${StaticNukeTimer2}]}s
		/varset JustHealed TRUE
	}
	/if (${Spawn[${MTank}].PctHPs}>98 || !${Spawn[${MTank}].Type.Equal[PC]}) /return
	| This sets up Deathrate of Tank 
	/if (${Spawn[${MTank}].Type.Equal[PC]} && ${Spawn[${MTank}].PctHPs}>0 && ${Spawn[${MTank}].PctHPs}<=99 && ${Bool[${Spawn[${MTank}].LineOfSight}]} && ${DeathRate}==0) {
		/varset HRate 0
		/varset HRate ${Math.Calc[(${MTHPs}-${Spawn[${MTank}].PctHPs})/(${Macro.RunTime}-${MobTime})]}
| This is how the program Determines DeathRate 
| Main Healing Area Factor Area Changing Figures Here determines how You Heal 
		| This means the tank is taking little dmg This checks for .6* setting and heals after that or is dying faster then you can heal him 
		/if (${HRate}<.1 && ${Spawn[${MTank}].PctHPs}>=.6*${Heal[1]}  || ${HRate}>60) /return
		| DeathRate 1 means the tank is taking little dmg and doesnt need to be healed as fast 
		/if (${HRate}<=3) /varset DeathRate 1
		| DeathRate 2 means the tank is taking average dmg per second and should be healed at normal rate
		/if (${HRate}>=4 && ${HRate}<=7) /varset DeathRate 2
		| Death Rate 3 means the tank is taking more dmg per second and should be healed faster 
		/if (${HRate}>8) /varset DeathRate 3
		|/echo \agHeal DeathRate ${DeathRate} the Rate was ${HRate} in 20s
		}
	}
	/if (${DeathRate}!=0 && ${Spawn[${MTank}].PctHPs}<=1.2*${Heal[1]} && !${Bool[${Me.Casting}]} && !${Me.Moving} && ${Spawn[${MTank}].LineOfSight}) {
	| This heals the Tank Quicker if it Looks like CH will be to long
		/if (${DeathRate}==1 && ${Spawn[${MTank}].PctHPs}<=.4*${Heal[1]} && ${Spell[${SpellHeals[2]}].Mana}<=${Me.CurrentMana} && ${Spawn[${MTank}].Distance3D}<=${Spell[${SpellHeals[2]}].Range} && ${Spawn[${MTank}].Type.Equal[PC]} || ${DeathRate}==2 && ${Spawn[${MTank}].PctHPs}<=.6*${Heal[1]} && ${Spell[${SpellHeals[2]}].Mana}<=${Me.CurrentMana} && ${Spawn[${MTank}].Distance3D}<=${Spell[${SpellHeals[2]}].Range} && ${Spawn[${MTank}].Type.Equal[PC]} || ${DeathRate}==3 && ${Spawn[${MTank}].PctHPs}<=.8*${Heal[1]} && ${Spell[${SpellHeals[2]}].Mana}<=${Me.CurrentMana} && ${Spawn[${MTank}].Distance3D}<=${Spell[${SpellHeals[2]}].Range} && ${Spawn[${MTank}].Type.Equal[PC]}) {
			/docommand /timed ${Math.Calc[(${Spell[${SpellHeals[2]}].MyCastTime.Float}*10)]} /if (${Spawn[${MTank}].PctHPs}>95) /stopcast
			/call SpellCast ${Spawn[${MTank}].ID} "${SpellHeals[2]}" "Sub HealMT:SpellHeals[2]"
			/echo \agHEALING: Quick Heal a on ${MTank}
			/g Quick Heal a on ${MTank}
			/varset DeathRate 0
			/varset SitTimer ${Math.Calc[${StaticSitTimer}]}s
			/varset NukeTimer ${Math.Calc[${StaticNukeTimer2}]}s
			/varset JustHealed TRUE
			/return
		}
		/if (${DeathRate}==1 && ${Spawn[${MTank}].PctHPs}<=.8*${Heal[1]} && ${Spell[${SpellHeals[1]}].Mana}<=${Me.CurrentMana} && ${Spawn[${MTank}].Distance3D}<=${Spell[${SpellHeals[1]}].Range} && ${Spawn[${MTank}].Type.Equal[PC]} || ${DeathRate}==2 && ${Spawn[${MTank}].PctHPs}<=${Heal[1]} && ${Spell[${SpellHeals[1]}].Mana}<=${Me.CurrentMana} && ${Spawn[${MTank}].Distance3D}<=${Spell[${SpellHeals[1]}].Range} && ${Spawn[${MTank}].Type.Equal[PC]} || ${DeathRate}==3 && ${Spawn[${MTank}].PctHPs}<=1.2*${Heal[1]} && ${Spell[${SpellHeals[1]}].Mana}<=${Me.CurrentMana} && ${Spawn[${MTank}].Distance3D}<=${Spell[${SpellHeals[1]}].Range} && ${Spawn[${MTank}].Type.Equal[PC]}) {
			/docommand /timed ${Math.Calc[(${Spell[${SpellHeals[1]}].MyCastTime.Float}*10)]} /if (${Spawn[${MTank}].PctHPs}>95) /stopcast
			/echo \agHEALING: Large Heal on ${MTank}
			/g Large Heal on ${MTank}
			/call SpellCast ${Spawn[${MTank}].ID} "${SpellHeals[1]}" "Sub HealMT:SpellHeals[1]"
			/varset DeathRate 0
			/varset SitTimer ${Math.Calc[${StaticSitTimer}]}s
			/varset NukeTimer ${Math.Calc[${StaticNukeTimer2}]}s
			/varset JustHealed TRUE
			/return
		}
	} else {
		/varset DeathRate 0
	}
/return
| This Section is for Healing Over Time on the MTank
Sub HealHoT
/if (${CheckDebug}) /echo \agCalling Main Tank Heal
	| This Line Checks for Open Windows if Merchant or Trade windows are Open Macro will Just Loop
	/call CheckWindows /if (${Macro.Return}==1) /return
	/if (!${Bool[${Spawn[${MTank}]}]} || ${Me.Dead}) /return
	| This Casts Heal Over Time on Tank if hes in combat or out of combat and below 90 hps
	/if (${HoTTimer}==0 && ${Spawn[${MTank}].PctHPs}<=${Heal[3]} && ${Spawn[${MTank}].Distance3D}<=${Spell[${SpellHeals[3]}].Range} && ${Spawn[${MTank}].Type.Equal[PC]} || ${HoTTimer}==0 && ${Spawn[${MTank}].Type.Equal[PC]} && ${Spell[${SpellHeals[2]}].Mana}<${Me.CurrentMana} && ${Spawn[${MTank}].PctHPs}<${Heal[3]} && ${Spawn[${MTank}].Distance3D}<=${Spell[${SpellHeals[2]}].Range} && !${Bool[${Me.XTarget[1].ID}]}) {
		/call SpellCast ${Spawn[${MTank}].ID} "${SpellHeals[3]}" "Sub HealHoT:SpellHeals[3]"
		/echo \agHEALING: Casting ${SpellHeals[3]} on ${MTank}
		|/g HoT on ${MTank}
		/varset HoTTimer ${Spell[${SpellHeals[3]}].Duration.Seconds}s
		/varset SitTimer ${Math.Calc[${StaticSitTimer}]}s
		/varset NukeTimer ${Math.Calc[${StaticNukeTimer2}]}s
		/varset JustHealed TRUE
	/return
	}

/return 

Sub HealGroup
/if (${CheckDebug}) /echo \agCalling HealGroup
	| This Line Checks for Open Windows if Merchant or Trade windows are Open Macro will Just Loop
	/call CheckWindows /if (${Macro.Return}==1) /return
| Looking to use Group Heal 
	/if (${Bool[${Spell[${SpellHeals[4]}]}]} && ${Group}>=3) {
		/varset GHeal 0
		/for i 0 to ${Group}
			/if (!${Bool[${Spawn[${Group.Member[${i}]}]}]} || ${Group.Member[${i}].PctHPs}<=1 || ${Group.Member[${i}].Type.Equal[PC]}) /varcalc GHeal (${GHeal}+100)
			/if (!${Group.Member[${i}].Name.Equal[${MTank}]} && ${Group.Member[${i}].PctHPs}>5 && ${Bool[${Spawn[${Group.Member[${i}]}]}]}) /varcalc GHeal (${GHeal}+${Group.Member[${i}].PctHPs})
		/next i
		/if (${Group}>=3) /varcalc GHeal ${GHeal}/${Group}
| Change GHeal Value to adjust when Group heal will activate: its an average of the groups HPs
		/if (${GHeal}<=${Heal[4]} && ${Me.PctMana}>=25 && ${GHeal}!=0 && ${Group}>=3) {
			/call SpellCast ${Me.ID} "${SpellHeals[4]}" "Sub HealGroup:SpellHeals[4]"
	 		/echo \agHEALING: Group Heal Inc
			/g Group Heal Inc
			/varset SitTimer ${Math.Calc[${StaticSitTimer}]}s
			/varset JustHealed TRUE
		} else {
			/varset GHeal 0
		}
	}
| Looking to do Individual Heals of group members 
	/for i 0 to ${Group}
 		/if (${Group.Member[${i}].PctHPs}<=${Heal[2]} && ${Group.Member[${i}].PctHPs.Float}>=0.1 && ${Group.Member[${i}].Type.Equal[PC]} && ${Spell[${SpellHeals[2]}].Mana}<=${Me.CurrentMana} && ${Spawn[${Group.Member[${i}]}].Distance3D}<=${Spell[${SpellHeals[2]}].Range}) {
			/if (${Group.Member[${i}].ID}==${Spawn[${MTank}].ID} && ${IWantToHealMT}) /continue
			/docommand /timed ${Math.Calc[(${Spell[${SpellHeals[2]}].MyCastTime.Float}*10)]} /if (${Group.Member[${i}].PctHPs}>95) /stopcast
			/call SpellCast ${Group.Member[${i}].ID} "${SpellHeals[2]}" "Sub HealGroup:SpellHeals[2]"
			/g Healing ${Group.Member[${i}]}
			/varset SitTimer ${Math.Calc[${StaticSitTimer}]}s
			/varset JustHealed TRUE
		}
	/next i
/return

Sub DivineSelf
/if (${CheckDebug}) /echo \agCalling HealSelf
	| This Line Checks for Open Windows if Merchant or Trade windows are Open Macro will Just Loop
	/call CheckWindows /if (${Macro.Return}==1) /return
| Here is the Self Protection Script
	/if (${Me.PctHPs}<=${Heal[5]} && ${SelfTimer}==0 ) {
		/delay 40 !${Me.Moving}
		/echo \agHEALING: Low On HPs I am now casting ${DivineAura} Please Kill It or Run !!
		/g Low On HPs I am now casting ${DivineAura} Please Kill It or Run !!
		/docommand /timed ${Math.Calc[(${Spell[${SpellHeals[7]}].MyCastTime.Float}*10)]} /if (${Me.PctHPs}>95) /stopcast
		/call SpellCast ${Me.ID} "${SpellHeals[7]}" "Sub DivineSelf:SpellHeals[7]"
		/varset SitTimer ${StaticSitTimer}s
		/echo \agHEALING: Low On HPs I am now casting ${DivineAura} Please Kill It or Run !!
		/g Divine Aura is UP you Can Not Cast On Me ! 
		/docommand /timed ${Math.Calc[(${Spell[${SpellHeals[7]}].Duration.Raw}*10)]} /g Divine Aura is Down HEAL ME ! 
		/varset SelfTimer 5m
		/varset JustHealed TRUE
	}
/return

Sub CheckRez 
/if (${CheckDebug}) /echo \agCalling CheckRez
	| This Line Checks for Open Windows if Merchant or Trade windows are Open Macro will Just Loop
	/call CheckWindows /if (${Macro.Return}==1) /return
	/if (${Me.Dead} || ${Me.XTarget} > 0) /return
	/for i 0 to ${Group}
		/if (${Group.Member[${i}].Type.Equal[CORPSE]} && ${Group.Member[${i}].Distance3D} < 180 && ${TriedToRezTimer}==0) {
			/call SpellCast ${Group.Member[${i}].ID} "${RezSpell}" "Sub CheckRez:RezSpell"
			/varset TriedToRezTimer 4m
		}
	/next i
/return