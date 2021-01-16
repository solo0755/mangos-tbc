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
SDName: Example_Misc
SD%Complete: 100
SDComment: Item, Areatrigger and other small code examples
SDCategory: Script Examples
EndScriptData */

#include "AI/ScriptDevAI/include/example.h"


bool GossipHello_ItemPzx(Player *pPlayer, Item *_item)
{
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
	}
	else {
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
	pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _item->GetObjectGuid());
	return true;
}

bool GossipSelect_ItemPzx(Player *pPlayer, Item *_item, uint32 sender, const uint32 uiAction, char const* reStr)
{

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
		pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _item->GetObjectGuid());
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
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"炼金术", GOSSIP_SENDER_MAIN, 301 + 1);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"锻造", GOSSIP_SENDER_MAIN, 301 + 2);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"制皮", GOSSIP_SENDER_MAIN, 301 + 3);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"裁缝", GOSSIP_SENDER_MAIN, 301 + 4);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"工程学", GOSSIP_SENDER_MAIN, 301 + 5);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"附魔", GOSSIP_SENDER_MAIN, 301 + 6);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"珠宝", GOSSIP_SENDER_MAIN, 301 + 7);
		//pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"铭文",        GOSSIP_SENDER_MAIN, 301+8);773
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"草药", GOSSIP_SENDER_MAIN, 301 + 9);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"剥皮", GOSSIP_SENDER_MAIN, 301 + 10);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"采矿", GOSSIP_SENDER_MAIN, 301 + 11);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"急救", GOSSIP_SENDER_MAIN, 301 + 12);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"钓鱼", GOSSIP_SENDER_MAIN, 301 + 13);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"烹饪", GOSSIP_SENDER_MAIN, 301 + 14);

		pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _item->GetObjectGuid());
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
	else if (uiAction - 301 <= 14 && uiAction - 301 >= 1) {
		return GossipSelect_ProfessionNPC(pPlayer, sender, uiAction - 301);
	}


	if (uiAction == 778) {
		sPzxConfig.Reload();
		ChatHandler(pPlayer).PSendSysMessage(u8"[系统消息]:config文件加载成功");
		pPlayer->CLOSE_GOSSIP_MENU();
	}
	pPlayer->CLOSE_GOSSIP_MENU();
	return true;

}
void AddSC_example_item()
{
	Script* pNewScript = new Script;

	pNewScript->Name = "example_item_pzx";
	pNewScript->pGossipHelloPzx = &GossipHello_ItemPzx;
	pNewScript->pGossipSelectPzx = &GossipSelect_ItemPzx;
	pNewScript->RegisterSelf(false);
}
