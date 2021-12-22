|AUTOBOT XP AND RAIDING MACRO v4.12
| 20060826 mod by moeymoejoe (4.12)
| dkaa update 4.9
| 1.  Fix numerous bugs
| 2.  Don't fast heal tanks if TankHealPct set
| I have used code from Raid Druid here The Sub LoadVar 


Sub LoadVar(IniSection,IniVar,IniValue,MacroVar,MyIni,Function,VarType)
	/if (!${Defined[${MacroVar}]} && ${Defined[VarType]}) {
		/if (${Bool[${String[${MacroVar}].Find[[]}]} && ${Defined[${String[${MacroVar}].Left[${Math.Calc[${String[${MacroVar}].Length}-3]}]}]}) {
		} else {
			/echo \arVariable Not Defined 2 ${String[${MacroVar}].Left[${Math.Calc[${String[${MacroVar}].Length}-3]}]} 
			/declare ${MacroVar} ${VarType} outer
		}
	}
	/if (${Function.Equal[LOAD]}) {
		/declare IniString string local ${Ini[${MyIni},${IniSection},${IniVar},NOTFOUND]}
		/varset ${MacroVar} ${IniString}
	}
	/if (${IniString.Equal["NOTFOUND"]} || ${Function.Equal[SAVE]}) {
		/if (${IniString.Equal["NOTFOUND"]} && ${Function.Equal[LOAD]}) {
			/if (${MacroVar.Equal["AuthList"]}) {
				/echo \arFirstTime Detected Setup Defaults 
				/varset IniValue ${MTank} ${MainCamp}
				/varset AuthList ${IniValue}
			} else {
				/echo \arFirstTime Detected Setup Defaults 
				/varset ${MacroVar} ${IniValue}
			}
		}
		/ini ${MyIni} "${IniSection}" "${IniVar}" "${IniValue}"
	}
/return

Sub LoadIni(MyIni,Function)
	/echo \a-yI have entered ${Me.Name} setup file ${MyIni}
	/pet who leader
	/hidecorpse looted
	/useadvlooting off
	/assist off
	/nav reload
	/fastdrop on 
	/lootn never 

	| General Settings All Bots
	/if (${Bool[${Group.Members}]}==FALSE) {
		/call LoadVar GeneralStuff AuthList "Put your list of master bots here" AuthList "${MyIni}" ${Function} string
	} else {
		/declare TempString string local ${Group.Member[0].CleanName} ${Group.Member[1].CleanName} ${Group.Member[2].CleanName} ${Group.Member[3].CleanName} ${Group.Member[4].CleanName} ${Group.Member[5].CleanName} ${Group.Member[6].CleanName}
		/echo Group Members added to Master List ${TempString}
		/call LoadVar GeneralStuff AuthList "${TempString}" AuthList "${MyIni}" ${Function} string
	}
	/call LoadVar GeneralStuff CheckDebug FALSE CheckDebug "${MyIni}" ${Function} bool
	/call LoadVar GeneralStuff IAskForFood FALSE IAskForFood "${MyIni}" ${Function} bool
	/call LoadVar GeneralStuff StayAtMainCamp TRUE StayAtMainCamp "${MyIni}" ${Function} bool
	/call LoadVar GeneralStuff IWantToSit TRUE IWantToSit "${MyIni}" ${Function} bool
	/call LoadVar GeneralStuff StaticSitTimer 10 StaticSitTimer "${MyIni}" ${Function} int
	/call LoadVar GeneralStuff IHaveMount FALSE IHaveMount "${MyIni}" ${Function} bool
	/call LoadVar GeneralStuff MountItemSlot "#Name Of Item here" MountItemSlot "${MyIni}" ${Function} string
	/call LoadVar GeneralStuff MountSpellName "#Name of Mount Spell" MountSpellName "${MyIni}" ${Function} string
	/call LoadVar GeneralStuff SpellAura "#Aura spell Here" SpellAura "${MyIni}" ${Function} string

	| Melee Settings
	/call LoadVar MeleeStuff IWantToMelee TRUE IWantToMelee "${MyIni}" ${Function} bool
	/call LoadVar MeleeStuff IWantToRange FALSE IWantToRange "${MyIni}" ${Function} bool
	/call LoadVar MeleeStuff StickToMob TRUE StickToMob "${MyIni}" ${Function} bool
	/call LoadVar MeleeStuff IWantToMoveBehind FALSE IWantToMoveBehind "${MyIni}" ${Function} bool
	/call LoadVar MeleeStuff MobDistance 20 MobDistance "${MyIni}" ${Function} int
	| Pull Settings 
	/if (${Select[${Me.Class.ShortName},WAR,MNK,ROG,BER,SHD,PAL,RNG,BRD,BST]}) {
		/call LoadVar PullSettings IWantToPull FALSE IWantToPull "${MyIni}" ${Function} bool
		/call LoadVar PullSettings PctCallTarget 99 PctCallTarget "${MyIni}" ${Function} int
		/call LoadVar PullSettings IWantToUseMQ2Nav FALSE IWantToUseMQ2Nav "${MyIni}" ${Function} bool
		/call LoadVar PullSettings SpawnD 60 SpawnD "${MyIni}" ${Function} int
		|/call LoadVar PullSettings PullD 480 PullD "${MyIni}" ${Function} int
		/call LoadVar PullSettings PullRate 5 PullRate "${MyIni}" ${Function} int
		/call LoadVar PullSettings PullN 7 PullN "${MyIni}" ${Function} int
		|/call LoadVar PullSettings PullSize 1 PullSize "${MyIni}" ${Function} int
		/call LoadVar PullSettings StaticNukeTimer2 20 StaticNukeTimer2 "${MyIni}" ${Function} int
		/call LoadVar PullSettings PullSpell "#Pulling Spell" PullSpell "${MyIni}" ${Function} string
		/call LoadVar PullSettings PullCA "#Pulling CA" PullCA "${MyIni}" ${Function} string
		/call LoadVar PullSettings RapidPulling "FALSE" RapidPulling "${MyIni}" ${Function} string
		/call LoadVar PullSettings MinPullHP 5 MinPullHP "${MyIni}" ${Function} int
	}
	| Tank Taunt 
	/if (${Select[${Me.Class.ShortName},WAR,SHD,PAL]}) {
		/call LoadVar TankStuff IWantToTaunt TRUE IWantToTaunt "${MyIni}" ${Function} bool
		/call LoadVar TankStuff IWantToTauntCast FALSE IWantToTauntCast "${MyIni}" ${Function} bool
		/call LoadVar TankStuff NoMezzer TRUE NoMezzer "${MyIni}" ${Function} bool
		/call LoadVar TankStuff ICallTarget FALSE ICallTarget "${MyIni}" ${Function} bool
	}
	| Hybrid Setup
	/if (${Select[${Me.Class.ShortName},SHD,PAL]}) {
		/for i 1 to 4
			/call LoadVar TankStuff SpellTaunts[${i}] "#Anger Spells Here" SpellTaunts[${i}] ${MyIni} ${Function} string
		/next i
	}
	| Group Healing Setup
	/if (${Select[${Me.Class.ShortName},CLR,DRU,SHM]}) {
		/call LoadVar HealSettings IAmHealing TRUE IAmHealing "${MyIni}" ${Function} bool
		/call LoadVar HealSettings IWantToHealMT FALSE IWantToHealMT "${MyIni}" ${Function} bool
		/call LoadVar HealSettings IWantToHoT FALSE IWantToHoT "${MyIni}" ${Function} bool
		/call LoadVar HealSettings IHealGroup FALSE IHealGroup "${MyIni}" ${Function} bool
		/call LoadVar HealSettings Heal[1] "70 | Changing this value will change when the cleric casts a Big heal on the main tank (Recommended value 60-80) Slot 1" Heal[1] "${MyIni}" ${Function} int
		/call LoadVar HealSettings Heal[2] "65 | this value is the % at which the macro will heal group Member Slot 2" Heal[2] "${MyIni}" ${Function} int
		/call LoadVar HealSettings Heal[3] "95 | Changing this value will change when the cleric casts Heal over Time on the Main Tank (Recommended Value 80-99) Slot 3" Heal[3] "${MyIni}" ${Function} int
		/call LoadVar HealSettings Heal[4] "70 | this value is the % Average at which the macro will Group Heal Slot 4 (Not counting the Tank)" Heal[4] "${MyIni}" ${Function} int
		/call LoadVar HealSettings Heal[5] "24 | This value is the % at which the macro will use Divine Aura to protect you" Heal[5] "${MyIni}" ${Function} int
		/call LoadVar HealSettings Heal[6] "35 | This is the Value that it will just Quick Heal the Tank" Heal[6] "${MyIni}" ${Function} int

		/call LoadVar HealSpells SpellHeals[1] "#Largest Heals for Main Tank Slot 1" SpellHeals[1] "${MyIni}" ${Function} string
		/call LoadVar HealSpells SpellHeals[2] "#Spell for Quick Heal on Group and MT Slot 2" SpellHeals[2] "${MyIni}" ${Function} string
		/call LoadVar HealSpells SpellHeals[3] "#Heal over Time  Slot 3" SpellHeals[3] "${MyIni}" ${Function} string
		/call LoadVar HealSpells SpellHeals[4] "#Group Heal Slot 4" SpellHeals[4] "${MyIni}" ${Function} string
		
		/call LoadVar RezSetup IWantToRez FALSE IWantToRez "${MyIni}" ${Function} bool
		/call LoadVar RezSetup RezSpell "#Rez Spell" RezSpell "${MyIni}" ${Function} string

	}
	| Melee Ability 
	/if (${Select[${Me.Class.ShortName},WAR,MNK,ROG,BER,SHD,PAL,RNG,BRD,BST]}) {
		/for i 1 to 7
			/call LoadVar MeleeAbility MAbility[${i}] "#Name of MA here" MAbility[${i}] "${MyIni}" ${Function} string
		/next i
		/for i 1 to 5
			/call LoadVar MeleeAbility CAbility[${i}] "#Name of CA here" CAbility[${i}] "${MyIni}" ${Function} string
		/next i
	}
	| Spell Nukes 
	/if (${Select[${Me.Class.ShortName},PAL,SHD,RNG,CLR,DRU,SHM,BST,ENC,MAG,NEC,WIZ]}) {
		/call LoadVar NukeStuff IWantToNuke FALSE IWantToNuke "${MyIni}" ${Function} bool
		/call LoadVar NukeStuff StaticNukeTimer2 10 StaticNukeTimer2 "${MyIni}" ${Function} int
		/call LoadVar NukeStuff ManaNukeLevel 5 ManaNukeLevel "${MyIni}" ${Function} int

		/call LoadVar NukeStuff SpellNukes[1] "#Largest Slowest Nuke Slot 5" SpellNukes[1] "${MyIni}" ${Function} string
		/call LoadVar NukeStuff SpellNukes[2] "#Medium Speed Nuke Slot 6" SpellNukes[2] "${MyIni}" ${Function} string
		/call LoadVar NukeStuff SpellNukes[3] "#Fastest Nuke Slot 7" SpellNukes[3] "${MyIni}" ${Function} string
		
	}
	| Dot Setup
	/if (${Select[${Me.Class.ShortName},SHD,RNG,DRU,SHM,BST,ENC,MAG,NEC,WIZ]}) {
		/call LoadVar DotStuff IWantToDot FALSE IWantToDot "${MyIni}" ${Function} bool
			/for i 1 to 5
				/call LoadVar DotStuff SpellDoT[${i}] "#DoT spells/Debuffs Slot 8" SpellDoT[${i}] "${MyIni}" ${Function} string
			/next i
	}

	| Pet Buffs 
	/if (${Select[${Me.Class.ShortName},SHD,DRU,SHM,BST,ENC,MAG,NEC]}) {
		/call LoadVar PetStuff ICheckPet FALSE ICheckPet "${MyIni}" ${Function} bool
		/call LoadVar PetStuff IHealPet FALSE IHealPet "${MyIni}" ${Function} bool	
		/call LoadVar PetStuff PetSpell "#Pet Spell Slot 8" PetSpell "${MyIni}" ${Function} string
		/call LoadVar PetStuff PetHeal "#Pet Heal Slot 7" PetHeal "${MyIni}" ${Function} string

		/for i 1 to 3
			/call LoadVar PetStuff PetBuffs[${i}] "#Pet Buffs Slot 8" PetBuffs[${i}] "${MyIni}" ${Function} string
		/next i

	|	/call LoadVar PetStuff PetBuffItemSlot[1] "#Item Name" PetBuffItemSlot[1] ${MyIni} ${Function} string
	|	/call LoadVar PetStuff PetBuffItemSpellName[1] "#Item Buff Name" PetBuffItemSpellName[1] ${MyIni} ${Function} string
	|	/call LoadVar PetStuff PetBuffItemSlot[2] "#Item Name" PetBuffItemSlot[2] ${MyIni} ${Function} string
	|	/call LoadVar PetStuff PetBuffItemSpellName[2] "#Item Buff Name" PetBuffItemSpellName[2] ${MyIni} ${Function} string
	}
	| Buff Setup
	/if (${Select[${Me.Class.ShortName},PAL,SHD,RNG,CLR,DRU,SHM,BST,ENC,MAG,NEC,WIZ]}) {
		/call LoadVar BuffStuff IWantToBuff FALSE IWantToBuff "${MyIni}" ${Function} bool
		/for i 1 to 8
			/call LoadVar BuffStuff SpellBuffs[${i}] "#Buff spells Slot 8" SpellBuffs[${i}] "${MyIni}" ${Function} string
		/next i

	}
	| Debuff Setup
	/if (${Select[${Me.Class.ShortName},SHD,RNG,CLR,DRU,SHM,BST,ENC,MAG,NEC,WIZ]}) {
		/call LoadVar DebuffStuff IWantToDeBuff FALSE IWantToDeBuff "${MyIni}" ${Function} bool
		/call LoadVar DebuffStuff DebuffDelay 70 DebuffDelay "${MyIni}" ${Function} int
		/for i 1 to 5
			/call LoadVar DebuffStuff SpellDebuffs[${i}] "#Debuff spells Slot 4" SpellDebuffs[${i}] "${MyIni}" ${Function} string
		/next i

	}

	/if (!${Select[${Me.Class.ShortName},MNK,WAR,BER,ROG]}) {

	}
	/if (${Me.Class.ShortName.Equal[MAG]}) {
	}
	/if (${Select[${Me.Class.ShortName},WIZ,NEC,DRU,RNG,SHM,SHD]}) {
	/call LoadVar SnareStuff IWantToSnare FALSE IWantToSnare "${MyIni}" ${Function} bool
	/call LoadVar SnareStuff SpellSnare "#Snare Spell" SpellSnare "${MyIni}" ${Function} string
	/call LoadVar SnareStuff SnareHPs "35" SnareHPs "${MyIni}" ${Function} string
	}

	/if (${Select[${Me.Class.ShortName},CLR,SHM,DRU,BST]}) {
	}
	/if (${Select[${Me.Class.ShortName},WIZ,SHM,NEC,MAG,ENC]}) {
	}
	/if (${Me.Class.ShortName.Equal[DRU]}) {
	}
	/if (${Select[${Me.Class.ShortName},CLR,PAL]}) {
	}
	/if (${Me.Class.ShortName.Equal[CLR]}) {
	}
	/if (${Me.Class.ShortName.Equal[ENC]}) {}
	/if (${Select[${Me.Class.ShortName},DRU,SHM,BST,BRD,ENC,MAG,NEC,SHD,WIZ]}) {
	}
	/if (${Select[${Me.Class.ShortName},ENC]}) {
	/call LoadVar Enchanter_Mez IWantToMez FALSE IWantToMez "${MyIni}" ${Function} bool
	/call LoadVar Enchanter_Mez SpellMez "#Mez Spell" SpellMez "${MyIni}" ${Function} string
	}

	/if (${Me.Class.ShortName.Equal[BRD]}) {
	/call LoadVar BardStuff IWantToTwist FALSE IWantToTwist "${MyIni}" ${Function} bool
	/for i 1 to 4
		/call LoadVar BardStuff SongTwist[${i}] "#Non Combat Spells" SongTwist[${i}] "${MyIni}" ${Function} string
	/next i
	/for i 1 to 4
		/call LoadVar BardStuff CSongTwist[${i}] "#Combat Spells" CSongTwist[${i}] "${MyIni}" ${Function} string
	/next i

	/call LoadVar BardStuff IWantToMez FALSE IWantToMez "${MyIni}" ${Function} bool
	/call LoadVar BardStuff SpellMez "#Mez Spell" SpellMez "${MyIni}" ${Function} string
	/call LoadVar BardStuff IWantToSnare FALSE IWantToSnare "${MyIni}" ${Function} bool
	/call LoadVar BardStuff SpellSnare "#Snare Spell" SpellSnare "${MyIni}" ${Function} string
	/call LoadVar BardStuff SnareHPs "35" SnareHPs "${MyIni}" ${Function} string

	}
	/if (${Me.Class.ShortName.Equal[WAR]}) {
	}
	/if (${Me.Class.ShortName.Equal[ROG]}) {
	}
	/call LoadVar FinalSettings IWantToCampOut TRUE IWantToCampOut "${MyIni}" ${Function} bool
	/call LoadVar FinalSettings IWantToLoot FALSE IWantToLoot "${MyIni}" ${Function} bool
/return
