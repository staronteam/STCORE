#include "ScriptPCH.h"

class npc_honor_changer : public CreatureScript
{
    public:
        npc_honor_changer() : CreatureScript("npc_honor_changer") { }

                bool OnGossipHello(Player *player, Creature *_creature)
                {
                        if (player->isInCombat())
                        {
                                player->CLOSE_GOSSIP_MENU();
                                _creature->MonsterWhisper("Si v combate!", player->GetGUID());
                        return true;
                        }
                else
                        {
                                player->ADD_GOSSIP_ITEM( 2, "10k HONORS -> 100 ARENAPOINTS"                         , GOSSIP_SENDER_MAIN, 1);
                                player->ADD_GOSSIP_ITEM( 2, "100 ARENAPOINTS -> 10k HONORS"                         , GOSSIP_SENDER_MAIN, 2);
                }

                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());         
                return true;
                }

                bool OnGossipSelect(Player *player, Creature *_creature, uint32 sender, uint32 uiAction)
                {
                    if (player->isInCombat())
                    {
                                player->CLOSE_GOSSIP_MENU();
                                _creature->MonsterWhisper("Si v combate!", player->GetGUID());
                                return true;
                   }
                    if (sender == GOSSIP_SENDER_MAIN)
                    {
                         player->PlayerTalkClass->ClearMenus();
                      switch(uiAction)
                         {  
                         case 1: //Honors -> ArenaPoints
                                if (player->GetHonorPoints() >= 10000)
                                {
                                    player->CLOSE_GOSSIP_MENU();
                                    player->ModifyHonorPoints(-10000);
                                    player->ModifyArenaPoints(100);
                                    _creature->MonsterWhisper("Vymenil si 10k Honorov za 100 ArenaPointov.", player->GetGUID());
                                }
                               break;

                         case 2: //ArenaPoints -> Honors
                                if (player->GetArenaPoints() >= 100)
                                {
                                    player->CLOSE_GOSSIP_MENU();                    
                                    player->ModifyArenaPoints(-100);
                                    player->ModifyHonorPoints(10000);
                                    _creature->MonsterWhisper("Vymenil si 100 ArenaPointov za 10k Honorov.", player->GetGUID());
                                }
                               break;

                         default:
                                 break;
                         }
                  }
                  return true;
          }
};

void AddSC_npc_honor_changer()
{
    new npc_honor_changer();
}