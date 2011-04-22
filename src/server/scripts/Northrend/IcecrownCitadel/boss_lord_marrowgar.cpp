/*
 * Copyright (C) 2008-2010 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ScriptPCH.h"
#include "icecrown_citadel.h"
#include "SpellAuraEffects.h"

enum Yells
{
    SAY_INTRO     = -1631000,
    SAY_AGGRO     = -1631001,
    SAY_STORM     = -1631002,
    SAY_SPIKE_1   = -1631003,
    SAY_SPIKE_2   = -1631004,
    SAY_SPIKE_3   = -1631005,
    SAY_KILL_1    = -1631006,
    SAY_KILL_2    = -1631007,
    SAY_DEATH     = -1631008,
    SAY_BERSERK   = -1631009,
    STORM_EMOTE   = -1631010
};

enum Spells
{
    SPELL_SABER_SLASH        = 69055,
    SPELL_COLD_FLAME         = 69146,
    SPELL_BONE_SPIKE         = 73142,
    SPELL_SPIKE_IMPALING     = 69057,
    SPELL_IMPALE             = 69062,
    SPELL_IMPALED            = 69065,
    SPELL_BONE_STORM         = 69076,
    SPELL_COLD_FLAME_SPAWN   = 69138,
    SPELL_COLD_FLAME_SPAWN_B = 72701
};

enum ePoint
{
    POINT_PLAYER              = 366121
};

class npc_bone_spike : public CreatureScript
{
    public:
        npc_bone_spike() : CreatureScript("npc_bone_spike") { }

        struct npc_bone_spikeAI : public Scripted_NoMovementAI
        {
            npc_bone_spikeAI(Creature *creature) : Scripted_NoMovementAI(creature), vehicle(creature->GetVehicleKit())
            {
                ASSERT(vehicle);
                BoneSpikeGUID = 0;
                instance = creature->GetInstanceScript();
            }

            void IsSummonedBy(Unit* summoner)
            {
                BoneSpikeGUID = summoner->GetGUID();
                summoner->EnterVehicle(vehicle, 0);
                DoCast(summoner, SPELL_IMPALED);
                uiAchievBonedTimer = 8000;
            }

            void Reset()
            {
                BoneSpikeGUID = 0;
            }

            void JustDied(Unit* /*pKiller*/)
            {
                if (Player* boned = ObjectAccessor::GetPlayer(*me, BoneSpikeGUID))
                    boned->RemoveAurasDueToSpell(SPELL_IMPALED);
            }

            void KilledUnit(Unit* /*pVictim*/)
            {
                me->Kill(me);
            }

            void UpdateAI(const uint32 uiDiff)
            {
                if (!BoneSpikeGUID || !instance)
                    return;

                if (Player* boned = ObjectAccessor::GetPlayer(*me, BoneSpikeGUID))
                    if (!boned->HasAura(SPELL_IMPALED))
                        me->Kill(me);

                if (uiAchievBonedTimer <= uiDiff)
                {
                    instance->SetData(DATA_BONED, FAIL);
                    uiAchievBonedTimer = 8000;
                } else uiAchievBonedTimer -= uiDiff;
            }

        private:
            InstanceScript* instance;

            uint64 BoneSpikeGUID;
            uint32 uiAchievBonedTimer;

            Vehicle* vehicle;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_bone_spikeAI(creature);
        }
};

class npc_cold_flame : public CreatureScript
{
    public:
        npc_cold_flame() : CreatureScript("npc_cold_flame") { }

        struct npc_cold_flameAI : public ScriptedAI
        {
            npc_cold_flameAI(Creature *creature) : ScriptedAI(creature)
            {
                instance = creature->GetInstanceScript();
            }

            void Reset()
            {
                uiColdFlameTimer = 900;

                SpellEntry* spell = GET_SPELL(SPELL_COLD_FLAME);
                if (spell)
                    spell->EffectRadiusIndex[0] = 16; //prevent stack damage
                DoCast(me, SPELL_COLD_FLAME);

                me->SetVisible(false);
                DoStartNoMovement(me->getVictim());

                uiStage = 1;
                uiRadius = 2;
                uiOwnerEntry = 0;
            }

            void IsSummonedBy(Unit* owner)
            {
                if(owner)
                {
                    if(owner->HasAura(SPELL_BONE_STORM))
                        bCrossfire = true;
                    else
                        bCrossfire = false;

                    uiOwnerEntry = owner->GetEntry();
                }
            }

            void UpdateAI(const uint32 uiDiff)
            {
                if(uiColdFlameTimer <= uiDiff)
                {
                    if(uiOwnerEntry == CREATURE_MARROWGAR)
                    {
                        if(bCrossfire)
                        {
                            float x, y;
                            me->GetNearPoint2D(x, y, uiRadius*uiStage, 0);
                            me->SummonCreature(CREATURE_COLD_FLAME, x, y, me->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 4000);
                            me->GetNearPoint2D(x, y, uiRadius*uiStage, M_PI/2);
                            me->SummonCreature(CREATURE_COLD_FLAME, x, y, me->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 4000);
                            me->GetNearPoint2D(x, y, uiRadius*uiStage, M_PI*(M_PI/2));
                            me->SummonCreature(CREATURE_COLD_FLAME, x, y, me->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 4000);
                            me->GetNearPoint2D(x, y, uiRadius*uiStage, M_PI);
                            me->SummonCreature(CREATURE_COLD_FLAME, x, y, me->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 4000);
                        }
                        else if (!bCrossfire)
                        {
                            float x, y;
                            float angle = instance->GetData(DATA_ANGLE / 1000);
                            me->GetNearPoint2D(x, y, uiRadius*uiStage, angle);
                            me->SummonCreature(CREATURE_COLD_FLAME, x, y, me->GetPositionZ(), angle, TEMPSUMMON_TIMED_DESPAWN, 4000);
                        }
                        ++uiStage;
                        uiColdFlameTimer = 1000;
                    }
                } else uiColdFlameTimer -= uiDiff;
            }

        private:
            InstanceScript* instance;

            uint32 uiColdFlameTimer;
            uint8 uiRadius;
            uint8 uiStage;
            uint32 uiOwnerEntry;
            bool bCrossfire;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_cold_flameAI(creature);
        }
};

class boss_lord_marrowgar : public CreatureScript
{
    public:
        boss_lord_marrowgar() : CreatureScript("boss_lord_marrowgar") { }

        struct boss_lord_marrowgarAI : public BossAI
        {
            boss_lord_marrowgarAI(Creature* creature) : BossAI(creature, DATA_MARROWGAR), summons(me)
            {
                instance = creature->GetInstanceScript();
                fBaseSpeed = me->GetSpeedRate(MOVE_RUN);
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
                bIntro = false;
            }

            void Reset()
            {
                uiSaberSlashTimer = 7000;
                uiBoneSpikeGraveyardTimer = 15000;
                uiColdFlameTimer = 10000;
                uiBoneStormTimer = 45000;
                uiBoneStormRemoveTimer = 20000;
                uiBerserkTimer = 600000;

                me->SetSpeed(MOVE_RUN, fBaseSpeed, true);

                if (!instance)
                    return;

                instance->SetData(DATA_BONED, DONE);
                instance->SetData(DATA_MARROWGAR_EVENT, NOT_STARTED);

                if (me->GetMotionMaster()->GetCurrentMovementGeneratorType() == POINT_MOTION_TYPE)
                    me->GetMotionMaster()->MovementExpired();
            }

            void EnterCombat(Unit* /*pWho*/)
            {
                DoScriptText(SAY_AGGRO, me);

                if (instance)
                    instance->SetData(DATA_MARROWGAR_EVENT, IN_PROGRESS);

                summons.DespawnAll();
            }

            void JustSummoned(Creature* summon)
            {
                summons.Summon(summon);
            }

            void JustDied(Unit* /*pKiller*/)
            {
                if (!instance)
                    return;

                DoScriptText(SAY_DEATH, me);

                instance->SetData(DATA_MARROWGAR_EVENT, DONE);
                if(instance->GetData(DATA_BONED) == DONE)
                    instance->DoCompleteAchievement(RAID_MODE(ACHIEV_BONED_10, ACHIEV_BONED_25));

                summons.DespawnAll();
            }

            void JustReachedHome()
            {
                if (instance)
                    instance->SetData(DATA_MARROWGAR_EVENT, FAIL);

                summons.DespawnAll();
            }

            void KilledUnit(Unit* pVictim)
            {
                if (pVictim->GetTypeId() == TYPEID_PLAYER)
                {
                    switch(rand()%1)
                    {
                        case 0: DoScriptText(SAY_KILL_1, me); break;
                        case 1: DoScriptText(SAY_KILL_2, me); break;
                    }
                }
            }

            void MoveInLineOfSight(Unit* pWho)
            {
                if (!bIntro && me->IsWithinDistInMap(pWho, 90.0f, true))
                {
                    DoScriptText(SAY_INTRO, me);
                    bIntro = true;
                }
            }

            void UpdateAI(const uint32 uiDiff)
            {
                if (!UpdateVictim())
                    return;

                if (uiBerserkTimer <= uiDiff)
                {
                    DoScriptText(SAY_BERSERK, me);
                    DoCast(SPELL_BERSERK);
                    uiBerserkTimer = 600000;
                } else uiBerserkTimer -= uiDiff;

                if (IsHeroic() || !me->HasAura(SPELL_BONE_STORM))
                {
                    if (uiBoneSpikeGraveyardTimer < uiDiff)
                    {
                        DoCast(me, SPELL_SPIKE_IMPALING);
                        uiBoneSpikeGraveyardTimer = 15000;
                    } else uiBoneSpikeGraveyardTimer -= uiDiff;
                }

                if (!me->HasAura(SPELL_BONE_STORM))
                {
                    if (uiBoneStormTimer <= uiDiff)
                    {
                        DoCast(SPELL_BONE_STORM);
                        DoScriptText(SAY_STORM, me);
                        DoScriptText(STORM_EMOTE, me);
                        DoStartNoMovement(me->getVictim());
                        me->SetSpeed(MOVE_RUN, fBaseSpeed*3.0f, true);
                        if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 100.0f, true))
                            me->GetMotionMaster()->MovePoint(POINT_PLAYER, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ());
                        uiBoneStormTimer = 45000; //bone storm 30 second + other spell casting time
                    } else uiBoneStormTimer -= uiDiff;

                    if (uiColdFlameTimer <= uiDiff)
                    {
                        if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 6.0f, true))
                        {
                            instance->SetData(DATA_ANGLE, (me->GetAngle(target)*1000));
                            DoCast(target, SPELL_COLD_FLAME_SPAWN);
                        }
                        else
                        {
                            instance->SetData(DATA_ANGLE, (me->GetAngle(me->getVictim())*1000));
                            DoCastVictim( SPELL_COLD_FLAME_SPAWN);
                        }
                        uiColdFlameTimer = 10000;
                    } else uiColdFlameTimer -= uiDiff;

                    if (uiSaberSlashTimer <= uiDiff)
                    {
                        DoCastVictim( SPELL_SABER_SLASH);
                        uiSaberSlashTimer = 7000;
                    } else uiSaberSlashTimer -= uiDiff;
                }
                else
                {
                    if (uiBoneStormRemoveTimer <= uiDiff)
                    {
                        me->RemoveAurasDueToSpell(SPELL_BONE_STORM);
                        DoStartMovement(me->getVictim());
                        me->SetSpeed(MOVE_RUN, fBaseSpeed, true);
                        uiBoneStormRemoveTimer = RAID_MODE(20000,30000,20000,30000);
                    } else uiBoneStormRemoveTimer -= uiDiff;
                }

                DoMeleeAttackIfReady();
            }

            void MovementInform(uint32 type, uint32 id)
            {
                if(type != POINT_MOTION_TYPE)
                    return;

                if(id = POINT_PLAYER)
                {
                    if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                        me->GetMotionMaster()->MovePoint(POINT_PLAYER, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ());
                    DoCast(SPELL_COLD_FLAME_SPAWN_B);
                }
            }

        private:
            InstanceScript* instance;

            uint32 uiSaberSlashTimer;
            uint32 uiBoneSpikeGraveyardTimer;
            uint32 uiBoneStormTimer;
            uint32 uiBoneStormRemoveTimer;
            uint32 uiColdFlameTimer;
            uint32 uiBerserkTimer;
            float fBaseSpeed;
            bool bIntro;

            SummonList summons;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_lord_marrowgarAI(creature);
        }
};

class spell_lord_marrowgar_bone_storm : public SpellScriptLoader
{
    public:
        spell_lord_marrowgar_bone_storm() : SpellScriptLoader("spell_lord_marrowgar_bone_storm") { }


        class spell_lord_marrowgar_bone_storm_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_lord_marrowgar_bone_storm_SpellScript)

            void onEffect(SpellEffIndex /*effIndex*/)
            {
                int32 dmg = GetHitDamage();
                float distance = GetHitUnit()->GetExactDist2d(GetCaster());
                float distVariant = distance >= 20.0f ? 4 : (10.0f/3.0f);
                if (distance < 5)
                    distance = 5; //prevent exploit

                SetHitDamage(int32(dmg * distVariant / distance));
            }

            void Register()
            {
                OnEffect += SpellEffectFn(spell_lord_marrowgar_bone_storm_SpellScript::onEffect, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }

            bool Load()
            {
                if (GetCaster()->GetEntry() != CREATURE_MARROWGAR)
                    return false;
                return true;
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_lord_marrowgar_bone_storm_SpellScript();
        }
};

typedef boss_lord_marrowgar::boss_lord_marrowgarAI MarrowgarAI;

class spell_marrowgar_bone_spike_graveyard : public SpellScriptLoader
{
    public:
        spell_marrowgar_bone_spike_graveyard() : SpellScriptLoader("spell_marrowgar_bone_spike_graveyard") { }


        class spell_marrowgar_bone_spike_graveyard_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_marrowgar_bone_spike_graveyard_SpellScript);

            void HandleSpikes(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                if (Creature* marrowgar = GetCaster()->ToCreature())
                {
                    CreatureAI* marrowgarAI = marrowgar->AI();
                    bool yell = false;
                    uint8 boneSpikeCount = GetCaster()->GetMap()->GetSpawnMode() & 1 ? 3 : 1;
                    for (uint8 i = 0; i < boneSpikeCount; ++i)
                    {
                        Unit* target = marrowgarAI->SelectTarget(SELECT_TARGET_RANDOM, 1, 100.0f, true, -SPELL_IMPALED);
                        if (!target && !i)
                            target = marrowgarAI->SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true, -SPELL_IMPALED);
                        if (!target)
                            break;
                        yell = true;
                        target->CastCustomSpell(SPELL_IMPALE, SPELLVALUE_BASE_POINT0, 0, target, true);
                    }

                    if (yell)
                        DoScriptText(RAND(SAY_SPIKE_1,SAY_SPIKE_2,SAY_SPIKE_3), GetCaster());
                }
            }

            void Register()
            {
                OnEffect += SpellEffectFn(spell_marrowgar_bone_spike_graveyard_SpellScript::HandleSpikes, EFFECT_1, SPELL_EFFECT_APPLY_AURA);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_marrowgar_bone_spike_graveyard_SpellScript();
        }
};

void AddSC_boss_lord_marrowgar()
{
    new npc_bone_spike();
    new npc_cold_flame();
    new boss_lord_marrowgar();
    new spell_lord_marrowgar_bone_storm();
    new spell_marrowgar_bone_spike_graveyard();

    if (VehicleSeatEntry* vehSeat = const_cast<VehicleSeatEntry*>(sVehicleSeatStore.LookupEntry(6206)))
        vehSeat->m_flags |= 0x400;
}
