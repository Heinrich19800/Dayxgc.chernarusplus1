/*---------------------------------------------------------------------------------------------------------------------\
| MAIN                                                                                                                 |
\---------------------------------------------------------------------------------------------------------------------*/
void main()
{

	Hive ce = CreateHive();
	if ( ce )
		ce.InitOffline();
	Weather weather = g_Game.GetWeather();
	weather.GetOvercast().SetLimits( 0.0 , 1.0 );
	weather.GetRain().SetLimits( 0.0 , 1.0 );
	weather.GetFog().SetLimits( 0.0 , 0.25 );
	weather.GetOvercast().SetForecastChangeLimits( 0.0, 0.2 );
	weather.GetRain().SetForecastChangeLimits( 0.0, 0.1 );
	weather.GetFog().SetForecastChangeLimits( 0.15, 0.45 );
	weather.GetOvercast().SetForecastTimeLimits( 1800 , 1800 );
	weather.GetRain().SetForecastTimeLimits( 600 , 600 );
	weather.GetFog().SetForecastTimeLimits( 1800 , 1800 );
	weather.GetOvercast().Set( Math.RandomFloatInclusive(0.0, 0.3), 0, 0);
	weather.GetRain().Set( Math.RandomFloatInclusive(0.0, 0.2), 0, 0);
	weather.GetFog().Set( Math.RandomFloatInclusive(0.0, 0.1), 0, 0);
	weather.SetWindMaximumSpeed(30);
	weather.SetWindFunctionParams(0.1, 1.0, 50);
}
/*---------------------------------------------------------------------------------------------------------------------\
| Custom Mission                                                                                                       |
\---------------------------------------------------------------------------------------------------------------------*/
class CustomMission: MissionServer
{	
	override PlayerBase CreateCharacter(PlayerIdentity identity, vector pos, ParamsReadContext ctx, string characterName)
	{
		Entity playerEnt;
		playerEnt = GetGame().CreatePlayer(identity, characterName, pos, 0, "NONE");  //Creates random player
		Class.CastTo(m_player, playerEnt);
		GetGame().SelectPlayer(identity, m_player);	
		return m_player;
	}
	void SetRandomHealth(EntityAI itemEnt)
	{
		int rndHlt = Math.RandomInt(40,100);
		itemEnt.SetHealth("","",rndHlt);
	}
/*---------------------------------------------------------------------------------------------------------------------\
| Starting Equip Setup                                                                                                 |
\---------------------------------------------------------------------------------------------------------------------*/	
//<!---------------------------------  ADMIN  -------------------------------->
	override void StartingEquipSetup(PlayerBase player, bool clothesChosen)
	{
	bool IsAdmin = false;  
	string SteamId_Malotru = "00000000000000000"; // Steam id

	if (player.GetIdentity().GetPlainId() == SteamId_Malotru){IsAdmin = true;}
	if (IsAdmin){
            player.RemoveAllItems();                                                 // Remove All Items
			player.GetStatWater().Set(1000);                                         // Water       100%
			player.SetHealth(100);                                                   // Health      100%

            player.GetInventory().CreateInInventory("TTsKOJacket_Camo");	         // TTsKO Jacket     Camo
            player.GetInventory().CreateInInventory("HighCapacityVest_Olive");       // HighCapacityVest Olive
            player.GetInventory().CreateInInventory("TTSKOPants");                   // TTSKO Pants
            player.GetInventory().CreateInInventory("CombatBoots_Green");            // CombatBoots Green

            player.GetInventory().CreateInInventory("Chemlight_Green");	             // Chemlight Green
            player.GetInventory().CreateInInventory("CombatKnife");	                 // Combat Knife
			player.GetInventory().CreateInInventory("Mag_IJ70_8Rnd");                // Mag MakarovIJ70
			player.GetInventory().CreateInInventory("Mag_IJ70_8Rnd");                // Mag MakarovIJ70
			player.GetInventory().CreateInInventory("Rag");                          // Rag			

            EntityAI Makaa = player.GetInventory().CreateInInventory("MakarovIJ70"); // MakarovIJ70
            Makaa.GetInventory().CreateAttachment("PistolSuppressor");		         // Suppressor

			player.SetPosition("4107.80 342.7 11205.29");			                 // Position Vybor airport
						
//<!--------------------------------  PLAYER  -------------------------------->
	} else {
		switch (Math.RandomInt(0, 1)) {
            case 0:
			EntityAI itemEnt;
		    player.GetInventory().CreateInInventory("Roadflare");			         // Roadflare

            ItemBase rags0 = player.GetInventory().CreateInInventory("Rag");         // Rag
            rags0.SetQuantity(4);
			SetRandomHealth(itemEnt);
            break;
		}
	}
	}
/*---------------------------------------------------------------------------------------------------------------------\
| Admin Tool                                                https://github.com/Malotruu/DayZMalotru.chernarusplus/wiki |
\---------------------------------------------------------------------------------------------------------------------*/
  //=========== Mods ===========
  bool m_FullStatus     = false;  // Warning Full Stat all player
  bool m_FullStamina    = false;  // Warning Full Stamina all player
  bool m_DebugMonitor   = false;  // Debug Monitor off restart serv
  bool verify_admins    = false;  // true = verify presence of BI UID in admin list
  bool m_FreeCamera     = false;  // CameraTool
  bool m_AutoFreezeWalk = false;  // Auto Walk and Freeze Player
  //=========== Misc ===========
  string cmd_prefix = "!";        // Must be special character
  ref TStringArray admins = {};   // Add your BI UID
  //============================
//<!---------------------------  IsPlayerAnAdmin  ---------------------------->
	bool IsPlayerAnAdmin(PlayerBase player) {
		bool found = false;
		for ( int i = 0; i < admins.Count(); ++i ) {
			if(player.GetIdentity().GetId() == admins[i]) { found=true; break; }
		}
		return found;
	}
//<!-------------------------------  IsPlayer  ------------------------------->
	bool IsPlayer(string name) {
		PlayerBase p;
		array<Man> players = new array<Man>; GetGame().GetPlayers(players);
		for ( int i = 0; i < players.Count(); ++i ) {
			p = players.Get(i);
			if(p.GetIdentity().GetName() == name) return true;
		}
		return false;
	}
//<!-------------------------  SendMessageToPlayer  -------------------------->
	void SendMessageToPlayer(PlayerBase player, string message) {
		Param1<string> param = new Param1<string>( message );
		GetGame().RPCSingleParam(player, ERPCs.RPC_USER_ACTION_MESSAGE, param, true, player.GetIdentity());
	}	
//<!------------------------------  GetPlayer  ------------------------------->
	PlayerBase GetPlayer(string name) {
		PlayerBase p;
		array<Man> players = new array<Man>; GetGame().GetPlayers(players);
		for ( int i = 0; i < players.Count(); ++i ) {
			p = players.Get(i);
			if(p.GetIdentity().GetName() == name) return p;
		}
		return NULL;
	}
//<!----------------------------  TickScheduler  ----------------------------->	
override void TickScheduler(float timeslice)
{
	
	GetGame().GetWorld().GetPlayerList(m_Players);
	if( m_Players.Count() == 0 ) return;
	for(int i = 0; i < SCHEDULER_PLAYERS_PER_TICK; i++)
	{
		if(m_currentPlayer >= m_Players.Count() )
		{
			m_currentPlayer = 0;
		}
		PlayerBase player = PlayerBase.Cast(m_Players.Get(m_currentPlayer));
		
        if (m_FullStamina) { player.GetStatStamina().Set(1000); }

		if (m_FullStatus) { 
		player.GetStatHeatComfort().Set(0); 
		player.GetStatStamina().Set(1000); 
		player.GetStatEnergy().Set(1000); 
		player.GetStatWater().Set(1000); 
		player.GetStatWet().Add( -1 ); 
		player.SetHealth( "","Blood", player.GetMaxHealth( "", "Blood" ) );
		player.SetHealth("","Shock", player.GetMaxHealth("","Shock") ); 
		player.SetHealth( player.GetMaxHealth( "", "" ) );
        //-------------------------------------------------------------------------------------------------------	
		EntityAI CurrentWeapon = player.GetHumanInventory().GetEntityInHands();
		if( CurrentWeapon )
			{
				CurrentWeapon.SetHealth( CurrentWeapon.GetMaxHealth( "", "" ) );
				Magazine foundMag = ( Magazine ) CurrentWeapon.GetAttachmentByConfigTypeName( "DefaultMagazine" );
				if( foundMag && foundMag.IsMagazine())
				{
					foundMag.ServerSetAmmoMax();
				}

				Object Suppressor = ( Object ) CurrentWeapon.GetAttachmentByConfigTypeName( "SuppressorBase" );
				if( Suppressor )
				{
			    	Suppressor.SetHealth( Suppressor.GetMaxHealth( "", "" ) );
				}
			}
		}	
	    //-------------------------------------------------------------------------------------------------------	
		player.OnTick();
		m_currentPlayer++;
	}
}

//<!-------------------------------  ON EVENT  ------------------------------->	
	override void OnEvent(EventType eventTypeId, Param params)  {
		super.OnEvent(eventTypeId,params);
		int i;
		PlayerBase player, temp_player;
		array<Man> players = new array<Man>; GetGame().GetPlayers(players);
		if(eventTypeId != ChatMessageEventTypeID) return; // Is chat message
		ChatMessageEventParams chat_params = ChatMessageEventParams.Cast( params );
		if(chat_params.param1 != 0 || chat_params.param2 == "") return; 
		player = GetPlayer(chat_params.param2);
		if(player == NULL) return;
		if(verify_admins && !IsPlayerAnAdmin(player)) { GetGame().AdminLog("[ADMCMD] (Unauthorized) " + player.GetIdentity().GetName() +" ("+player.GetIdentity().GetPlainId()+", "+player.GetIdentity().GetId()+") tried to execute "+ chat_params.param3); return; }
		string message = chat_params.param3, prefix, param0, command;
		TStringArray tokens = new TStringArray;
		message.Split(" ", tokens); int count = tokens.Count();
		param0 = tokens.Get(0);
		param0.ParseStringEx(prefix); if(prefix != cmd_prefix) return;
		param0.ParseStringEx(command);
		GetGame().AdminLog("[ADMCMD] PLAYER: "+ player.GetIdentity().GetName() +" ("+player.GetIdentity().GetPlainId()+", "+player.GetIdentity().GetId()+") CMD: "+ command);
		switch(command) {	
            // ---------------------------------------------------------------------------------------------------------
			case "strip": {
				if(count != 2) { SendMessageToPlayer(player, "!strip [player]"); return; }
				temp_player = GetPlayer(tokens[1]);
				if(temp_player == NULL) {
					SendMessageToPlayer(player, "Can't find player called: '"+tokens[1]+"'");
				} else {
					temp_player.RemoveAllItems();
					SendMessageToPlayer(player, "You stripped " + temp_player.GetIdentity().GetName());
					SendMessageToPlayer(temp_player, "You have been stripped by an admin");
				}
				break;
			}
            // ---------------------------------------------------------------------------------------------------------			
			case "slap": {
				if(count != 2) { SendMessageToPlayer(player, "!slap [player]"); return; }
				temp_player = GetPlayer(tokens[1]);
				if(temp_player == NULL) {
					SendMessageToPlayer(player, "Can't find player called: '"+tokens[1]+"'");
				} else {
					temp_player.SetPosition(temp_player.GetPosition() + "0 3 0");
					SendMessageToPlayer(player, "You stripped " + temp_player.GetIdentity().GetName());
					SendMessageToPlayer(temp_player, "You have been slapped by an admin");
				}
				break;
			}
            // ---------------------------------------------------------------------------------------------------------	           
			case "cam": {
				PlayerBase pBody = player;
				if (m_FreeCamera)
				{
					GetGame().SelectPlayer(player.GetIdentity(), pBody);
					player.GetInputController().OverrideMovementSpeed( false, 0 );
                    player.GetInputController().OverrideMovementAngle( false, 0 );
                    player.GetInputController().OverrideAimChangeX( false, 0 );
                    player.GetInputController().OverrideAimChangeY( false, 0 );
					m_FreeCamera = false;
				}
				else
				{
					GetGame().SelectPlayer(player.GetIdentity(), NULL);
					GetGame().SelectSpectator(player.GetIdentity(), "freedebugcamera", player.GetPosition());
					player.GetInputController().OverrideMovementSpeed( true, 0 );
                    player.GetInputController().OverrideMovementAngle( true, 0 );
					player.GetInputController().OverrideAimChangeX( true, 0 );
					player.GetInputController().OverrideAimChangeY( true, 0 );
					m_FreeCamera = true;
				}
				break;
			}
            // ---------------------------------------------------------------------------------------------------------	
			case "stat": {
				if (m_FullStatus)
				{
					m_FullStatus = false;
				} else {
					m_FullStatus = true;
				}
				break;
			}
            // ---------------------------------------------------------------------------------------------------------	
			case "stamina": {
				if (m_FullStamina)
				{
					m_FullStamina = false;
				} else {
					m_FullStamina = true;
				}
				break;
			}
            // ---------------------------------------------------------------------------------------------------------	
			case "walk": {
				if (m_AutoFreezeWalk)
				{
					player.GetInputController().OverrideMovementSpeed( false, 0 );
                    player.GetInputController().OverrideMovementAngle( false, 0 );
					m_AutoFreezeWalk = false;
				} else {
					player.GetInputController().OverrideMovementSpeed( true, 5 );
                    player.GetInputController().OverrideMovementAngle( true, 0 );
					m_AutoFreezeWalk = true;
				}
				break;
			}
            // ---------------------------------------------------------------------------------------------------------	
			case "db": {
				if (m_DebugMonitor) {
					GetGame().SetDebugMonitorEnabled(0);
					m_DebugMonitor = false;
				} else {
					GetGame().SetDebugMonitorEnabled(1);
					m_DebugMonitor = true;
				}
				break;
			}
            // ---------------------------------------------------------------------------------------------------------
			case "remove": {
				SendMessageToPlayer(player, "Object remove");
				EntityAI CurrentInHands = player.GetHumanInventory().GetEntityInHands();
				string rdisplayName = CurrentInHands.ConfigGetString("rdisplayName");
				GetGame().ObjectDelete(CurrentInHands);
				player.OnItemInHandsChanged();
				break;
			}
            // ---------------------------------------------------------------------------------------------------------			
			case "spawn": {
				if(count != 2) { SendMessageToPlayer(player, "!spawn [object]"); return; }
				GetGame().CreateObject(tokens[1], player.GetPosition(), false, true );
				SendMessageToPlayer(player, "Object spawned: " + tokens[1]);
				break;
			}
            // ---------------------------------------------------------------------------------------------------------			
			case "goto": {
				if(count != 2) { SendMessageToPlayer(player, "!goto [player]"); return; }
				temp_player = GetPlayer(tokens[1]);
				if(temp_player == NULL) {
					SendMessageToPlayer(player, "Can't find player called: '"+tokens[1]+"'");
				} else {
					player.SetPosition(temp_player.GetPosition());
					SendMessageToPlayer(player, "You teleported to " + temp_player.GetIdentity().GetName());
				}
				break;
			}
            // ---------------------------------------------------------------------------------------------------------
			case "allgoto": {
				PlayerBase allgoto_target;
				if(count != 2) { SendMessageToPlayer(player, "!allgoto [player]"); return; }
				allgoto_target = GetPlayer(tokens[1]);
				if(allgoto_target == NULL) {
					SendMessageToPlayer(player, "Can't find player called: '"+tokens[1]+"'");
				} else {
					SendMessageToPlayer(player, "You teleported everyone to your location");
					for (i = 0; i < players.Count(); i++) {
						temp_player = players.Get(i);
						SendMessageToPlayer(temp_player, "You have been teleported to player " + allgoto_target.GetIdentity().GetName());
						temp_player.SetPosition(allgoto_target.GetPosition());
					}
				}
				break;
			}
            // ---------------------------------------------------------------------------------------------------------
			case "here": {
				if(count != 2) { SendMessageToPlayer(player, "!here [player]"); return; }
				temp_player = GetPlayer(tokens[1]);
				if(temp_player == NULL) {
					SendMessageToPlayer(player, "Can't find player called: '"+tokens[1]+"'");
				} else {
					temp_player.SetPosition(player.GetPosition());
					SendMessageToPlayer(temp_player, "You have been teleported to admin " + player.GetIdentity().GetName());
					SendMessageToPlayer(player, "You teleported " + temp_player.GetIdentity().GetName() + " to your location");
				}
				break;
			}
            // ---------------------------------------------------------------------------------------------------------
			case "allhere": {
				SendMessageToPlayer(player, "You teleported everyone to your location");
				for (i = 0; i < players.Count(); i++) {
					temp_player = players.Get(i);
					SendMessageToPlayer(temp_player, "You have been teleported to admin " + player.GetIdentity().GetName());
					temp_player.SetPosition(player.GetPosition());
				}
				break;
			}
            // ---------------------------------------------------------------------------------------------------------			
			case "tppos": {
				if (count < 3) { SendMessageToPlayer(player, "!tppos [X] [Y] [player]"); return; }
				float ATL_Z = GetGame().SurfaceY(tokens[1].ToFloat(), tokens[2].ToFloat());
				vector reqpos = Vector(tokens[1].ToFloat(), ATL_Z, tokens[2].ToFloat());
				temp_player = player;
				if (count == 4) {
					temp_player = GetPlayer(tokens[3]);
					if (temp_player == NULL) {
						SendMessageToPlayer(player, "Can't find player called: '"+tokens[3]+"'"); return;
					} else {
						SendMessageToPlayer(temp_player, "You've been teleported to " + reqpos + " by admin " + player.GetIdentity().GetName());
					}
				}
				temp_player.SetPosition(reqpos);
				SendMessageToPlayer(player, "Target teleported to " + temp_player.GetPosition());
				break;
			}
            // ---------------------------------------------------------------------------------------------------------
			case "time": {
				if(count != 3) { SendMessageToPlayer(player, "!time [hour] [minute]"); return; }
				GetGame().GetWorld().SetDate( 2018, 1, 7, tokens[1].ToInt(), tokens[2].ToInt());
				SendMessageToPlayer(player, "You have set the servertime to " + tokens[1] + ":"+tokens[2]);
				break;
			}
            // ---------------------------------------------------------------------------------------------------------
			case "night": {
				GetGame().GetWorld().SetDate(2018, 14, 11, 22, 0);
                SendMessageToPlayer(player, "NIGHT TIME!!");
				break;
			}
            // ---------------------------------------------------------------------------------------------------------
			case "day": {
				GetGame().GetWorld().SetDate(2018, 14, 11, 12, 0);
			    SendMessageToPlayer(player, "DAY TIME!!");
				break;
			}
            // ---------------------------------------------------------------------------------------------------------
			case "rain": {
				if(count != 2) { SendMessageToPlayer(player, "!rain [0-100]"); return; }
				float rain = tokens[1].ToFloat() / 100;
				GetGame().GetWeather().GetRain().Set(rain, 2, 600);
				SendMessageToPlayer(player, "You have set Rain to " + tokens[1] + "% ["+rain+"]");
				break;
			}

            // ---------------------------------------------------------------------------------------------------------			
			case "fog": {
				if(count != 2) { SendMessageToPlayer(player, "!fog [0-100]"); return; }
				float fog = tokens[1].ToFloat() / 100;
				GetGame().GetWeather().GetFog().Set(fog, 2, 600);
				SendMessageToPlayer(player, "You have set Fog to " + tokens[1] + "% ["+fog+"]");
				break;
			}
            // ---------------------------------------------------------------------------------------------------------
			case "overcast": {
				if(count != 2) { SendMessageToPlayer(player, "!overcast [0-100]"); return; }
				float overcast = tokens[1].ToFloat() / 100;
				GetGame().GetWeather().GetOvercast().Set(overcast, 2, 600);
				SendMessageToPlayer(player, "You have set Overcast to " + tokens[1] + "% ["+overcast+"]");
				break;
			}			
            // ---------------------------------------------------------------------------------------------------------
			case "kill": {
				if(count == 2) {
					temp_player = GetPlayer(tokens[1]);
					if(temp_player == NULL) {
						SendMessageToPlayer(player, "Can't find player called: '"+tokens[1]+"'");
					} else {
						temp_player.SetHealth(0);
						SendMessageToPlayer(player, "You killed " + temp_player.GetIdentity().GetName());
					}
				} else {
					player.SetHealth(0);
					SendMessageToPlayer(player, "You killed yourself");
				}
				break;
			}
            // ---------------------------------------------------------------------------------------------------------
			case "killall": {
				SendMessageToPlayer(player, "You killed everyone");
				for (i = 0; i < players.Count(); i++) {
					temp_player = players.Get(i);
					if(temp_player.GetIdentity().GetId() == player.GetIdentity().GetId()) continue;
					temp_player.SetHealth(0);
				}
				break;
			}	
            // ---------------------------------------------------------------------------------------------------------
			case "heal": {
				PlayerBase heal_target;
				if(count == 2) {
					heal_target = GetPlayer(tokens[1]);
					if(heal_target == NULL) {
						SendMessageToPlayer(player, "Can't find player called: '"+tokens[1]+"'");
					} else {
						SendMessageToPlayer(player, "You healed " + heal_target.GetIdentity().GetName());
					}
				} else {
					heal_target = player;
					SendMessageToPlayer(player, "You healed yourself");
				}
				if(heal_target != NULL) {
					heal_target.SetHealth("", "Blood", heal_target.GetMaxHealth("", "Blood"));
					heal_target.SetHealth("","Shock", heal_target.GetMaxHealth("","Shock") ); 
					heal_target.SetHealth(heal_target.GetMaxHealth("", ""));					
					heal_target.GetStatStamina().Set(1000);
					heal_target.GetStatEnergy().Set(1000);
					heal_target.GetStatWater().Set(1000);
				}
				break;
			}
            // ---------------------------------------------------------------------------------------------------------
			case "offroad": {
				PlayerBase offroad_target;
				if(count == 2) {
					offroad_target = GetPlayer(tokens[1]);
					if(offroad_target == NULL) {
						SendMessageToPlayer(player, "Can't find player called: '"+tokens[1]+"'");
					} else {
						SendMessageToPlayer(player, "You Give offroad to " + offroad_target.GetIdentity().GetName());
					}
				} else {
					offroad_target = player;
					SendMessageToPlayer(player, "Vehicle spawned");
				}
				if(offroad_target != NULL) {
					EntityAI offroadt;
					offroadt = GetGame().CreateObject( "OffroadHatchback", offroad_target.GetPosition()- "5 0 0", false, true );																		
					offroadt.GetInventory().CreateAttachment("HatchbackWheel");
					offroadt.GetInventory().CreateAttachment("HatchbackWheel");
					offroadt.GetInventory().CreateAttachment("HatchbackWheel");
					offroadt.GetInventory().CreateAttachment("HatchbackWheel");
					offroadt.GetInventory().CreateAttachment("HatchbackWheel");
					offroadt.GetInventory().CreateAttachment("HatchbackDoors_Driver");
					offroadt.GetInventory().CreateAttachment("HatchbackDoors_CoDriver");
					offroadt.GetInventory().CreateAttachment("HatchbackHood");
					offroadt.GetInventory().CreateAttachment("HatchbackTrunk");	
					offroadt.GetInventory().CreateAttachment("SparkPlug");
					offroadt.GetInventory().CreateAttachment("EngineBelt");
					offroadt.GetInventory().CreateAttachment("CarRadiator");
					offroadt.GetInventory().CreateAttachment("CarBattery");
					offroadt.GetInventory().CreateAttachment("HeadlightH7");
					offroadt.GetInventory().CreateAttachment("HeadlightH7");					
					offroadt.GetInventory().CreateInInventory("CanisterGasoline");
					offroadt.GetInventory().CreateInInventory("WaterBottle");
					offroadt.GetInventory().CreateInInventory("HeadlightH7");
					offroadt.GetInventory().CreateInInventory("EngineOil");
					offroadt.GetInventory().CreateInInventory("BrakeFluid");				
					auto carfluids = Car.Cast( offroadt );
					carfluids.Fill( CarFluid.FUEL, 1000 );
					carfluids.Fill( CarFluid.OIL, 1000 );
					carfluids.Fill( CarFluid.BRAKE, 1000 );
					carfluids.Fill( CarFluid.COOLANT, 1000 );
				}
				break;
			}
            // ---------------------------------------------------------------------------------------------------------
			case "v3s": {
				PlayerBase V3Sch_target;
				if(count == 2) {
					V3Sch_target = GetPlayer(tokens[1]);
					if(V3Sch_target == NULL) {
						SendMessageToPlayer(player, "Can't find player called: '"+tokens[1]+"'");
					} else {
						SendMessageToPlayer(player, "You Give V3S Chassis to " + V3Sch_target.GetIdentity().GetName());
					}
				} else {
					V3Sch_target = player;
					SendMessageToPlayer(player, "Vehicle spawned");
				}
				if(V3Sch_target != NULL) {
					EntityAI V3Scht;
					V3Scht = GetGame().CreateObject( "V3S_Chassis", V3Sch_target.GetPosition()- "5 0 0", false, true );		
					V3Scht.GetInventory().CreateAttachment("V3SWheel");
					V3Scht.GetInventory().CreateAttachment("V3SWheel");
					V3Scht.GetInventory().CreateAttachment("V3SWheel");
					V3Scht.GetInventory().CreateAttachment("V3SWheel");
					V3Scht.GetInventory().CreateAttachment("V3SWheelDouble");
					V3Scht.GetInventory().CreateAttachment("V3SWheelDouble");
					V3Scht.GetInventory().CreateAttachment("V3SWheelDouble");
					V3Scht.GetInventory().CreateAttachment("V3SWheelDouble");
					V3Scht.GetInventory().CreateAttachment("V3SDoors_Driver");
					V3Scht.GetInventory().CreateAttachment("V3SHood");
					V3Scht.GetInventory().CreateAttachment("GlowPlug");
					V3Scht.GetInventory().CreateAttachment("TruckRadiator");
					V3Scht.GetInventory().CreateAttachment("TruckExhaust");
					V3Scht.GetInventory().CreateAttachment("TruckBattery");
					V3Scht.GetInventory().CreateAttachment("HeadlightH7");
					V3Scht.GetInventory().CreateInInventory("CanisterGasoline");	
					V3Scht.GetInventory().CreateInInventory("WaterBottle");
					auto carfluidsV3Scht = Car.Cast( V3Scht );
					carfluidsV3Scht.Fill( CarFluid.FUEL, 1000 );
					carfluidsV3Scht.Fill( CarFluid.OIL, 1000 );
					carfluidsV3Scht.Fill( CarFluid.BRAKE, 1000 );
					carfluidsV3Scht.Fill( CarFluid.COOLANT, 1000 );					
				}
				break;
			}	
            // ---------------------------------------------------------------------------------------------------------
			case "sedan": {
				PlayerBase Sedan_target;
				if(count == 2) {
					Sedan_target = GetPlayer(tokens[1]);
					if(Sedan_target == NULL) {
						SendMessageToPlayer(player, "Can't find player called: '"+tokens[1]+"'");
					} else {
						SendMessageToPlayer(player, "You Give Sedan to " + Sedan_target.GetIdentity().GetName());
					}
				} else {
					Sedan_target = player;
					SendMessageToPlayer(player, "Vehicle spawned");
				}
				if(Sedan_target != NULL) {
					EntityAI sedant;
					sedant = GetGame().CreateObject( "CivilianSedan", Sedan_target.GetPosition()- "5 0 0", false, true );		
					sedant.GetInventory().CreateAttachment("CivSedanWheel");
					sedant.GetInventory().CreateAttachment("CivSedanWheel");
					sedant.GetInventory().CreateAttachment("CivSedanWheel");
					sedant.GetInventory().CreateAttachment("CivSedanWheel");
					sedant.GetInventory().CreateAttachment("CivSedanWheel");
					sedant.GetInventory().CreateAttachment("CivSedanDoors_Driver");
					sedant.GetInventory().CreateAttachment("CivSedanDoors_CoDriver");
					sedant.GetInventory().CreateAttachment("CivSedanDoors_BackLeft");
					sedant.GetInventory().CreateAttachment("CivSedanDoors_BackRight");
					sedant.GetInventory().CreateAttachment("CivSedanTrunk");
					sedant.GetInventory().CreateAttachment("CivSedanHood");
					sedant.GetInventory().CreateAttachment("TruckRadiator");
					sedant.GetInventory().CreateAttachment("SparkPlug");
					sedant.GetInventory().CreateAttachment("EngineBelt");
					sedant.GetInventory().CreateAttachment("HeadlightH7");
					sedant.GetInventory().CreateAttachment("CarRadiator");	
					sedant.GetInventory().CreateAttachment("CarBattery");
					sedant.GetInventory().CreateAttachment("HeadlightH7");
					sedant.GetInventory().CreateInInventory("CanisterGasoline");
					sedant.GetInventory().CreateInInventory("WaterBottle");
					auto carfluidssedant = Car.Cast( sedant );
					carfluidssedant.Fill( CarFluid.FUEL, 1000 );
					carfluidssedant.Fill( CarFluid.OIL, 1000 );
					carfluidssedant.Fill( CarFluid.BRAKE, 1000 );
					carfluidssedant.Fill( CarFluid.COOLANT, 1000 );					
				}
				break;
			}
            // ---------------------------------------------------------------------------------------------------------
			case "v3sc": {
				PlayerBase v3sc_target;
				if(count == 2) {
					v3sc_target = GetPlayer(tokens[1]);
					if(v3sc_target == NULL) {
						SendMessageToPlayer(player, "Can't find player called: '"+tokens[1]+"'");
					} else {
						SendMessageToPlayer(player, "You Give V3S Cargo to " + v3sc_target.GetIdentity().GetName());
					}
				} else {
					v3sc_target = player;
					SendMessageToPlayer(player, "Vehicle spawned");
				}
				if(v3sc_target != NULL) {
					EntityAI v3sct;
					v3sct = GetGame().CreateObject( "V3S_Cargo", v3sc_target.GetPosition()- "5 0 0", false, true );		
					v3sct.GetInventory().CreateAttachment("V3SWheel");
					v3sct.GetInventory().CreateAttachment("V3SWheel");
					v3sct.GetInventory().CreateAttachment("V3SWheel");
					v3sct.GetInventory().CreateAttachment("V3SWheel");
					v3sct.GetInventory().CreateAttachment("V3SWheelDouble");
					v3sct.GetInventory().CreateAttachment("V3SWheelDouble");
					v3sct.GetInventory().CreateAttachment("V3SWheelDouble");
					v3sct.GetInventory().CreateAttachment("V3SWheelDouble");
					v3sct.GetInventory().CreateAttachment("V3SDoors_Driver");
					v3sct.GetInventory().CreateAttachment("V3SHood");
					v3sct.GetInventory().CreateAttachment("GlowPlug");
					v3sct.GetInventory().CreateAttachment("TruckRadiator");
					v3sct.GetInventory().CreateAttachment("TruckExhaust");
					v3sct.GetInventory().CreateAttachment("TruckBattery");
					v3sct.GetInventory().CreateAttachment("HeadlightH7");
					v3sct.GetInventory().CreateInInventory("CanisterGasoline");
					v3sct.GetInventory().CreateInInventory("WaterBottle");
					auto carfluidsv3sct = Car.Cast( v3sct );
					carfluidsv3sct.Fill( CarFluid.FUEL, 1000 );
					carfluidsv3sct.Fill( CarFluid.OIL, 1000 );
					carfluidsv3sct.Fill( CarFluid.BRAKE, 1000 );
					carfluidsv3sct.Fill( CarFluid.COOLANT, 1000 );					
				}
				break;
			}			
            // ---------------------------------------------------------------------------------------------------------				
			case "refuel": {
				ref array<Object> nearest_objects = new array<Object>;
				ref array<CargoBase> proxy_cargos = new array<CargoBase>;
				Car toBeFilled;
				vector position = player.GetPosition();
				GetGame().GetObjectsAtPosition ( position, 10, nearest_objects, proxy_cargos );
		
				for (i = 0; i < nearest_objects.Count(); i++) {
					if (nearest_objects[i].IsKindOf("CarScript")) {
						SendMessageToPlayer(player, "[Refuel] Found car: '"+nearest_objects[i]+"'");
						toBeFilled = Car.Cast(nearest_objects[i]);
						float fuelReq = toBeFilled.GetFluidCapacity( CarFluid.FUEL ) - (toBeFilled.GetFluidCapacity( CarFluid.FUEL ) * toBeFilled.GetFluidFraction( CarFluid.FUEL ));
						float oilReq = toBeFilled.GetFluidCapacity( CarFluid.OIL ) - (toBeFilled.GetFluidCapacity( CarFluid.OIL ) * toBeFilled.GetFluidFraction( CarFluid.OIL ));
						float coolantReq = toBeFilled.GetFluidCapacity( CarFluid.COOLANT ) - (toBeFilled.GetFluidCapacity( CarFluid.COOLANT ) * toBeFilled.GetFluidFraction( CarFluid.COOLANT ));
						float brakeReq = toBeFilled.GetFluidCapacity( CarFluid.BRAKE ) - (toBeFilled.GetFluidCapacity( CarFluid.BRAKE ) * toBeFilled.GetFluidFraction( CarFluid.BRAKE ));
						toBeFilled.Fill( CarFluid.FUEL, fuelReq );
						toBeFilled.Fill( CarFluid.OIL, oilReq );
						toBeFilled.Fill( CarFluid.COOLANT, coolantReq );
						toBeFilled.Fill( CarFluid.BRAKE, brakeReq );
						SendMessageToPlayer(player, "[Refuel] "+fuelReq+"L added, all fluids maxed");
					}
				}
				
				break;
			}			
            // ---------------------------------------------------------------------------------------------------------	
			case "prison": {
				player.SetPosition("2651.42 1.7 1395.8");       // Prison Island         
				break;
			}		
			case "mogilevka": {
				player.SetPosition("7572.65 214.7 5182.3");	    // mogilevka        
				break;
			}	
			case "stary": {
				player.SetPosition("6192.39 299.7 7666.5");	    // stary      
				break;
			}	
			case "msta": {
				player.SetPosition("11206.6 254.7 5398.70");    // msta        
				break;
			}
			case "solni": {
				player.SetPosition("13436.5 6.7 6158.7");	    // solnicni
				break;
			}	
			case "cherno": {
				player.SetPosition("6350.99 9.7 2666.12");	    // cherno
				break;
			}	
			case "elektro": {
				player.SetPosition("10432.1 6.7 2218.56");	    // elektro
				break;
			}	
			case "berez": {
				player.SetPosition("12661.4 6.7 9465.03");	    // berez
				break;
			}	
			case "tisy": {
				player.SetPosition("1890.45 448.94 13704.6");	// tisy
				break;
			}	
            // ---------------------------------------------------------------------------------------------------------
			case "severo": {
				player.SetPosition("8318.51 109.7 12743.4");           // Severograd center
				if(count != 2) { SendMessageToPlayer(player, "!severo [airport] [police] [radio]"); return; }
				switch(tokens[1]) {
					case "police": {
						player.SetPosition("7740.11 118.7 12641.4");   // Severograd police
						break;
					}
					case "radio": {
						player.SetPosition("7986.21 114.7 12737.1");   // Severograd radio
						break;
					}					
					default: {
						SendMessageToPlayer(player, "[TPLocations] " + tokens[1] + " not found");
						break;
					}
				}
				break;
			}				
            // ---------------------------------------------------------------------------------------------------------
			case "balota": {
				player.SetPosition("4546.92 2.7 2416.4");              // Balota center
				if(count != 2) { SendMessageToPlayer(player, "!balota [airport]"); return; }
				switch(tokens[1]) {
					case "airport": {
						player.SetPosition("4996.84 9.7 2416.6");      // Balota airport
						break;
					}
					default: {
						SendMessageToPlayer(player, "[TPLocations] " + tokens[1] + " not found");
						break;
					}
				}
				break;
			}			
            // ---------------------------------------------------------------------------------------------------------
			case "gorka": {
				player.SetPosition("9678.94 298.7 8828.93");            // Gorka center
				if(count != 2) { SendMessageToPlayer(player, "!gorka [police]"); return; }
				switch(tokens[1]) {
					case "police": {
						player.SetPosition("9546.24 302.7 8804.0");     // Gorka police
						break;
					}
					default: {
						SendMessageToPlayer(player, "[TPLocations] " + tokens[1] + " not found");
						break;
					}
				}
				break;
			}		
            // ---------------------------------------------------------------------------------------------------------
			case "vybor": {
				player.SetPosition("3916.85 311.7 8795.59");            // Vybor center
				if(count != 2) { SendMessageToPlayer(player, "!vybor [airport]"); return; }
				switch(tokens[1]) {
					case "airport": {
						player.SetPosition("4107.80 342.7 11205.29");   // Vybor airport
						break;
					}
					default: {
						SendMessageToPlayer(player, "[TPLocations] " + tokens[1] + " not found");
						break;
					}
				}
				break;
			}
            // ---------------------------------------------------------------------------------------------------------
			case "krasno": {
				player.SetPosition("11172.0 198.7 12314.1");            // Krasnostav center 
				if(count != 2) { SendMessageToPlayer(player, "!krasno [castle] [airport]"); return; }
				switch(tokens[1]) {
					case "castle": {
						player.SetPosition("10240.2 366.7 12022.1");    // Krasnostav castle
						break;
					}
					case "airport": {
						player.SetPosition("12004.3 140.7 12655.3");    // Krasnostav airport
						break;
					}
					default: {
						SendMessageToPlayer(player, "[TPLocations] " + tokens[1] + " not found");
						break;
					}
				}
				break;
			}				
            // ---------------------------------------------------------------------------------------------------------
			case "sniper": {
				PlayerBase sniper_target;
				if(count == 2) {
					sniper_target = GetPlayer(tokens[1]);
					if(sniper_target == NULL) {
						SendMessageToPlayer(player, "Can't find player called: '"+tokens[1]+"'");
					} else {
						SendMessageToPlayer(player, "You Give stuff sniper to " + sniper_target.GetIdentity().GetName());
					}
				} else {
					sniper_target = player;
					SendMessageToPlayer(player, "You Give stuff sniper");
				}
				if(sniper_target != NULL) {
					sniper_target.RemoveAllItems();                                                 // Remove All Items
					sniper_target.GetStatWater().Set(1000);                                         // Water       100%

					sniper_target.GetInventory().CreateInInventory("Balaclava3Holes_Green");
					sniper_target.GetInventory().CreateInInventory("TacticalGloves_Green");         // Tactical Gloves  Green
					sniper_target.GetInventory().CreateInInventory("TTsKOJacket_Camo");	            // TTsKO Jacket     Camo
					sniper_target.GetInventory().CreateInInventory("HighCapacityVest_Olive");       // HighCapacityVest Olive
					sniper_target.GetInventory().CreateInInventory("TTSKOPants");                   // TTSKO Pants
			
					EntityAI mbs = sniper_target.GetInventory().CreateInInventory("MilitaryBoots_Brown");
					mbs.GetInventory().CreateAttachment("CombatKnife");

					sniper_target.GetInventory().CreateInInventory("GhillieHood_Mossy");            // Ghillie Hood
					sniper_target.GetInventory().CreateInInventory("GhillieSuit_Mossy");            // Ghillie Suit
		
					EntityAI Smapct = sniper_target.GetInventory().CreateInInventory("SmallProtectorCase");
					Smapct.GetInventory().CreateInInventory("Lockpick");
					Smapct.GetInventory().CreateInInventory("ChernarusMap");
					Smapct.GetInventory().CreateInInventory("DuctTape");
					Smapct.GetInventory().CreateInInventory("Roadflare");
					Smapct.GetInventory().CreateInInventory("Rope");
					Smapct.GetInventory().CreateInInventory("WaterBottle");
					Smapct.GetInventory().CreateInInventory("Battery9V");
					Smapct.GetInventory().CreateInInventory("Compass");
					Smapct.GetInventory().CreateInInventory("Matchbox");

					EntityAI rds = sniper_target.GetInventory().CreateInInventory("PersonalRadio");
					rds.GetInventory().CreateAttachment("Battery9V");		

					EntityAI boxtf = sniper_target.GetInventory().CreateInInventory("AmmoBox");	
					EntityAI ttaksa = boxtf.GetInventory().CreateInInventory("FNX45");
					ttaksa.GetInventory().CreateAttachment("PistolSuppressor");
					ttaksa.GetInventory().CreateAttachment("FNP45_MRDSOptic");
					ttaksa.GetInventory().CreateAttachment("UniversalLight");
					boxtf.GetInventory().CreateInInventory("AmmoBox_45ACP_25rnd");
					boxtf.GetInventory().CreateInInventory("AmmoBox_45ACP_25rnd");
					boxtf.GetInventory().CreateInInventory("AmmoBox_762x54_20Rnd");
					boxtf.GetInventory().CreateInInventory("AmmoBox_762x54_20Rnd");
					boxtf.GetInventory().CreateInInventory("Mag_FNX45_15Rnd");
					boxtf.GetInventory().CreateInInventory("Mag_FNX45_15Rnd");
					boxtf.GetInventory().CreateInInventory("Mag_SVD_10Rnd");
					boxtf.GetInventory().CreateInInventory("Mag_SVD_10Rnd");
					boxtf.GetInventory().CreateInInventory("Mag_SVD_10Rnd");
					boxtf.GetInventory().CreateInInventory("Mag_SVD_10Rnd");
							
					sniper_target.GetInventory().CreateInInventory("Binoculars");
					sniper_target.GetInventory().CreateInInventory("Cannabis");
					sniper_target.GetInventory().CreateInInventory("Cannabis");
					sniper_target.GetInventory().CreateInInventory("Cannabis");				
					
					EntityAI boxms = sniper_target.GetInventory().CreateInInventory("FirstAidKit");
					boxms.GetInventory().CreateInInventory("SalineBagIV");		
					boxms.GetInventory().CreateInInventory("Morphine");	
					boxms.GetInventory().CreateInInventory("Epinephrine");	
					boxms.GetInventory().CreateInInventory("BandageDressing");	
					boxms.GetInventory().CreateInInventory("BandageDressing");	
					boxms.GetInventory().CreateInInventory("BandageDressing");	
					boxms.GetInventory().CreateInInventory("BandageDressing");					

					sniper_target.GetInventory().CreateInInventory("Chemlight_Green");	            // Chemlight Green
					sniper_target.GetInventory().CreateInInventory("SodaCan_Cola");
					sniper_target.GetInventory().CreateInInventory("SodaCan_Cola");
					sniper_target.GetInventory().CreateInInventory("SardinesCan_Opened");					

					EntityAI aSVD = sniper_target.GetHumanInventory().CreateInHands("SVD");         // SVD
					aSVD.GetInventory().CreateAttachment("GhillieAtt_Woodland");                    // Ghillie Weapons
					aSVD.GetInventory().CreateAttachment("PSO1Optic");                              // PSO1 optique   
				}
				break;
			}		
            // ---------------------------------------------------------------------------------------------------------
			case "m4": {
				PlayerBase m4a1_target;
				if(count == 2) {
					m4a1_target = GetPlayer(tokens[1]);
					if(m4a1_target == NULL) {
						SendMessageToPlayer(player, "Can't find player called: '"+tokens[1]+"'");
					} else {
						SendMessageToPlayer(player, "You Give M4A1 to " + m4a1_target.GetIdentity().GetName());
					}
				} else {
					m4a1_target = player;
					SendMessageToPlayer(player, "M4A1 spawned");
				}
				if(m4a1_target != NULL) {
					EntityAI m4a1t;
					m4a1t = GetGame().CreateObject( "M4A1_Black", m4a1_target.GetPosition(), false, true, true );          
					m4a1t.GetInventory().CreateAttachment("M4_Suppressor");    
					m4a1t.GetInventory().CreateAttachment("M4_MPBttstck_Black"); 
					m4a1t.GetInventory().CreateAttachment("M4_MPHndgrd_Black"); 	
					m4a1t.GetInventory().CreateAttachment("M9A1_Bayonet"); 	
					m4a1t.GetInventory().CreateAttachment("ACOGOptic");   	 
					GetGame().CreateObject( "Mag_STANAGCoupled_30Rnd", m4a1_target.GetPosition(), false, true, true );	
				}
				break;
			}		
            // ---------------------------------------------------------------------------------------------------------	
			case "zombie": {
				PlayerBase zombie_target;
				if(count == 2) {
					zombie_target = GetPlayer(tokens[1]);
					if(zombie_target == NULL) {
						SendMessageToPlayer(player, "Can't find player called: '"+tokens[1]+"'");
					} else {
						SendMessageToPlayer(player, "You spawn zombie to " + zombie_target.GetIdentity().GetName());
					}
				} else {
					zombie_target = player;
					SendMessageToPlayer(player, "Zombie spawned");
				}
				if(zombie_target != NULL) {
					GetGame().CreateObject( "ZmbF_CitizenANormal_Blue", zombie_target.GetPosition()- "10 0 0", false, true );
				}
				break;
			}
            // ---------------------------------------------------------------------------------------------------------				
			case "horde": {
				PlayerBase horde_target;
				if(count == 2) {
					horde_target = GetPlayer(tokens[1]);
					if(horde_target == NULL) {
						SendMessageToPlayer(player, "Can't find player called: '"+tokens[1]+"'");
					} else {
						SendMessageToPlayer(player, "You spawn horde to " + horde_target.GetIdentity().GetName());
					}
				} else {
					horde_target = player;
					SendMessageToPlayer(player, "Horde spawned");
				}
				if(horde_target != NULL) {

					GetGame().CreateObject( "ZmbF_BlueCollarFat_Blue", horde_target.GetPosition()- "50 0 0", false, true );			
					GetGame().CreateObject( "ZmbF_BlueCollarFat_Green", horde_target.GetPosition()- "0 0 50", false, true );
					GetGame().CreateObject( "ZmbF_BlueCollarFat_Red", horde_target.GetPosition()- "50 0 -50", false, true );	
					GetGame().CreateObject( "ZmbF_BlueCollarFat_White", horde_target.GetPosition()- "-50 0 50", false, true );	
					GetGame().CreateObject( "ZmbF_CitizenANormal_Beige", horde_target.GetPosition()- "51 0 0", false, true );		
					GetGame().CreateObject( "ZmbF_CitizenANormal_Blue", horde_target.GetPosition()- "51 0 0", false, true );			
					GetGame().CreateObject( "ZmbF_CitizenANormal_Brown", horde_target.GetPosition()- "0 0 51", false, true );
					GetGame().CreateObject( "ZmbF_CitizenBSkinny", horde_target.GetPosition()- "51 0 -51", false, true );	
					GetGame().CreateObject( "ZmbF_Clerk_Normal_Blue", horde_target.GetPosition()- "-51 0 51", false, true );	
					GetGame().CreateObject( "ZmbF_Clerk_Normal_Green", horde_target.GetPosition()- "52 0 0", false, true );		
					GetGame().CreateObject( "ZmbF_Clerk_Normal_Red", horde_target.GetPosition()- "52 0 0", false, true );			
					GetGame().CreateObject( "ZmbF_Clerk_Normal_White", horde_target.GetPosition()- "0 0 52", false, true );
					GetGame().CreateObject( "ZmbF_DoctorSkinny", horde_target.GetPosition()- "52 0 -52", false, true );	
					GetGame().CreateObject( "ZmbF_HikerSkinny_Blue", horde_target.GetPosition()- "-52 0 52", false, true );		
					GetGame().CreateObject( "ZmbF_HikerSkinny_Green", horde_target.GetPosition()- "53 0 0", false, true );			
					GetGame().CreateObject( "ZmbF_HikerSkinny_Grey", horde_target.GetPosition()- "0 0 53", false, true );
					GetGame().CreateObject( "ZmbF_HikerSkinny_Red", horde_target.GetPosition()- "53 0 -53", false, true );	
					GetGame().CreateObject( "ZmbF_JoggerSkinny_Blue", horde_target.GetPosition()- "-53 0 53", false, true );
					GetGame().CreateObject( "ZmbF_JoggerSkinny_Brown", horde_target.GetPosition()- "54 0 0", false, true );			
					GetGame().CreateObject( "ZmbF_JoggerSkinny_Green", horde_target.GetPosition()- "0 0 54", false, true );
					GetGame().CreateObject( "ZmbF_JoggerSkinny_Red", horde_target.GetPosition()- "54 0 -54", false, true );	
					GetGame().CreateObject( "ZmbF_JournalistNormal_Blue", horde_target.GetPosition()- "-54 0 54", false, true );
					GetGame().CreateObject( "ZmbF_JournalistNormal_Green", horde_target.GetPosition()- "55 0 0", false, true );			
					GetGame().CreateObject( "ZmbF_JournalistNormal_Red", horde_target.GetPosition()- "0 0 55", false, true );
					GetGame().CreateObject( "ZmbF_JournalistNormal_White", horde_target.GetPosition()- "55 0 -55", false, true );	
					GetGame().CreateObject( "ZmbF_MechanicNormal_Beige", horde_target.GetPosition()- "-55 0 55", false, true );
					GetGame().CreateObject( "ZmbF_MechanicNormal_Green", horde_target.GetPosition()- "56 0 0", false, true );			
					GetGame().CreateObject( "ZmbF_MechanicNormal_Grey", horde_target.GetPosition()- "0 0 56", false, true );
					GetGame().CreateObject( "ZmbF_MechanicNormal_Orange", horde_target.GetPosition()- "56 0 -56", false, true );	
					GetGame().CreateObject( "ZmbF_MilkMaidOld_Beige", horde_target.GetPosition()- "-56 0 56", false, true );	
					GetGame().CreateObject( "ZmbF_MilkMaidOld_Black", horde_target.GetPosition()- "57 0 0", false, true );			
					GetGame().CreateObject( "ZmbF_MilkMaidOld_Green", horde_target.GetPosition()- "0 0 57", false, true );
					GetGame().CreateObject( "ZmbF_MilkMaidOld_Grey", horde_target.GetPosition()- "57 0 -57", false, true );	
					GetGame().CreateObject( "ZmbF_NurseFat", horde_target.GetPosition()- "-57 0 57", false, true );	
					GetGame().CreateObject( "ZmbF_ParamedicNormal_Blue", horde_target.GetPosition()- "58 0 0", false, true );			
					GetGame().CreateObject( "ZmbF_ParamedicNormal_Green", horde_target.GetPosition()- "0 0 58", false, true );
					GetGame().CreateObject( "ZmbF_ParamedicNormal_Red", horde_target.GetPosition()- "58 0 -58", false, true );	
					GetGame().CreateObject( "ZmbF_PatientOld", horde_target.GetPosition()- "-58 0 58", false, true );	
					GetGame().CreateObject( "ZmbF_PoliceWomanNormal", horde_target.GetPosition()- "59 0 0", false, true );			
					GetGame().CreateObject( "ZmbF_ShortSkirt_beige", horde_target.GetPosition()- "0 0 59", false, true );
					GetGame().CreateObject( "ZmbF_ShortSkirt_black", horde_target.GetPosition()- "59 0 -59", false, true );	
					GetGame().CreateObject( "ZmbF_ShortSkirt_brown", horde_target.GetPosition()- "-59 0 59", false, true );	
					GetGame().CreateObject( "ZmbF_ShortSkirt_checks", horde_target.GetPosition()- "60 0 0", false, true );			
					GetGame().CreateObject( "ZmbF_ShortSkirt_green", horde_target.GetPosition()- "0 0 60", false, true );
					GetGame().CreateObject( "ZmbF_ShortSkirt_grey", horde_target.GetPosition()- "60 0 -60", false, true );	
					GetGame().CreateObject( "ZmbF_ShortSkirt_red", horde_target.GetPosition()- "-60 0 60", false, true );	
					GetGame().CreateObject( "ZmbF_ShortSkirt_stripes", horde_target.GetPosition()- "61 0 0", false, true );			
					GetGame().CreateObject( "ZmbF_ShortSkirt_white", horde_target.GetPosition()- "0 0 61", false, true );
					GetGame().CreateObject( "ZmbF_ShortSkirt_yellow", horde_target.GetPosition()- "61 0 -61", false, true );	
					GetGame().CreateObject( "ZmbF_SkaterYoung_Brown", horde_target.GetPosition()- "-61 0 61", false, true );						
				}
				break;
			}
            // ---------------------------------------------------------------------------------------------------------				
			case "wolf": {
				PlayerBase wolf_target;
				if(count == 2) {
					wolf_target = GetPlayer(tokens[1]);
					if(wolf_target == NULL) {
						SendMessageToPlayer(player, "Can't find player called: '"+tokens[1]+"'");
					} else {
						SendMessageToPlayer(player, "You spawn wolf to " + wolf_target.GetIdentity().GetName());
					}
				} else {
					wolf_target = player;
					SendMessageToPlayer(player, "Wolf spawned");
				}
				if(wolf_target != NULL) {
					GetGame().CreateObject( "Animal_CanisLupus_Grey", wolf_target.GetPosition()- "10 0 0", false, true );
				}
				break;
			}		
            // ---------------------------------------------------------------------------------------------------------	
			default: {
				SendMessageToPlayer(player, "Unknown command: " + command);
				break;
			}
		}
	}
};
/*---------------------------------------------------------------------------------------------------------------------\
| Create Custom Mission                                                                                                |
\---------------------------------------------------------------------------------------------------------------------*/  
Mission CreateCustomMission(string path)
{
	return new CustomMission();
}
