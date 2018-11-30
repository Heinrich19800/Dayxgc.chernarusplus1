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

	weather.GetOvercast().SetForecastChangeLimits( 0.5, 0.8 );
	weather.GetRain().SetForecastChangeLimits( 0.1, 0.3 );
	weather.GetFog().SetForecastChangeLimits( 0.05, 0.10 );

	weather.GetOvercast().SetForecastTimeLimits( 3600 , 3600 );
	weather.GetRain().SetForecastTimeLimits( 300 , 300 );
	weather.GetFog().SetForecastTimeLimits( 3600 , 3600 );

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
	void SetRandomHealth(EntityAI itemEnt)
	{
		int rndHlt = Math.RandomInt(40,100);
		itemEnt.SetHealth("","",rndHlt);
	}

	override PlayerBase CreateCharacter(PlayerIdentity identity, vector pos, ParamsReadContext ctx, string characterName)
	{
		Entity playerEnt;
		playerEnt = GetGame().CreatePlayer(identity, characterName, pos, 0, "NONE");//Creates random player
		Class.CastTo(m_player, playerEnt);
		
		GetGame().SelectPlayer(identity, m_player);
		
		return m_player;
	}
/*---------------------------------------------------------------------------------------------------------------------\
| Starting Equip Setup                                                                                                 |
\---------------------------------------------------------------------------------------------------------------------*/	
//<!---------------------------------  ADMIN  -------------------------------->
	override void StartingEquipSetup(PlayerBase player, bool clothesChosen)
	{
	bool IsAdmin = false;  
	string SteamId_Adminx = "00000000000000000"; // Your Steam ID *Optional spawn Admin vybor! 

	if (player.GetIdentity().GetPlainId() == SteamId_Adminx){IsAdmin = true;}
	if (IsAdmin){
        player.RemoveAllItems();
		player.GetStatWater().Add(1000);
		player.SetHealth(100);
		player.GetInventory().CreateInInventory("TTsKOJacket_Camo");
		player.GetInventory().CreateInInventory("TTSKOPants");
		player.GetInventory().CreateInInventory("CombatBoots_Green");
		player.GetInventory().CreateInInventory("Chemlight_Green");
		player.GetInventory().CreateInInventory("Mag_IJ70_8Rnd");
		player.GetInventory().CreateInInventory("Rag");	
		EntityAI Makaa = player.GetInventory().CreateInInventory("MakarovIJ70");
		Makaa.GetInventory().CreateAttachment("PistolSuppressor");
		player.SetPosition("4107.80 342.7 11205.29");
	}				
//<!--------------------------------  PLAYER  -------------------------------->
	 else { switch (Math.RandomInt(0, 1)) {
		case 0:
/*
		player.RemoveAllItems();
		EntityAI item = player.GetInventory().CreateInInventory(topsArray.GetRandomElement());
		EntityAI item2 = player.GetInventory().CreateInInventory(pantsArray.GetRandomElement());
		EntityAI item3 = player.GetInventory().CreateInInventory(shoesArray.GetRandomElement());
*/
		EntityAI itemEnt;
		ItemBase itemBs;
		
		itemEnt = player.GetInventory().CreateInInventory("Rag");
		itemBs = ItemBase.Cast(itemEnt);
		itemBs.SetQuantity(4);
		SetRandomHealth(itemEnt);

		itemEnt = player.GetInventory().CreateInInventory("RoadFlare");
		itemBs = ItemBase.Cast(itemEnt);
		break;
		}
	  }
	}	
/*---------------------------------------------------------------------------------------------------------------------\
| Admin Tool                                                https://github.com/Malotruu/DayZMalotru.chernarusplus/wiki |
\---------------------------------------------------------------------------------------------------------------------*/
  //=========== Mods ===========
  bool verify_admins    = false;  // true = verify presence of BI UID in admin list  
  bool m_Stamina        = false;  // Unlimited stamina    for everyone
  bool m_Status         = false;  // Unlimited statistics for everyone
  bool m_DebugMonitor   = false;  // Debug Monitor off restart serv
  bool m_FreeCamera     = false;  // Advanced camera manager for content creators
  bool m_AutoFreezeWalk = false;  // Auto Walk and Freeze Player
  bool m_FreezePlayer   = false;  // Block the player on the spot
  //========= Entities =========  
  int Spawned_Canis     = 15;     // Number of Canis lupus
  int Spawned_Horde     = 50;     // Number of Zombie
  int Spawned_Animals   = 25;     // Number of Animals
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
//<!-------------------------  FindSelectedPlayer  --------------------------->	
    Man FindSelectedPlayer(string pName) {
        PlayerBase selectedPlayer = NULL;
        array<Man> players = new array<Man>;
        GetGame().GetPlayers( players );
        for (int zm = 0; zm < players.Count(); zm++) {
            PlayerBase currentPlayer = players.Get(zm);
            PlayerIdentity selectedIdentity = currentPlayer.GetIdentity();
            if (selectedIdentity.GetName() == pName) {
                selectedPlayer = currentPlayer;
                zm=100;
            }
        }
        return selectedPlayer;
    }
//<!-------------------------------  Entities  ------------------------------->		
    string GetRandomAnimals() { TStringArray loot = {"Animal_BosTaurusF_Spotted","Animal_BosTaurusF_White","Animal_BosTaurus_Brown","Animal_BosTaurus_Spotted","Animal_BosTaurus_White","Animal_CanisLupus_Grey","Animal_CanisLupus_White","Animal_CapraHircusF","Animal_CapraHircus_Black","Animal_CapraHircus_Brown","Animal_CapraHircus_White","Animal_CapreolusCapreolus","Animal_CapreolusCapreolusF","Animal_CervusElaphus","Animal_CervusElaphusF","Animal_GallusGallusDomesticus","Animal_GallusGallusDomesticusF_Brown","Animal_GallusGallusDomesticusF_Spotted","Animal_GallusGallusDomesticusF_White","Animal_OvisAries","Animal_OvisAriesF","Animal_SusDomesticus","Animal_SusScrofa"}; return loot.GetRandomElement(); }
    string GetRandomHorde()   { TStringArray loot = {"ZmbM_HermitSkinny_Base","ZmbM_HermitSkinny_Beige","ZmbM_HermitSkinny_Black","ZmbM_HermitSkinny_Green","ZmbM_HermitSkinny_Red","ZmbM_FarmerFat_Base","ZmbM_FarmerFat_Beige","ZmbM_FarmerFat_Blue","ZmbM_FarmerFat_Brown","ZmbM_FarmerFat_Green","ZmbF_CitizenANormal_Base","ZmbF_CitizenANormal_Beige","ZmbF_CitizenANormal_Brown","ZmbF_CitizenANormal_Blue","ZmbM_CitizenASkinny_Base","ZmbM_CitizenASkinny_Blue","ZmbM_CitizenASkinny_Brown","ZmbM_CitizenASkinny_Grey","ZmbM_CitizenASkinny_Red","ZmbM_CitizenBFat_Base","ZmbM_CitizenBFat_Blue","ZmbM_CitizenBFat_Red","ZmbM_CitizenBFat_Green","ZmbF_CitizenBSkinny_Base","ZmbF_CitizenBSkinny","ZmbM_PrisonerSkinny_Base","ZmbM_PrisonerSkinny","ZmbM_FirefighterNormal_Base","ZmbM_FirefighterNormal","ZmbM_FishermanOld_Base","ZmbM_FishermanOld_Blue","ZmbM_FishermanOld_Green","ZmbM_FishermanOld_Grey","ZmbM_FishermanOld_Red","ZmbM_JournalistSkinny_Base","ZmbM_JournalistSkinny","ZmbF_JournalistNormal_Base","ZmbF_JournalistNormal_Blue","ZmbF_JournalistNormal_Green","ZmbF_JournalistNormal_Red","ZmbF_JournalistNormal_White","ZmbM_ParamedicNormal_Base","ZmbM_ParamedicNormal_Blue","ZmbM_ParamedicNormal_Green","ZmbM_ParamedicNormal_Red","ZmbM_ParamedicNormal_Black","ZmbF_ParamedicNormal_Base","ZmbF_ParamedicNormal_Blue","ZmbF_ParamedicNormal_Green","ZmbF_ParamedicNormal_Red","ZmbM_HikerSkinny_Base","ZmbM_HikerSkinny_Blue","ZmbM_HikerSkinny_Green","ZmbM_HikerSkinny_Yellow","ZmbF_HikerSkinny_Base","ZmbF_HikerSkinny_Blue","ZmbF_HikerSkinny_Grey","ZmbF_HikerSkinny_Green","ZmbF_HikerSkinny_Red","ZmbM_HunterOld_Base","ZmbM_HunterOld_Autumn","ZmbM_HunterOld_Spring","ZmbM_HunterOld_Summer","ZmbM_HunterOld_Winter"}; return loot.GetRandomElement(); } 	
    string GetRandomCanis()   { TStringArray loot = {"Animal_CanisLupus_Grey","Animal_CanisLupus_White"}; return loot.GetRandomElement(); }	
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

        if (m_Stamina) { player.GetStatStamina().Add(1000); }

		if (m_Status) {					
		player.SetBleedingBits(0);							
		player.GetStatTremor().Add(0);
		player.GetStatWet().Add( -1 );	
        player.GetStatDiet().Add(2500);		
        player.GetStatWater().Add(1000);
		player.GetStatEnergy().Add(1000);
		player.GetStatSpecialty().Add(1);
		player.GetStatStamina().Add(1000);
		player.GetStatHeatComfort().Add(0);
		player.GetStatStomachVolume().Add(0);
		player.GetStatStomachWater().Add(5000);
		player.GetStatStomachEnergy().Add(20000);
		player.SetHealth( player.GetMaxHealth( "", "" ) );
		player.SetHealth( "","Blood", player.GetMaxHealth( "", "Blood" ) );
		player.SetHealth( "","Shock", player.GetMaxHealth( "", "Shock" ) );
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
		PlayerBase selectedPlayer;
		PlayerIdentity selectedIdentity;
		PlayerBase player, temp_player;
		array<Man> players = new array<Man>; GetGame().GetPlayers(players);
		if(eventTypeId != ChatMessageEventTypeID) return;
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
//Admin Tool// ---------------------------------------------------------------------------------------------------------
			case "remove":    { if(count == 2) { temp_player = GetPlayer(tokens[1]); if(temp_player == NULL) { SendMessageToPlayer(player, "Can't find player called: '"+tokens[1]+"'"); } else { SendMessageToPlayer(player, "Delete an object in hand "                + temp_player.GetIdentity().GetName()); } } else { temp_player = player; } if(temp_player != NULL) { EntityAI CurrentInHandsA = temp_player.GetHumanInventory().GetEntityInHands(); string rdisplayNameA = CurrentInHandsA.ConfigGetString("rdisplayNameA"); GetGame().ObjectDelete(CurrentInHandsA); temp_player.OnItemInHandsChanged(); } break; }
			case "camera":    { PlayerBase pBody = player; if (m_FreeCamera) { GetGame().SelectPlayer(player.GetIdentity(), pBody); player.GetInputController().OverrideMovementSpeed( false, 0 ); player.GetInputController().OverrideMovementAngle( false, 0 ); player.GetInputController().OverrideAimChangeX( false, 0 ); player.GetInputController().OverrideAimChangeY( false, 0 ); m_FreeCamera = false; } else { GetGame().SelectPlayer(player.GetIdentity(), NULL); GetGame().SelectSpectator(player.GetIdentity(), "freedebugcamera", player.GetPosition()); player.GetInputController().OverrideMovementSpeed( true, 0 ); player.GetInputController().OverrideMovementAngle( true, 0 ); player.GetInputController().OverrideAimChangeX( true, 0 ); player.GetInputController().OverrideAimChangeY( true, 0 ); m_FreeCamera = true; } break; }
			case "list":      { for (int list = 0; list < players.Count(); list++) { selectedPlayer = players.Get(list); selectedIdentity = selectedPlayer.GetIdentity(); SendMessageToPlayer(player, "Player Name: " + selectedIdentity.GetName()); } break; }			
			case "state":     { if (m_Status)  { m_Status = false; }  else { m_Status = true; }              break; }
			case "stamina":   { if (m_Stamina) { m_Stamina = false; } else { m_Stamina = true; }             break; }
			case "debug":     { if (m_DebugMonitor) { GetGame().SetDebugMonitorEnabled(0); m_DebugMonitor = false; } else { GetGame().SetDebugMonitorEnabled(1); m_DebugMonitor = true; } break; } 
			case "run":       { if (m_AutoFreezeWalk) { player.GetInputController().OverrideMovementSpeed( false, 0 ); player.GetInputController().OverrideMovementAngle( false, 0 ); m_AutoFreezeWalk = false; } else { player.GetInputController().OverrideMovementSpeed( true, 5 ); player.GetInputController().OverrideMovementAngle( true, 0 ); m_AutoFreezeWalk = true; } break; } 
			case "spawn":     { if(count != 2) { SendMessageToPlayer(player, "!spawn [object]");            return; } GetGame().CreateObject(tokens[1], player.GetPosition(), false, true ); SendMessageToPlayer(player, "Object spawned: " + tokens[1]); break; }
			case "give":      { if(count != 2) { SendMessageToPlayer(player, "!give [object]");             return; } player.GetInventory().CreateInInventory(tokens[1]); SendMessageToPlayer(player, "Object generated in the inventory: " + tokens[1]); break; }		
			case "heal":      { if(count == 2) { temp_player = GetPlayer(tokens[1]); if(temp_player == NULL) { SendMessageToPlayer(player, "Can't find player called: '"+tokens[1]+"'"); } else { SendMessageToPlayer(player, "You healed "                              + temp_player.GetIdentity().GetName()); } } else { temp_player = player; } if(temp_player != NULL) { temp_player.GetStatHeatComfort().Add(0); temp_player.GetStatStamina().Add(1000); temp_player.GetStatEnergy().Add(1000); temp_player.GetStatWater().Add(1000); temp_player.GetStatWet().Add( -1 ); temp_player.SetBleedingBits(0); temp_player.SetHealth( temp_player.GetMaxHealth( "", "" ) ); temp_player.SetHealth( "","Blood", temp_player.GetMaxHealth( "", "Blood" ) ); temp_player.SetHealth( "","Shock", temp_player.GetMaxHealth( "", "Shock" ) ); } break; }			
			case "healall":   { SendMessageToPlayer(player, "You healed everyone"); for (i = 0; i < players.Count(); i++) { temp_player = players.Get(i); SendMessageToPlayer(temp_player, "You have been healed by an admin"); temp_player.SetHealth("", "Blood", temp_player.GetMaxHealth("", "Blood")); temp_player.SetHealth("","Shock", temp_player.GetMaxHealth("","Shock") ); temp_player.SetHealth(temp_player.GetMaxHealth("", "")); temp_player.GetStatStamina().Set(1000); temp_player.GetStatEnergy().Set(1000); temp_player.GetStatWater().Set(1000); } break; }			
// Weather  // ---------------------------------------------------------------------------------------------------------
			case "night":     { GetGame().GetWorld().SetDate(2018, 14, 11, 22, 0); SendMessageToPlayer(player, "NIGHT TIME!!"); break; }
			case "day":       { GetGame().GetWorld().SetDate(2018, 14, 11, 12, 0); SendMessageToPlayer(player, "DAY TIME!!");   break; }
			case "time":      { if(count != 3) { SendMessageToPlayer(player, "!time [hour] [minute]");      return; } GetGame().GetWorld().SetDate( 2018, 1, 7, tokens[1].ToInt(), tokens[2].ToInt()); SendMessageToPlayer(player, "You have set the servertime to " + tokens[1] + ":"+tokens[2]); break; }
			case "rain":      { if(count != 2) { SendMessageToPlayer(player, "!rain [0-100]");              return; } float rain = tokens[1].ToFloat() / 100; GetGame().GetWeather().GetRain().Set(rain, 2, 600); SendMessageToPlayer(player, "You have set Rain to " + tokens[1] + "% ["+rain+"]"); break; }
			case "fog":       { if(count != 2) { SendMessageToPlayer(player, "!fog [0-100]");               return; } float fog = tokens[1].ToFloat() / 100; GetGame().GetWeather().GetFog().Set(fog, 2, 600); SendMessageToPlayer(player, "You have set Fog to " + tokens[1] + "% ["+fog+"]"); break; }
			case "overcast":  { if(count != 2) { SendMessageToPlayer(player, "!overcast [0-100]");          return; } float overcast = tokens[1].ToFloat() / 100; GetGame().GetWeather().GetOvercast().Set(overcast, 2, 600); SendMessageToPlayer(player, "You have set Overcast to " + tokens[1] + "% ["+overcast+"]"); break; }							
// Teleport // ---------------------------------------------------------------------------------------------------------
			case "allhere":   { SendMessageToPlayer(player, "You teleported everyone to your location"); for (i = 0; i < players.Count(); i++) { temp_player = players.Get(i); SendMessageToPlayer(temp_player, "You have been teleported to admin " + player.GetIdentity().GetName()); temp_player.SetPosition(player.GetPosition()); } break; }
			case "allgoto":   { PlayerBase allgoto_target; if(count != 2) { SendMessageToPlayer(player, "!allgoto [player]"); return; } allgoto_target = GetPlayer(tokens[1]); if(allgoto_target == NULL) { SendMessageToPlayer(player, "Can't find player called: '"+tokens[1]+"'"); } else { SendMessageToPlayer(player, "You teleported everyone to your location"); for (i = 0; i < players.Count(); i++) { temp_player = players.Get(i); SendMessageToPlayer(temp_player, "You have been teleported to player " + allgoto_target.GetIdentity().GetName()); temp_player.SetPosition(allgoto_target.GetPosition()); } } break; }
			case "goto":      { if(count != 2) { SendMessageToPlayer(player, "!goto [player]");             return; } temp_player = GetPlayer(tokens[1]); if(temp_player == NULL) { SendMessageToPlayer(player, "Can't find player called: '"+tokens[1]+"'"); } else { player.SetPosition(temp_player.GetPosition()); SendMessageToPlayer(player, "You teleported to " + temp_player.GetIdentity().GetName()); } break; }
			case "here":      { if(count != 2) { SendMessageToPlayer(player, "!here [player]");             return; } temp_player = GetPlayer(tokens[1]); if(temp_player == NULL) { SendMessageToPlayer(player, "Can't find player called: '"+tokens[1]+"'"); } else { temp_player.SetPosition(player.GetPosition()); SendMessageToPlayer(temp_player, "You have been teleported to admin " + player.GetIdentity().GetName()); SendMessageToPlayer(player, "You teleported " + temp_player.GetIdentity().GetName() + " to your location"); } break; }
			case "tppos":     { if (count < 3) { SendMessageToPlayer(player, "!tppos [X] [Y] [player]");    return; } float ATL_Z = GetGame().SurfaceY(tokens[1].ToFloat(), tokens[2].ToFloat()); vector reqpos = Vector(tokens[1].ToFloat(), ATL_Z, tokens[2].ToFloat()); temp_player = player; if (count == 4) { temp_player = GetPlayer(tokens[3]); if (temp_player == NULL) { SendMessageToPlayer(player, "Can't find player called: '"+tokens[3]+"'"); return; } else { SendMessageToPlayer(temp_player, "You've been teleported to " + reqpos + " by admin " + player.GetIdentity().GetName()); } } temp_player.SetPosition(reqpos); SendMessageToPlayer(player, "Target teleported to " + temp_player.GetPosition()); break; }
			case "tp":        { if(count != 2) { SendMessageToPlayer(player, "!tp [Locations]");            return; } switch(tokens[1]) { case "prison":    { player.SetPosition("2651.42 1.7 1395.8");     break; } case "mogilevka": { player.SetPosition("7572.65 214.7 5182.3");   break; } case "stary":     { player.SetPosition("6192.39 299.7 7666.5");   break; } case "msta":      { player.SetPosition("11206.6 254.7 5398.70");  break; } case "solni":     { player.SetPosition("13436.5 6.7 6158.7");     break; } case "cherno":    { player.SetPosition("6350.99 9.7 2666.12");    break; } case "elektro":   { player.SetPosition("10432.1 6.7 2218.56");    break; } case "berez":     { player.SetPosition("12661.4 6.7 9465.03");    break; } case "tisy":      { player.SetPosition("1890.45 448.94 13704.6"); break; } case "gorka":     { player.SetPosition("9678.94 298.7 8828.93");  break; } case "balota":    { player.SetPosition("4546.92 2.7 2416.4");     break; } case "vybor":     { player.SetPosition("3916.85 311.7 8795.59");  break; } case "vybora":    { player.SetPosition("4107.80 342.7 11205.29"); break; } case "severo":    { player.SetPosition("8318.51 109.7 12743.4");  break; } case "krasno":    { player.SetPosition("11172.0 198.7 12314.1");  break; } default: { SendMessageToPlayer(player, "[TP Locations] " + tokens[1] + " not found"); break; } } break; }
//Punishment // ---------------------------------------------------------------------------------------------------------
			case "killall":   { SendMessageToPlayer(player, "You killed everyone"); for (i = 0; i < players.Count(); i++) { temp_player = players.Get(i); if(temp_player.GetIdentity().GetId() == player.GetIdentity().GetId()) continue; temp_player.SetHealth(0); } break; }
			case "stripall":  { SendMessageToPlayer(player, "You strip everyone");  for (i = 0; i < players.Count(); i++) { temp_player = players.Get(i); if(temp_player.GetIdentity().GetId() == player.GetIdentity().GetId()) continue; temp_player.RemoveAllItems(); SendMessageToPlayer(temp_player, "You have been stripped by an admin"); } break; }	
			case "kill":      { if(count == 2) { temp_player = GetPlayer(tokens[1]); if(temp_player == NULL) { SendMessageToPlayer(player, "Can't find player called: '"+tokens[1]+"'"); } else { temp_player.SetHealth(0); SendMessageToPlayer(player, "You killed " + temp_player.GetIdentity().GetName()); } } else { player.SetHealth(0); SendMessageToPlayer(player, "You killed yourself"); } break; }
			case "freeze":    { if(count == 2) { temp_player = GetPlayer(tokens[1]); if(temp_player == NULL) { SendMessageToPlayer(player, "Can't find player called: '"+tokens[1]+"'"); } } else { temp_player = player; } if(temp_player != NULL) { if (m_FreezePlayer) { SendMessageToPlayer(player, "You have released " + temp_player.GetIdentity().GetName()); SendMessageToPlayer(temp_player, "You are released"); temp_player.GetInputController().OverrideMovementSpeed( false, 0 ); temp_player.GetInputController().OverrideMovementAngle( false, 0 ); temp_player.GetInputController().OverrideAimChangeX( false, 0 ); temp_player.GetInputController().OverrideAimChangeY( false, 0 ); m_FreezePlayer = false; } else { SendMessageToPlayer(player, "You freeze " + temp_player.GetIdentity().GetName()); SendMessageToPlayer(temp_player, "You have been freeze by an admin"); temp_player.GetInputController().OverrideMovementSpeed( true, 0 ); temp_player.GetInputController().OverrideMovementAngle( true, 0 ); temp_player.GetInputController().OverrideAimChangeX( true, 0 ); temp_player.GetInputController().OverrideAimChangeY( true, 0 ); m_FreezePlayer = true; } } break; }
			case "strip":     { if(count == 2) { temp_player = GetPlayer(tokens[1]); if(temp_player == NULL) { SendMessageToPlayer(player, "Can't find player called: '"+tokens[1]+"'"); } else { SendMessageToPlayer(player, "You stripped "                            + temp_player.GetIdentity().GetName()); } } else { temp_player = player; } if(temp_player != NULL) { temp_player.RemoveAllItems(); SendMessageToPlayer(temp_player, "You have been stripped by an admin"); } break; }	
			case "shove":     { if(count == 2) { temp_player = GetPlayer(tokens[1]); if(temp_player == NULL) { SendMessageToPlayer(player, "Can't find player called: '"+tokens[1]+"'"); } else { SendMessageToPlayer(player, "You shove "                               + temp_player.GetIdentity().GetName()); } } else { temp_player = player; } if(temp_player != NULL) { temp_player.SetPosition(temp_player.GetPosition() + "1 3 0"); SendMessageToPlayer(temp_player, "You have been shove by an admin"); } break; }
			case "jail":      { if(count == 2) { temp_player = GetPlayer(tokens[1]); if(temp_player == NULL) { SendMessageToPlayer(player, "Can't find player called: '"+tokens[1]+"'"); } else { SendMessageToPlayer(player, "You punished "                            + temp_player.GetIdentity().GetName()); } } else { temp_player = player; } if(temp_player != NULL) { temp_player.RemoveAllItems(); temp_player.SetHealth(60); temp_player.GetStatEnergy().Set(900); temp_player.GetStatWater().Set(800); temp_player.GetInventory().CreateInInventory("PrisonerCap"); temp_player.GetInventory().CreateInInventory("PrisonUniformJacket"); temp_player.GetInventory().CreateInInventory("PrisonUniformPants"); temp_player.GetInventory().CreateInInventory("WorkingBoots_Grey"); temp_player.SetPosition("2763.1 26.7 1307.1"); SendMessageToPlayer(temp_player, "You have been jail by an admin"); } break; }
// Weapons  // ---------------------------------------------------------------------------------------------------------
			case "m4":        { if(count == 2) { temp_player = GetPlayer(tokens[1]); if(temp_player == NULL) { SendMessageToPlayer(player, "Can't find player called: '"+tokens[1]+"'"); } else { SendMessageToPlayer(player, "You Give M4A1 to "                        + temp_player.GetIdentity().GetName()); } } else { temp_player = player; } if(temp_player != NULL) { EntityAI wm4a1; wm4a1 = GetGame().CreateObject( "M4A1_Black", temp_player.GetPosition(), false, true, true ); wm4a1.GetInventory().CreateAttachment("M4_Suppressor"); wm4a1.GetInventory().CreateAttachment("M4_MPBttstck_Black"); wm4a1.GetInventory().CreateAttachment("M4_MPHndgrd_Black"); wm4a1.GetInventory().CreateAttachment("M9A1_Bayonet"); wm4a1.GetInventory().CreateAttachment("ACOGOptic"); GetGame().CreateObject( "Mag_STANAGCoupled_30Rnd", temp_player.GetPosition(), false, true, true ); } break; }		
			case "akm":       { if(count == 2) { temp_player = GetPlayer(tokens[1]); if(temp_player == NULL) { SendMessageToPlayer(player, "Can't find player called: '"+tokens[1]+"'"); } else { SendMessageToPlayer(player, "You Give AKM to "                         + temp_player.GetIdentity().GetName()); } } else { temp_player = player; } if(temp_player != NULL) { EntityAI wakm; wakm = GetGame().CreateObject( "AKM", temp_player.GetPosition(), false, true, true ); wakm.GetInventory().CreateAttachment("AK_Suppressor"); wakm.GetInventory().CreateAttachment("AK_FoldingBttstck_Black"); wakm.GetInventory().CreateAttachment("AK_RailHndgrd_Black"); wakm.GetInventory().CreateAttachment("AK_Bayonet"); wakm.GetInventory().CreateAttachment("PSO11Optic"); GetGame().CreateObject( "Mag_AKM_Drum75Rnd_Black", temp_player.GetPosition(), false, true, true ); } break; }		
			case "mp5":       { if(count == 2) { temp_player = GetPlayer(tokens[1]); if(temp_player == NULL) { SendMessageToPlayer(player, "Can't find player called: '"+tokens[1]+"'"); } else { SendMessageToPlayer(player, "You Give MP5K to "                        + temp_player.GetIdentity().GetName()); } } else { temp_player = player; } if(temp_player != NULL) { EntityAI wmp5; wmp5 = GetGame().CreateObject( "MP5K", temp_player.GetPosition(), false, true, true ); wmp5.GetInventory().CreateAttachment("MP5_Compensator"); wmp5.GetInventory().CreateAttachment("MP5k_StockBttstck"); wmp5.GetInventory().CreateAttachment("MP5_PlasticHndgrd"); wmp5.GetInventory().CreateAttachment("ACOGOptic"); GetGame().CreateObject( "Mag_MP5_30Rnd", temp_player.GetPosition(), false, true, true ); } break; }						
			case "fnx":       { if(count == 2) { temp_player = GetPlayer(tokens[1]); if(temp_player == NULL) { SendMessageToPlayer(player, "Can't find player called: '"+tokens[1]+"'"); } else { SendMessageToPlayer(player, "You Give FNX45 to "                       + temp_player.GetIdentity().GetName()); } } else { temp_player = player; } if(temp_player != NULL) { EntityAI wfnx; wfnx = GetGame().CreateObject( "FNX45", temp_player.GetPosition(), false, true, true ); wfnx.GetInventory().CreateAttachment("PistolSuppressor"); wfnx.GetInventory().CreateAttachment("FNP45_MRDSOptic"); wfnx.GetInventory().CreateAttachment("UniversalLight"); GetGame().CreateObject( "Mag_FNX45_15Rnd", temp_player.GetPosition(), false, true, true ); } break; }	
			case "mosin":     { if(count == 2) { temp_player = GetPlayer(tokens[1]); if(temp_player == NULL) { SendMessageToPlayer(player, "Can't find player called: '"+tokens[1]+"'"); } else { SendMessageToPlayer(player, "You Give Mosin9130 to "                   + temp_player.GetIdentity().GetName()); } } else { temp_player = player; } if(temp_player != NULL) { EntityAI wmosin; wmosin = GetGame().CreateObject( "Mosin9130_Camo", temp_player.GetPosition(), false, true, true ); wmosin.GetInventory().CreateAttachment("Mosin_Bayonet"); wmosin.GetInventory().CreateAttachment("Mosin_Compensator"); wmosin.GetInventory().CreateAttachment("PUScopeOptic"); wmosin.GetInventory().CreateAttachment("GhillieAtt_Woodland"); GetGame().CreateObject( "Ammo_762x54", temp_player.GetPosition(), false, true, true ); } break; }			
			case "sniper":    { if(count == 2) { temp_player = GetPlayer(tokens[1]); if(temp_player == NULL) { SendMessageToPlayer(player, "Can't find player called: '"+tokens[1]+"'"); } else { SendMessageToPlayer(player, "You Give a complete sniper equipment to " + temp_player.GetIdentity().GetName()); } } else { temp_player = player; } if(temp_player != NULL) { temp_player.RemoveAllItems(); temp_player.GetStatWater().Set(1000); temp_player.GetInventory().CreateInInventory("Balaclava3Holes_Green"); temp_player.GetInventory().CreateInInventory("TacticalGloves_Green"); temp_player.GetInventory().CreateInInventory("TTsKOJacket_Camo"); temp_player.GetInventory().CreateInInventory("HighCapacityVest_Olive"); temp_player.GetInventory().CreateInInventory("TTSKOPants"); EntityAI mbs = temp_player.GetInventory().CreateInInventory("MilitaryBoots_Brown"); mbs.GetInventory().CreateAttachment("CombatKnife"); temp_player.GetInventory().CreateInInventory("GhillieHood_Mossy"); temp_player.GetInventory().CreateInInventory("GhillieSuit_Mossy"); EntityAI Smapct = temp_player.GetInventory().CreateInInventory("SmallProtectorCase"); Smapct.GetInventory().CreateInInventory("Lockpick"); Smapct.GetInventory().CreateInInventory("ChernarusMap"); Smapct.GetInventory().CreateInInventory("DuctTape"); Smapct.GetInventory().CreateInInventory("Roadflare"); Smapct.GetInventory().CreateInInventory("Rope"); Smapct.GetInventory().CreateInInventory("WaterBottle"); Smapct.GetInventory().CreateInInventory("Battery9V"); Smapct.GetInventory().CreateInInventory("Compass"); Smapct.GetInventory().CreateInInventory("Matchbox"); EntityAI rds = temp_player.GetInventory().CreateInInventory("PersonalRadio"); rds.GetInventory().CreateAttachment("Battery9V"); EntityAI boxtf = temp_player.GetInventory().CreateInInventory("AmmoBox"); EntityAI ttaksa = boxtf.GetInventory().CreateInInventory("FNX45"); ttaksa.GetInventory().CreateAttachment("PistolSuppressor"); ttaksa.GetInventory().CreateAttachment("FNP45_MRDSOptic"); ttaksa.GetInventory().CreateAttachment("UniversalLight"); boxtf.GetInventory().CreateInInventory("AmmoBox_45ACP_25rnd"); boxtf.GetInventory().CreateInInventory("AmmoBox_45ACP_25rnd"); boxtf.GetInventory().CreateInInventory("AmmoBox_762x54_20Rnd"); boxtf.GetInventory().CreateInInventory("AmmoBox_762x54_20Rnd"); boxtf.GetInventory().CreateInInventory("Mag_FNX45_15Rnd"); boxtf.GetInventory().CreateInInventory("Mag_FNX45_15Rnd"); boxtf.GetInventory().CreateInInventory("Mag_SVD_10Rnd"); boxtf.GetInventory().CreateInInventory("Mag_SVD_10Rnd"); boxtf.GetInventory().CreateInInventory("Mag_SVD_10Rnd"); boxtf.GetInventory().CreateInInventory("Mag_SVD_10Rnd"); temp_player.GetInventory().CreateInInventory("Binoculars"); temp_player.GetInventory().CreateInInventory("Cannabis"); temp_player.GetInventory().CreateInInventory("Cannabis"); temp_player.GetInventory().CreateInInventory("Cannabis"); EntityAI boxms = temp_player.GetInventory().CreateInInventory("FirstAidKit"); boxms.GetInventory().CreateInInventory("SalineBagIV"); boxms.GetInventory().CreateInInventory("Morphine"); boxms.GetInventory().CreateInInventory("Epinephrine"); boxms.GetInventory().CreateInInventory("BandageDressing"); boxms.GetInventory().CreateInInventory("BandageDressing"); boxms.GetInventory().CreateInInventory("BandageDressing"); boxms.GetInventory().CreateInInventory("BandageDressing"); temp_player.GetInventory().CreateInInventory("Chemlight_Green"); temp_player.GetInventory().CreateInInventory("SodaCan_Cola"); temp_player.GetInventory().CreateInInventory("SodaCan_Cola"); temp_player.GetInventory().CreateInInventory("SardinesCan_Opened"); EntityAI aSVD = temp_player.GetHumanInventory().CreateInInventory("SVD"); aSVD.GetInventory().CreateAttachment("GhillieAtt_Woodland"); aSVD.GetInventory().CreateAttachment("PSO1Optic"); } break; }			
			case "weapons":   { if(count == 2) { temp_player = GetPlayer(tokens[1]); if(temp_player == NULL) { SendMessageToPlayer(player, "Can't find player called: '"+tokens[1]+"'"); } else { SendMessageToPlayer(player, "You Give all weapons to "                 + temp_player.GetIdentity().GetName()); } } else { temp_player = player; } if(temp_player != NULL) { EntityAI wm4a1all; wm4a1all = GetGame().CreateObject( "M4A1_Black", temp_player.GetPosition()- "1 0 0", false, true ); wm4a1all.GetInventory().CreateAttachment("M4_Suppressor"); wm4a1all.GetInventory().CreateAttachment("M4_MPBttstck_Black"); wm4a1all.GetInventory().CreateAttachment("M4_MPHndgrd_Black"); wm4a1all.GetInventory().CreateAttachment("M9A1_Bayonet"); wm4a1all.GetInventory().CreateAttachment("ACOGOptic"); GetGame().CreateObject( "Mag_STANAGCoupled_30Rnd", temp_player.GetPosition()- "1 0 0", false, true ); EntityAI wakmall; wakmall = GetGame().CreateObject( "AKM", temp_player.GetPosition()- "0 0 1", false, true ); wakmall.GetInventory().CreateAttachment("AK_Suppressor"); wakmall.GetInventory().CreateAttachment("AK_FoldingBttstck_Black"); wakmall.GetInventory().CreateAttachment("AK_RailHndgrd_Black"); wakmall.GetInventory().CreateAttachment("AK_Bayonet"); wakmall.GetInventory().CreateAttachment("PSO11Optic"); GetGame().CreateObject( "Mag_AKM_Drum75Rnd_Black", temp_player.GetPosition()- "0 0 1", false, true ); EntityAI wmp5all; wmp5all = GetGame().CreateObject( "MP5K", temp_player.GetPosition()- "0 0 -1", false, true ); wmp5all.GetInventory().CreateAttachment("MP5_Compensator"); wmp5all.GetInventory().CreateAttachment("MP5k_StockBttstck"); wmp5all.GetInventory().CreateAttachment("MP5_PlasticHndgrd"); wmp5all.GetInventory().CreateAttachment("ACOGOptic"); GetGame().CreateObject( "Mag_MP5_30Rnd", temp_player.GetPosition()- "0 0 -1", false, true ); EntityAI wfnxall; wfnxall = GetGame().CreateObject( "FNX45", temp_player.GetPosition()- "-1 0 0", false, true ); wfnxall.GetInventory().CreateAttachment("PistolSuppressor"); wfnxall.GetInventory().CreateAttachment("FNP45_MRDSOptic"); wfnxall.GetInventory().CreateAttachment("UniversalLight"); GetGame().CreateObject( "Mag_FNX45_15Rnd", temp_player.GetPosition()- "-1 0 0", false, true ); EntityAI wmosinall; wmosinall = GetGame().CreateObject( "Mosin9130_Camo", temp_player.GetPosition()- "2 0 0", false, true ); wmosinall.GetInventory().CreateAttachment("Mosin_Bayonet"); wmosinall.GetInventory().CreateAttachment("Mosin_Compensator"); wmosinall.GetInventory().CreateAttachment("PUScopeOptic"); wmosinall.GetInventory().CreateAttachment("GhillieAtt_Woodland"); GetGame().CreateObject( "Ammo_762x54", temp_player.GetPosition()- "2 0 0", false, true ); } break; }			
// Entities // ---------------------------------------------------------------------------------------------------------
			case "canis":     { if(count == 2) { temp_player = GetPlayer(tokens[1]); if(temp_player == NULL) { SendMessageToPlayer(player, "Can't find player called: '"+tokens[1]+"'"); } else { SendMessageToPlayer(player, "You have spawned Canis lupus on "         + temp_player.GetIdentity().GetName()); } } else { temp_player = player; } if(temp_player != NULL) { vector base_pos = temp_player.GetPosition(), dynamic_pos; for(int cani = 0; cani < Spawned_Canis; cani++) { float a = Math.RandomFloat(0.4, 1.0) * 2 * Math.PI; float r = 50.0 * Math.Sqrt(Math.RandomFloat(0.4, 1.0)); dynamic_pos = base_pos; dynamic_pos[0] = dynamic_pos[0]+(r * Math.Cos(a)); dynamic_pos[2] = dynamic_pos[2]+(r * Math.Sin(a)); dynamic_pos[1] = GetGame().SurfaceY(dynamic_pos[0], dynamic_pos[2]) + 0.3; string item = GetRandomCanis(); GetGame().CreateObject(item, dynamic_pos, false, true); } } break; }						
			case "animals":   { if(count == 2) { temp_player = GetPlayer(tokens[1]); if(temp_player == NULL) { SendMessageToPlayer(player, "Can't find player called: '"+tokens[1]+"'"); } else { SendMessageToPlayer(player, "You have spawned animals on "             + temp_player.GetIdentity().GetName()); } } else { temp_player = player; } if(temp_player != NULL) { vector anibase_pos = temp_player.GetPosition(), anidynamic_pos; for(int ani = 0; ani < Spawned_Animals; ani++) { float an = Math.RandomFloat(0.4, 1.0) * 2 * Math.PI; float rn = 50.0 * Math.Sqrt(Math.RandomFloat(0.4, 1.0)); anidynamic_pos = anibase_pos; anidynamic_pos[0] = anidynamic_pos[0]+(rn * Math.Cos(an)); anidynamic_pos[2] = anidynamic_pos[2]+(rn * Math.Sin(an)); anidynamic_pos[1] = GetGame().SurfaceY(anidynamic_pos[0], anidynamic_pos[2]) + 0.3; string anitem = GetRandomAnimals(); GetGame().CreateObject(anitem, anidynamic_pos, false, true); } } break; }						
			case "horde":     { if(count == 2) { temp_player = GetPlayer(tokens[1]); if(temp_player == NULL) { SendMessageToPlayer(player, "Can't find player called: '"+tokens[1]+"'"); } else { SendMessageToPlayer(player, "You have spawned horde on "               + temp_player.GetIdentity().GetName()); } } else { temp_player = player; } if(temp_player != NULL) { vector horde_pos = temp_player.GetPosition(), hdynamic_pos; for(int hor = 0; hor < Spawned_Horde; hor++) { float ha = Math.RandomFloat(0.4, 1.0) * 2 * Math.PI; float hr = 50.0 * Math.Sqrt(Math.RandomFloat(0.4, 1.0)); hdynamic_pos = horde_pos; hdynamic_pos[0] = hdynamic_pos[0]+(hr * Math.Cos(ha)); hdynamic_pos[2] = hdynamic_pos[2]+(hr * Math.Sin(ha)); hdynamic_pos[1] = GetGame().SurfaceY(hdynamic_pos[0], hdynamic_pos[2]) + 0.3; string horde = GetRandomHorde(); GetGame().CreateObject(horde, hdynamic_pos, false, true); } } break; }					
			case "dean":      { if(count == 2) { temp_player = GetPlayer(tokens[1]); if(temp_player == NULL) { SendMessageToPlayer(player, "Can't find player called: '"+tokens[1]+"'"); } else { SendMessageToPlayer(player, "You spawn Dean Hall to "                  + temp_player.GetIdentity().GetName()); } } else { temp_player = player; } if(temp_player != NULL) { EntityAI surv; surv = GetGame().CreateObject( "ZmbM_SurvivorDean_Grey", temp_player.GetPosition()- "10 0 0", false, true ); surv.GetInventory().CreateInInventory("ImprovisedBag"); surv.GetInventory().CreateInInventory("ChernarusMap"); surv.GetInventory().CreateInInventory("Compass"); EntityAI survfnx = surv.GetInventory().CreateInInventory("FNX45"); survfnx.GetInventory().CreateAttachment("Mag_FNX45_15Rnd"); } break; }
// Vehicles // ---------------------------------------------------------------------------------------------------------
			case "offroad":   { if(count == 2) { temp_player = GetPlayer(tokens[1]); if(temp_player == NULL) { SendMessageToPlayer(player, "Can't find player called: '"+tokens[1]+"'"); } else { SendMessageToPlayer(player, "You Give offroad to "                     + temp_player.GetIdentity().GetName()); } } else { temp_player = player; } if(temp_player != NULL) { EntityAI offroadt; offroadt = GetGame().CreateObject( "OffroadHatchback", temp_player.GetPosition()- "5 0 0", false, true ); offroadt.GetInventory().CreateAttachment("HatchbackWheel"); offroadt.GetInventory().CreateAttachment("HatchbackWheel"); offroadt.GetInventory().CreateAttachment("HatchbackWheel"); offroadt.GetInventory().CreateAttachment("HatchbackWheel"); offroadt.GetInventory().CreateAttachment("HatchbackWheel"); offroadt.GetInventory().CreateAttachment("HatchbackDoors_Driver"); offroadt.GetInventory().CreateAttachment("HatchbackDoors_CoDriver"); offroadt.GetInventory().CreateAttachment("HatchbackHood"); offroadt.GetInventory().CreateAttachment("HatchbackTrunk"); offroadt.GetInventory().CreateAttachment("SparkPlug"); offroadt.GetInventory().CreateAttachment("EngineBelt"); offroadt.GetInventory().CreateAttachment("CarRadiator"); offroadt.GetInventory().CreateAttachment("CarBattery"); offroadt.GetInventory().CreateAttachment("HeadlightH7"); offroadt.GetInventory().CreateAttachment("HeadlightH7"); auto carfluids = Car.Cast( offroadt ); carfluids.Fill( CarFluid.FUEL, 1000 ); carfluids.Fill( CarFluid.OIL, 1000 ); carfluids.Fill( CarFluid.BRAKE, 1000 ); carfluids.Fill( CarFluid.COOLANT, 1000 ); } break; }			
//			case "v3s":       { if(count == 2) { temp_player = GetPlayer(tokens[1]); if(temp_player == NULL) { SendMessageToPlayer(player, "Can't find player called: '"+tokens[1]+"'"); } else { SendMessageToPlayer(player, "You Give V3S Chassis to "                 + temp_player.GetIdentity().GetName()); } } else { temp_player = player; } if(temp_player != NULL) { EntityAI V3Scht; V3Scht = GetGame().CreateObject( "V3S_Chassis", temp_player.GetPosition()- "5 0 0", false, true ); V3Scht.GetInventory().CreateAttachment("V3SWheel"); V3Scht.GetInventory().CreateAttachment("V3SWheel"); V3Scht.GetInventory().CreateAttachment("V3SWheel"); V3Scht.GetInventory().CreateAttachment("V3SWheel"); V3Scht.GetInventory().CreateAttachment("V3SWheelDouble"); V3Scht.GetInventory().CreateAttachment("V3SWheelDouble"); V3Scht.GetInventory().CreateAttachment("V3SWheelDouble"); V3Scht.GetInventory().CreateAttachment("V3SWheelDouble"); V3Scht.GetInventory().CreateAttachment("V3SDoors_Driver"); V3Scht.GetInventory().CreateAttachment("V3SHood"); V3Scht.GetInventory().CreateAttachment("GlowPlug"); V3Scht.GetInventory().CreateAttachment("TruckRadiator"); V3Scht.GetInventory().CreateAttachment("TruckExhaust"); V3Scht.GetInventory().CreateAttachment("TruckBattery"); V3Scht.GetInventory().CreateAttachment("HeadlightH7"); auto carfluidsV3Scht = Car.Cast( V3Scht ); carfluidsV3Scht.Fill( CarFluid.FUEL, 1000 ); carfluidsV3Scht.Fill( CarFluid.OIL, 1000 ); carfluidsV3Scht.Fill( CarFluid.BRAKE, 1000 ); carfluidsV3Scht.Fill( CarFluid.COOLANT, 1000 ); } break; }
//			case "v3sc":      { if(count == 2) { temp_player = GetPlayer(tokens[1]); if(temp_player == NULL) { SendMessageToPlayer(player, "Can't find player called: '"+tokens[1]+"'"); } else { SendMessageToPlayer(player, "You Give V3S Cargo to "                   + temp_player.GetIdentity().GetName()); } } else { temp_player = player; } if(temp_player != NULL) { EntityAI v3sct; v3sct = GetGame().CreateObject( "V3S_Cargo", temp_player.GetPosition()- "5 0 0", false, true ); v3sct.GetInventory().CreateAttachment("V3SWheel"); v3sct.GetInventory().CreateAttachment("V3SWheel"); v3sct.GetInventory().CreateAttachment("V3SWheel"); v3sct.GetInventory().CreateAttachment("V3SWheel"); v3sct.GetInventory().CreateAttachment("V3SWheelDouble"); v3sct.GetInventory().CreateAttachment("V3SWheelDouble"); v3sct.GetInventory().CreateAttachment("V3SWheelDouble"); v3sct.GetInventory().CreateAttachment("V3SWheelDouble"); v3sct.GetInventory().CreateAttachment("V3SDoors_Driver"); v3sct.GetInventory().CreateAttachment("V3SHood"); v3sct.GetInventory().CreateAttachment("GlowPlug"); v3sct.GetInventory().CreateAttachment("TruckRadiator"); v3sct.GetInventory().CreateAttachment("TruckExhaust"); v3sct.GetInventory().CreateAttachment("TruckBattery"); v3sct.GetInventory().CreateAttachment("HeadlightH7"); auto carfluidsv3sct = Car.Cast( v3sct ); carfluidsv3sct.Fill( CarFluid.FUEL, 1000 ); carfluidsv3sct.Fill( CarFluid.OIL, 1000 ); carfluidsv3sct.Fill( CarFluid.BRAKE, 1000 ); carfluidsv3sct.Fill( CarFluid.COOLANT, 1000 ); } break; }			
//			case "sedan":     { if(count == 2) { temp_player = GetPlayer(tokens[1]); if(temp_player == NULL) { SendMessageToPlayer(player, "Can't find player called: '"+tokens[1]+"'"); } else { SendMessageToPlayer(player, "You Give Sedan to "                       + temp_player.GetIdentity().GetName()); } } else { temp_player = player; } if(temp_player != NULL) { EntityAI sedant; sedant = GetGame().CreateObject( "CivilianSedan", temp_player.GetPosition()- "5 0 0", false, true ); sedant.GetInventory().CreateAttachment("CivSedanWheel"); sedant.GetInventory().CreateAttachment("CivSedanWheel"); sedant.GetInventory().CreateAttachment("CivSedanWheel"); sedant.GetInventory().CreateAttachment("CivSedanWheel"); sedant.GetInventory().CreateAttachment("CivSedanWheel"); sedant.GetInventory().CreateAttachment("CivSedanDoors_Driver"); sedant.GetInventory().CreateAttachment("CivSedanDoors_CoDriver"); sedant.GetInventory().CreateAttachment("CivSedanDoors_BackLeft"); sedant.GetInventory().CreateAttachment("CivSedanDoors_BackRight"); sedant.GetInventory().CreateAttachment("CivSedanTrunk"); sedant.GetInventory().CreateAttachment("CivSedanHood"); sedant.GetInventory().CreateAttachment("TruckRadiator"); sedant.GetInventory().CreateAttachment("SparkPlug"); sedant.GetInventory().CreateAttachment("EngineBelt"); sedant.GetInventory().CreateAttachment("HeadlightH7"); sedant.GetInventory().CreateAttachment("CarRadiator"); sedant.GetInventory().CreateAttachment("CarBattery"); sedant.GetInventory().CreateAttachment("HeadlightH7"); auto carfluidssedant = Car.Cast( sedant ); carfluidssedant.Fill( CarFluid.FUEL, 1000 ); carfluidssedant.Fill( CarFluid.OIL, 1000 ); carfluidssedant.Fill( CarFluid.BRAKE, 1000 ); carfluidssedant.Fill( CarFluid.COOLANT, 1000 ); } break; }
			case "refuel":    { if(count == 2) { temp_player = GetPlayer(tokens[1]); if(temp_player == NULL) { SendMessageToPlayer(player, "Can't find player called: '"+tokens[1]+"'"); } else { SendMessageToPlayer(player, "You Refuel to "                           + temp_player.GetIdentity().GetName()); } } else { temp_player = player; } if(temp_player != NULL) { ref array<Object> nearest_objects = new array<Object>; ref array<CargoBase> proxy_cargos = new array<CargoBase>; Car toBeFilled; vector position = temp_player.GetPosition(); GetGame().GetObjectsAtPosition ( position, 10, nearest_objects, proxy_cargos ); for (i = 0; i < nearest_objects.Count(); i++) { if (nearest_objects[i].IsKindOf("CarScript")) { SendMessageToPlayer(temp_player, "[Refuel] Found car: '"+nearest_objects[i]+"'"); toBeFilled = Car.Cast(nearest_objects[i]); float fuelReq = toBeFilled.GetFluidCapacity( CarFluid.FUEL ) - (toBeFilled.GetFluidCapacity( CarFluid.FUEL ) * toBeFilled.GetFluidFraction( CarFluid.FUEL )); float oilReq = toBeFilled.GetFluidCapacity( CarFluid.OIL ) - (toBeFilled.GetFluidCapacity( CarFluid.OIL ) * toBeFilled.GetFluidFraction( CarFluid.OIL )); float coolantReq = toBeFilled.GetFluidCapacity( CarFluid.COOLANT ) - (toBeFilled.GetFluidCapacity( CarFluid.COOLANT ) * toBeFilled.GetFluidFraction( CarFluid.COOLANT )); float brakeReq = toBeFilled.GetFluidCapacity( CarFluid.BRAKE ) - (toBeFilled.GetFluidCapacity( CarFluid.BRAKE ) * toBeFilled.GetFluidFraction( CarFluid.BRAKE )); toBeFilled.Fill( CarFluid.FUEL, fuelReq ); toBeFilled.Fill( CarFluid.OIL, oilReq ); toBeFilled.Fill( CarFluid.COOLANT, coolantReq ); toBeFilled.Fill( CarFluid.BRAKE, brakeReq ); SendMessageToPlayer(temp_player, "[Refuel] "+fuelReq+"L added, all fluids maxed"); } } } break; }			
// Test Dev // ---------------------------------------------------------------------------------------------------------

			case "builder":   { PlayerBase builder_target; if(count == 2) { builder_target = GetPlayer(tokens[1]); if(builder_target == NULL) { SendMessageToPlayer(player, "Can't find player called: '"+tokens[1]+"'"); } else { SendMessageToPlayer(player, "You Give stuff builder to " + builder_target.GetIdentity().GetName()); } } else { builder_target = player; SendMessageToPlayer(player, "You Give stuff builder"); } if(builder_target != NULL) { builder_target.RemoveAllItems(); builder_target.GetStatWater().Set(1000); builder_target.GetInventory().CreateInInventory("ConstructionHelmet_White"); builder_target.GetInventory().CreateInInventory("WorkingGloves_Beige"); builder_target.GetInventory().CreateInInventory("TShirt_White"); builder_target.GetInventory().CreateInInventory("WorkingBoots_Beige");
					EntityAI CargoPantst = builder_target.GetInventory().CreateInInventory("CargoPants_Beige"); CargoPantst.GetInventory().CreateInInventory("CombinationLock"); CargoPantst.GetInventory().CreateInInventory("Pliers"); CargoPantst.GetInventory().CreateInInventory("FenceKit"); CargoPantst.GetInventory().CreateInInventory("WatchtowerKit"); CargoPantst.GetInventory().CreateInInventory("MetalWire"); CargoPantst.GetInventory().CreateInInventory("Lockpick"); CargoPantst.GetInventory().CreateInInventory("XmasLights"); CargoPantst.GetInventory().CreateInInventory("XmasLights"); EntityAI buildert = builder_target.GetInventory().CreateInInventory("MountainBag_Green"); buildert.GetInventory().CreateInInventory("Spotlight"); buildert.GetInventory().CreateInInventory("CableReel"); buildert.GetInventory().CreateInInventory("Hatchet"); buildert.GetInventory().CreateInInventory("Rope"); buildert.GetInventory().CreateInInventory("BarbedWire"); buildert.GetInventory().CreateInInventory("Hammer"); buildert.GetInventory().CreateInInventory("EpoxyPutty"); buildert.GetInventory().CreateInInventory("Hacksaw"); buildert.GetInventory().CreateInInventory("MetalPlate"); buildert.GetInventory().CreateInInventory("MetalPlate"); buildert.GetInventory().CreateInInventory("MetalPlate"); buildert.GetInventory().CreateInInventory("MetalPlate"); buildert.GetInventory().CreateInInventory("Camonet"); buildert.GetInventory().CreateInInventory("Camonet"); buildert.GetInventory().CreateInInventory("Camonet"); buildert.GetInventory().CreateInInventory("Camonet"); buildert.GetInventory().CreateInInventory("NailBox"); buildert.GetInventory().CreateInInventory("NailBox"); buildert.GetInventory().CreateInInventory("NailBox"); buildert.GetInventory().CreateInInventory("NailBox"); builder_target.GetInventory().CreateInInventory("Shovel"); builder_target.GetHumanInventory().CreateInHands("LargeTent");
					EntityAI offroadtbuilder; offroadtbuilder = GetGame().CreateObject( "OffroadHatchback", builder_target.GetPosition()- "5 0 0", false, true ); offroadtbuilder.GetInventory().CreateAttachment("HatchbackWheel"); offroadtbuilder.GetInventory().CreateAttachment("HatchbackWheel"); offroadtbuilder.GetInventory().CreateAttachment("HatchbackWheel"); offroadtbuilder.GetInventory().CreateAttachment("HatchbackWheel"); offroadtbuilder.GetInventory().CreateAttachment("HatchbackWheel"); offroadtbuilder.GetInventory().CreateAttachment("HatchbackDoors_Driver"); offroadtbuilder.GetInventory().CreateAttachment("HatchbackDoors_CoDriver"); offroadtbuilder.GetInventory().CreateAttachment("HatchbackHood"); offroadtbuilder.GetInventory().CreateAttachment("HatchbackTrunk"); offroadtbuilder.GetInventory().CreateAttachment("SparkPlug"); offroadtbuilder.GetInventory().CreateAttachment("EngineBelt"); offroadtbuilder.GetInventory().CreateAttachment("CarRadiator"); offroadtbuilder.GetInventory().CreateAttachment("CarBattery"); offroadtbuilder.GetInventory().CreateAttachment("HeadlightH7"); offroadtbuilder.GetInventory().CreateAttachment("HeadlightH7"); auto carfluidsbuilder = Car.Cast( offroadtbuilder ); carfluidsbuilder.Fill( CarFluid.FUEL, 1000 ); carfluidsbuilder.Fill( CarFluid.OIL, 1000 ); carfluidsbuilder.Fill( CarFluid.BRAKE, 1000 ); carfluidsbuilder.Fill( CarFluid.COOLANT, 1000 );	
                    EntityAI Generatort = offroadtbuilder.GetInventory().CreateInInventory("PowerGenerator"); Generatort.GetInventory().CreateAttachment("SparkPlug"); auto carfluidsGeneratort = Car.Cast( Generatort ); carfluidsGeneratort.Fill( CarFluid.FUEL, 1000 ); offroadtbuilder.GetInventory().CreateInInventory("Spotlight"); offroadtbuilder.GetInventory().CreateInInventory("MediumTent"); offroadtbuilder.GetInventory().CreateInInventory("Camonet"); offroadtbuilder.GetInventory().CreateInInventory("Camonet"); offroadtbuilder.GetInventory().CreateInInventory("Camonet"); offroadtbuilder.GetInventory().CreateInInventory("Camonet"); offroadtbuilder.GetInventory().CreateInInventory("Camonet"); offroadtbuilder.GetInventory().CreateInInventory("XmasLights"); offroadtbuilder.GetInventory().CreateInInventory("XmasLights");	ItemBase Plank0 = offroadtbuilder.GetInventory().CreateInInventory("WoodenPlank"); Plank0.SetQuantity(20); ItemBase Plank1 = offroadtbuilder.GetInventory().CreateInInventory("WoodenPlank"); Plank1.SetQuantity(20); offroadtbuilder.GetInventory().CreateInInventory("WoodenLog"); offroadtbuilder.GetInventory().CreateInInventory("WoodenLog"); offroadtbuilder.GetInventory().CreateInInventory("WoodenLog"); offroadtbuilder.GetInventory().CreateInInventory("WoodenLog"); offroadtbuilder.GetInventory().CreateInInventory("CableReel"); offroadtbuilder.GetInventory().CreateInInventory("NailBox"); offroadtbuilder.GetInventory().CreateInInventory("NailBox"); offroadtbuilder.GetInventory().CreateInInventory("NailBox"); offroadtbuilder.GetInventory().CreateInInventory("NailBox"); offroadtbuilder.GetInventory().CreateInInventory("Hatchet"); offroadtbuilder.GetInventory().CreateInInventory("Hammer"); offroadtbuilder.GetInventory().CreateInInventory("FenceKit"); offroadtbuilder.GetInventory().CreateInInventory("WatchtowerKit"); offroadtbuilder.GetInventory().CreateInInventory("Rope"); offroadtbuilder.GetInventory().CreateInInventory("BarbedWire"); offroadtbuilder.GetInventory().CreateInInventory("NailBox"); offroadtbuilder.GetInventory().CreateInInventory("NailBox"); offroadtbuilder.GetInventory().CreateInInventory("NailBox"); offroadtbuilder.GetInventory().CreateInInventory("NailBox"); offroadtbuilder.GetInventory().CreateInInventory("NailBox"); offroadtbuilder.GetInventory().CreateInInventory("NailBox"); offroadtbuilder.GetInventory().CreateInInventory("NailBox"); offroadtbuilder.GetInventory().CreateInInventory("NailBox"); } break; }
            // ---------------------------------------------------------------------------------------------------------
			default:          { SendMessageToPlayer(player, "Unknown command: " + command);                  break; }
		}
	}
};
/*---------------------------------------------------------------------------------------------------------------------\
| Create Custom Mission                                                                                                |
\---------------------------------------------------------------------------------------------------------------------*/
Mission CreateCustomMission(string path)
{
return new CustomMission();
