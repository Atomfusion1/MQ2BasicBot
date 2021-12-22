Sub Start_Up
	/declare AuthList			string 	outer	FALSE
	| EDIT THIS WITH YOU IN GAME CHARACTER NAMES
	| Just List Names with a space after MasterList Name1 Name2 Name3 Name4
	|/varset  AuthList Epeak Finkka Foador Saiyena Grotar Suag Lilola Pettal Ruriis Lariz Snari Mulril Petalon Dralu Rikni Nenno Luvilea
	| YOU MUST CHANGE THIS LINE AND SETUP MQ2IRC In order to Run the program 
	| MQ2Nav mesh load
	/if (FALSE) {
		/echo \arYou Must Setup IWantToStartup.inc first /
		/endm
	} else {
	| type Plugin mq2irc
	| now type /connect irc.mibbit.net 6666 <ServerName> <CharacterName>
	| now type /i join <ServerName>
	| to type into channel type /i say "Say What"
	/echo Setup IRC In IWANTTOSTARTUP.inc
		| /plugin mq2irc
		| /iconnect irc.mibbit.net 6666 <ServerName> <CharacterName>
		| /delay 40
		| /i join <ServerName>
		| /delay 15
		| /i Test
	}


/echo \a-yI have Entered Startup
	| Program Controls
	/declare IWantToTaunt		bool	outer 	FALSE
	/declare IWantToRange		bool	outer	FALSE
	/declare IWantToTauntCast	bool	outer	FALSE
	/declare IWantToMelee		bool	outer	FALSE
	/declare IWantToPull		bool 	outer	FALSE
	/declare IWantToUseMQ2Nav	bool	outer	FALSE
	/declare ICallTarget		bool	outer	FALSE
	/declare IWantToLoot		bool 	outer	FALSE
	/declare IWantToCampOut		bool 	outer	FALSE
	/declare IAmHealing			bool 	outer	FALSE
	/declare IHealGroup			bool 	outer	FALSE
	/declare IDivineSelf		bool 	outer	FALSE
	/declare IHaveMount			bool	outer	FALSE
	/declare ICheckPet			bool 	outer	FALSE
	/declare IAmTargeting		bool 	outer	FALSE
	/declare IWantToMoveBehind	bool	outer 	FALSE
	/declare IWillBuff			bool 	outer	FALSE
	/declare IWantToDot			bool 	outer	FALSE
	/declare IWantToHealMT		bool	outer	FALSE
	/declare IWantToHoT			bool	outer	FALSE
	/declare IWantToNuke		bool 	outer	FALSE
	/declare IWantToMez			bool 	outer	FALSE	
	/declare IWantToTwist		bool 	outer	FALSE
	/declare IWantToSnare		bool 	outer	FALSE
	/declare IWantToBuff		bool 	outer	FALSE
	/declare IWantToDeBuff		bool	outer	FALSE
	/declare IWantToDS			bool 	outer	FALSE
	/declare IWantToSit			bool 	outer	FALSE
	/declare IMakeFood			bool	outer	FALSE
	/declare IGiveFood 			bool	outer	FALSE
	/declare IAskForFood		bool 	outer 	FALSE
	/declare IWantToRez			bool	outer	FALSE
	/declare ISummonItems		bool	outer	FALSE
	/declare StickToMob			bool	outer 	FALSE
	/declare StayAtMainCamp		bool	outer 	FALSE
	/declare GroupChatEvents   	bool 	outer	FALSE
	/declare MQ2IRCEvents		bool	outer	FALSE
	/declare CheckDebug			bool	outer	FALSE
	/declare MeAggro			bool	outer	FALSE
	/declare JustHealed			bool	outer	FALSE
	/declare IHealPet			bool	outer	FALSE
	/declare CallTargetOn		bool	outer	FALSE
	/declare StopFollow 		bool	outer	FALSE
	/declare IsBool				bool	outer 	FALSE
	/declare LazyMode			bool	outer 	FALSE
	/declare IPaused			bool 	outer	FALSE
	/declare IDebuffNow			bool	outer	TRUE
	/declare NoMezzer			bool 	outer	FALSE
	/declare IWantToCheck		bool 	outer	TRUE
	/declare CampUp				bool	outer 	TRUE 
	/declare IHaveArrows		bool	outer	TRUE
	/declare ImTwisting			bool 	outer	TRUE
	/declare MezzingMobs		bool	outer	TRUE
	/declare ResistDet			bool	outer	FALSE
	/declare IDied				bool	outer	FALSE
	/declare DeadMember			bool	outer	FALSE
	/declare BuffsCalled		bool	outer	FALSE
	/declare MezImmune			bool 	outer 	FALSE
	/declare SnareImmune		bool	outer	FALSE
	/declare Temp				bool 	outer 	FALSE
	/declare TurnOffRanged		bool 	outer 	FALSE
	/declare IgnoreMobs			bool	outer	NULL
	/declare RapidPulling		bool	outer	FALSE
		
	| Group Setup
	/declare MTank				string	outer ${Param0}
	/declare WhoIsTank			string	outer ${Param0}
	/declare WhoIsAssist		string	outer ${Param0}
	/declare WhoIsPuller		string	outer ${Param0}
	/declare WhoIsHealer		string	outer ${Param1}
	/declare MainCamp			string 	outer ${Param1}
	
	| Declares
	/declare MAbility[10] 			string outer NULL
	/declare CAbility[10]			string outer NULL
	/declare Strafe					string outer NULL
	/declare IncMob					string outer NULL	
	/declare Bash 					int outer 1
	/declare CampOut				int outer 0
	/declare GroupHPs[5] 			int outer 0
	/declare MoveDistance			int outer 0
	/declare MobPulling				int outer 0
	/declare PullD					int outer 0
	/declare PullN					int outer 0
	/declare PullRate				int outer 0
	/declare Random					int outer 0
	/declare SpawnD					int outer 0
	/declare TargetID 				int outer 0
	/declare OldTargetID			int outer 0
	/declare HitTargetID			int outer 0
	/declare TargetHPs				int outer 98
	/declare SnareHPs				int outer 35
	/declare Index 					int outer 0
	/declare Index2					int outer 0
	/declare i 						int outer 0
	/declare GHeal 					int outer 0
	/declare PctCallTarget			int outer 0
	/declare LocY					int outer ${Me.Y}
	/declare LocX					int outer ${Me.X}
	/declare LocZ					int outer ${Me.Z}
	/declare MezzedMobs				int outer 0
	/declare ManaNukeLevel			int outer 0
	/declare PCNear 				int outer 0
	/declare PullSize				int outer 1
	/declare MobLevelOverMe			int outer 0
	/declare DebuffDelay			int outer 100
	/declare TotalExp 				float outer 0	
	/declare MyExp 					float outer 0
	/declare MyAAExp 				float outer 0
	/declare MyLExp 				float outer 0
	
	
	| Caster Area
	/declare SpellBuffs[10] 			string outer NULL
	/declare SpellTaunts[5]				string outer NULL
	/declare SpellDebuffs[5] 			string outer NULL
	/declare SpellHeals[8]				string outer NULL
	/declare RezSpell					string outer NULL 
	/declare SpellMez					string outer NULL
	/declare SpellDoT[5]				string outer NULL
	/declare SpellNukes[5] 				string outer NULL
	/declare PetBuffs[5] 				string outer NULL
	/declare PetItem[5] 				string outer NULL
	/declare SpellFood[2] 				string outer NULL
	/declare PetBuffItemSlot[3] 		string outer NULL
	/declare PetBuffItemSpellName[3] 	string outer NULL
	/declare SpellSnare					string outer NULL
	/declare DSBuffs 					string outer NULL
	/declare PetHeal 					string outer NULL
	/declare PetSpell 					string outer NULL	
	/declare MountItemSlot				string outer NULL
	/declare MountSpellName				string outer NULL
	/declare SpellAura					string outer NULL
	/declare SpellSwarmPet 				string outer NULL
	/declare PullSpell					string outer NULL 
	/declare PullCA						string outer NULL
	/declare TrackDebuff[6,5] 			int outer 0
	/declare TrackDoT[6,5]				int outer 0
	/declare TrackMez[6,5]				int outer 0
	/declare MobMezList[10]				int outer 0	
	/declare DoTMobList[10]				int outer 0
	/declare Heal[8]					int outer 0	
	/declare DontMezID					int outer 0
	/declare MobTime					int outer 0
	/declare MTHPs 						int outer 0
	/declare MobHPs 					int outer 0	
	/declare StaticSitTimer 			int outer 40
	/declare DeathRate 					int outer 0
	/declare NukeRate					int outer 0
	/declare NukeTimer 					int outer 0
	/declare StaticNukeTimer2 			int outer 15
	/declare TargetIDID 				int outer 0	
	/declare TauntSpellNumber			int outer 1
	/declare HPChange 					float outer 0	
	/declare NRate 						float outer 0
	/declare HRate 						float outer 0
	/declare MinPullHP					int outer 0
	
	| Bard Area
	/declare MezTwistTimer				timer outer 0
	/declare TwistTimer					timer outer 0
	/declare SelosTimer					timer outer 0
	/declare SongCounter 				int outer 1
	/declare SongMezCounter				int outer 2
	/declare SongTwist[4]				string outer NULL 
	/declare CSongTwist[4]				string outer NULL
	
	| Timers	
	/declare SpellMezDuration			timer outer 0
	/declare SpellDotDuration			timer outer 0
	/declare CastTauntTimer				timer outer 0
	/declare DoTDuration				timer outer 0
	/declare PetBuffTimer 				timer outer 1m
	/declare BuffTimer 					timer outer 1m
	/declare SnareTimer 				timer outer 0
	/declare MeCombatTimer				timer outer 0
	/declare DSTimer					timer outer 0
	/declare SitTimer 					timer outer 0
	/declare NukeTimer2 				timer outer 0 
	/declare GroupTimer 				timer outer 0
	/declare TauntTimer 				timer outer 0
	/declare PullTimer 					timer outer 0
	/declare PPullTimer					timer outer 0
	/declare CampOutTimer   			timer outer 0
	/declare HoTTimer					timer outer 0
	/declare FoodTimer					timer outer 60s
	/declare MoveBehindTimer 			timer outer 1	
	/declare IDiedTimer 				timer outer 0
	/declare IAmPausedTimer				timer outer	0
	/declare LootTimer 					timer outer 30s
	/declare DeadMemberTimer			timer outer 0
	/declare TriedToRezTimer 			timer outer 0
	/declare PCNearTimer 				timer outer 0
	
	| IWantToMoveBehind Declares
	/declare FTarget 			string outer TRUE
	/declare MoveToT			int outer 0
	/declare MobDistance		int	outer 15
	/declare MobFight			int outer 1
	/declare CalcHeading		int outer 1
	/declare Stay				int outer 1
	/declare IDist				int outer 25

	
	| Variables Set
	/varset MyExp ${Me.PctExp}
	/varset MyAAExp ${Me.PctAAExp}
	
	/call ReadINI "Panther/PantherMob.ini" "${Zone.Name}" Mob
	
	/echo \a-yTo Turn on Mob Pulling Type /echo Mob Pulling Type again to Turn off 
	/echo \a-yType /echo Help to see all commands
/return

|-------------------------------------------------------------------------------- 
|SUB: Reading from an INI File 
|-------------------------------------------------------------------------------- 
Sub ReadINI(FileName,SectionName,ArrayType) 
   /delay 1s 
   /if (${Ini[${FileName},${SectionName}].Count[|]}==0) {
      /echo \ar"${SectionName}" is not a Valid Section for FILE:${FileName}, 
      /delay 1s 
	  /echo \arCreating a new Section for your Zone
	  /echo \arYOU NEED TO MODIFY PantherMob.INI
	  /ini "${FileName}" "${SectionName}" "IgnoreList" "TRUE"
	  /ini "${FileName}" "${SectionName}" "MobLevelOverMe" "5"
	  /ini "${FileName}" "${SectionName}" "PullRange" "400"
	  /ini "${FileName}" "${SectionName}" "PullSize" "1"
	  /ini "${FileName}" "${SectionName}" "mob1" "Ignore Mob Name"
   } 
   /declare nValues     int local  1 
   /declare nArray      int local  0 
   /declare KeySet      string local  ${Ini[${FileName},${SectionName}]} 
   :CounterLoop 
   /if (!${KeySet.Arg[${nValues},|].Length}) { 
      /varcalc nValues ${nValues}-1 
      /goto :MakeArray 
   } 
   /varcalc nValues ${nValues}+1 
   /goto :CounterLoop 
   :MakeArray 
   /if (!${nValues}) /return 
   /if (${FileName.Equal["Panther/PantherMob.ini"]}&&${nValues}>0) { 
	/if (${Defined[RV_MobArray]}) {
	   /deletevar RV_MobArray
	   /deletevar RV_MobStats
   }
      /declare RV_MobArray[${nValues}]   string outer 
      /declare RV_MobStats[${nValues}]   string outer 
   } 
   | ${Ini["Panther/PantherMob.ini","${Zone.Name}",Mob1]}
   /varset IgnoreMobs 		${Ini["Panther/PantherMob.ini","${Zone.Name}",IgnoreList].Equal[TRUE]}
   /varset MobLevelOverMe 	${Ini["Panther/PantherMob.ini","${Zone.Name}",MobLevelOverMe]}
   /varset PullD 			${Ini["Panther/PantherMob.ini","${Zone.Name}",PullRange]}
   /varset PullSize 		${Ini["Panther/PantherMob.ini","${Zone.Name}",PullSize]}
   |"
   /echo \a-yPulling Mobs up to \ar${MobLevelOverMe} \a-yLevels over me 
   /if (${IgnoreMobs}) {
		/echo \a-yIgnoring these \ar${Math.Calc[${nValues}-4]} \a-yMobs
   } else {
		/echo \a-yPulling these \ar${Math.Calc[${nValues}-4]} \a-yMobs
   }
   
   
   /for nArray 1 to ${Math.Calc[${nValues}-4]}
      /if (${FileName.Equal["Panther/PantherMob.ini"]}) { 
         /varset RV_MobArray[${nArray}] ${Ini[${FileName},${SectionName},${ArrayType}${nArray}]} 
         /varset RV_MobStats[${nArray}] 0 
         /echo \a-ySet RV_MobArray${nArray} to '${RV_MobArray[${nArray}]}'
      } 
   /next nArray 
   /delay 1s 
/return 
