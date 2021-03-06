/* This file is part of the ScriptDev2 Project. See AUTHORS file for Copyright information
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* ScriptData
SDName: boss_kiljaeden
SD%Complete: 90
SDComment: Sinister Reflection needs AI support.
SDCategory: Sunwell Plateau
EndScriptData */

#include "AI/ScriptDevAI/include/sc_common.h"
#include "sunwell_plateau.h"
#include "Entities/TemporarySpawn.h"

enum
{
    SAY_EMERGE                  = -1580069,
    SAY_SLAY_1                  = -1580070,
    SAY_SLAY_2                  = -1580071,
    SAY_REFLECTION_1            = -1580072,
    SAY_REFLECTION_2            = -1580073,
    SAY_DARKNESS_1              = -1580074,
    SAY_DARKNESS_2              = -1580075,
    SAY_DARKNESS_3              = -1580076,
    SAY_PHASE_3                 = -1580077,
    SAY_PHASE_4                 = -1580078,
    SAY_PHASE_5                 = -1580079,
    SAY_KALECGOS_INTRO          = -1580080,
    SAY_KALECGOS_AWAKE_1        = -1580081,
    SAY_ANVEENA_IMPRISONED      = -1580082,
    SAY_KALECGOS_AWAKE_2        = -1580083,
    SAY_ANVEENA_LOST            = -1580084,
    SAY_KALECGOS_AWAKE_4        = -1580085,
    SAY_ANVEENA_AWAKE           = -1580086,
    SAY_KALECGOS_AWAKE_5        = -1580087,
    SAY_ANVEENA_SACRIFICE       = -1580088,
    SAY_KALECGOS_GOODBYE        = -1580089,
    SAY_KALECGOS_ENCOURAGE      = -1580090,
    SAY_KALECGOS_ORB_1          = -1580091,
    SAY_KALECGOS_ORB_2          = -1580092,
    SAY_KALECGOS_ORB_3          = -1580093,
    SAY_KALECGOS_ORB_4          = -1580094,

    // outro
    SAY_OUTRO_1                 = -1580095,         // Velen
    SAY_OUTRO_2                 = -1580096,
    SAY_OUTRO_3                 = -1580097,
    SAY_OUTRO_4                 = -1580098,
    SAY_OUTRO_5                 = -1580099,         // Liadrin
    SAY_OUTRO_6                 = -1580100,         // Velen
    SAY_OUTRO_7                 = -1580101,         // Liadrin
    SAY_OUTRO_8                 = -1580102,         // Velen
    SAY_OUTRO_9                 = -1580103,
    SAY_OUTRO_10                = -1580104,         // Liadrin
    SAY_OUTRO_11                = -1580105,         // Velen
    SAY_OUTRO_12                = -1580106,

    // generic spells
    SPELL_BIRTH                 = 37745,            // Kiljaeden spawn animation 鸡蛋出生

    // transition spells
    SPELL_DESTROY_DRAKES        = 46707,//毁灭所有龙
    SPELL_SINISTER_REFLECTION   = 45892, //邪恶镜像
    SPELL_SHADOW_SPIKE          = 46680, //暗影之刺

    // phase 1
    SPELL_SOUL_FLY              = 45442,//灵魂鞭笞
    SPELL_LEGION_LIGHTING       = 45664,//军团闪电
    SPELL_FIRE_BLOOM            = 45641,//火焰之花

    // phase 2
    SPELL_FLAME_DART            = 45740,//烈焰之刺
    SPELL_DARKNESS_OF_SOULS     = 46605,//"千魂之暗",伴随着“基尔加丹开始引导黑暗能量”的提示，基尔加丹的双翼将其遮盖

    // phase 3
    SPELL_ARMAGEDDON            = 45921,        //召唤流星 used from 50% hp - summons 25735 on target location

    // Npc spells
    SPELL_SHADOW_BOLT_AURA      = 45679,        // periodic aura on shield orbs 暗影箭
    SPELL_RING_BLUE_FLAME       = 45825,        // cast by the orb targets when activated 蓝色烈焰之环
    SPELL_ANVEENA_PRISON        = 46367,		//安薇娜的牢笼
    SPELL_SACRIFICE_ANVEENA     = 46474,		//安薇娜的牺牲 使鸡蛋受到的神圣伤害提高100%?
    SPELL_ARCANE_BOLT           = 45670,        // used by Kalec 奥术之箭
    SPELL_SINISTER_REFL_CLASS   = 45893,        // increase the size of the clones 邪恶镜像职业
    SPELL_SINISTER_REFL_CLONE   = 45785,        // clone the player 邪恶镜像克隆
    SPELL_VENGEANCE_BLUE_FLIGHT = 45839,        // possess the dragon  宝珠灌注能量，玩家可以使用这些宝珠获得 [蓝龙的复仇] 效果，该效果会将使用者变成一条蓝龙，持续2分钟
    SPELL_POSSESS_DRAKE_IMMUNE  = 45838,        // immunity while the player possesses the dragon  龙类免疫

    // Npcs
    NPC_SHIELD_ORB              = 25502,
    NPC_SINISTER_REFLECTION     = 25708,
    NPC_ARMAGEDDON              = 25735,        // npc handled by eventAI
    NPC_BLUE_ORB_TARGET         = 25640,        // dummy npc near gameobjects 187869, 188114, 188115, 188116

    // phases
    PHASE_INFERNO               = 1,
    PHASE_DARKNESS              = 2,
    PHASE_ARMAGEDDON            = 3,
    PHASE_SACRIFICE             = 4,
    PHASE_TRANSITION            = 5,

    // dummy members, used in the phase switch event
    EVENT_SWITCH_PHASE_2        = 6,
    EVENT_SWITCH_PHASE_3        = 7,
    EVENT_SWITCH_PHASE_4        = 8,
    EVENT_DRAGON_ORB            = 9,

    // outro
    SPELL_TELEPORT_VISUAL       = 35517,
    SPELL_KALEC_TELEPORT        = 46473,            // teleports and transforms Kalec in human form
    SPELL_ARCANE_PORTAL         = 42047,
    SPELL_CALL_ENTROPIUS        = 46818,
    SPELL_ENTROPIUS_BODY        = 46819,
    SPELL_BLAZE_TO_LIGHT        = 46821,
    SPELL_SUNWELL_IGNITION      = 46822,
    SPELL_PORTAL_FROM_SHATTRATH = 46801,            // Riftwalker after reached point 1, summon NPC_RIFTWALKER and cast on him this spell.

    NPC_INERT_PORTAL            = 26254,
    NPC_CORE_ENTROPIUS          = 26262,
    NPC_SOLDIER                 = 26259,            // summoned in 2 waves before Velen. Should move into 2 circle formations
    NPC_RIFTWALKER              = 26289,
    NPC_SHATTRATH_PORTAL        = 26251,

    POINT_SUMMON_SOLDIERS       = 1,
    POINT_MOVE_LIADRIN          = 2,
    POINT_EVENT_EXIT            = 3,
};

// Encounter phase dialogue
static const DialogueEntry aPhaseDialogue[] =
{
    {PHASE_DARKNESS,            0,              2000},
    {EVENT_SWITCH_PHASE_2,      0,              17000},
    {SAY_KALECGOS_AWAKE_1,      NPC_KALECGOS,   6000},
    {SAY_ANVEENA_IMPRISONED,    NPC_ANVEENA,    5000},
    {SAY_PHASE_3,               NPC_KILJAEDEN,  6000},
    {SAY_KALECGOS_ORB_1,        NPC_KALECGOS,   0},         // phase 2 transition end
    {PHASE_ARMAGEDDON,          0,              2000},
    {EVENT_SWITCH_PHASE_3,      0,              14000},
    {SAY_KALECGOS_AWAKE_2,      NPC_KALECGOS,   7000},
    {SAY_ANVEENA_LOST,          NPC_ANVEENA,    7000},
    {SAY_PHASE_4,               NPC_KILJAEDEN,  6000},
    {EVENT_DRAGON_ORB,          0,              0},         // phase 3 transition end
    {PHASE_SACRIFICE,           0,              2000},
    {EVENT_SWITCH_PHASE_4,      0,              5000},
    {SAY_KALECGOS_AWAKE_4,      NPC_KALECGOS,   10000},
    {SAY_ANVEENA_AWAKE,         NPC_ANVEENA,    2000},
    {SAY_KALECGOS_AWAKE_5,      NPC_KALECGOS,   6000},
    {SAY_ANVEENA_SACRIFICE,     NPC_ANVEENA,    5000},
    {SAY_PHASE_5,               NPC_KILJAEDEN,  13000},
    {SAY_KALECGOS_ORB_4,        NPC_KALECGOS,   5000},
    {SAY_KALECGOS_ENCOURAGE,    NPC_KALECGOS,   0},         // phase 4 transition end
    {0, 0, 0},
};

// Epilogue dialogue
static const DialogueEntry aOutroDialogue[] =
{
    {NPC_KALECGOS,          0,              15000},
    {SAY_KALECGOS_GOODBYE,  NPC_KALECGOS,   8000},
    {NPC_RIFTWALKER,        0,              11000},
    {NPC_INERT_PORTAL,      0,              10000},
    {POINT_SUMMON_SOLDIERS, 0,              18000},
    {NPC_VELEN,             0,              1000},
    {NPC_LIADRIN,           0,              4000},
    {SAY_OUTRO_1,           NPC_VELEN,      25000},
    {SAY_OUTRO_2,           NPC_VELEN,      15000},
    {SAY_OUTRO_3,           NPC_VELEN,      13000},
    {SPELL_CALL_ENTROPIUS,  0,              10000},
    {SAY_OUTRO_4,           NPC_VELEN,      20000},
    {POINT_MOVE_LIADRIN,    0,              5000},
    {SAY_OUTRO_5,           NPC_LIADRIN,    10000},
    {SAY_OUTRO_6,           NPC_VELEN,      15000},
    {SAY_OUTRO_7,           NPC_LIADRIN,    3000},
    {SAY_OUTRO_8,           NPC_VELEN,      4000},
    {SPELL_BLAZE_TO_LIGHT,  0,              13000},
    {SAY_OUTRO_9,           NPC_VELEN,      14000},
    {SAY_OUTRO_10,          NPC_LIADRIN,    20000},
    {SAY_OUTRO_11,          NPC_VELEN,      8000},
    {SAY_OUTRO_12,          NPC_VELEN,      4000},
    {POINT_EVENT_EXIT,      0,              0},
    {0, 0, 0},
};

static const EventLocations aOutroLocations[] =
{
    {1725.469f, 650.939f, 30.314f, 3.78f},      // portal summon loc
    {1717.776f, 645.178f, 28.223f, 3.83f},      // velen summon loc
    {1720.024f, 643.233f, 28.133f, 3.76f},      // liadrin summon loc
    {1712.110f, 641.044f, 27.80f},              // velen move forward
    {1711.537f, 637.600f, 27.34f},              // liadrin move forward
    {1698.946f, 628.206f, 83.003f, 0.76f},      // entropius core summon loc
};

// Note: the Z loc should be 143.69 but currently we are not using it because it's too far away
static const float aKalegSpawnLoc[4] = {1734.431f, 593.1974f, 130.6977f, 4.55f};

/*######
## npc_kiljaeden_controller
######*/

struct npc_kiljaeden_controllerAI : public Scripted_NoMovementAI, private DialogueHelper
{
    npc_kiljaeden_controllerAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature),
        DialogueHelper(aOutroDialogue)
    {
        m_pInstance = ((instance_sunwell_plateau*)pCreature->GetInstanceData());
        InitializeDialogueHelper(m_pInstance);
        Reset();
    }

    instance_sunwell_plateau* m_pInstance;

    ObjectGuid m_EntropiusGuid;
    ObjectGuid m_PortalGuid;
    ObjectGuid m_firstRiftwalkerGuid;
    ObjectGuid m_secondRiftwalkerGuid;

    void Reset() override
    {
        // Visual spell before the encounter starts
        DoCastSpellIfCan(m_creature, SPELL_ANVEENA_DRAIN);
    }

    void JustDidDialogueStep(int32 iEntry) override
    {
        if (!m_pInstance)
            return;

        switch (iEntry)
        {
            case NPC_KALECGOS:
                if (Creature* pKalec = m_pInstance->GetSingleCreatureFromStorage(NPC_KALECGOS))
                {
                    pKalec->GetMotionMaster()->Clear();
                    pKalec->GetMotionMaster()->MoveIdle();
                    pKalec->CastSpell(pKalec, SPELL_KALEC_TELEPORT, TRIGGERED_OLD_TRIGGERED);
                    pKalec->SetLevitate(false);
                }
                m_creature->SummonCreature(NPC_CORE_ENTROPIUS, aOutroLocations[5].m_fX, aOutroLocations[5].m_fY, aOutroLocations[5].m_fZ, aOutroLocations[5].m_fO, TEMPSPAWN_CORPSE_DESPAWN, 0);
                break;
            case NPC_RIFTWALKER:
                if (Creature* pRiftwalker = m_creature->SummonCreature(NPC_RIFTWALKER, 1688.42f, 641.82f, 27.60f, 0.67f, TEMPSPAWN_DEAD_DESPAWN, 0))
                {
                    pRiftwalker->SetWalk(false);
                    pRiftwalker->CastSpell(pRiftwalker, SPELL_TELEPORT_VISUAL, TRIGGERED_OLD_TRIGGERED);
                    pRiftwalker->GetMotionMaster()->MovePoint(1, 1727.08f, 656.82f, 28.37f);
                    m_firstRiftwalkerGuid = pRiftwalker->GetObjectGuid();
                }
                if (Creature* pRiftwalker = m_creature->SummonCreature(NPC_RIFTWALKER, 1712.58f, 616.29f, 27.78f, 0.76f, TEMPSPAWN_DEAD_DESPAWN, 0))
                {
                    pRiftwalker->SetWalk(false);
                    pRiftwalker->CastSpell(pRiftwalker, SPELL_TELEPORT_VISUAL, TRIGGERED_OLD_TRIGGERED);
                    pRiftwalker->GetMotionMaster()->MovePoint(1, 1738.84f, 627.32f, 28.26f);
                    m_secondRiftwalkerGuid = pRiftwalker->GetObjectGuid();
                }
                break;
            case NPC_INERT_PORTAL:
                // ToDo: summon soldiers to the right
                m_creature->SummonCreature(NPC_INERT_PORTAL, aOutroLocations[0].m_fX, aOutroLocations[0].m_fY, aOutroLocations[0].m_fZ, aOutroLocations[0].m_fO, TEMPSPAWN_CORPSE_DESPAWN, 0);
                break;
            case POINT_SUMMON_SOLDIERS:
                // ToDo: summon soldiers to the left
                break;
            case NPC_VELEN:
                m_creature->SummonCreature(NPC_VELEN, aOutroLocations[1].m_fX, aOutroLocations[1].m_fY, aOutroLocations[1].m_fZ, aOutroLocations[1].m_fO, TEMPSPAWN_CORPSE_DESPAWN, 0);
                break;
            case NPC_LIADRIN:
                m_creature->SummonCreature(NPC_LIADRIN, aOutroLocations[2].m_fX, aOutroLocations[2].m_fY, aOutroLocations[2].m_fZ, aOutroLocations[2].m_fO, TEMPSPAWN_TIMED_DESPAWN, 4 * MINUTE * IN_MILLISECONDS);
                break;
            case SPELL_CALL_ENTROPIUS:
                if (Creature* pVelen = m_pInstance->GetSingleCreatureFromStorage(NPC_VELEN))
                    pVelen->CastSpell(pVelen, SPELL_CALL_ENTROPIUS, TRIGGERED_NONE);
                // Set point id = 1 for movement event
                if (Creature* pEntropius = m_creature->GetMap()->GetCreature(m_EntropiusGuid))
                {
                    pEntropius->SetWalk(false);
                    pEntropius->GetMotionMaster()->MovePoint(1, m_creature->GetPositionX(), m_creature->GetPositionY(), 35.0f);
                }
                break;
            case POINT_MOVE_LIADRIN:
                if (Creature* pLiadrin = m_pInstance->GetSingleCreatureFromStorage(NPC_LIADRIN))
                    pLiadrin->GetMotionMaster()->MovePoint(0, aOutroLocations[4].m_fX, aOutroLocations[4].m_fY, aOutroLocations[4].m_fZ);
                break;
            case SPELL_BLAZE_TO_LIGHT:
                if (Creature* pEntropius = m_creature->GetMap()->GetCreature(m_EntropiusGuid))
                {
                    pEntropius->CastSpell(pEntropius, SPELL_BLAZE_TO_LIGHT, TRIGGERED_OLD_TRIGGERED);
                    pEntropius->RemoveAurasDueToSpell(SPELL_ENTROPIUS_BODY);
                    pEntropius->SetWalk(true);
                    pEntropius->GetMotionMaster()->MovePoint(2, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ());
                }
                break;
            case POINT_EVENT_EXIT:
                // Set point id = 1 for the despawn event
                if (Creature* pVelen = m_pInstance->GetSingleCreatureFromStorage(NPC_VELEN))
                    pVelen->GetMotionMaster()->MovePoint(1, aOutroLocations[1].m_fX, aOutroLocations[1].m_fY, aOutroLocations[1].m_fZ);
                break;
        }
    }

    void JustSummoned(Creature* pSummoned) override
    {
        switch (pSummoned->GetEntry())
        {
            case NPC_VELEN:
                pSummoned->CastSpell(pSummoned, SPELL_TELEPORT_VISUAL, TRIGGERED_OLD_TRIGGERED);
                pSummoned->GetMotionMaster()->MovePoint(0, aOutroLocations[3].m_fX, aOutroLocations[3].m_fY, aOutroLocations[3].m_fZ);
            // no break here
            case NPC_LIADRIN:
                pSummoned->CastSpell(pSummoned, SPELL_TELEPORT_VISUAL, TRIGGERED_OLD_TRIGGERED);
                break;
            case NPC_CORE_ENTROPIUS:
                pSummoned->CastSpell(pSummoned, SPELL_ENTROPIUS_BODY, TRIGGERED_OLD_TRIGGERED);
                pSummoned->SetLevitate(true);
                m_EntropiusGuid = pSummoned->GetObjectGuid();
                break;
            case NPC_INERT_PORTAL:
                m_PortalGuid = pSummoned->GetObjectGuid();
                pSummoned->CastSpell(pSummoned, SPELL_ARCANE_PORTAL, TRIGGERED_OLD_TRIGGERED);
                break;
        }
    }

    void SummonedCreatureJustDied(Creature* pSummoned) override
    {
        // Start outro dialogue when Kil'jaeden is killed
        if (pSummoned->GetEntry() == NPC_KILJAEDEN)
            StartNextDialogueText(NPC_KALECGOS);
    }

    void SummonedMovementInform(Creature* pSummoned, uint32 uiType, uint32 uiPointId) override
    {
        if (uiType != POINT_MOTION_TYPE)
            return;

        if (uiPointId == 1)
        {
            if (pSummoned->GetEntry() == NPC_CORE_ENTROPIUS)
            {
                // Interrupt Velen's casting when entropius has reached the ground
                if (Creature* pVelen = m_pInstance->GetSingleCreatureFromStorage(NPC_VELEN))
                    pVelen->InterruptNonMeleeSpells(false);
            }
            else if (pSummoned->GetEntry() == NPC_VELEN)
            {
                // Cast teleport and despawn Velen, the portal and Kalec; Liadrin will despawn on timer
                pSummoned->CastSpell(pSummoned, SPELL_TELEPORT_VISUAL, TRIGGERED_OLD_TRIGGERED);
                pSummoned->ForcedDespawn(1000);

                // Note: portal should despawn only after all the soldiers have reached this point and "teleported" outside
                if (Creature* pPortal = m_creature->GetMap()->GetCreature(m_PortalGuid))
                    pPortal->ForcedDespawn(5000);

                if (Creature* pKalec = m_pInstance->GetSingleCreatureFromStorage(NPC_KALECGOS))
                    pKalec->ForcedDespawn(1000);
            }
        }
        else if (uiPointId == 2 && pSummoned->GetEntry() == NPC_CORE_ENTROPIUS)
        {
            // When the purified Muru reaches the ground the sunwell ignites and Muru despawns
            DoCastSpellIfCan(m_creature, SPELL_SUNWELL_IGNITION);

            if (Creature* pLiadrin = m_pInstance->GetSingleCreatureFromStorage(NPC_LIADRIN))
                pLiadrin->SetStandState(UNIT_STAND_STATE_KNEEL);

            pSummoned->ForcedDespawn();
        }
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        DialogueUpdate(uiDiff);
    }
};

/*######
## boss_kiljaeden
######*/

struct boss_kiljaedenAI : public Scripted_NoMovementAI, private DialogueHelper
{
    boss_kiljaedenAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature),
        DialogueHelper(aPhaseDialogue)
    {
        m_pInstance = ((instance_sunwell_plateau*)pCreature->GetInstanceData());
        InitializeDialogueHelper(m_pInstance);
        Reset();
    }

    instance_sunwell_plateau* m_pInstance;

    uint8 m_uiPhase;
    uint8 m_uiMaxShieldOrbs;
    uint8 m_uiShieldOrbCount;

    uint32 m_uiKalecSummonTimer;

    uint32 m_uiSoulFlyTimer;
    uint32 m_uiLegionLightingTimer;
    uint32 m_uiFireBloomTimer;
    uint32 m_uiShieldOrbTimer;

    uint32 m_uiFlameDartTimer;
    uint32 m_uiDarknessOfSoulsTimer;

    uint32 m_uiArmageddonTimer;
	uint32 m_countLIUXIONG;
	uint32 m_uishottTimer;
	Player* jingxiangplayer;
    void Reset() override
    {
        m_uiPhase                   = PHASE_INFERNO;
        m_uiKalecSummonTimer        = 35000;
        m_uiMaxShieldOrbs           = 1;
        m_uiShieldOrbCount          = 0;
		m_countLIUXIONG				= 3;//释放3个流星
        m_uiSoulFlyTimer            = 10000;
        m_uiLegionLightingTimer     = urand(10000, 15000);
        m_uiFireBloomTimer          = urand(15000, 20000);
        m_uiShieldOrbTimer          = 30000;

        m_uiFlameDartTimer          = urand(20000, 25000);
        m_uiDarknessOfSoulsTimer    = urand(45000, 50000);

        m_uiArmageddonTimer         = 20000;
		m_uishottTimer = 0;
    }

    void Aggro(Unit* /*pWho*/) override
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_KILJAEDEN, IN_PROGRESS);

        DoScriptText(SAY_EMERGE, m_creature);
        DoCastSpellIfCan(m_creature, SPELL_BIRTH);
    }

    void JustReachedHome() override
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_KILJAEDEN, FAIL);

            // Reset the corrupt Sunwell aura
            if (Creature* pKiljaedenController = m_pInstance->GetSingleCreatureFromStorage(NPC_KILJAEDEN_CONTROLLER))
                pKiljaedenController->CastSpell(pKiljaedenController, SPELL_ANVEENA_DRAIN, TRIGGERED_OLD_TRIGGERED);
        }

        // Despawn on wipe
        m_creature->ForcedDespawn();
    }

    void KilledUnit(Unit* pVictim) override
    {
        if (pVictim->GetTypeId() != TYPEID_PLAYER)
            return;

        DoScriptText(urand(0, 1) ? SAY_SLAY_1 : SAY_SLAY_2, m_creature);
    }

    void JustDied(Unit* /*pKiller*/) override
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_KILJAEDEN, DONE);
    }

    void JustSummoned(Creature* pSummoned) override
    {
        if (pSummoned->GetEntry() == NPC_KALECGOS)
        {
            DoScriptText(SAY_KALECGOS_INTRO, pSummoned);
            pSummoned->CastSpell(pSummoned, SPELL_ARCANE_BOLT, TRIGGERED_OLD_TRIGGERED);
            pSummoned->GetMotionMaster()->MoveRandomAroundPoint(m_creature->GetPositionX(), m_creature->GetPositionY(), pSummoned->GetPositionZ(), 30.0f);
        }
        else if (pSummoned->GetEntry() == NPC_SHIELD_ORB)
        {
            //pSummoned->CastSpell(pSummoned, SPELL_SHADOW_BOLT_AURA, TRIGGERED_OLD_TRIGGERED);

            // Start the movement of the shadow orb - calculate new position based on the angle between the boss and orb
            float fX, fY;
            float fAng = m_creature->GetAngle(pSummoned) + M_PI_F / 8;
            // Normalize angle
            if (fAng > 2 * M_PI_F)
                fAng = fAng - 2 * M_PI_F;

            m_creature->GetNearPoint2d(fX, fY, 25.0f, fAng);

            // Move to new position
            pSummoned->GetMotionMaster()->Clear();
            pSummoned->GetMotionMaster()->MovePoint(1, fX, fY, pSummoned->GetPositionZ());
        }
        else if (pSummoned->GetEntry() == NPC_SINISTER_REFLECTION)
        {
            if (pSummoned->IsTemporarySummon())
            {
                //if (Player* pPlayer = m_creature->GetMap()->GetPlayer(pSummoned->GetSpawnerGuid()))
				if(jingxiangplayer)
                {
					//jingxiangplayer->CastSpell(pSummoned, SPELL_SINISTER_REFL_CLONE, TRIGGERED_OLD_TRIGGERED);
					//jingxiangplayer->CastSpell(pSummoned, SPELL_SINISTER_REFL_CLASS, TRIGGERED_OLD_TRIGGERED);
                    pSummoned->AI()->AttackStart(jingxiangplayer);
                }
            }
        }
		else if (pSummoned->GetEntry() == 25735) {
			//调整高度
			//float fX, fY, fZ;
			//pSummoned->GetRandomPoint(pSummoned->GetPositionX(), pSummoned->GetPositionY(), pSummoned->GetPositionZ(), 2.0f, fX, fY, fZ);
			//pSummoned->NearTeleportTo(fX, fY, fZ+15, pSummoned->GetOrientation());//增加15码高度
			pSummoned->GetMotionMaster()->Clear();
			pSummoned->GetMotionMaster()->MoveIdle();
			pSummoned->StopMoving(true);
			sLog.outError("25735 need to move to  a player stationg");
			
		}
    }

    void SummonedCreatureJustDied(Creature* pSummoned) override
    {
        if (pSummoned->GetEntry() == NPC_SHIELD_ORB)
            --m_uiShieldOrbCount;
    }

    void GetAIInformation(ChatHandler& reader) override
    {
        reader.PSendSysMessage("Kil'jaeden is currently in phase %u", uint32(m_uiPhase));
    }

    void JustDidDialogueStep(int32 iEntry) override
    {
        if (!m_pInstance)
            return;

		
        switch (iEntry)
        {
            case PHASE_DARKNESS:
            case PHASE_ARMAGEDDON:
            case PHASE_SACRIFICE:

				if (Player* player = m_pInstance->SelectRandomAliveWithDist(m_creature, m_pInstance, 40, true)) {
						jingxiangplayer = player;
					for (uint8 i = 0; i < 4; ++i) {
						player->CastSpell(player, 45891, TRIGGERED_NONE, nullptr, nullptr, m_creature->GetObjectGuid());
					}

					DoScriptText(irand(0, 1) ? SAY_REFLECTION_1 : SAY_REFLECTION_2, m_creature);
					// In the 2nd and 3rd transition kill all drakes
					if (iEntry == PHASE_ARMAGEDDON || iEntry == PHASE_SACRIFICE)
						DoCastSpellIfCan(m_creature, SPELL_DESTROY_DRAKES, CAST_TRIGGERED);

					m_uiPhase = PHASE_TRANSITION;
					// Darkness of Souls needs the timer reseted
					m_uiDarknessOfSoulsTimer = iEntry == PHASE_SACRIFICE ? 30000 : 45000;
				}
                //if (DoCastSpellIfCan(m_creature, SPELL_SINISTER_REFLECTION, CAST_INTERRUPT_PREVIOUS) == CAST_OK)
                //{
                //    DoScriptText(irand(0, 1) ? SAY_REFLECTION_1 : SAY_REFLECTION_2, m_creature);

                //    // In the 2nd and 3rd transition kill all drakes
                //    if (iEntry == PHASE_ARMAGEDDON || iEntry == PHASE_SACRIFICE)
                //        DoCastSpellIfCan(m_creature, SPELL_DESTROY_DRAKES, CAST_TRIGGERED);

                //    m_uiPhase = PHASE_TRANSITION;
                //    // Darkness of Souls needs the timer reseted
                //    m_uiDarknessOfSoulsTimer = iEntry == PHASE_SACRIFICE ? 30000 : 45000;
                //}
                break;
            case EVENT_SWITCH_PHASE_2:
            case EVENT_SWITCH_PHASE_3:
            case EVENT_SWITCH_PHASE_4:
				if (DoCastSpellIfCan(m_creature, SPELL_SHADOW_SPIKE) != CAST_OK) {
					sLog.outError("CCCC SPELL_SHADOW_SPIKE failed");
					m_uishottTimer = 1;
				}
                break;
            case EVENT_DRAGON_ORB:
                // Activate blue orbs
                if (Creature* pKalec = m_pInstance->GetSingleCreatureFromStorage(NPC_KALECGOS))
                    DoScriptText(irand(0, 1) ? SAY_KALECGOS_ORB_2 : SAY_KALECGOS_ORB_3, pKalec);
                DoActivateDragonOrb(GO_ORB_BLUE_FLIGHT_2);
                break;
            case SAY_KALECGOS_ORB_1:
                DoActivateDragonOrb(GO_ORB_BLUE_FLIGHT_1);
                break;
            case SAY_KALECGOS_ORB_4:
                DoActivateDragonOrb(GO_ORB_BLUE_FLIGHT_3);
                DoActivateDragonOrb(GO_ORB_BLUE_FLIGHT_4);
                break;
            case SAY_PHASE_3:
                // Set next phase and increase the max shield orbs
                m_uiPhase = PHASE_DARKNESS;
                ++m_uiMaxShieldOrbs;
                break;
            case SAY_PHASE_4:
                // Set next phase and increase the max shield orbs
                m_uiPhase = PHASE_ARMAGEDDON;
                ++m_uiMaxShieldOrbs;
                break;
            case SAY_PHASE_5:
                // Set next phase and sacrifice Anveena
                if (Creature* pAnveena = m_pInstance->GetSingleCreatureFromStorage(NPC_ANVEENA))
                {
                    pAnveena->RemoveAurasDueToSpell(SPELL_ANVEENA_PRISON);
                    pAnveena->CastSpell(pAnveena, SPELL_SACRIFICE_ANVEENA, TRIGGERED_OLD_TRIGGERED);
                    pAnveena->ForcedDespawn(3000);
                }
                m_uiPhase = PHASE_SACRIFICE;
                break;
        }
    }

    // Wrapper to activate dragon orbs
    void DoActivateDragonOrb(uint32 uiEntry)
    {
        if (!m_pInstance)
            return;

        // Set the visual around the Orb
        if (GameObject* pGo = m_pInstance->GetSingleGameObjectFromStorage(uiEntry))
        {
            if (Creature* pTarget = GetClosestCreatureWithEntry(pGo, NPC_BLUE_ORB_TARGET, 5.0f))
                pTarget->CastSpell(pTarget, SPELL_RING_BLUE_FLAME, TRIGGERED_NONE);//使宝珠可以用
        }

        // Make the orb usable
        m_pInstance->DoToggleGameObjectFlags(uiEntry, GO_FLAG_NO_INTERACT, false);
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        DialogueUpdate(uiDiff);

		//AI前面增加个循环判断
		if (Spell* curr=m_creature->GetCurrentSpell(CURRENT_CHANNELED_SPELL)) {//3秒一个暗影刺
			if (curr->m_spellInfo->Id == 46680) {//如果在引导暗影之刺
				if (m_uishottTimer < uiDiff)
				{
					sLog.outError("CURRENT_CHANNELED_SPELL %u", curr->m_spellInfo->Id);
					if (Player* player = m_pInstance->SelectRandomAliveWithDist(m_creature, m_pInstance, 40, true)) {
						//if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, nullptr, SELECT_FLAG_PLAYER)) {
						m_creature->CastSpell(player, 46589, TRIGGERED_OLD_TRIGGERED);//直接发送一个刺
					}
					m_uishottTimer = 3500;
				}
				else
					m_uishottTimer -= uiDiff;
			}
				return;
		}

        switch (m_uiPhase)
        {
            case PHASE_TRANSITION:
                // Transition phase is handled in the dialogue helper; however we don't want the spell timers to be decreased so we use a specific phase
                break;
            case PHASE_SACRIFICE:
            // Final phase - use the same spells
            // no break;
            case PHASE_ARMAGEDDON:

                // In the last phase he uses Armageddon continuously


				if (m_uiArmageddonTimer < uiDiff)
				{
					if (m_countLIUXIONG > 0) {
						m_countLIUXIONG--;
					}
						if(m_countLIUXIONG==2)
							DoCastSpellIfCan(m_creature, 45914, CAST_TRIGGERED);//火焰效果
					if (Player* pTarget = m_pInstance->SelectRandomAliveWithDist(m_creature, m_pInstance, 40, true)) {
						CanCastResult ret = DoCastSpellIfCan(pTarget, 45910, CAST_TRIGGERED);
						m_uiArmageddonTimer = m_uiPhase == PHASE_SACRIFICE ? 2000 : 3000;//三秒间隔
						sLog.outError("PPPPPPP  SPELL_ARMAGEDDON index %u ret:%u", m_countLIUXIONG, ret);//测试代码，第一阶段也释放
					}
					if (m_countLIUXIONG == 0) {
						m_uiArmageddonTimer = m_uiPhase == PHASE_SACRIFICE ? 20000 : 30000;
						sLog.outError("PPPPPPP  SPELL_ARMAGEDDON finish ！！！！！！1");
						m_countLIUXIONG = 3;
					}
					return;
				}
				else
					m_uiArmageddonTimer -= uiDiff;

                // Go to next phase and start transition dialogue
                if (m_uiPhase == PHASE_ARMAGEDDON && m_creature->GetHealthPercent() < 25.0f)
                    StartNextDialogueText(PHASE_SACRIFICE);

            // no break - use the spells from the phases below;
            case PHASE_DARKNESS:

                // In the last phase he uses this spell more often
                if (m_uiDarknessOfSoulsTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_DARKNESS_OF_SOULS) == CAST_OK)
                        m_uiDarknessOfSoulsTimer = m_uiPhase == PHASE_SACRIFICE ? 30000 : 45000;
                }
                else
                    m_uiDarknessOfSoulsTimer -= uiDiff;

                if (m_uiFlameDartTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_FLAME_DART) == CAST_OK)
                        m_uiFlameDartTimer = urand(25000, 30000);
                }
                else
                    m_uiFlameDartTimer -= uiDiff;

                // Go to next phase and start transition dialogue
                if (m_uiPhase == PHASE_DARKNESS && m_creature->GetHealthPercent() < 55.0f)
                    StartNextDialogueText(PHASE_ARMAGEDDON);

            // no break - use the spells from the phase below;
            case PHASE_INFERNO:

                if (m_uiKalecSummonTimer)
                {
                    if (m_uiKalecSummonTimer <= uiDiff)
                    {
                        m_creature->SummonCreature(NPC_KALECGOS, aKalegSpawnLoc[0], aKalegSpawnLoc[1], aKalegSpawnLoc[2], aKalegSpawnLoc[3], TEMPSPAWN_CORPSE_DESPAWN, 0);
                        m_uiKalecSummonTimer = 0;
                    }
                    else
                        m_uiKalecSummonTimer -= uiDiff;
                }

                if (m_uiLegionLightingTimer < uiDiff)
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, nullptr, SELECT_FLAG_PLAYER))
                    {
                        if (DoCastSpellIfCan(pTarget, SPELL_LEGION_LIGHTING) == CAST_OK)
                            m_uiLegionLightingTimer = urand(10000, 15000);
                    }
                }
                else
                    m_uiLegionLightingTimer -= uiDiff;

                if (m_uiFireBloomTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_FIRE_BLOOM) == CAST_OK)
                        m_uiFireBloomTimer = 20000;
                }
                else
                    m_uiFireBloomTimer -= uiDiff;

                if (m_uiSoulFlyTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature->GetVictim(), SPELL_SOUL_FLY) == CAST_OK)
                        m_uiSoulFlyTimer = urand(3000, 10000);
                }
                else
                    m_uiSoulFlyTimer -= uiDiff;

                // Only spawn a Shadow orb when necessary
                if (m_uiShieldOrbCount < m_uiMaxShieldOrbs)
                {
                    if (m_uiShieldOrbTimer < uiDiff)
                    {
                        // Get some random coords for the Orb
                        float fX, fY;
                        m_creature->GetNearPoint2d(fX, fY, 25.0f, frand(0, 2 * M_PI_F));
                        float fZ = frand(35.0f, 45.0f);

                        m_creature->SummonCreature(NPC_SHIELD_ORB, fX, fY, fZ, 0, TEMPSPAWN_CORPSE_DESPAWN, 0);
                        ++m_uiShieldOrbCount;

                        m_uiShieldOrbTimer = 30000;//宝珠的召唤周期30秒
                    }
                    else
                        m_uiShieldOrbTimer -= uiDiff;
                }



                // Go to next phase and start transition dialogue
                if (m_uiPhase == PHASE_INFERNO && m_creature->GetHealthPercent() < 85.0f)
                    StartNextDialogueText(PHASE_DARKNESS);

                DoMeleeAttackIfReady();

                break;
        }
    }
};

bool EffectAuraDummy_spell_aura_dummy_darkness_of_souls(const Aura* pAura, bool bApply)
{
    // On Aura removal cast the explosion and yell
    // This is a special case when the dummy effect should be triggered at the end of the channeling
    if (pAura->GetId() == SPELL_DARKNESS_OF_SOULS && pAura->GetEffIndex() == EFFECT_INDEX_0 && !bApply)
    {
        if (Creature* pTarget = (Creature*)pAura->GetTarget())
        {
            pTarget->CastSpell(pTarget, pAura->GetSpellProto()->CalculateSimpleValue(EFFECT_INDEX_2), TRIGGERED_OLD_TRIGGERED);

            switch (urand(0, 2))
            {
                case 0: DoScriptText(SAY_DARKNESS_1, pTarget); break;
                case 1: DoScriptText(SAY_DARKNESS_2, pTarget); break;
                case 2: DoScriptText(SAY_DARKNESS_3, pTarget); break;
            }
        }
    }
    return true;
}

/*######
## npc_shield_orb
######*/

struct npc_shield_orbAI : public ScriptedAI
{
	uint32 m_uishottTimer;
    npc_shield_orbAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((instance_sunwell_plateau*)pCreature->GetInstanceData());
        Reset();
    }

    instance_sunwell_plateau* m_pInstance;

	void Reset() override { m_uishottTimer = 2000; }

    // Handle circel movement around the boss
    void MovementInform(uint32 uiMoveType, uint32 uiPointId) override
    {
        if (uiMoveType != POINT_MOTION_TYPE || !uiPointId || !m_pInstance)
            return;

        if (Creature* pSummoner = m_pInstance->GetSingleCreatureFromStorage(NPC_KILJAEDEN))
        {
            // Calculate new position based on the angle between the boss and self
            float fX, fY;
            float fAng = pSummoner->GetAngle(m_creature) + M_PI_F / 8;
            // Normalize angle
            if (fAng > 2 * M_PI_F)
                fAng = fAng - 2 * M_PI_F;

            pSummoner->GetNearPoint2d(fX, fY, 25.0f, fAng);

            // Move to new position
            m_creature->GetMotionMaster()->Clear();
            m_creature->GetMotionMaster()->MovePoint(1, fX, fY, m_creature->GetPositionZ());
        }
    }

    void AttackStart(Unit* /*pWho*/) override {}
    void MoveInLineOfSight(Unit* /*pWho*/) override {}
    void UpdateAI(const uint32 uiDiff) override {
	
		if (m_uishottTimer < uiDiff)
		{
		
			if(Player* player=m_pInstance->SelectRandomAliveWithDist(m_creature, m_pInstance, 20, true)){
			//if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, nullptr, SELECT_FLAG_PLAYER)) {
				m_creature->CastSpell(player, 45680, TRIGGERED_OLD_TRIGGERED);//扔宝珠
			}
			m_uishottTimer = urand(2000, 3000);
		}
		else
			m_uishottTimer -= uiDiff;
	}
};

/*######
## npc_power_blue_flight
######*/

struct npc_power_blue_flightAI : public ScriptedAI
{
    npc_power_blue_flightAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        SetCombatMovement(false);
        m_bHasPossessed = false;
        Reset();
    }

    bool m_bHasPossessed;

    void Reset() override { }

    void JustDied(Unit* /*pKiller*/) override
    {
        if (m_creature->IsTemporarySummon())
        {
            if (Player* pPlayer = m_creature->GetMap()->GetPlayer(m_creature->GetSpawnerGuid()))
                pPlayer->RemoveAurasDueToSpell(SPELL_POSSESS_DRAKE_IMMUNE);
        }
    }

    void UpdateAI(const uint32 /*uiDiff*/) override
    {
        if (!m_bHasPossessed)
        {
            if (m_creature->IsTemporarySummon())
            {
                if (Player* pPlayer = m_creature->GetMap()->GetPlayer(m_creature->GetSpawnerGuid()))
                {
                    pPlayer->CastSpell(m_creature, SPELL_VENGEANCE_BLUE_FLIGHT, TRIGGERED_OLD_TRIGGERED);//45839:131="蓝龙的复仇",165="蓝龙军团的能量在你的体内奔涌。",182="为蓝龙军团复仇。"
                    pPlayer->CastSpell(pPlayer, SPELL_POSSESS_DRAKE_IMMUNE, TRIGGERED_OLD_TRIGGERED);//45838:131="龙类免疫"
                }
            }

            // Reset the No Interact flag of the closest orb
            GameObject* pOrb = GetClosestGameObjectWithEntry(m_creature, GO_ORB_BLUE_FLIGHT_1, 10.0f);
            if (!pOrb)
                pOrb = GetClosestGameObjectWithEntry(m_creature, GO_ORB_BLUE_FLIGHT_2, 10.0f);
            if (!pOrb)
                pOrb = GetClosestGameObjectWithEntry(m_creature, GO_ORB_BLUE_FLIGHT_3, 10.0f);
            if (!pOrb)
                pOrb = GetClosestGameObjectWithEntry(m_creature, GO_ORB_BLUE_FLIGHT_4, 10.0f);

            if (pOrb)
                pOrb->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_NO_INTERACT);

            m_bHasPossessed = true;
        }
    }
};

UnitAI* GetAI_boss_kiljaeden(Creature* pCreature)
{
    return new boss_kiljaedenAI(pCreature);
}

UnitAI* GetAI_npc_kiljaeden_controller(Creature* pCreature)
{
    return new npc_kiljaeden_controllerAI(pCreature);
}

UnitAI* GetAI_npc_shield_orb(Creature* pCreature)
{
    return new npc_shield_orbAI(pCreature);
}

UnitAI* GetAI_npc_power_blue_flight(Creature* pCreature)
{
    return new npc_power_blue_flightAI(pCreature);
}

void AddSC_boss_kiljaeden()
{
    Script* pNewScript = new Script;
    pNewScript->Name = "boss_kiljaeden";
    pNewScript->GetAI = &GetAI_boss_kiljaeden;
    pNewScript->pEffectAuraDummy = &EffectAuraDummy_spell_aura_dummy_darkness_of_souls;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_kiljaeden_controller";
    pNewScript->GetAI = &GetAI_npc_kiljaeden_controller;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_shield_orb";
    pNewScript->GetAI = &GetAI_npc_shield_orb;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_power_blue_flight";
    pNewScript->GetAI = &GetAI_npc_power_blue_flight;
    pNewScript->RegisterSelf();
}
