#include "ScriptPCH.h"

class npc_teleport : public CreatureScript
{
    public:
        npc_teleport() : CreatureScript("npc_teleport") { }

                bool OnGossipHello(Player *player, Creature *_creature)
                {
                        if (player->isInCombat())
                        {
                                player->CLOSE_GOSSIP_MENU();
                                _creature->MonsterWhisper("Combat!", player->GetGUID());
                        return true;
                        }
                else
                        {
                                player->ADD_GOSSIP_ITEM( 2, "Mesta"                         , GOSSIP_SENDER_MAIN, 1);
                                player->ADD_GOSSIP_ITEM( 2, "Areny"                         , GOSSIP_SENDER_MAIN, 5);
                                player->ADD_GOSSIP_ITEM( 2, "OutdoorPVP"                     , GOSSIP_SENDER_MAIN, 6);
                                player->ADD_GOSSIP_ITEM( 2, "Shopy"                     , GOSSIP_SENDER_MAIN, 50);
                                player->ADD_GOSSIP_ITEM( 2, "Dungeony"                     , GOSSIP_SENDER_MAIN, 53);
                                player->ADD_GOSSIP_ITEM( 2, "Pet Zona"                     , GOSSIP_SENDER_MAIN, 60);
                                player->ADD_GOSSIP_ITEM( 2, "RP Zona"                     , GOSSIP_SENDER_MAIN, 70);
                }

                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());         
                return true;
                }

                bool OnGossipSelect(Player *player, Creature *_creature, uint32 sender, uint32 uiAction)
                {
                    if (player->isInCombat())
                    {
                                player->CLOSE_GOSSIP_MENU();
                                _creature->MonsterWhisper("Combat !", player->GetGUID());
                                return true;
                   }
                    if (sender == GOSSIP_SENDER_MAIN)
                    {
                         player->PlayerTalkClass->ClearMenus();
                      switch(uiAction)
                         {
                         case 1: //Cities
                           if (player->GetTeam() == ALLIANCE ) //Aliancia
                           {
                                player->ADD_GOSSIP_ITEM( 0, "Stormwind"           , GOSSIP_SENDER_MAIN, 7);
                                player->ADD_GOSSIP_ITEM( 0, "Ironforge"           , GOSSIP_SENDER_MAIN, 8);
                                player->ADD_GOSSIP_ITEM( 0, "Darnassus"           , GOSSIP_SENDER_MAIN, 9);
                                player->ADD_GOSSIP_ITEM( 0, "Exodar"              , GOSSIP_SENDER_MAIN, 10);
                           }
                           else //Horda
                           {
                                player->ADD_GOSSIP_ITEM( 0, "Orgrimmar"           , GOSSIP_SENDER_MAIN, 11);
                                player->ADD_GOSSIP_ITEM( 0, "Thunderbluff"        , GOSSIP_SENDER_MAIN, 12);
                                player->ADD_GOSSIP_ITEM( 0, "Undercity"           , GOSSIP_SENDER_MAIN, 13);
                                player->ADD_GOSSIP_ITEM( 0, "Silvermoon City"     , GOSSIP_SENDER_MAIN, 14);
                           }
                                player->ADD_GOSSIP_ITEM( 0, "Shattrath City"      , GOSSIP_SENDER_MAIN, 15);
                                player->ADD_GOSSIP_ITEM( 0, "Dalaran"                 , GOSSIP_SENDER_MAIN, 16);
                                player->ADD_GOSSIP_ITEM( 7, "<-- BACK"      , GOSSIP_SENDER_MAIN, 17);

                                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                               break;

                         case 5: //Arenas
                                player->ADD_GOSSIP_ITEM( 0, "Gurubashi Arena"                   , GOSSIP_SENDER_MAIN, 43);
                                player->ADD_GOSSIP_ITEM( 0, "Dire Maul Arena"                   , GOSSIP_SENDER_MAIN, 44);
                                player->ADD_GOSSIP_ITEM( 0, "Circle of Blood Arena"             , GOSSIP_SENDER_MAIN, 45);
                                player->ADD_GOSSIP_ITEM( 0, "Nagrand Arena"                     , GOSSIP_SENDER_MAIN, 46);
                                player->ADD_GOSSIP_ITEM( 7, "<-- BACK"                    , GOSSIP_SENDER_MAIN, 17);

                                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                               break;

                         case 6: //OutdoorPvP
                                player->ADD_GOSSIP_ITEM( 0, "Wintergrasp"                               , GOSSIP_SENDER_MAIN, 47);
                                player->ADD_GOSSIP_ITEM( 7, "<-- BACK"                           , GOSSIP_SENDER_MAIN, 17);

                                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                               break;

                         case 17: //<- [Main Menu]
                                player->ADD_GOSSIP_ITEM( 2, "Mesta"                         , GOSSIP_SENDER_MAIN, 1);
                                player->ADD_GOSSIP_ITEM( 2, "Areny"                         , GOSSIP_SENDER_MAIN, 5);
                                player->ADD_GOSSIP_ITEM( 2, "OutdoorPvP"                     , GOSSIP_SENDER_MAIN, 6);
                                player->ADD_GOSSIP_ITEM( 2, "Shopy"                     , GOSSIP_SENDER_MAIN, 50);
                                player->ADD_GOSSIP_ITEM( 2, "Dungeony"                     , GOSSIP_SENDER_MAIN, 53);
                                player->ADD_GOSSIP_ITEM( 2, "Pet Zona"                     , GOSSIP_SENDER_MAIN, 60);
                                player->ADD_GOSSIP_ITEM( 2, "RP Zona"                     , GOSSIP_SENDER_MAIN, 70);
                                

                                player->SEND_GOSSIP_MENU(907, _creature->GetGUID());
                               break;

                         case 7: //Stormwind
                                player->CLOSE_GOSSIP_MENU();
                                player->TeleportTo(0, -8895.368164f, 567.118835f, 92.6534f, 0.694642f);
                               break;
                         
                         case 8: //Ironforge
                                player->CLOSE_GOSSIP_MENU();
                                player->TeleportTo(0, -8895.368164f, 567.118835f, 92.6534f, 0.694642f);
                               break;

                         case 9: //Darnassus
                                player->CLOSE_GOSSIP_MENU();
                                player->TeleportTo(1, 9949.559570f, 2284.209961f, 1341.394165f, 1.525185f);
                               break;

                         case 10: //Exodar
                                player->CLOSE_GOSSIP_MENU();
                                player->TeleportTo(530, -3965.699951f, -11653.599609f, -138.843994f, 6.192861f);
                               break;

                         case 11: //Orgrimmar
                                player->CLOSE_GOSSIP_MENU();
                                player->TeleportTo(1, 1629.601440f, -4373.439453f, 31.293652f, 3.528754f);
                               break;

                         case 12: //Thunder Bluff
                                player->CLOSE_GOSSIP_MENU();
                                player->TeleportTo(1, -1277.369995f, 124.804001f, 131.287003f, 5.222740f);
                               break;

                         case 13: //Undercity
                                player->CLOSE_GOSSIP_MENU();
                                player->TeleportTo(0, 1584.069946f, 241.987000f, -52.153400f, 0.049647f);
                               break;

                         case 14: //Silvermoon City
                                player->CLOSE_GOSSIP_MENU();
                                player->TeleportTo(530, 9487.690430f, -7279.200195f, 14.286600f, 6.164780f);
                               break;

                         case 15: //Shattrath City
                                player->CLOSE_GOSSIP_MENU();
                                player->TeleportTo(530, -1838.160034f, 5301.790039f, -12.428000f, 5.951700f);
                               break;

                         case 16: //Dalaran
                                player->CLOSE_GOSSIP_MENU();
                                player->TeleportTo(571, 5804.149902f, 624.770996f, 647.767029f, 1.640000f);
                               break;

                         case 43: //Gurubashi Arena 
                                player->CLOSE_GOSSIP_MENU();
                                player->TeleportTo(0, -13309.700195f, 78.886703f, 22.289927f, 0.940000f);
                               break;

                         case 44: //Dire Maul Arena 
                                player->CLOSE_GOSSIP_MENU();
                                player->TeleportTo(1, -3717.760010f, 1124.719971f, 132.315994f, 4.340000f);
                               break;

                         case 45: //Circle of Blood Arena 
                                player->CLOSE_GOSSIP_MENU();
                                player->TeleportTo(530, 2837.373535f, 5930.557129f, 11.215009f, 5.700000f);
                               break;

                         case 46: //Nagrand Arena 
                                player->CLOSE_GOSSIP_MENU();
                                player->TeleportTo(530, -1998.135498f, 6579.937988f, 11.166474f, 2.360000f);
                               break;

                         case 47: //Wintergrasp 
                                player->CLOSE_GOSSIP_MENU();
                                player->TeleportTo(571, 4561.580078f, 2835.330078f, 389.790009f, 0.340000f);
                               break;
                               
                         case 50: //Shops
                                if (player->GetTeam() == ALLIANCE ) //Aliancia
                                {
                                     player->ADD_GOSSIP_ITEM( 0, "Alliance shop"           , GOSSIP_SENDER_MAIN, 51);
                                }
                                else //Horda
                                {
                                     player->ADD_GOSSIP_ITEM( 0, "Horde shop"           , GOSSIP_SENDER_MAIN, 52);
                                }
                                player->ADD_GOSSIP_ITEM( 7, "<-- BACK"                    , GOSSIP_SENDER_MAIN, 17);

                                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                               break;
                          
                          case 51: //Alliance shop 
                                player->CLOSE_GOSSIP_MENU();
                                player->TeleportTo(1, -9707.610352f, -4398.009766f, 3.680270f, 2.694670f);
                               break;
                               
                          case 52: //Horde shop 
                                player->CLOSE_GOSSIP_MENU();
                                player->TeleportTo(0, -11821.799805f, 1243.250000f, 1.843190f, 2.694670f);
                               break;
                               
                          case 53: //Dungeony
                                player->ADD_GOSSIP_ITEM( 0, "Opicí dungeon - 5 MAN"                               , GOSSIP_SENDER_MAIN, 54);
                                player->ADD_GOSSIP_ITEM( 0, "Zul'Aman"                               , GOSSIP_SENDER_MAIN, 55);
                                player->ADD_GOSSIP_ITEM( 7, "<-- BACK"                           , GOSSIP_SENDER_MAIN, 17);

                                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                               break;
                               
                          case 54: //Opičí dungeon
                                player->CLOSE_GOSSIP_MENU();
                                player->TeleportTo(1, -893.006775f, -2053.609863f, 82.128624f, 0.395017f);
                               break;
                               
                          case 54: //Zul'Aman
                                player->CLOSE_GOSSIP_MENU();
                                player->TeleportTo(530, 6852.976562f, -7951.400879f, 170.099167f, 4.774150f);
                               break;
                               
                          case 60: //Pet zóna
                                player->CLOSE_GOSSIP_MENU();
                                player->TeleportTo(1, -1153.11f, -569.305f, -57.7827f, 5.49056f);
                               break;
                               
                          case 70: //RP Zony
                                if (player->GetTeam() == ALLIANCE ) //Aliancia
                                {
                                     player->ADD_GOSSIP_ITEM( 0, "Alliance RP Zona"           , GOSSIP_SENDER_MAIN, 71);
                                }
                                else //Horda
                                {
                                     player->ADD_GOSSIP_ITEM( 0, "Horde RP Zona"           , GOSSIP_SENDER_MAIN, 72);
                                }
                                player->ADD_GOSSIP_ITEM( 7, "<-- BACK"                    , GOSSIP_SENDER_MAIN, 17);

                                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
                               break;
                          
                          case 71: //Alliance RP Zona
                                player->CLOSE_GOSSIP_MENU();
                                player->TeleportTo(571, 2117.698242f, 5305.651367f, 32.771912f, 2.819346f);
                               break;
                          
                          case 72: //Horde RP Zona
                                player->CLOSE_GOSSIP_MENU();
                                player->TeleportTo(571, 2837.052979f, 6185.955078f, 104.220200f, 3.840961f);
                               break;

                         default:
                                 break;
                         }
                  }
                  return true;
          }
};

void AddSC_npc_teleport()
{
    new npc_teleport();
}