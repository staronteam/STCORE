/*
* Copyright (C) 2009 - 2010 TrinityCore <http://www.trinitycore.org/>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

//Need fix 69783 and 69785 target

#include "ScriptPCH.h"
#include "icecrown_citadel.h"

enum Yells
{
    SAY_PRECIOUS    = -1631094,
    SAY_AGGRO       = -1631095,
    SAY_DEATH       = -1631103,
    SAY_KILL_1      = -1631100,
    SAY_KILL_2      = -1631101,
    SAY_BERSERK     = -1631102,
    SAY_SPRAY       = -1631097,
    EMOTE_SPRAY     = -1631096,
    SAY_OOZE_FLOOD  = -1631099,
    EMOTE_EXPLOSION = -1631098,
    SAY_EXPLOSION   = -1631099
};

enum Spells
{
    SPELL_OOZE_FLOOD_AURA    = 69788,
    SPELL_OOZE_FLOOD         = 69783,
    SPELL_OOZE_FLOOD_1       = 69785,
    //SPELL_OOZE_FLOOD         = 70069,
    SPELL_SLIME_SPRAY        = 69508,
    SPELL_SLIME_SPRAY_1      = 70881,
    SPELL_SLIME_SPRAY_SUMMON = 70883,
    SPELL_STICKY_OOZE        = 69774,
    SPELL_STICKY_OOZE_1      = 69776,
    SPELL_RADIATING_OOZE     = 69750,
    SPELL_RADIATING_OOZE_1   = 69760,
    SPELL_UNSTABLE_OOZE      = 69558,
    SPELL_EXPLOSION          = 69839,
    SPELL_EXPLOSION_1        = 69832,
    SPELL_MERGE_OOZE         = 69889,
    SPELL_VOLATILE_OOZE      = 70447,
    SPELL_DAZE               = 57416,
    SPELL_INFECTION_AURA     = 69674,
    SPELL_MORTAL_WOUND       = 71127,
    SPELL_DECIMATE           = 71123,
    SPELL_PRECIOUS_RIBBON    = 70404,
    SPELL_LITTLE_OOZE        = 69537,
    SPELL_LARGE_OOZE         = 69552
};

const Position SpawnLoc[]=
{
    {4468.825f, 3094.986f, 372.385f, 0.0f},
    {4487.825f, 3114.452f, 372.385f, 0.0f},
    {4489.825f, 3159.452f, 372.385f, 0.0f},
    {4467.825f, 3178.986f, 372.385f, 0.0f},
    {4424.421f, 3178.986f, 372.385f, 0.0f},
    {4404.821f, 3158.452f, 372.385f, 0.0f},
    {4404.825f, 3116.452f, 372.385f, 0.0f},
    {4424.825f, 3095.986f, 372.385f, 0.0f}
};

const Position StalkerSpawnLoc[]=
{
    {4468.825f, 3094.986f, 360.385f, 0.0f},
    {4487.825f, 3114.452f, 360.385f, 0.0f},
    {4489.825f, 3159.452f, 360.385f, 0.0f},
    {4467.825f, 3178.986f, 360.385f, 0.0f},
    {4424.421f, 3178.986f, 360.385f, 0.0f},
    {4404.821f, 3158.452f, 360.385f, 0.0f},
    {4404.825f, 3116.452f, 360.385f, 0.0f},
    {4424.825f, 3095.986f, 360.385f, 0.0f}
};

class boss_rotface : public CreatureScript
{
    public:
        boss_rotface() : CreatureScript("boss_rotface") { }

        struct boss_rotfaceAI : public BossAI
        {
            boss_rotfaceAI(Creature* creature) : BossAI(creature, DATA_ROTFACE), summons(me)
            {
                instance = me->GetInstanceScript();
                bFlood = false;
            }

            void Reset()
            {
                uiFloodTimer = 35000;
                uiSlimeSprayTimer = 15000;
                uiBerserkTimer = 600000;
                uiFloodEffectTimer = 1000;
                uiInfectionTimer = 14000;

                uiStage = 0;
                uiFloodStage = 1;

                if(instance)
                    instance->SetData(DATA_ROTFACE_EVENT, NOT_STARTED);
            }

            void JustDied(Unit* /*pKiller*/)
            {
                if(!instance)
                    return;

                DoScriptText(SAY_DEATH, me);

                instance->SetData(DATA_ROTFACE_EVENT, DONE);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_INFECTION_AURA);

                if (Creature* professor = Unit::GetCreature(*me, instance->GetData64(DATA_PROFESSOR_PUTRICIDE)))
                    professor->AI()->DoAction(ACTION_ROTFACE_DEATH);

                summons.DespawnAll();
            }

            void EnterCombat(Unit* /*who*/)
            {
                DoScriptText(SAY_AGGRO, me);

                if(instance)
                    instance->SetData(DATA_ROTFACE_EVENT, IN_PROGRESS);

                if (Creature* professor = Unit::GetCreature(*me, instance->GetData64(DATA_PROFESSOR_PUTRICIDE)))
                    professor->AI()->DoAction(ACTION_ROTFACE_COMBAT);
            }

            void JustReachedHome()
            {
                if(!instance)
                    return;

                instance->SetData(DATA_ROTFACE_EVENT, FAIL);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_INFECTION_AURA);

                summons.DespawnAll();
            }

            void JustSummoned(Creature* summon)
            {
                summons.Summon(summon);
            }

            void KilledUnit(Unit* victim)
            {
                if (victim->GetTypeId() == TYPEID_PLAYER)
                    DoScriptText(RAND(SAY_KILL_1,SAY_KILL_1), me);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (uiSlimeSprayTimer <= diff)
                {
                    if(Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 1, 100.0f, true, 0))
                    {
                        float x, y, z;
                        pTarget->GetPosition(x, y, z);
                        if(Creature* trigger = me->SummonCreature(CREATURE_OOZE_SPRAY_STALKER, x, y, z, 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 4000))
                        {
                            me->CastSpell(trigger, SPELL_SLIME_SPRAY_1, true);
                            me->CastSpell(trigger, SPELL_SLIME_SPRAY, true);
                        }
                        DoScriptText(SAY_SPRAY, me);
                        DoScriptText(EMOTE_SPRAY, me);
                    }
                    uiSlimeSprayTimer = 15000;
                } else uiSlimeSprayTimer -= diff;

                if (uiInfectionTimer <= diff)
                {
                    if(Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 1, 100.0f, true, -SPELL_INFECTION_AURA))
                        me->AddAura(SPELL_INFECTION_AURA, pTarget);
                    uiInfectionTimer = 14000;
                } else uiInfectionTimer -= diff;

                if (uiFloodEffectTimer <= diff)
                {
                    if(bFlood)
                    {
                        switch(uiFloodStage)
                        {
                            case 1:
                            {
                                if(Creature* stalker = me->FindNearestCreature(CREATURE_PUDDLE_STALKER, 100.0f, true))
                                {
                                    stalker->CastSpell(stalker, SPELL_OOZE_FLOOD, true);
                                    ++uiFloodStage;
                                    uiFloodEffectTimer = 4000;
                                }
                            }
                            break;
                            case 2:
                            {
                                if(Creature* stalker_puddle = me->FindNearestCreature(CREATURE_GREEN_GAS_STALKER, 100.0f, true))
                                {
                                    stalker_puddle->CastSpell(stalker_puddle, SPELL_OOZE_FLOOD_1, true);
                                    --uiFloodStage;
                                    bFlood = false;
                                }
                            }
                            break;
                        }
                    }
                } else uiFloodEffectTimer -= diff;

                if (uiFloodTimer <= diff)
                {
                    if(uiStage > 3)
                        uiStage = 0;

                    DoScriptText(SAY_OOZE_FLOOD, me);

                    me->SummonCreature(CREATURE_PUDDLE_STALKER, SpawnLoc[((uiStage + 0)*2)], TEMPSUMMON_TIMED_DESPAWN, 4000);
                    me->SummonCreature(CREATURE_PUDDLE_STALKER, SpawnLoc[((uiStage*2) + 1)], TEMPSUMMON_TIMED_DESPAWN, 4000);
                    me->SummonCreature(CREATURE_GREEN_GAS_STALKER, StalkerSpawnLoc[((uiStage + 0)*2)], TEMPSUMMON_TIMED_DESPAWN, 24000);
                    me->SummonCreature(CREATURE_GREEN_GAS_STALKER, StalkerSpawnLoc[((uiStage*2) + 1)], TEMPSUMMON_TIMED_DESPAWN, 24000);
                    ++uiStage;
                    bFlood = true;
                    uiFloodTimer = 25000;
                } else uiFloodTimer -= diff;

                if (uiBerserkTimer <= diff)
                {
                    DoScriptText(SAY_BERSERK, me);
                    DoCast(me, SPELL_BERSERK);
                    uiBerserkTimer = 600000;
                } else uiBerserkTimer -= diff;

                DoMeleeAttackIfReady();
            }
        private:
            InstanceScript* instance;
            uint32 uiFloodTimer;
            uint32 uiSlimeSprayTimer;
            uint32 uiBerserkTimer;
            uint32 uiFloodEffectTimer;
            uint32 uiInfectionTimer;
            uint8 uiStage;
            uint8 uiFloodStage;
            bool bFlood;
            SummonList summons;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_rotfaceAI(creature);
        }
};

class npc_ooze_big : public CreatureScript
{
    public:
        npc_ooze_big() : CreatureScript("npc_ooze_big") { }

        struct npc_ooze_bigAI : public ScriptedAI
        {
            npc_ooze_bigAI(Creature* creature) : ScriptedAI(creature)
            {
                instance = creature->GetInstanceScript();
            }

            void Reset()
            {
                uiStickyOozeTimer = 9000;

                DoCast(SPELL_UNSTABLE_OOZE);
                DoCast(SPELL_RADIATING_OOZE_1);
                DoCast(SPELL_LARGE_OOZE);
                me->AddUnitMovementFlag(MOVEMENTFLAG_WALKING);
                me->SetSpeed(MOVE_WALK, 0.5f);
                me->SetSpeed(MOVE_RUN, 0.5);
            }

            void EnterCombat(Unit* /*who*/) { }

            void KilledUnit(Unit* /*pVictim*/) { }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (uiStickyOozeTimer <= diff)
                {
                    DoCast(me->getVictim(), SPELL_STICKY_OOZE);
                    uiStickyOozeTimer = 10000;
                } else uiStickyOozeTimer -= diff;

                DoMeleeAttackIfReady();
            }
        private:
            InstanceScript* instance;

            uint32 uiStickyOozeTimer;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_ooze_bigAI(creature);
        }
};

class npc_ooze_little : public CreatureScript
{
    public:
        npc_ooze_little() : CreatureScript("npc_ooze_little") { }

        struct npc_ooze_littleAI : public ScriptedAI
        {
            npc_ooze_littleAI(Creature* creature) : ScriptedAI(creature)
            {
                instance = creature->GetInstanceScript();
            }

            void Reset()
            {
                uiStickyOozeTimer = 10000;
                bMerge = false;

                DoCast(SPELL_RADIATING_OOZE);
                DoCast(SPELL_LITTLE_OOZE);
                me->AddUnitMovementFlag(MOVEMENTFLAG_WALKING);
                me->SetSpeed(MOVE_WALK, 0.5f);
                me->SetSpeed(MOVE_RUN, 0.5f);
            }

            void EnterCombat(Unit* /*who*/) { }

            void KilledUnit(Unit* /*victim*/) { }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (uiStickyOozeTimer <= diff)
                {
                    DoCast(me->getVictim(), SPELL_STICKY_OOZE);
                    uiStickyOozeTimer = 11000;
                } else uiStickyOozeTimer -= diff;

                DoMeleeAttackIfReady();
            }
        private:
            InstanceScript* instance;
            uint32 uiStickyOozeTimer;
            bool bMerge;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_ooze_littleAI(creature);
        }
};

class npc_icc_puddle_stalker : public CreatureScript
{
    public:
        npc_icc_puddle_stalker() : CreatureScript("npc_icc_puddle_stalker") { }

        struct npc_icc_puddle_stalkerAI : public ScriptedAI
        {
            npc_icc_puddle_stalkerAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset()
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                me->SetInCombatWithZone();
                DoStartNoMovement(me->getVictim());

                DoCast(me, SPELL_OOZE_FLOOD_AURA);
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_icc_puddle_stalkerAI(creature);
        }
};

class npc_ooze_explode_stalker : public CreatureScript
{
    public:
        npc_ooze_explode_stalker() : CreatureScript("npc_ooze_explode_stalker") { }

        struct npc_ooze_explode_stalkerAI : public ScriptedAI
        {
            npc_ooze_explode_stalkerAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset()
            {
                uiExplosionTimer = 2000;

                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                DoStartNoMovement(me->getVictim());
                me->SetInCombatWithZone();
                DoCast(SPELL_EXPLOSION_1);
            }

            void UpdateAI(const uint32 uiDiff)
            {
                if (uiExplosionTimer <= uiDiff)
                {
                    DoCast(SPELL_EXPLOSION_1);
                    me->ForcedDespawn();
                    uiExplosionTimer = 2000;
                } else uiExplosionTimer -= uiDiff;
            }
        private:
            uint32 uiExplosionTimer;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_ooze_explode_stalkerAI(creature);
        }
};

class npc_precious_icc : public CreatureScript
{
    public:
        npc_precious_icc() : CreatureScript("npc_precious_icc") { }

        struct npc_precious_iccAI : public ScriptedAI
        {
            npc_precious_iccAI(Creature* creature) : ScriptedAI(creature)
            {
                instance = creature->GetInstanceScript();
            }

            void Reset()
            {
                uiDecimateTimer = 23000;
                uiMortalTimer = urand(8000, 10000);

                DoCast(me, SPELL_PRECIOUS_RIBBON);
            }

            void UpdateAI(const uint32 uiDiff)
            {
                if (!UpdateVictim())
                    return;

                if(uiDecimateTimer < uiDiff)
                {
                    DoCast(me->getVictim(), SPELL_DECIMATE);
                    uiDecimateTimer = 23000;
                } else uiDecimateTimer -= uiDiff;

                if(uiMortalTimer < uiDiff)
                {
                    DoCast(me->getVictim(), SPELL_MORTAL_WOUND);
                    uiMortalTimer = urand(8000,10000);
                } else uiMortalTimer -= uiDiff;

                DoMeleeAttackIfReady();
            }

            void JustDied(Unit* who)
            {
                uint64 rotfaceGUID = instance ? instance->GetData64(DATA_ROTFACE) : 0;
                if (Creature *rotface = me->GetCreature(*me, rotfaceGUID))
                    DoScriptText(SAY_PRECIOUS, rotface);
            }
        private:
            InstanceScript* instance;
            uint32 uiDecimateTimer;
            uint32 uiMortalTimer;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_precious_iccAI(creature);
        }
};

class spell_rotface_unstable_explosion : public SpellScriptLoader
{
    public:
        spell_rotface_unstable_explosion() : SpellScriptLoader("spell_rotface_unstable_explosion") { } //69558


        class spell_rotface_unstable_explosion_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_rotface_unstable_explosion_AuraScript)

            void OnApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                if(GetStackAmount() >= 5)
                {
                    GetCaster()->RemoveAurasDueToSpell(SPELL_UNSTABLE_OOZE);
                    GetCaster()->CastSpell(GetCaster(), SPELL_EXPLOSION, true);
                    DoScriptText(EMOTE_EXPLOSION, GetCaster());
                    if (InstanceScript* instance = GetCaster()->GetInstanceScript())
                        if (Creature* rotface = Unit::GetCreature(*GetCaster(), instance->GetData64(DATA_ROTFACE)))
                            if (rotface->isAlive())
                                DoScriptText(SAY_EXPLOSION, rotface);
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_rotface_unstable_explosion_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_rotface_unstable_explosion_AuraScript();
        }
};

class spell_rotface_little_ooze_combine : public SpellScriptLoader
{
    public:
        spell_rotface_little_ooze_combine() : SpellScriptLoader("spell_rotface_little_ooze_combine") { }


        class spell_rotface_little_ooze_combine_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rotface_little_ooze_combine_SpellScript);

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                if (!(GetHitUnit() && GetHitUnit()->isAlive()))
                    return;

                GetCaster()->RemoveAurasDueToSpell(SPELL_LITTLE_OOZE);
                GetHitUnit()->RemoveAurasDueToSpell(SPELL_LITTLE_OOZE);
                GetHitUnit()->CastSpell(GetCaster(), SPELL_MERGE_OOZE, true);
                if (TempSummon* summ = GetHitUnit()->ToTempSummon())
                    summ->UnSummon();
                else
                    GetHitCreature()->ForcedDespawn();
            }

            void Register()
            {
                OnEffect += SpellEffectFn(spell_rotface_little_ooze_combine_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rotface_little_ooze_combine_SpellScript();
        }
};

class spell_rotface_large_ooze_combine : public SpellScriptLoader
{
    public:
        spell_rotface_large_ooze_combine() : SpellScriptLoader("spell_rotface_large_ooze_combine") { }


        class spell_rotface_large_ooze_combine_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rotface_large_ooze_combine_SpellScript);

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                if (!(GetHitUnit() && GetHitUnit()->isAlive()))
                    return;

                if (Aura* unstable = GetCaster()->GetAura(SPELL_UNSTABLE_OOZE))
                {
                    if (Aura* targetAura = GetHitUnit()->GetAura(SPELL_UNSTABLE_OOZE))
                        unstable->ModStackAmount(targetAura->GetStackAmount());
                    else
                        unstable->ModStackAmount(1);
                }

                GetHitUnit()->RemoveAurasDueToSpell(SPELL_LARGE_OOZE);

                if (TempSummon* summ = GetHitUnit()->ToTempSummon())
                    summ->UnSummon();
                else if (GetHitCreature())
                    GetHitCreature()->ForcedDespawn();
            }

            void Register()
            {
                OnEffect += SpellEffectFn(spell_rotface_large_ooze_combine_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };


        SpellScript* GetSpellScript() const
        {
            return new spell_rotface_large_ooze_combine_SpellScript();
        }
};

void AddSC_boss_rotface()
{
    new boss_rotface();
    new npc_ooze_big();
    new npc_ooze_little();
    new npc_icc_puddle_stalker();
    new npc_ooze_explode_stalker();
    new npc_precious_icc();
    new spell_rotface_unstable_explosion();
    new spell_rotface_little_ooze_combine();
    new spell_rotface_large_ooze_combine();
}
