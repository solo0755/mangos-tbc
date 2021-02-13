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
	if (pPlayer->GetGroup()) {//TODO 换成全员都显示，但是只能团长执行
		Player* leader = sObjectMgr.GetPlayer(pPlayer->GetGroup()->GetLeaderGuid());
		if (leader&&leader->GetSession()&&pPlayer->GetCustomPzxAuaraMutil(PLAYED_PZXAURA_ONOFF) > 0 ) {

			std::ostringstream oss;
			float dx = leader->GetCustomPzxAuaraMutil(PLAYED_PZXAURA_ONOFF);
			oss << u8"|cffff0000关闭|r 副本弹性模式--->当前设置值[|cff00ff00" << dx << "|r]";
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, oss.str().c_str(), GOSSIP_SENDER_MAIN, 508);

		}else {
			pPlayer->ADD_GOSSIP_ITEM_EXTENDED(6, u8"|cff00ff00开启|r 当前副本弹性模式,默认值:[1]", GOSSIP_SENDER_MAIN, 509, u8"在弹框中输入1~10.0 数量\n 例:|cFF00F0ff更改团队弹性值百分比200%|r，请输入:|cFFF0FF002.0|r", 0, true);
		}
	}
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, u8"一键|cff0070dd复活拉人|r", GOSSIP_SENDER_MAIN, 501);
		pPlayer->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_TALK, u8"一键|cff0070dd秒杀全团|r", GOSSIP_SENDER_MAIN, 502, u8"确定要|cFFF0FF00秒杀全团|r吗?", 0, false);
	if (!pPlayer->IsInCombat()|| pPlayer->IsGameMaster()) {//战斗中不显示菜单
		//if (pPlayer->getLevel() < 70|| !addRep(pPlayer, false)|| !check(pPlayer, false)|| pPlayer->GetSkillValue(SKILL_FIRST_AID)<MYMAXSKILL|| pPlayer->GetSkillValue(SKILL_FISHING)<MYMAXSKILL|| pPlayer->GetSkillValue(SKILL_COOKING)<MYMAXSKILL) {
		//	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, u8"开始新的旅途(必选哦~)", GOSSIP_SENDER_MAIN, 101);
		//}
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送--> 沙塔斯城（|cffFF00c8新手接待|r）", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
		
		
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送--> 其他区域", GOSSIP_SENDER_MAIN, 300);


		if (sPzxConfig.GetIntDefault("show.morebuff", 1)) {
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, u8"一键全BUFF、满血蓝怒、修理、冷却", GOSSIP_SENDER_MAIN, 208);
		}
		//pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, u8"免费学习-|cff6247c8商业技能|h|r", GOSSIP_SENDER_MAIN, 301);
		if (sPzxConfig.GetIntDefault("show.additem", 1)) {
			pPlayer->ADD_GOSSIP_ITEM_EXTENDED(6, u8"输入|cff0070dd物品ID|r获取限制物品", GOSSIP_SENDER_MAIN, 777, u8"在弹框中输入物品ID编号 数量\n 例:|cFF00F0ff需要4个无底包|r，请输入:|cFFF0FF0014156   4|r", 0, true);
			pPlayer->ADD_GOSSIP_ITEM(7, u8"免费获取-|cff6247c8职业套装|h|r", GOSSIP_SENDER_MAIN, 400);
		}

		//pPlayer->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_CHAT, u8"->天赋重置", GOSSIP_SENDER_MAIN, 105, u8"确定要|cff0070dd重置天赋|r吗?", 0, false);
		//pPlayer->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_CHAT, u8"->角色更名", GOSSIP_SENDER_MAIN, 106, u8"确定要|cff0070dd更改此角色的名称|r吗?", 0, false);
		pPlayer->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_CHAT, u8"->清理副本CD", GOSSIP_SENDER_MAIN, 108, u8"确定要|cff0070dd清理所有副本CD|r吗?", 0, false);
		if (pPlayer->getClass() == CLASS_HUNTER) {
			pPlayer->ADD_GOSSIP_ITEM(3, u8"提升 我的宠物|cff6247c8忠诚度和等级|h|r ", GOSSIP_SENDER_MAIN, 205);
		}

	}
	//pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, u8"切换[占星者/奥尔多]声望", GOSSIP_SENDER_MAIN, 206);
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
	if (uiAction > 100 && uiAction <= 500) {
		return GossipMainMenu(pPlayer, _item->GetObjectGuid(), sender, uiAction, reStr);
	}
	else if (uiAction == 501) {
		if (!pPlayer->GetGroup() || !(pPlayer->GetGroup() && pPlayer->HasFlag(PLAYER_FLAGS, PLAYER_FLAGS_GROUP_LEADER))) {
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
		if (!pPlayer->GetGroup() || !(pPlayer->GetGroup() && pPlayer->HasFlag(PLAYER_FLAGS, PLAYER_FLAGS_GROUP_LEADER))) {
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
		if (!pPlayer->GetGroup() || !(pPlayer->GetGroup() && pPlayer->HasFlag(PLAYER_FLAGS, PLAYER_FLAGS_GROUP_LEADER))) {
			ChatHandler(pPlayer).PSendSysMessage(u8"|cffff0000[系统消息]:|h|r只有团队领袖才能使用此功能");
			pPlayer->CLOSE_GOSSIP_MENU();
			return false;
		}
		pPlayer->setCustomPzxAuaraMutil(PLAYED_PZXAURA_ONOFF, 0.0f);
		pPlayer->CLOSE_GOSSIP_MENU();
		return false;
	}
	else if (uiAction == 509) {
		if (!pPlayer->GetGroup()||!(pPlayer->GetGroup() && pPlayer->HasFlag(PLAYER_FLAGS, PLAYER_FLAGS_GROUP_LEADER))) {
			ChatHandler(pPlayer).PSendSysMessage(u8"|cffff0000[系统消息]:|h|r只有团队领袖才能使用此功能");
			pPlayer->CLOSE_GOSSIP_MENU();
			return false;
		}

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

	else if(uiAction == 777) {
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

		}
	else if(uiAction == 778) {
			sPzxConfig.Reload();
			ChatHandler(pPlayer).PSendSysMessage(u8"[系统消息]:config文件加载成功");
			pPlayer->CLOSE_GOSSIP_MENU();
			return true;
	}

	pPlayer->CLOSE_GOSSIP_MENU();
	return true;
   }catch (...)
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
