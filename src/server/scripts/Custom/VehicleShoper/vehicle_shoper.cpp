#include "ScriptPCH.h"

class npc_engineer : public CreatureScript
{
private:
    bool GiveMeMyFuckedVehicle(Player *player, Creature *_creature, uint32 vehicleID, int32 vehicleCost)
    {
        if(player->GetMoney() < vehicleCost)
        {
            _creature->MonsterSay("Nemas dostatok goldov.", 0, player->GetGUID());
            return false;
        }
        player->SummonCreature(vehicleID, player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), 0);
        player->ModifyMoney(vehicleCost*(-1));
        _creature->MonsterSay("Tu je tvoje vozidlo.", 0, player->GetGUID());
        return true;
    }

public:
    npc_engineer() : CreatureScript("npc_engineer") { }

    bool OnGossipHello(Player *player, Creature *_creature)
    {
        if(player->isInCombat())
        {
            player->CLOSE_GOSSIP_MENU();
            _creature->MonsterYell("Jsi v combatu!", 0, player->GetGUID());
            return true;
        }
        player->ADD_GOSSIP_ITEM( 6, "Vrazdiaci stroj 100g",  GOSSIP_SENDER_MAIN, 5);
        player->ADD_GOSSIP_ITEM( 6, "Motorka 150g",         GOSSIP_SENDER_MAIN, 1);
        player->ADD_GOSSIP_ITEM( 6, "Katapult 300g",        GOSSIP_SENDER_MAIN, 2);
        player->ADD_GOSSIP_ITEM( 6, "Stihacka 400g",        GOSSIP_SENDER_MAIN, 3);
        player->ADD_GOSSIP_ITEM( 6, "Tank 600g",            GOSSIP_SENDER_MAIN, 4);
        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());         
        return true;
    }

    bool OnGossipSelect(Player *player, Creature *_creature, uint32 sender, uint32 uiAction)
    {
        if(sender != GOSSIP_SENDER_MAIN)
            return true;

        player->PlayerTalkClass->ClearMenus();
        switch(uiAction)
        {
        case 1: //Motorka
            GiveMeMyFuckedVehicle(player, _creature, 33062, 1500000);
            break;
        case 2: //Katapult
            GiveMeMyFuckedVehicle(player, _creature, 33109, 3000000);
            break;
        case 3: //Letadlo
            GiveMeMyFuckedVehicle(player, _creature, 9200001, 4000000);
            break;
        case 4: //Tank
            GiveMeMyFuckedVehicle(player, _creature, 33060, 6000000);
            break;
        case 5: //Vrazdici stroj
            GiveMeMyFuckedVehicle(player, _creature, 27496, 1000000);
            break;
        default:
            break;
        }
        player->CLOSE_GOSSIP_MENU();
        return true;
    }
};

void AddSC_npc_engineer()
{
    new npc_engineer();
}