#include "ScriptPCH.h"

class npc_at_beastmaster : public CreatureScript
{
private:
    void CreatePet(Player *player, Creature * m_creature, uint32 entry)
    {
        uint32 level = player->getLevel();

        if(player->getClass() != CLASS_HUNTER)
        {
            m_creature->MonsterWhisper("You are not hunter! Your order have been rejected.", player->GetGUID());
            player->CLOSE_GOSSIP_MENU();
            return;
        }

        if(player->GetPet())
        {
            m_creature->MonsterWhisper("First you must drop your pet!", player->GetGUID());
            player->CLOSE_GOSSIP_MENU();
            return;
        }

        Creature *creatureTarget = m_creature->SummonCreature(entry, player->GetPositionX(), player->GetPositionY()+2, player->GetPositionZ(), player->GetOrientation(), TEMPSUMMON_CORPSE_TIMED_DESPAWN, 500);

        if(!creatureTarget)
            return;
        
        Pet* pet = player->CreateTamedPetFrom(creatureTarget, 0);

        if(!pet)
            return;

        creatureTarget->setDeathState(JUST_DIED);
        creatureTarget->RemoveCorpse();
        creatureTarget->SetHealth(0);
        pet->SetPower(POWER_HAPPINESS, 1048000);
        pet->GetMap()->Add((Creature*)pet);
        pet->SetUInt32Value(UNIT_FIELD_LEVEL, uint32(player->getLevel()));

        if(!pet->InitStatsForLevel(pet->getLevel()))
            m_creature->MonsterWhisper("Nemas dostatocny level pre tohto peta!", player->GetGUID()); 

        pet->UpdateAllStats();
        player->SetMinion(pet, true);
        pet->SavePetToDB(PET_SAVE_AS_CURRENT);
        pet->InitTalentForLevel();
        player->PetSpellInitialize();
        player->CLOSE_GOSSIP_MENU();
        m_creature->MonsterWhisper("Pet pridany. Caka na vase nakrmenie a vyber mena.", player->GetGUID());
    }

public:
    npc_at_beastmaster() : CreatureScript("npc_at_beastmaster") { }

    bool OnGossipHello(Player *player, Creature *_Creature)
    {
        if(player->getClass() != CLASS_HUNTER)
        {
            _Creature->MonsterWhisper("Ty nie si Hunter!", player->GetGUID());
            return true;
        }

        player->ADD_GOSSIP_ITEM(4, "Ziskat noveho peta", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 30);

        if (player->CanTameExoticPets())
        {
            player->ADD_GOSSIP_ITEM(4, "Ziskat noveho exotickeho peta", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 50);
        }

        player->ADD_GOSSIP_ITEM(2, "Take me to stable", GOSSIP_SENDER_MAIN, GOSSIP_OPTION_STABLEPET);
        player->ADD_GOSSIP_ITEM(6, "Sell me some food", GOSSIP_SENDER_MAIN, GOSSIP_OPTION_VENDOR);
        player->SEND_GOSSIP_MENU(1,_Creature->GetGUID());

        return true;
    }

    bool OnGossipSelect(Player *player, Creature *_Creature, uint32 sender, uint32 action)
    {
        switch(action)
        {
        case GOSSIP_ACTION_INFO_DEF + 100:
            player->ADD_GOSSIP_ITEM(4, "Ziskat noveho peta", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 30);
            if (player->CanTameExoticPets())
            {
                player->ADD_GOSSIP_ITEM(4, "Ziskat noveho exotickeho peta", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 50);
            }
            player->ADD_GOSSIP_ITEM(2, "Take me to stable", GOSSIP_SENDER_MAIN, GOSSIP_OPTION_STABLEPET);
            player->ADD_GOSSIP_ITEM(6, "Sell me some food", GOSSIP_SENDER_MAIN, GOSSIP_OPTION_VENDOR);
            player->SEND_GOSSIP_MENU(1,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 30:
            player->ADD_GOSSIP_ITEM(2, "<-- BACK", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 100);
            player->ADD_GOSSIP_ITEM(4, "NEXT -->", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 31);
            player->ADD_GOSSIP_ITEM(6, "Bat", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 18);
            player->ADD_GOSSIP_ITEM(6, "Bear", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->ADD_GOSSIP_ITEM(6, "Boar", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
            player->ADD_GOSSIP_ITEM(6, "Cat", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
            player->ADD_GOSSIP_ITEM(6, "Carrion Bird", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
            player->ADD_GOSSIP_ITEM(6, "Crab", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
            player->ADD_GOSSIP_ITEM(6, "Crocolisk", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
            player->ADD_GOSSIP_ITEM(6, "Dragonhawk", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 17);
            player->ADD_GOSSIP_ITEM(6, "Gorilla", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);
            player->ADD_GOSSIP_ITEM(6, "Hound", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9);
            player->ADD_GOSSIP_ITEM(6, "Hyena", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 10);
            player->ADD_GOSSIP_ITEM(6, "Moth", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);
            player->ADD_GOSSIP_ITEM(6, "Owl", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 12);
            player->SEND_GOSSIP_MENU(1,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 31:  
            player->ADD_GOSSIP_ITEM(2, "<-- BACK", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 100);
            player->ADD_GOSSIP_ITEM(4, "<-- PREVIOUS PAGE", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 30);
            player->ADD_GOSSIP_ITEM(6, "Raptor", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 20);
            player->ADD_GOSSIP_ITEM(6, "Ravager", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 19);
            player->ADD_GOSSIP_ITEM(6, "Strider", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 13);
            player->ADD_GOSSIP_ITEM(6, "Scorpid", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 14);
            player->ADD_GOSSIP_ITEM(6, "Spider", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 16);
            player->ADD_GOSSIP_ITEM(6, "Serpent", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 21);  
            player->ADD_GOSSIP_ITEM(6, "Turtle", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 15);
            player->ADD_GOSSIP_ITEM(6, "Wasp", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
            player->SEND_GOSSIP_MENU(1,_Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 50:  
            player->ADD_GOSSIP_ITEM(2, "<-- BACK", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 100);
            player->ADD_GOSSIP_ITEM(6, "Chimaera", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 51);
            player->ADD_GOSSIP_ITEM(6, "Core Hound", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 52);
            player->ADD_GOSSIP_ITEM(6, "Devilsaur", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 53);
            player->ADD_GOSSIP_ITEM(6, "Rhino", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 54);
            player->ADD_GOSSIP_ITEM(6, "Silithid", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 55);
            player->ADD_GOSSIP_ITEM(6, "Worm", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 56);  
            player->SEND_GOSSIP_MENU(1,_Creature->GetGUID());
            break;
        case GOSSIP_OPTION_STABLEPET:
            player->GetSession()->SendStablePet(_Creature->GetGUID());
            break; 
        case GOSSIP_OPTION_VENDOR:
            player->SEND_VENDORLIST( _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 51: //chimera
            CreatePet(player, _Creature, 21879);
            break;
        case GOSSIP_ACTION_INFO_DEF + 52: //core hound
            CreatePet(player, _Creature, 21108);
            break;
        case GOSSIP_ACTION_INFO_DEF + 53: //Devilsaur
            CreatePet(player, _Creature, 20931);
            break;
        case GOSSIP_ACTION_INFO_DEF + 54: //rhino
            CreatePet(player, _Creature, 30445);
            break;
        case GOSSIP_ACTION_INFO_DEF + 55: //silithid
            CreatePet(player, _Creature, 5460);
            break;
        case GOSSIP_ACTION_INFO_DEF + 56: //Worm
            CreatePet(player, _Creature, 30148);
            break;
        case GOSSIP_ACTION_INFO_DEF + 16: //Spider
            CreatePet(player, _Creature, 2349);
            break;
        case GOSSIP_ACTION_INFO_DEF + 17: //Dragonhawk
            CreatePet(player, _Creature, 27946);
            break;
        case GOSSIP_ACTION_INFO_DEF + 18: //Bat
            CreatePet(player, _Creature, 28233);
            break;
        case GOSSIP_ACTION_INFO_DEF + 19: //Ravager
            CreatePet(player, _Creature, 17199);
            break;
        case GOSSIP_ACTION_INFO_DEF + 20: //Raptor
            CreatePet(player, _Creature, 14821);
            break;
        case GOSSIP_ACTION_INFO_DEF + 21: //Serpent
            CreatePet(player, _Creature, 28358);
            break;
        case GOSSIP_ACTION_INFO_DEF + 1:  //bear
            CreatePet(player, _Creature, 29319);
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:  //Boar
            CreatePet(player, _Creature, 29996);
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:  //Bug
            CreatePet(player, _Creature, 28085);
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:  //cat
            CreatePet(player, _Creature, 28097);
            break;
        case GOSSIP_ACTION_INFO_DEF + 5:  //carrion
            CreatePet(player, _Creature, 26838);
            break;
        case GOSSIP_ACTION_INFO_DEF + 6:  //crab
            CreatePet(player, _Creature, 24478);
            break;   
        case GOSSIP_ACTION_INFO_DEF + 7:  //crocolisk
            CreatePet(player, _Creature, 1417);
            break;  
        case GOSSIP_ACTION_INFO_DEF + 8:  //gorila
            CreatePet(player, _Creature, 28213);
            break;
        case GOSSIP_ACTION_INFO_DEF + 9:  //hound
            CreatePet(player, _Creature, 29452);
            break;
        case GOSSIP_ACTION_INFO_DEF + 10: //hynea
            CreatePet(player, _Creature, 13036);
            break;
        case GOSSIP_ACTION_INFO_DEF + 11: //Moth
            CreatePet(player, _Creature, 27421);
            break;
        case GOSSIP_ACTION_INFO_DEF + 12: //owl
            CreatePet(player, _Creature, 23136);
            break;
        case GOSSIP_ACTION_INFO_DEF + 13: //strider
            CreatePet(player, _Creature, 22807);
            break;
        case GOSSIP_ACTION_INFO_DEF + 14: //scorpid
            CreatePet(player, _Creature, 9698);
            break;
        case GOSSIP_ACTION_INFO_DEF + 15: //turtle
            CreatePet(player, _Creature, 25482);
            break;
        }
        return true;
    }
};

void AddSC_npc_at_beastmaster()
{
    new npc_at_beastmaster();
}