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
	if (pPlayer->GetGroup() && pPlayer->HasFlag(PLAYER_FLAGS, PLAYER_FLAGS_GROUP_LEADER)) {
		Player* leader = sObjectMgr.GetPlayer(pPlayer->GetGroup()->GetLeaderGuid());
		if (leader&&leader->GetSession()&&pPlayer->GetCustomPzxAuaraMutil(PLAYED_PZXAURA_ONOFF) > 0 ) {

			std::ostringstream oss;
			float dx = leader->GetCustomPzxAuaraMutil(PLAYED_PZXAURA_ONOFF);
			oss << u8"|cffff0000关闭|r 副本弹性模式--->当前设置值[|cff00ff00" << dx << "|r]";
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, oss.str().c_str(), GOSSIP_SENDER_MAIN, 508);

		}else {
			pPlayer->ADD_GOSSIP_ITEM_EXTENDED(6, u8"|cff00ff00开启|r 当前副本弹性模式,默认值:[1]", GOSSIP_SENDER_MAIN, 509, u8"在弹框中输入1~10.0 数量\n 例:|cFF00F0ff更改团队弹性值百分比200%|r，请输入:|cFFF0FF002.0|r", 0, true);
		}
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, u8"一键|cff0070dd复活拉人|r", GOSSIP_SENDER_MAIN, 501);
		pPlayer->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_TALK, u8"一键|cff0070dd秒杀全团|r", GOSSIP_SENDER_MAIN, 502, u8"确定要|cFFF0FF00秒杀全团|r吗?", 0, false);
	}
	if (!pPlayer->IsInCombat()|| pPlayer->IsGameMaster()) {//战斗中不显示菜单
		if (pPlayer->getLevel() < 70|| !addRep(pPlayer, false)|| !check(pPlayer, false)|| pPlayer->GetSkillValue(SKILL_FIRST_AID)<MYMAXSKILL|| pPlayer->GetSkillValue(SKILL_FISHING)<MYMAXSKILL|| pPlayer->GetSkillValue(SKILL_COOKING)<MYMAXSKILL) {
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, u8"开始新的旅途(必选哦~)", GOSSIP_SENDER_MAIN, 101);
		}
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送--> 沙塔斯城（|cffFF00c8新手接待|r）", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
		pPlayer->ADD_GOSSIP_ITEM(7, u8"免费获取-|cff6247c8职业套装|h|r", GOSSIP_SENDER_MAIN, 400);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, u8"免费学习-|cff6247c8商业技能|h|r", GOSSIP_SENDER_MAIN, 301);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"其他传送-->", GOSSIP_SENDER_MAIN, 300);

		if (pPlayer->getClass() == CLASS_HUNTER) {
			pPlayer->ADD_GOSSIP_ITEM(3, u8"提升 我的宠物|cff6247c8忠诚度和等级|h|r ", GOSSIP_SENDER_MAIN, 205);
		}
		if (sPzxConfig.GetIntDefault("show.additem", 1)) {
			pPlayer->ADD_GOSSIP_ITEM_EXTENDED(6, u8"输入|cff0070dd物品ID|r获取限制物品", GOSSIP_SENDER_MAIN, 777, u8"在弹框中输入物品ID编号 数量\n 例:|cFF00F0ff需要4个无底包|r，请输入:|cFFF0FF0014156   4|r", 0, true);
		}
		if (sPzxConfig.GetIntDefault("show.morebuff", 1)) {
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, u8"一键全BUFF、满血蓝怒、修理、冷却", GOSSIP_SENDER_MAIN, 208);
		}
	}



	if (!pPlayer->IsInCombat()|| pPlayer->IsGameMaster()) {//战斗中不显示菜单
		pPlayer->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_CHAT, u8"->天赋重置", GOSSIP_SENDER_MAIN, 105, u8"确定要|cff0070dd重置天赋|r吗?", 0, false);
		pPlayer->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_CHAT, u8"->角色更名", GOSSIP_SENDER_MAIN, 106, u8"确定要|cff0070dd更改此角色的名称|r吗?", 0, false);
		pPlayer->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_CHAT, u8"->清理副本CD", GOSSIP_SENDER_MAIN, 108, u8"确定要|cff0070dd清理所有副本CD|r吗?", 0, false);

		if (pPlayer->getLevel() < 70 || !addRep(pPlayer, false) || !check(pPlayer, false) || pPlayer->GetSkillValue(SKILL_FIRST_AID)<MYMAXSKILL || pPlayer->GetSkillValue(SKILL_FISHING)<MYMAXSKILL || pPlayer->GetSkillValue(SKILL_COOKING)<MYMAXSKILL) {
		}
		else {
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, u8"切换[占星者/奥尔多]声望", GOSSIP_SENDER_MAIN, 206);
		}
	}
	
	if (pPlayer->IsGameMaster()) {
		pPlayer->ADD_GOSSIP_ITEM(3, u8"重新加载系统参数", GOSSIP_SENDER_MAIN, 778);
	}
	pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _item->GetObjectGuid());
	return false;
}

bool GossipSelect_ItemPzx(Player *pPlayer, Item *_item, uint32 sender, const uint32 uiAction, char const* reStr)
{
	try
	{

	
	sLog.outString("[pzx-select] (%s:%d) Select action   [%d]", pPlayer->GetName(), pPlayer->GetObjectGuid().GetCounter(), uiAction);
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

		}
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
		if (sPzxConfig.GetIntDefault("show.morebuff", 1)) {
			if (!pPlayer->HasAura(35076))//阿达尔的祝福
				pPlayer->CastSpell(pPlayer, 35076, TRIGGERED_FULL_MASK);
			if (!pPlayer->HasAura(25392))//耐力
				pPlayer->CastSpell(pPlayer, 25392, TRIGGERED_FULL_MASK);
			if (!pPlayer->HasAura(26991))//爪子
				pPlayer->CastSpell(pPlayer, 26991, TRIGGERED_FULL_MASK);
			if (!pPlayer->HasAura(27127))
				pPlayer->CastSpell(pPlayer, 27127, TRIGGERED_FULL_MASK);//奥术光辉
			if (!pPlayer->HasAura(32999))
				pPlayer->CastSpell(pPlayer, 32999, TRIGGERED_FULL_MASK);//精神
			if (!pPlayer->HasAura(25289))
				pPlayer->CastSpell(pPlayer, 25289, TRIGGERED_FULL_MASK);//战斗怒吼

			if (pPlayer->GetPowerType() == POWER_MANA) {
				if (!pPlayer->HasAura(27143))
					pPlayer->CastSpell(pPlayer, 27143, TRIGGERED_FULL_MASK);//智慧祝福
			}
			else {
				if (!pPlayer->HasAura(20217))//王者
					pPlayer->CastSpell(pPlayer, 20217, TRIGGERED_FULL_MASK);
			}
			//if (!pPlayer->HasAura(27141))
			//		pPlayer->CastSpell(pPlayer, 27141, TRIGGERED_FULL_MASK);//力量祝福
		}
			pPlayer->DurabilityRepairAll(false, 0, false);//修理
			pPlayer->UpdateSkillsForLevel(true);//提升武器熟练度
			pPlayer->RemoveAllCooldowns();//冷却所有技能
			if (pPlayer->HasAura(15007))//移除复活虚弱
				pPlayer->RemoveAurasDueToSpell(15007);	

			pPlayer->SetHealth(pPlayer->GetMaxHealth());//满血满蓝满怒
			if (pPlayer->GetPowerType() == POWER_RAGE) {
				pPlayer->SetPower(POWER_RAGE, 100000);
			}
			else if (pPlayer->GetPowerType() == POWER_MANA) {
				pPlayer->SetPower(POWER_MANA, 100000);
			}
		
			ChatHandler(pPlayer).PSendSysMessage(u8"[系统消息]:您已经被强化了.奔跑吧...勇士");
	}
	else if (uiAction == 501) {
		if (!(pPlayer->GetGroup() && pPlayer->HasFlag(PLAYER_FLAGS, PLAYER_FLAGS_GROUP_LEADER))) {
			ChatHandler(pPlayer).PSendSysMessage(u8"|cffff0000[系统消息]:|h|r只有团队领袖才能使用此功能");
			pPlayer->CLOSE_GOSSIP_MENU();
			return false;
		}
		Player * _player = pPlayer;
		for (GroupReference* itr = pPlayer->GetGroup()->GetFirstMember(); itr != nullptr; itr = itr->next())
		{
			Player* target = itr->getSource();
			// check online security
			/*if (ChatHandler(pPlayer).HasLowerSecurity(target))
				return false;
				
			std::string chrNameLink = playerLink(target_name);*/

			if (!target || !target->GetSession()|| target->GetSession()->isLogingOut())
				continue;
			if (_player == target) {
				continue;
			}

			// check online security
			if (target->IsInCombat() || target->IsBeingTeleported() || target->IsTaxiFlying()) {//战斗中正在传送中不可再次使用
				ChatHandler(pPlayer).PSendSysMessage(u8"团队成员[|cffff0000%s|h|r]正在战斗中", target->GetName());
				continue;
			}

			Map* cMap = target->GetMap();
			if (cMap->IsDungeon())
			{
				// we have to go to instance, and can go to player only if:
				//   1) we are in his group (either as leader or as member)
				//   2) we are not bound to any group and have GM mode on
				if (_player->GetGroup() && _player->GetGroup() != target->GetGroup()) {
					sLog.outError("[系统错误] (%d)-[%s]", target->GetGUIDLow(), target->GetName());
					_player->CLOSE_GOSSIP_MENU();
					return false;
				}

				// if the player or the player's group is bound to another instance
				// the player will not be bound to another one
				InstancePlayerBind* pBind = _player->GetBoundInstance(target->GetMapId(), target->GetDifficulty());
				if (!pBind)
				{
					Group* group = _player->GetGroup();
					// if no bind exists, create a solo bind
					InstanceGroupBind* gBind = group ? group->GetBoundInstance(target->GetMapId()) : nullptr;
					// if no bind exists, create a solo bind
					if (!gBind)
					{
						DungeonPersistentState* save = ((DungeonMap*)target->GetMap())->GetPersistanceState();

						// if player is group leader then we need add group bind
						if (group && group->IsLeader(_player->GetObjectGuid()))
							group->BindToInstance(save, !save->CanReset());
						else
							_player->BindToInstance(save, !save->CanReset());
					}
				}

				_player->SetDifficulty(target->GetDifficulty());
			}


	/*		if ( _player->IsVisibleGloballyFor(target)) {
				ChatHandler(target).PSendSysMessage(u8"[|cffff0000系统消息|h|r]:团长正在召唤你[%s]", target->GetName());
			}*/
			// stop flight if need
			if (!_player->TaxiFlightInterrupt())
				_player->SaveRecallPosition(); 

			if (target->IsAlive() && target->GetDistance(_player,false, DIST_CALC_BOUNDING_RADIUS) < sPzxConfig.GetIntDefault("raidTool.instance", 30)&& target->IsWithinLOSInMap(_player)) {
				//小于50码而且在同一地图，不召唤
				continue;
			}
			if (target->IsAlive()) {
				_player->SetTarget(target);
				_player->SetSelectionGuid(target->GetObjectGuid());
				SpellCastResult rlt=_player->CastSpell(target, 7720, TRIGGERED_OLD_TRIGGERED);
				if (rlt != SPELL_CAST_OK) {
					ChatHandler(_player).PSendSysMessage(u8"[|cffff0000系统消息|h|r]:召唤失败[%s]-[原因:%d]", target->GetName(), rlt);
				}
				else {
					ChatHandler(target).PSendSysMessage(u8"[|cffff0000系统消息|h|r]:团长正在召唤你[%s],请尽快脱离战斗或副本战场区域", target->GetName());
				}
			}
			else {
				//开始拉人代码
				try
				{
					float x, y, z;
					//_player->GetPosition(x, y, z);
					_player->GetContactPoint(_player, x, y, z);
					Corpse* pcor = target->GetCorpse();
					if (!pcor) {//没释放，还没有尸体
						if (_player->IsWithinLOSInMap(target)) {
							_player->SetTarget(target);
							_player->SetSelectionGuid(target->GetObjectGuid());
							_player->CastSpell(target, 20770, TRIGGERED_OLD_TRIGGERED);
						}
						else {//不在视野的直接拉过来
							target->TeleportTo(_player->GetMapId(), x, y, z + 2.0f, target->GetAngle(_player));
						}
						continue;
					}
					//释放了,直接复活再拉人
					target->ResurrectPlayer(0.99f);
					target->SpawnCorpseBones();
	/*				if (target->IsAlive()) {
						SpellCastResult rlt = _player->CastSpell(target, 7720, TRIGGERED_OLD_TRIGGERED);
						ChatHandler(target).PSendSysMessage(u8"call %s->(%d)", target->GetName(), rlt);
					}*/
				}
				catch (const std::exception&)
				{
					//PSendSysMessage(player, u8"召唤玩家 |cffff0000[%s]|h|r 失败", pl->GetName());
					sLog.outError("[pzx-exception-call] (%d)-[%s]", target->GetGUIDLow(), target->GetName());
				}
			}
			// to point to see at target with same orientation
			/*float x, y, z;
			target->GetContactPoint(target, x, y, z);
			if (GenericTransport* transport = target->GetTransport())
				transport->CalculatePassengerOffset(x, y, z);
			_player->TeleportTo(target->GetMapId(), x, y, z, _player->GetAngle(target), TELE_TO_GM_MODE, nullptr, target->GetTransport());*/
		}
		ChatHandler(_player).PSendSysMessage(u8"[|cffff0000系统消息|h|r]:您的队伍已经|cff00ff00集合完毕|h|r");
		_player->CLOSE_GOSSIP_MENU();

		return true;
	}
	else if (uiAction == 502) {
		if (!(pPlayer->GetGroup() && pPlayer->HasFlag(PLAYER_FLAGS, PLAYER_FLAGS_GROUP_LEADER))) {
			ChatHandler(pPlayer).PSendSysMessage(u8"|cffff0000[系统消息]:|h|r只有团队领袖才能使用此功能");
			pPlayer->CLOSE_GOSSIP_MENU();
			return false;
		}
		for (GroupReference* itr = pPlayer->GetGroup()->GetFirstMember(); itr != nullptr; itr = itr->next())
		{
			Player* pl = itr->getSource();
			if (!pl || !pl->GetSession())
				continue;
			if (pl->IsAlive() &&  pl->GetMap() && pl->GetMap()->IsDungeon()&& pl->GetMap()== pPlayer->GetMap()) {
				//杀死副本内所有玩家
				pPlayer->DealDamage(pl, pl, pl->GetHealth(), nullptr, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, nullptr, false);
				//pPlayer->DealDamage(pl, pl->GetHealth(), nullptr, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, nullptr, false);
				//PSendSysMessage(player, u8"团队成员[|cff00ff00%s|h|r]已经被杀死",pl->GetName());
			}
		}
		//
		pPlayer->CLOSE_GOSSIP_MENU();
		return GossipHello_ItemPzx(pPlayer, _item);
	}
	else if (uiAction == 508) {
		pPlayer->setCustomPzxAuaraMutil(PLAYED_PZXAURA_ONOFF, 0.0f);
		pPlayer->CLOSE_GOSSIP_MENU();
		return false;
	}
	else if (uiAction == 509) {

		if (!reStr || strlen(reStr)>4) {
			ChatHandler(pPlayer).PSendSysMessage(u8"[系统消息]:请输入正确的物品ID和数量ID");
			pPlayer->CLOSE_GOSSIP_MENU();
			return false;
		}
		std::string a(reStr);
		try {
				float input = std::stof(a);
				if (input > 10.0f) {
					input = 10.0f;
				}
				if (input < 0.0f) {
					input = 0.0f;
				}
				pPlayer->setCustomPzxAuaraMutil(PLAYED_PZXAURA_ONOFF, input);
			}
			catch (...) {
			ChatHandler(pPlayer).PSendSysMessage(u8"[系统消息]:请输入正确的区间值");
		}
		pPlayer->CLOSE_GOSSIP_MENU();
		return false;
	}
	else if (uiAction == 505) {
		pPlayer->setCustomPzxAuaraMutil(PLAYED_PZXAURA_ONOFF, 1.0f);
		pPlayer->CLOSE_GOSSIP_MENU();
		return false;
	}
	else if (uiAction == 505) {
		pPlayer->setCustomPzxAuaraMutil(PLAYED_PZXAURA_ONOFF, 1.0f);
		pPlayer->CLOSE_GOSSIP_MENU();
		return false;
	}
	else if (uiAction == 507) {
		pPlayer->setCustomPzxAuaraMutil(PLAYED_PZXAURA_ONOFF, 1.0f);
		pPlayer->CLOSE_GOSSIP_MENU();
		return false;
	}
	else if (uiAction == 300) {
		if (pPlayer->IsGameMaster()) {//GM 都可以传送
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送--> 奥格瑞玛", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送--> 幽暗城", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送--> 雷霆崖", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送--> 银月城", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送--> 铁炉堡", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送--> 暴风城", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送--> 达纳苏斯", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送--> 埃索达", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 10);
		}else if (pPlayer->GetTeam() == HORDE) {
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送--> 奥格瑞玛", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送--> 幽暗城", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送--> 雷霆崖", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送--> 银月城", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9);
		}else {
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送--> 铁炉堡", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送--> 暴风城", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送--> 达纳苏斯", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送--> 埃索达", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 10);
		}
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送区域--> 卡拉赞", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
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
				pPlayer->TeleportTo(530, 3100.48f, 1536.49f, 190.3f, 4.62226f);
				break;

			case 12://盘牙水库
				pPlayer->CLOSE_GOSSIP_MENU();
				pPlayer->TeleportTo(530, 738.865f, 6865.77f, -69.4659f, 6.27655f);
				break;

			case 13://奥金顿60级"
				pPlayer->CLOSE_GOSSIP_MENU();
				pPlayer->TeleportTo(530, -3324.49f, 4943.45f, -101.239f, 4.63901f);
				break;
			case 14://地狱火堡垒60级
				pPlayer->CLOSE_GOSSIP_MENU();
				pPlayer->TeleportTo(530, -347.29f, 3089.82f, 21.394f, 5.68114f);
				break;
			case 15://时光之穴65级
				pPlayer->CLOSE_GOSSIP_MENU();
				pPlayer->TeleportTo(1, -8369.65f, -4253.11f, -204.272f, -2.70526f);
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
			sLog.outString(u8"[GetItembyID] (%s:%d) Input str: [%s]", pPlayer->GetName(),pPlayer->GetObjectGuid().GetCounter(),reStr );
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

				if (pProto->Quality < sPzxConfig.GetIntDefault("item.quality", ITEM_QUALITY_LEGENDARY) && pProto->ItemLevel < sPzxConfig.GetIntDefault("item.level", 151))
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
					ChatHandler(pPlayer).PSendSysMessage(u8"[系统消息]:获取物品等级过高,需小于[%d]等级;如需要特殊物品，请联系管理园微信:solomon0728", sPzxConfig.GetIntDefault("item.level", 155));
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
	catch (...)
	{
		sLog.outString("[pzx-exception] (%s:%d) Select action   [%d]", pPlayer->GetName(), pPlayer->GetObjectGuid().GetCounter(), uiAction);
		pPlayer->CLOSE_GOSSIP_MENU();
		return true;
}
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
