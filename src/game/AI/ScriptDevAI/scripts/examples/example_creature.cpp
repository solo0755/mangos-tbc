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
 SDName: Example_Creature
 SD%Complete: 100
 SDComment: Short custom scripting example
 SDCategory: Script Examples
 EndScriptData */

#include "AI/ScriptDevAI/include/example.h"

 // **** This script is designed as an example for others to build on ****
 // **** Please modify whatever you'd like to as this script is only for developement ****

 // **** Script Info ****
 // This script is written in a way that it can be used for both friendly and hostile monsters
 // Its primary purpose is to show just how much you can really do with scripts
 // I recommend trying it out on both an agressive NPC and on friendly npc

 // **** Quick Info ****
 // Functions with Handled Function marked above them are functions that are called automatically by the core
 // Functions that are marked Custom Function are functions I've created to simplify code

enum
{
	// List of text id's. The text is stored in database, also in a localized version
	// (if translation not exist for the textId, default english text will be used)
	SAY_AGGRO = -1999900,
	SAY_RANDOM_0 = -1999901,
	SAY_RANDOM_1 = -1999902,
	SAY_RANDOM_2 = -1999903,
	SAY_RANDOM_3 = -1999904,
	SAY_RANDOM_4 = -1999905,
	SAY_BESERK = -1999906,
	SAY_PHASE = -1999907,
	SAY_DANCE = -1999908,
	SAY_SALUTE = -1999909,

	// List of used spells
	SPELL_BUFF = 25661,
	SPELL_ONE = 12555,
	SPELL_ONE_ALT = 24099,
	SPELL_TWO = 10017,
	SPELL_THREE = 26027,
	SPELL_ENRAGE = 23537,
	SPELL_BESERK = 32309,

	// Some other information we need to store
	TEXT_ID_GREET = 907,
	FACTION_WORGEN = 24
};

// List of gossip item texts. Items will appear in the gossip window.
// Actually such gossip can already be handled in normal World-Database
// If (and only if) a gossip must be handled within SD2, then it should be moved to SD2-database!
#define GOSSIP_ITEM     "I'm looking for a fight"


struct example_creatureAI : public ScriptedAI
{
    // *** HANDLED FUNCTION ***
    // This is the constructor, called only once when the creature is first created
    example_creatureAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    // *** CUSTOM VARIABLES ****
    // These variables are for use only by this individual script.
    // Nothing else will ever call them but us.

    uint32 m_uiSayTimer;                                    // Timer for random chat
    uint32 m_uiRebuffTimer;                                 // Timer for rebuffing
    uint32 m_uiSpellOneTimer;                               // Timer for spell 1 when in combat
    uint32 m_uiSpellTwoTimer;                               // Timer for spell 1 when in combat
    uint32 m_uiSpellThreeTimer;                             // Timer for spell 1 when in combat
    uint32 m_uiBeserkTimer;                                 // Timer until we go into Beserk (enraged) mode
    uint32 m_uiPhase;                                       // The current battle phase we are in
    uint32 m_uiPhaseTimer;                                  // Timer until phase transition

    // *** HANDLED FUNCTION ***
    // This is called whenever the core decides we need to evade
    void Reset() override
    {
        m_uiPhase = 1;                                      // Start in phase 1
        m_uiPhaseTimer = 60000;                             // 60 seconds
        m_uiSpellOneTimer = 5000;                           // 5 seconds
        m_uiSpellTwoTimer = 37000;                          // 37 seconds
        m_uiSpellThreeTimer = 19000;                        // 19 seconds
        m_uiBeserkTimer = 120000;                           // 2 minutes
    }

    // *** HANDLED FUNCTION ***
    // Aggro is called when we enter combat, against an enemy, and haven't been in combat before
    void Aggro(Unit* pWho) override
    {
        // Say some stuff
        DoScriptText(SAY_AGGRO, m_creature, pWho);
    }

    // *** HANDLED FUNCTION ***
    // Our Recive emote function
    void ReceiveEmote(Player* /*pPlayer*/, uint32 uiTextEmote) override
    {
        m_creature->HandleEmote(uiTextEmote);

        switch (uiTextEmote)
        {
            case TEXTEMOTE_DANCE:
                DoScriptText(SAY_DANCE, m_creature);
                break;
            case TEXTEMOTE_SALUTE:
                DoScriptText(SAY_SALUTE, m_creature);
                break;
        }
    }

    // *** HANDLED FUNCTION ***
    // Update AI is called Every single map update (roughly once every 100ms if a player is within the grid)
    void UpdateAI(const uint32 uiDiff) override
    {
        // Out of combat timers
        if (!m_creature->GetVictim())
        {
            // Random Say timer
            if (m_uiSayTimer < uiDiff)
            {
                // Random switch between 5 outcomes
                switch (urand(0, 4))
                {
                    case 0: DoScriptText(SAY_RANDOM_0, m_creature); break;
                    case 1: DoScriptText(SAY_RANDOM_1, m_creature); break;
                    case 2: DoScriptText(SAY_RANDOM_2, m_creature); break;
                    case 3: DoScriptText(SAY_RANDOM_3, m_creature); break;
                    case 4: DoScriptText(SAY_RANDOM_4, m_creature); break;
                }

                m_uiSayTimer = 45 * IN_MILLISECONDS;        // Say something agian in 45 seconds
            }
            else
                m_uiSayTimer -= uiDiff;

            // Rebuff timer
            if (m_uiRebuffTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature, SPELL_BUFF);
                // Rebuff agian in 15 minutes
                m_uiRebuffTimer = 15 * MINUTE * IN_MILLISECONDS;
            }
            else
                m_uiRebuffTimer -= uiDiff;
        }

        // Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        // Abilities of all phases
        // Spell One timer
        if (m_uiSpellOneTimer < uiDiff)
        {
            // Cast spell one on our current target.
            if (rand() % 50 > 10)
                DoCastSpellIfCan(m_creature->GetVictim(), SPELL_ONE_ALT);
            else if (m_creature->IsWithinDist(m_creature->GetVictim(), 25.0f))
                DoCastSpellIfCan(m_creature->GetVictim(), SPELL_ONE);

            m_uiSpellOneTimer = 5000;
        }
        else
            m_uiSpellOneTimer -= uiDiff;

        // Spell Two timer
        if (m_uiSpellTwoTimer < uiDiff)
        {
            // Cast spell two on self (AoE spell with only self-target) if we can
            if (DoCastSpellIfCan(m_creature, SPELL_TWO) == CAST_OK)
                m_uiSpellTwoTimer = 37 * IN_MILLISECONDS;   // Only Update Timer, if we could start casting
        }
        else
            m_uiSpellTwoTimer -= uiDiff;

        // End of abliities of all phases

        // Phase 1 abilities
        if (m_uiPhase == 1)
        {
            // Phase timer
            if (m_uiPhaseTimer < uiDiff)
            {
                // Only switch phase and display phase-switich text, if out cast was started sucessfull
                if (DoCastSpellIfCan(m_creature, SPELL_ENRAGE) == CAST_OK)
                {
                    // Go to next phase
                    ++m_uiPhase;
                    DoScriptText(SAY_PHASE, m_creature);
                }
            }
            else
                m_uiPhaseTimer -= uiDiff;
        }
        // Phase 2 abilities
        else if (m_uiPhase > 1)
        {
            // Spell Three timer
            if (m_uiSpellThreeTimer < uiDiff)
            {
                // Cast spell three on self (AoE spell with only self-target)
                if (DoCastSpellIfCan(m_creature, SPELL_THREE) == CAST_OK)
                    m_uiSpellThreeTimer = 19000;
            }
            else
                m_uiSpellThreeTimer -= uiDiff;

            // Beserk timer
            if (m_uiBeserkTimer < uiDiff)
            {
                // Cast uber death spell if possible
                if (DoCastSpellIfCan(m_creature->GetVictim(), SPELL_BESERK) == CAST_OK)
                {
                    // Say our line if we cast
                    DoScriptText(SAY_BESERK, m_creature, m_creature->GetVictim());

                    // Cast our beserk spell agian in 12 seconds (if we didn't kill everyone)
                    m_uiBeserkTimer = 12000;
                }
            }
            else
                m_uiBeserkTimer -= uiDiff;
        }

        // Normal behaviour: if possible mobs do attack with melee
        DoMeleeAttackIfReady();
    }

};

// This is the GetAI method used by all scripts that involve AI
// It is called every time a new creature using this script is created
UnitAI* GetAI_example_creature(Creature* pCreature)
{
	return new example_creatureAI(pCreature);
}


// This function is called when the player opens the gossip menu
// In this case as there is nothing special about this gossip dialogue, it should be moved to world-DB
bool GossipHello_example_creature(Player* pPlayer, Creature* pCreature)
{
	pPlayer->PrepareGossipMenu(pCreature, pPlayer->GetDefaultGossipMenuForSource(pCreature));

	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, u8"提升到70级+初始套装+武器熟练度", GOSSIP_SENDER_MAIN, 122);
	if (pPlayer->getLevel() >= 60) {
		if (!addRep(pPlayer, false)) {
			pPlayer->ADD_GOSSIP_ITEM(3, u8"提升五大区域副本声望值崇拜 ", GOSSIP_SENDER_MAIN, 206);
		}
		else {
			pPlayer->ADD_GOSSIP_ITEM(3, u8"切换 [占星者/奥尔多] 声望值崇拜", GOSSIP_SENDER_MAIN, 206);
		}
		if (!check(pPlayer, false)) {//暂定60级才能学习
			const char* getmenu = all[pPlayer->getClass()].menuName.c_str();
			pPlayer->ADD_GOSSIP_ITEM(3, getmenu, GOSSIP_SENDER_MAIN, 201);//  职业菜单
		}
	}
	if (sPzxConfig.GetIntDefault("openT", 1)) {
		pPlayer->ADD_GOSSIP_ITEM(7, u8"请送我一组|cff6247c8职业套装|h|r", GOSSIP_SENDER_MAIN, 107);
	}
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, u8"学习-|cff6247c8商业技能|h|r", GOSSIP_SENDER_MAIN, 301);


	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送--> 卡拉赞 团队副本", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送--> 沙塔斯城", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
	if (pPlayer->GetTeam() == HORDE) {
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送--> 奥格瑞玛", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
	}else {
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送--> 铁炉堡", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
	}

	if (pPlayer->getClass() == CLASS_HUNTER) {
		pPlayer->ADD_GOSSIP_ITEM(3, u8"提升 我的宠物|cff6247c8忠诚度和等级|h|r ", GOSSIP_SENDER_MAIN, 205);
	}
	if (sPzxConfig.GetIntDefault("show.additem", 1)) {
		pPlayer->ADD_GOSSIP_ITEM_EXTENDED(6, u8"输入物品ID获取物品   **使用说明**|h|r:在弹出框中输入|cff6247c8物品ID 数量|h|r", GOSSIP_SENDER_MAIN, 777, u8"在弹框中输入物品ID编号 数量\n 例:|cFF00F0ff需要4个无底包|r，请输入:|cFFF0FF0014156 4|r", 0, true);
	}
	if (pPlayer->IsGameMaster()) {
		pPlayer->ADD_GOSSIP_ITEM(3, u8"重新加载系统参数", GOSSIP_SENDER_MAIN, 778);
	}
	// pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
	pPlayer->SEND_GOSSIP_MENU(TEXT_ID_GREET, pCreature->GetObjectGuid());

	return true;
}



// This function is called when the player clicks an option on the gossip menu
// In this case here the faction change could be handled by world-DB gossip, hence it should be handled there!
bool GossipSelect_example_creature(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
	//if (uiAction == GOSSIP_ACTION_INFO_DEF + 1)
	//{
	//    pPlayer->CLOSE_GOSSIP_MENU();
	//    // Set our faction to hostile towards all
	//    pCreature->SetFactionTemporary(FACTION_WORGEN, TEMPFACTION_RESTORE_RESPAWN);
	//    pCreature->AI()->AttackStart(pPlayer);
	//}
	if (uiAction < 20) {//系统菜单
		//pPlayer->OnGossipSelect(pCreature, 0);
		switch (uiAction)
		{
		case GOSSIP_OPTION_VENDOR:
		case GOSSIP_OPTION_ARMORER:
			pPlayer->GetSession()->SendListInventory(pCreature->GetObjectGuid());
			break;
		case GOSSIP_OPTION_TRAINER:
			pPlayer->GetSession()->SendTrainerList(pCreature->GetObjectGuid());
			break;
		}
		return true;

	}

	if (uiAction == 107) {
		if (sPzxConfig.GetIntDefault("openT3", 1)) {
			pPlayer->ADD_GOSSIP_ITEM(0, u8"请送我|cffe31bd2T4套装|h|r", GOSSIP_SENDER_MAIN, 109);
		}
		if (sPzxConfig.GetIntDefault("openT4", 1)) {
			pPlayer->ADD_GOSSIP_ITEM(0, u8"请送我|cffe31bd2T5套装|h|r", GOSSIP_SENDER_MAIN, 110);
		}
		if (sPzxConfig.GetIntDefault("openT5", 1)) {
			pPlayer->ADD_GOSSIP_ITEM(0, u8"请送我|cffe31bd2T6套装|h|r", GOSSIP_SENDER_MAIN, 111);
		}
		pPlayer->SEND_GOSSIP_MENU(TEXT_ID_GREET, pCreature->GetObjectGuid());
		return true;
	}
	if (uiAction == 109) {
		addItemSet(pPlayer, 0);
	}
	if (uiAction == 110) {
		addItemSet(pPlayer, 1);
	}
	if (uiAction == 111) {
		addItemSet(pPlayer, 2);
	}
	if (uiAction == 122)
	{
		pPlayer->CLOSE_GOSSIP_MENU();
		//player->LearnSpell(33389, false);
		ObjectGuid target_guid;
		if (pPlayer->getLevel() < 70) {
			pPlayer->GiveLevel(70);
			pPlayer->InitTalentForLevel();
		}
		//pPlayer->learnSpell(33392, false);//中级骑术
		pPlayer->learnSpell(34093, false);//专家级级骑术

		pPlayer->SetUInt32Value(PLAYER_XP, 0);
		pPlayer->UpdateSkillsForLevel(true);
		if (sPzxConfig.GetIntDefault("initItemSet", 1) <= 2) {
			addItemSet(pPlayer, sPzxConfig.GetIntDefault("initItemSet", 0));//增加T1套装
		}
	}

	if (uiAction == 201) {
		check(pPlayer, true); //学习职业技能
	}

	if (uiAction == 205) {

		if (pPlayer->GetPet() && pPlayer->GetPet()->getPetType() == HUNTER_PET) {
			uint32 maxlevel = 70;
			Pet* HunterPet = pPlayer->GetPet();
			if (HunterPet->getLevel() < maxlevel || HunterPet->GetLoyaltyLevel() < LoyaltyLevel(BEST_FRIEND)) {

				//player->ADD_GOSSIP_ITEM(3, u8"提升 我的宠物忠诚度和等级 ", GOSSIP_SENDER_MAIN, 205);
				pPlayer->GetPet()->GivePetXP(99999999);
				pPlayer->GetPet()->ModifyLoyalty(1000000.0);
			}
			else {
				ChatHandler(pPlayer).PSendSysMessage(u8"[系统消息]:|cff0000ff 您的宠物已经强化完成!|h|r");
			}
		}
		else {
			ChatHandler(pPlayer).PSendSysMessage(u8"[系统消息]:请先|cffff0000 驯服或者召唤出|h|r一只要强化的宠物");
		}
	}
	if (uiAction == 206) {
		addRep(pPlayer, true);
		FactionEntry const* factionEntry1 = sFactionStore.LookupEntry<FactionEntry>(932);//奥尔多
		FactionEntry const* factionEntry2 = sFactionStore.LookupEntry<FactionEntry>(934);//占星者
		if (pPlayer->GetReputationMgr().GetReputation(factionEntry1) < sPzxConfig.GetIntDefault("rep.init", 42001)) {
			pPlayer->GetReputationMgr().SetReputation(factionEntry1, sPzxConfig.GetIntDefault("rep.init", 42001));//声望值
			//pPlayer->GetReputationMgr().SetReputation(factionEntry2, 0);//声望值
		}
		else {
			//pPlayer->GetReputationMgr().SetReputation(factionEntry1, 0);//声望值
			pPlayer->GetReputationMgr().SetReputation(factionEntry2, sPzxConfig.GetIntDefault("rep.init", 42001));//声望值
		}

	}
	if (uiAction == 301) {
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"炼金术", GOSSIP_SENDER_MAIN, 301+1);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"锻造", GOSSIP_SENDER_MAIN, 301 + 2);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"制皮", GOSSIP_SENDER_MAIN, 301 + 3);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"裁缝", GOSSIP_SENDER_MAIN, 301 + 4);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"工程学", GOSSIP_SENDER_MAIN, 301 + 5);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"附魔", GOSSIP_SENDER_MAIN, 301 + 6);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"珠宝",      GOSSIP_SENDER_MAIN, 301+7);
		//pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"铭文",        GOSSIP_SENDER_MAIN, 301+8);773
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"草药", GOSSIP_SENDER_MAIN, 301 + 9);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"剥皮", GOSSIP_SENDER_MAIN, 301 + 10);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"采矿", GOSSIP_SENDER_MAIN, 301 + 11);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"急救", GOSSIP_SENDER_MAIN, 301 + 12);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"钓鱼", GOSSIP_SENDER_MAIN, 301 + 13);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"烹饪", GOSSIP_SENDER_MAIN, 301 + 14);

		pPlayer->SEND_GOSSIP_MENU(TEXT_ID_GREET, pCreature->GetObjectGuid());
		return true;
	}
	

	int index = uiAction - GOSSIP_ACTION_INFO_DEF;
	if (index > 0) {

		switch (index)
		{
		case 1://KLZ
			pPlayer->CLOSE_GOSSIP_MENU();
			pPlayer->TeleportTo(0, -11120.2f, -2015.27f, 47.1869f, 1.91823f);
			break;

		case 2://STS
			pPlayer->CLOSE_GOSSIP_MENU();
			pPlayer->TeleportTo(530, -1863.0f, 5430.1f, -9.70549f, 3.7f);
			break;

		case 3://ogrimmar
			pPlayer->CLOSE_GOSSIP_MENU();
			pPlayer->TeleportTo(1, 1541.0f, -4426.0f, 11.24f, 0.85f);
			break;

		case 4://IRONforge
			pPlayer->CLOSE_GOSSIP_MENU();
			pPlayer->TeleportTo(0, -4917.0f, -955.0f, 502.0f, 0.0f);
			break;
		default:
			break;
		}
		return true;
	}
	else if(uiAction-301<=14&& uiAction - 301>=1){
		return GossipSelect_ProfessionNPC(pPlayer, uiSender, uiAction - 301);

	}


	if (uiAction == 778) {
		sPzxConfig.Reload();
		ChatHandler(pPlayer).PSendSysMessage(u8"[系统消息]:config文件加载成功");
		pPlayer->CLOSE_GOSSIP_MENU();
	}

	pPlayer->CLOSE_GOSSIP_MENU();
	return true;
}



bool GossipSelect_example_creature_code(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction, const char* reStr) {

	if (uiAction == 777) {
		/*	if (!player->HasItemCount(sPzxConfig.GetIntDefault("vipItemID", 40003), 1, true)) {
		ChatHandler(pPlayer).PSendSysMessage( u8"[系统消息]:需要VIP认证卡才可以使用本功能，请联系GM获取");
		player->CLOSE_GOSSIP_MENU();

		return false;
		}*/
		sLog.outString("[pzx] get Input str =%s", reStr);
		std::string a(reStr);
		char * b = new char[a.length() + 1];
		uint32 ssitem[2] = { 0, 1 };
		try {
			std::strcpy(b, a.c_str());
			int index = 0;
			char *ptr;
			ptr = strtok(b, " ");
			while (ptr != NULL)
			{
				if (index == 2)
					break;
				ssitem[index] = std::stoi(ptr);
				ptr = strtok(NULL, " ");
				index++;
			}
			delete[] b;
			//getItemID = std::stoi(reStr);
			if (ssitem[0] <= 0) {
				ChatHandler(pPlayer).PSendSysMessage(u8"[系统消息]:请输入正确的物品ID");
				pPlayer->CLOSE_GOSSIP_MENU();

				return false;
			}
		}
		catch (...) {
			ChatHandler(pPlayer).PSendSysMessage(u8"[系统消息]:请输入正确的物品ID");
			pPlayer->CLOSE_GOSSIP_MENU();
			return false;
		}


		ItemPrototype const *pProto = sItemStorage.LookupEntry<ItemPrototype>(ssitem[0]);
		if (pProto) {

			if (pProto->Quality < sPzxConfig.GetIntDefault("item.quality", ITEM_QUALITY_LEGENDARY) && pProto->ItemLevel < sPzxConfig.GetIntDefault("item.level", 155))
			{
				if (pPlayer->HasItemCount(pProto->ItemId, 1, true)) {//已经有一件了
					ChatHandler(pPlayer).PSendSysMessage(u8"[系统消息]:该物品唯一");
				}
				ItemPosCountVec dest;
				InventoryResult msg = pPlayer->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, pProto->ItemId, ssitem[1]);
				if (msg == EQUIP_ERR_OK)
				{
					Item* item = pPlayer->StoreNewItem(dest, pProto->ItemId, true);
					pPlayer->SendNewItem(item, ssitem[1], true, false);
					ChatHandler(pPlayer).PSendSysMessage(u8"[系统消息]:%s 已经添加到你包中", item->GetProto()->Name1);
				}
				else
				{
					pPlayer->SendEquipError(msg, nullptr, nullptr, pProto->ItemId);
					ChatHandler(pPlayer).PSendSysMessage(u8"[系统消息]:请保持包包有足够空间");
				}
			}
			else {
				ChatHandler(pPlayer).PSendSysMessage(u8"[系统消息]:获取物品等级过高,需小于[%d]，请联系管理员", sPzxConfig.GetIntDefault("item.level", 155));
			}
		}
		else
			ChatHandler(pPlayer).PSendSysMessage(u8"[系统消息]:物品未找到");

		pPlayer->CLOSE_GOSSIP_MENU();
		return false;

	}
	pPlayer->CLOSE_GOSSIP_MENU();
	return true;
}
// This is the actual function called only once durring InitScripts()
// It must define all handled functions that are to be run in this script
void AddSC_example_creature()
{
	if (!sPzxConfig.SetSource("pzx.conf")) {
		sLog.outError(u8"未找到pzx.conf");
	}
	Script* pNewScript = new Script;
	pNewScript->Name = "example_creature";
	// pNewScript->GetAI = &GetAI_example_creature;
	pNewScript->pGossipHello = &GossipHello_example_creature;
	pNewScript->pGossipSelect = &GossipSelect_example_creature;
	pNewScript->pGossipSelectWithCode = &GossipSelect_example_creature_code;
	pNewScript->RegisterSelf(false);
}
