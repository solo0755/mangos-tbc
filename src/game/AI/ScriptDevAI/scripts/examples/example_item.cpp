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

//1.猎人稳固射击受益问题，稳固只有一级

bool GossipHello_ItemPzx(Player *pPlayer, Item *_item)
{

	
	if (pPlayer->getLevel() < 70|| !addRep(pPlayer, false)|| !check(pPlayer, false)|| pPlayer->GetSkillValue(SKILL_FIRST_AID)<MYMAXSKILL|| pPlayer->GetSkillValue(SKILL_FISHING)<MYMAXSKILL|| pPlayer->GetSkillValue(SKILL_COOKING)<MYMAXSKILL) {
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, u8"开始新的旅途(必选哦~)", GOSSIP_SENDER_MAIN, 101);
	}
	else {
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, u8"切换[占星者/奥尔多]声望+提升武器熟练度", GOSSIP_SENDER_MAIN, 206);
	}
	if (sPzxConfig.GetIntDefault("openT", 1)) {
		pPlayer->ADD_GOSSIP_ITEM(7, u8"获取一套|cff6247c8职业套装|h|r", GOSSIP_SENDER_MAIN, 400);
	}
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, u8"学习-|cff6247c8商业技能|h|r", GOSSIP_SENDER_MAIN, 301);
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送--> 沙塔斯城（新手接待）", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"-->其他位传送", GOSSIP_SENDER_MAIN, 300);

	if (pPlayer->getClass() == CLASS_HUNTER) {
		pPlayer->ADD_GOSSIP_ITEM(3, u8"提升 我的宠物|cff6247c8忠诚度和等级|h|r ", GOSSIP_SENDER_MAIN, 205);
	}
	if (sPzxConfig.GetIntDefault("show.additem", 1)) {
		pPlayer->ADD_GOSSIP_ITEM_EXTENDED(6, u8"输入ID|cff0070dd获取物品|r,仅限部分物品", GOSSIP_SENDER_MAIN, 777, u8"在弹框中输入物品ID编号 数量\n 例:|cFF00F0ff需要4个无底包|r，请输入:|cFFF0FF0014156 4|r", 0, true);
	}

	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"BUFF|满血、蓝、怒|修理|冷却", GOSSIP_SENDER_MAIN, 208);
	if (pPlayer->GetGroup() && pPlayer->HasFlag(PLAYER_FLAGS, PLAYER_FLAGS_GROUP_LEADER)) {
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"一键复活拉人", GOSSIP_SENDER_MAIN, 209);
	}

	pPlayer->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_TAXI, u8"天赋重置", GOSSIP_SENDER_MAIN, 105, u8"确定要|cff0070dd重置天赋|r吗?", 0, false);
	pPlayer->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_TAXI, u8"角色更名", GOSSIP_SENDER_MAIN, 106, u8"确定要|cff0070dd更改此角色的名称|r吗?", 0, false);
	pPlayer->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_TAXI, u8"清理副本CD", GOSSIP_SENDER_MAIN, 108, u8"确定要|cff0070dd清理所有副本CD|r吗?", 0, false);
	
	
	if (pPlayer->IsGameMaster()) {
		pPlayer->ADD_GOSSIP_ITEM(3, u8"重新加载系统参数", GOSSIP_SENDER_MAIN, 778);
	}
	// pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
	pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _item->GetObjectGuid());
	return false;
}

bool GossipSelect_ItemPzx(Player *pPlayer, Item *_item, uint32 sender, const uint32 uiAction, char const* reStr)
{
	sLog.outString("[pzx-select] (%s)-(%s) Select action   [%d]", pPlayer->GetName(), pPlayer->GetGuidStr(), uiAction);
	if (uiAction == 108) {
		resetIntance(pPlayer, 0, false);
	}
	else if (uiAction == 105) {
		pPlayer->resetTalents(true);
		pPlayer->CastSpell(pPlayer, 14867, TRIGGERED_OLD_TRIGGERED);
		ChatHandler(pPlayer).PSendSysMessage(u8"[系统消息]:|cff00ff00 天赋已经重置.|h|r");
	}else if (uiAction == 106) {
		pPlayer->SetAtLoginFlag(AT_LOGIN_RENAME);
		CharacterDatabase.PExecute("UPDATE characters SET at_login = at_login | '1' WHERE guid = '%u'", pPlayer->GetGUIDLow());
		ChatHandler(pPlayer).PSendSysMessage(u8"[系统消息]:|cff00ff00 请返回到人物界面后更改您的新角色名.|h|r");
	}else if (uiAction == 400) {
		std::vector<uint32> ids = itemset(pPlayer);
		for (uint32 i = 0; i<ids.size(); i++)
		{
			//获取套装中文名
			ItemSetEntry const* set = sItemSetStore.LookupEntry(ids[i]);
			if (set)
			{
				int loc = pPlayer->GetSession()->GetSessionDbcLocale();
				std::string name = set->name[loc];
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, name.c_str(), GOSSIP_SENDER_MAIN, 400+i);
			}
			// Search in ItemSet.dbc
			
		}
		//if (sPzxConfig.GetIntDefault("openT3", 1)) {
		//	pPlayer->ADD_GOSSIP_ITEM(0, u8"请送我|cffe31bd2T4套装|h|r", GOSSIP_SENDER_MAIN, 109);
		//}
		//if (sPzxConfig.GetIntDefault("openT4", 1)) {
		//	pPlayer->ADD_GOSSIP_ITEM(0, u8"请送我|cffe31bd2T5套装|h|r", GOSSIP_SENDER_MAIN, 110);
		//}
		//if (sPzxConfig.GetIntDefault("openT5", 1)) {
		//	pPlayer->ADD_GOSSIP_ITEM(0, u8"请送我|cffe31bd2T6套装|h|r", GOSSIP_SENDER_MAIN, 111);
		//}
		pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _item->GetObjectGuid());
		return true;
	}else if (uiAction == 101)
	{
		//player->LearnSpell(33389, false);
		ObjectGuid target_guid;
		if (pPlayer->getLevel() < 70) {
			pPlayer->GiveLevel(70);
			pPlayer->InitTalentForLevel();
		}
		//添加声望和钥匙
		addRep(pPlayer, true);
		FactionEntry const* factionEntry_adl = sFactionStore.LookupEntry<FactionEntry>(932);//奥尔多
		FactionEntry const* factionEntry_zxz = sFactionStore.LookupEntry<FactionEntry>(934);//占星者
		if (pPlayer->GetReputationMgr().GetReputation(factionEntry_adl) < sPzxConfig.GetIntDefault("rep.init", 42001)) {
			pPlayer->GetReputationMgr().SetReputation(factionEntry_adl, sPzxConfig.GetIntDefault("rep.init", 42001));//声望值
		}
		else {
			pPlayer->GetReputationMgr().SetReputation(factionEntry_zxz, sPzxConfig.GetIntDefault("rep.init", 42001));//声望值
		}
		//职业菜单
		if (!check(pPlayer, false)) {//暂定60级才能学习
			check(pPlayer, true); //学习职业技能
			//const char* getmenu = all[pPlayer->getClass()].menuName.c_str();
			//pPlayer->ADD_GOSSIP_ITEM(3, getmenu, GOSSIP_SENDER_MAIN, 201);//  职业菜单
		}


		//学习骑术
		//pPlayer->learnSpell(33392, false);//中级骑术
		pPlayer->learnSpell(34093, false);//专家级级骑术
		pPlayer->SetUInt32Value(PLAYER_XP, 0);
		pPlayer->UpdateSkillsForLevel(true);

		if (pPlayer->GetSkillValue(SKILL_FIRST_AID)<MYMAXSKILL)
			CompleteLearnProfession(pPlayer, SKILL_FIRST_AID);
		if (pPlayer->GetSkillValue(SKILL_FISHING)<MYMAXSKILL)
			CompleteLearnProfession(pPlayer, SKILL_FISHING);
		if (pPlayer->GetSkillValue(SKILL_COOKING)<MYMAXSKILL)
			CompleteLearnProfession(pPlayer, SKILL_COOKING);

		if (sPzxConfig.GetIntDefault("initItemSet", 1) <= 2) {
			addItemSet(pPlayer, sPzxConfig.GetIntDefault("initItemSet", 0));//增加T1套装
		}
		pPlayer->CLOSE_GOSSIP_MENU();
		return GossipHello_ItemPzx(pPlayer, _item);
	}else if (uiAction == 205) {
		if (pPlayer->GetPet() && pPlayer->GetPet()->getPetType() == HUNTER_PET) {


			uint32 maxlevel = 70;
			Pet* HunterPet = pPlayer->GetPet();
			if (HunterPet->getLevel() < maxlevel) {
				HunterPet->GivePetLevel(70);
			}
			uint32 loyaltyLevel = HunterPet->GetLoyaltyLevel();
			uint32 levelupCount = 7;
			for (; loyaltyLevel < BEST_FRIEND && levelupCount > 0; levelupCount--)
			{
				HunterPet->UpdateRequireXpForNextLoyaltyLevel(214748364);
				HunterPet->ModifyLoyalty(HunterPet->GetStartLoyaltyPoints(loyaltyLevel + 1));
				++loyaltyLevel;
			}
			if (loyaltyLevel == BEST_FRIEND) {
				ChatHandler(pPlayer).PSendSysMessage(u8"[系统消息]:|cff0000ff 您的宠物已经强化完成!|h|r");
			}
			else {
				ChatHandler(pPlayer).PSendSysMessage(u8"[系统消息]:|cffff0000 您的宠物还需要继续训练!|h|r");
				pPlayer->CLOSE_GOSSIP_MENU();
				return GossipHello_ItemPzx(pPlayer, _item);
			}
		}
		else {
			ChatHandler(pPlayer).PSendSysMessage(u8"[系统消息]:请先|cffff0000 驯服或者召唤出|h|r一只要强化的宠物");
			pPlayer->CLOSE_GOSSIP_MENU();
			return GossipHello_ItemPzx(pPlayer, _item);
		}
	}else if (uiAction == 206) {
		FactionEntry const* factionEntry_adl = sFactionStore.LookupEntry<FactionEntry>(932);//奥尔多
		FactionEntry const* factionEntry_zxz = sFactionStore.LookupEntry<FactionEntry>(934);//占星者
		if (pPlayer->GetReputationMgr().GetReputation(factionEntry_adl) < sPzxConfig.GetIntDefault("rep.init", 42001)) {
			pPlayer->GetReputationMgr().SetReputation(factionEntry_adl, sPzxConfig.GetIntDefault("rep.init", 42001));//声望值
		}
		else {
			pPlayer->GetReputationMgr().SetReputation(factionEntry_zxz, sPzxConfig.GetIntDefault("rep.init", 42001));//声望值
		}
		pPlayer->UpdateSkillsForLevel(true);
		pPlayer->CLOSE_GOSSIP_MENU();
		return GossipHello_ItemPzx(pPlayer, _item);
	}
	else if (uiAction == 208) {
		if (sPzxConfig.GetIntDefault("show.morebuff", 1) || pPlayer->IsGameMaster()) {
			if (!pPlayer->HasAura(35076))//阿达尔的祝福
				pPlayer->CastSpell(pPlayer, 35076, TRIGGERED_FULL_MASK);
			if (!pPlayer->HasAura(25898))//王者
				pPlayer->CastSpell(pPlayer, 25898, TRIGGERED_FULL_MASK);
			if (!pPlayer->HasAura(25392))
				pPlayer->CastSpell(pPlayer, 25392, TRIGGERED_FULL_MASK);
			if (!pPlayer->HasAura(26991))
				pPlayer->CastSpell(pPlayer, 26991, TRIGGERED_FULL_MASK);
			//player->CastSpell(player, 16877, true);
			if (pPlayer->GetPowerType() == POWER_MANA) {//蓝条职业
				if (!pPlayer->HasAura(27127))
					pPlayer->CastSpell(pPlayer, 27127, TRIGGERED_FULL_MASK);//奥术光辉
				if (!pPlayer->HasAura(32999))
					pPlayer->CastSpell(pPlayer, 32999, TRIGGERED_FULL_MASK);//精神
				if (!pPlayer->HasAura(27143))
					pPlayer->CastSpell(pPlayer, 27143, TRIGGERED_FULL_MASK);//智慧祝福
			}
			else {
				if (!pPlayer->HasAura(27141))
						pPlayer->CastSpell(pPlayer, 27141, TRIGGERED_FULL_MASK);//力量祝福
				if (!pPlayer->HasAura(25289))
					pPlayer->CastSpell(pPlayer, 25289, TRIGGERED_FULL_MASK);//战斗怒吼
			}

			pPlayer->DurabilityRepairAll(false, 0, false);//修理
			pPlayer->UpdateSkillsForLevel(true);//提升武器熟练度
			pPlayer->RemoveAllCooldowns();//冷却所有技能
			if (pPlayer->HasAura(15007))
				pPlayer->RemoveAurasDueToSpell(15007);	//移除复活虚弱
			pPlayer->SetHealth(pPlayer->GetMaxHealth());
			if (pPlayer->GetPowerType() == POWER_RAGE) {
				pPlayer->SetPower(POWER_RAGE, 100000);
			}
			else if (pPlayer->GetPowerType() == POWER_MANA) {
				pPlayer->SetPower(POWER_MANA, 100000);
			}
			ChatHandler(pPlayer).PSendSysMessage(u8"[系统消息]:您已经被强化了.奔跑吧...勇士");
		}
	}
	else if (uiAction == 209) {
		ChatHandler(pPlayer).PSendSysMessage(u8"[系统消息]:功能开放中....");
		pPlayer->CLOSE_GOSSIP_MENU();
		return GossipHello_ItemPzx(pPlayer, _item);
	}
	else if (uiAction == 300) {
		
		if (pPlayer->GetTeam() == HORDE) {
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送--> 奥格瑞玛", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送--> 幽暗城", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送--> 雷霆崖", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送--> 银月城", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9);
		}
		else {
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送--> 铁炉堡", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送--> 暴风城", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送--> 达纳苏斯", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送--> 埃索达", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 10);
		}
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送--> 卡拉赞 团队副本", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
		//5个区域本
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送区域--> 风暴要塞", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送区域--> 盘牙水库", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 12);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送区域--> 奥金顿", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 13);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送区域--> 地狱火堡垒", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 14);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送区域--> 时光之穴", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 15);

		pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _item->GetObjectGuid());
		return true;
	}else if (uiAction == 301) {
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"|cFF990066|TInterface\\ICONS\\Trade_Alchemy.blp:30|t|r炼金术", GOSSIP_SENDER_MAIN, 301 + 1);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"|cFF990066|TInterface\\ICONS\\Trade_BlackSmithing.blp:30|t|r 锻造", GOSSIP_SENDER_MAIN, 301 + 2);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"|cFF990066|TInterface\\ICONS\\Trade_LeatherWorking.blp:30|t|r制皮", GOSSIP_SENDER_MAIN, 301 + 3); 
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"|cFF990066|TInterface\\ICONS\\Trade_Tailoring.blp:30|t|r裁缝", GOSSIP_SENDER_MAIN, 301 + 4);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"|cFF990066|TInterface\\ICONS\\Trade_Engineering.blp:30|t|r 工程", GOSSIP_SENDER_MAIN, 301 + 5);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"|cFF990066|TInterface\\ICONS\\Trade_Engraving.blp:30|t|r 附魔", GOSSIP_SENDER_MAIN, 301 + 6);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"|cFF990066|TInterface\\ICONS\\INV_Misc_Gem_02.blp:30|t|r 珠宝", GOSSIP_SENDER_MAIN, 301 + 7);
		//pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"铭文",        GOSSIP_SENDER_MAIN, 301+8);773
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"草药", GOSSIP_SENDER_MAIN, 301 + 9);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"剥皮", GOSSIP_SENDER_MAIN, 301 + 10);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"采矿", GOSSIP_SENDER_MAIN, 301 + 11);

		//pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"急救", GOSSIP_SENDER_MAIN, 301 + 12);
		//pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"钓鱼", GOSSIP_SENDER_MAIN, 301 + 13);
		//pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"烹饪", GOSSIP_SENDER_MAIN, 301 + 14);

		pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _item->GetObjectGuid());
		return true;
	}
	else if (uiAction - 301 <= 14 && uiAction - 301 >= 1) {
		return GossipSelect_ProfessionNPC(pPlayer, sender, uiAction - 301);
	}
	else if (uiAction - 400 <= 15 && uiAction - 400 >= 0) {
		uint32 index = uiAction - 400;
		std::vector<uint32> ids = itemset(pPlayer);
		if (index <= ids.size() - 1) {
			addItemSet(pPlayer, ids[index]);
		}
		else {
			ChatHandler(pPlayer).PSendSysMessage(u8"[系统消息]:系统itemset配置异常");
		}
	}
	else if (uiAction> GOSSIP_ACTION_INFO_DEF) {
		int index = uiAction - GOSSIP_ACTION_INFO_DEF;
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
			case 5://幽暗
				pPlayer->CLOSE_GOSSIP_MENU();
				pPlayer->TeleportTo(0, 1633.75f, 240.167f, -43.1034f, 6.26128f);
				break;
			case 7://雷霆崖
				pPlayer->CLOSE_GOSSIP_MENU();
				pPlayer->TeleportTo(1, -1274.45f, 71.8601f, 128.159f, 2.80623f);
				break;
			case 9://银月城
				pPlayer->CLOSE_GOSSIP_MENU();
				pPlayer->TeleportTo(530, 9738.28f, -7454.19f, 13.5605f, 0.043914f);
				break;

			case 4://IRONforge
				pPlayer->CLOSE_GOSSIP_MENU();
				pPlayer->TeleportTo(0, -4917.0f, -955.0f, 502.0f, 0.0f);
				break;

			case 6://暴风城
				pPlayer->CLOSE_GOSSIP_MENU();
				pPlayer->TeleportTo(0, -8842.09f, 626.358f, 94.0867f, 3.61363f);
				break;
			case 8://达纳苏斯
				pPlayer->CLOSE_GOSSIP_MENU();
				pPlayer->TeleportTo(1, 9869.91f, 2493.58f, 1315.88f, 2.78897f);
				break;
			case 10://埃索达
				pPlayer->CLOSE_GOSSIP_MENU();
				pPlayer->TeleportTo(530, -3864.92f, -11643.7f, -137.644f, 5.50862f);
				break;
			case 11://风暴要塞
				pPlayer->CLOSE_GOSSIP_MENU();
				pPlayer->TeleportTo(530, 3100.48, 1536.49, 190.3, 4.62226);
				break;

			case 12://盘牙水库
				pPlayer->CLOSE_GOSSIP_MENU();
				pPlayer->TeleportTo(530, 738.865, 6865.77, -69.4659, 6.27655);
				break;

			case 13://奥金顿60级"
				pPlayer->CLOSE_GOSSIP_MENU();
				pPlayer->TeleportTo(530, -3324.49, 4943.45, -101.239, 4.63901);
				break;
			case 14://地狱火堡垒60级
				pPlayer->CLOSE_GOSSIP_MENU();
				pPlayer->TeleportTo(530, -347.29, 3089.82, 21.394, 5.68114);
				break;
			case 15://时光之穴65级
				pPlayer->CLOSE_GOSSIP_MENU();
				pPlayer->TeleportTo(1, -8369.65, -4253.11, -204.272, -2.70526);
				break;
			default:
				break;
			}
			return true;
		}else if(uiAction == 777) {
			/*	if (!player->HasItemCount(sPzxConfig.GetIntDefault("vipItemID", 40003), 1, true)) {
			ChatHandler(pPlayer).PSendSysMessage( u8"[系统消息]:需要VIP认证卡才可以使用本功能，请联系GM获取");
			player->CLOSE_GOSSIP_MENU();
			return false;
			}*/
			sLog.outString(u8"[pzx] (%s)-(%s) Input str: [%s]", pPlayer->GetName(),pPlayer->GetGuidStr() ,reStr );
			if (!reStr||strlen(reStr)>12) {
				ChatHandler(pPlayer).PSendSysMessage(u8"[系统消息]:请输入正确的物品ID和数量ID");
				pPlayer->CLOSE_GOSSIP_MENU();
				return false;
			}
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
					ChatHandler(pPlayer).PSendSysMessage(u8"[系统消息]:请输入正确的物品ID或数量ID");
					pPlayer->CLOSE_GOSSIP_MENU();
					return false;
				}
			}
			catch (...) {
				ChatHandler(pPlayer).PSendSysMessage(u8"[系统消息]:请输入正确的物品ID或数量ID");
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
			//继续显示这个页面
			pPlayer->CLOSE_GOSSIP_MENU();
			return GossipHello_ItemPzx(pPlayer, _item);

	}else if (uiAction == 778) {
		sPzxConfig.Reload();
		ChatHandler(pPlayer).PSendSysMessage(u8"[系统消息]:config文件加载成功");
		pPlayer->CLOSE_GOSSIP_MENU();
		return true;
	}

	pPlayer->CLOSE_GOSSIP_MENU();
	return true;

}
bool resetIntance(Player *player, const uint32 got_map, bool modify) {
	ChatHandler session(player);
	if (player->GetGroup() || player->GetMapRef()->IsDungeon()) {
		session.PSendSysMessage(u8"[系统消息]:|cffff0000您已经在队伍或者副本中|h|r，请先脱离队伍或离开副本区域.");
		return false;
	}
	uint32 counter = 0;
	uint32 mapid = 0;
	for (uint8 i = 0; i < MAX_DIFFICULTY; ++i)
	{
		Player::BoundInstancesMap& binds = player->GetBoundInstances(Difficulty(i));
		for (Player::BoundInstancesMap::iterator itr = binds.begin(); itr != binds.end();)
		{

			if (itr->first != player->GetMapId())
			{
				DungeonPersistentState* save = itr->second.state;
				std::string timeleft = secsToTimeString(save->GetResetTime() - time(nullptr), true);

				if (const MapEntry* entry = sMapStore.LookupEntry(itr->first))
				{
					ChatHandler(player).PSendSysMessage(u8"副本重置: %d (%s) inst: %d perm: %s diff: %s canReset: %s TTR: %s",
						itr->first, entry->name[4], save->GetInstanceId(), itr->second.perm ? "yes" : "no",
						save->GetDifficulty() == DUNGEON_DIFFICULTY_NORMAL ? "normal" : "heroic", save->CanReset() ? "yes" : "no", timeleft.c_str());
				}
				else
					session.PSendSysMessage("bound for a nonexistent map %u", itr->first);
				player->UnbindInstance(itr, Difficulty(i));
				++counter;
			}
			else
				++itr;
		}
	}
	session.PSendSysMessage("instances unbound: %d", counter);

	session.PSendSysMessage(u8"[系统消息]:您暂时还没有 副本进度. ");
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
