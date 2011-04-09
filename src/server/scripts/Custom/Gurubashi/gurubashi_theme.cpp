#include "ScriptPCH.h"

#define OFFSET_THEME 10000
int length = 0;

class npc_gurubashi_theme : public CreatureScript
{
private:
    typedef std::list<Creature *> CRL;
    typedef std::list<GameObject *> GOL;
    CRL creaturelist;
    GOL gameobjectlist;

    int GetThemeChangeReady(Creature *_Creature)
    {
        QueryResult result;
        result = WorldDatabase.PQuery("SELECT `time`, `refresh` FROM `gurubashi_givers` WHERE entry = %u", _Creature->GetEntry());

        if (result)
        {
            Field *fields = result->Fetch();
            return fields[0].GetInt32() + fields[1].GetInt32();
        }
        else return 0;
    }

public:
    npc_gurubashi_theme() : CreatureScript("npc_gurubashi_theme") { }

    bool OnGossipHello(Player *player, Creature *_creature)
    {
        char msg[100];
        int iTimeLast = GetThemeChangeReady(_creature);

        if (iTimeLast <= time(0))
        {
            QueryResult result;
            result = WorldDatabase.PQuery("SELECT `id`, `name` FROM `gurubashi_themes` WHERE giver = %u", _creature->GetEntry());
            if (result)
            {
                do
                {
                    Field *fields = result->Fetch();
                    player->ADD_GOSSIP_ITEM(4, fields[1].GetString(), GOSSIP_SENDER_MAIN, OFFSET_THEME + fields[0].GetInt32());
                } while (result->NextRow());
            }
        }
        else
        {
            int iTimeRem = iTimeLast - time(0);
            if (iTimeRem >= 60)
                sprintf(msg, "Zmena bude mozna za %u min. a %u sec.", iTimeRem / 60, iTimeRem - ((iTimeRem / 60) * 60));
            else
                sprintf(msg, "Zmena bude mozna za %u sec.", iTimeRem);
            player->ADD_GOSSIP_ITEM(0, msg, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
        }
        player->ADD_GOSSIP_ITEM(0, "Nazdar", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
        player->SEND_GOSSIP_MENU(1,_creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player *player, Creature *_creature, uint32 sender, uint32 action)
    {
        if (action > OFFSET_THEME)
        {
            if (GetThemeChangeReady(_creature) <= time(0))
            {
                int iTimeCur = time(0);
                QueryResult result;

                WorldDatabase.PExecute("UPDATE `gurubashi_givers` SET `time` = %u WHERE `entry` = %u", iTimeCur, _creature->GetEntry());

                // GameObjects
                result = WorldDatabase.PQuery("SELECT `x`, `y`, `z`, `o`, `entry` FROM `gurubashi_spawns_gameobject` WHERE `theme` = %u", action - OFFSET_THEME);
                GOL::iterator l;
                for (l = gameobjectlist.begin(); l != gameobjectlist.end(); ++l)
                {
                    GameObject * go = *l;
                    go->Delete();
                }
                gameobjectlist.clear();
                if (result)
                {
                    do
                    {
                        Field *fields = result->Fetch();                        
                        GameObject * gob = _creature->SummonGameObject(fields[4].GetInt32(), fields[0].GetFloat(), fields[1].GetFloat(), fields[2].GetFloat(), fields[3].GetFloat(), 0, 0, 0, 0, 2592000); 
                        gameobjectlist.push_back(gob);
                    } while (result->NextRow());
                }

                // Creatures
                result = WorldDatabase.PQuery("SELECT `x`, `y`, `z`, `o`, `entry`, `spawndist` FROM `gurubashi_spawns_creature` WHERE `theme` = %u", action - OFFSET_THEME);
                CRL::iterator i;
                for (i = creaturelist.begin(); i != creaturelist.end(); ++i)
                {
                    Creature * cr = *i;
                    cr->ForcedDespawn(0);           
                }
                creaturelist.clear();
                if (result)
                {
                    do
                    {
                        Field * fields = result->Fetch();   
                        Creature * tempCr = _creature->SummonCreature(fields[4].GetInt32(), fields[0].GetFloat(), fields[1].GetFloat(), fields[2].GetFloat(), fields[3].GetFloat(), TEMPSUMMON_MANUAL_DESPAWN, 0);
                        tempCr->GetMotionMaster()->MoveRandom(fields[5].GetFloat());
                        creaturelist.push_back(tempCr);
                    } while (result->NextRow());
                }
                player->PlayerTalkClass->CloseGossip();
            }
            else
            {
                OnGossipHello(player, _creature);
                _creature->MonsterSay("Tema bylo uspesne zvoleno. Prosim cekejte...", LANG_UNIVERSAL, player->GetGUID());
            }
        }
        else
        {
            switch (action)
            {
            case GOSSIP_ACTION_INFO_DEF + 1:
                player->PlayerTalkClass->CloseGossip();
                break;
            case GOSSIP_ACTION_INFO_DEF + 2:
                OnGossipHello(player, _creature);
                break;
            }
        }
        return true;
    }
};

void AddSC_npc_gurubashi_theme()
{
    new npc_gurubashi_theme();
}