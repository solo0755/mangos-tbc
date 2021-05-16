#include "AI/ScriptDevAI/include/example.h"

std::vector<uint32> itemset(Player *player) {
	uint8 clazz = player->getClass();
	std::string classIds("itemset_" + std::to_string(clazz));
	//std::string names(clazz + "_setname");
	Tokens tokensNames = StrSplit(sPzxConfig.GetStringDefault(classIds, ""), ",");
	std::vector<uint32> ids;
	for (auto& tokenName : tokensNames)
	{
		std::string name(tokenName.c_str());
		if (name.length() > 0) {
			try
			{
				uint32 vid = atoi(name.c_str());
				ids.push_back(vid);
			}
			catch (const std::exception&)
			{
				sLog.outError("[pzx itemset config error]:%s", name);
				break;
			}
		}
	}
	return ids;
}
bool check(Player *player, bool modify) {
	bool isok = true;
	const static uint32 NUM_BREATHS = sizeof(all) / sizeof(all[0]);
	for (uint32 id = 0; id < NUM_BREATHS; id++) {
		const initClazz clzz = all[id];
		if (clzz.clazz > 0 && player->getClass() == clzz.clazz) {
			//检查技能
			const uint32* spells = clzz.checkSpells;
			while (spells != nullptr&&*spells > 0) {
				if (!player->HasSpell(*spells)) {
					isok = false;
					if (modify) {
						player->learnSpell(*spells, false);
					}
					else
						break;
				}
				spells++;
			}

			//检查物品
			const uint32* items = clzz.checkItems;
			while (items != nullptr&&*items > 0) {
				if (!player->HasItemCount(*items, 1, true)) {
					isok = false;
					if (modify) {
						ItemPosCountVec dest;
						InventoryResult msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, *items, 1);
						if (msg == EQUIP_ERR_OK)
						{
							Item* item = player->StoreNewItem(dest, *items, true);
							//player->SendNewItem(item, 1, false, true);
							player->SendNewItem(item, 1, true, false);
							ChatHandler(player).PSendSysMessage(u8"[系统消息]:%s 已经添加到你包中", item->GetProto()->Name1);
						}
						else
						{
							player->SendEquipError(msg, nullptr, nullptr, *items);
							ChatHandler(player).PSendSysMessage(u8"[系统消息]:请保持包包有足够空间");
							isok = false;
						}
					}
					else
						break;//检查模式就停止
				}
				items++;
			}

		}
		if (!modify && !isok) {
			break;
		}
	}


	return isok;

}
void addOneItemToPlayer(uint32 itemid, Player* player) {
	if (!player->HasItemCount(itemid, 1, true)) {//已经有一件了
		ItemPosCountVec dest;
		InventoryResult msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, itemid, 1);
		if (msg == EQUIP_ERR_OK)
		{
			Item* item = player->StoreNewItem(dest, itemid, true);
			player->SendNewItem(item, 1, true, false);
			ChatHandler(player).PSendSysMessage(u8"[系统消息]:%s 已经添加到你包中", item->GetProto()->Name1);
		}
	}
}


bool addRep(Player *player, bool modify) {
	bool isok = true;
	const uint32* fas = player->GetTeam() == HORDE ? factionID[0] : factionID[1];

	//const static uint32 NUM_BREATHS = sizeof(fas) / sizeof(fas[0]);
	for (uint32 id = 0; id < 6; id++) {
		//FactionEntry const *factionEntry = sObjectMgr.getFactionEntry(fas[id]);//faction ID 参考DPS
		FactionEntry const* factionEntry = sFactionStore.LookupEntry<FactionEntry>(fas[id]);
		if (player->GetReputationMgr().GetReputation(factionEntry) < sPzxConfig.GetIntDefault("rep.init", 42001)) {
			if (modify) {
				player->GetReputationMgr().SetReputation(factionEntry, sPzxConfig.GetIntDefault("rep.init", 42001));//声望值

			}
			isok = false;
		}
	}
	if (modify) {
		addOneItemToPlayer(31704, player);//风暴钥匙
		addOneItemToPlayer(24490, player);//卡拉赞钥匙
		addOneItemToPlayer(31084, player);//禁魔监狱
		addOneItemToPlayer(30634, player);//平民窟区域
		addOneItemToPlayer(30637, player);//萨尔玛H本钥匙
		addOneItemToPlayer(28395, player);//破损大厅钥匙
		addOneItemToPlayer(27991, player);//暗影迷宫钥匙
		addOneItemToPlayer(30623, player);//水库钥匙
		addOneItemToPlayer(30633, player);//水库H本钥匙

	}

	return isok;
}

void addItemSet(Player *player, uint32 itemsetid) {
	//uint32 itemsetid = IDS[player->getClass()][itemindex];
	if (itemsetid) {
		for (uint32 id = 0; id < sItemStorage.GetMaxEntry(); id++)
		{
			ItemPrototype const *pProto = sItemStorage.LookupEntry<ItemPrototype>(id);
			if (!pProto)
				continue;

			if (pProto->ItemSet == itemsetid)
			{
				if (player->HasItemCount(pProto->ItemId, 1, true)) {//已经有一件了
					continue;
				}
				ItemPosCountVec dest;
				InventoryResult msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, pProto->ItemId, 1);
				if (msg == EQUIP_ERR_OK)
				{
					Item* item = player->StoreNewItem(dest, pProto->ItemId, true);

					// remove binding (let GM give it to another player later)

					//player->SendNewItem(item, 1, false, true);
					player->SendNewItem(item, 1, true, false);
					ChatHandler(player).PSendSysMessage(u8"[系统消息]:%s 已经添加到你包中", item->GetProto()->Name1);
				}
				else
				{
					player->SendEquipError(msg, nullptr, nullptr, pProto->ItemId);
					ChatHandler(player).PSendSysMessage(u8"[系统消息]:请保持包包有足够空间");
					//PSendSysMessage(LANG_ITEM_CANNOT_CREATE, pProto->ItemId, 1);
				}
			}
		}

	}
}
void LearnSkillRecipesHelper(Player *player, uint32 skill_id)
{
	uint32 classmask = player->getClassMask();

	for (uint32 j = 0; j < sSkillLineAbilityStore.GetNumRows(); ++j)
	{
		SkillLineAbilityEntry const* skillLine = sSkillLineAbilityStore.LookupEntry(j);
		if (!skillLine)
			continue;

		// wrong skill
		if (skillLine->skillId != skill_id)
			continue;

		// not high rank
		if (skillLine->forward_spellid)
			continue;

		// skip racial skills
		if (skillLine->racemask != 0)
			continue;

		// skip wrong class skills
		if (skillLine->classmask && (skillLine->classmask & classmask) == 0)
			continue;

		SpellEntry const* spellInfo = sSpellTemplate.LookupEntry<SpellEntry>(skillLine->spellId);
		if (!spellInfo || !SpellMgr::IsSpellValid(spellInfo, player, false))
			continue;

		player->learnSpell(skillLine->spellId, false);
	}
}

bool LearnAllRecipesInProfession(Player *pPlayer, SkillType skill)
{
	ChatHandler handler(pPlayer->GetSession());
	char* skill_name;

	SkillLineEntry const *SkillInfo = sSkillLineStore.LookupEntry(skill);
	skill_name = SkillInfo->name[4];

	if (!SkillInfo)
	{
		sLog.outError("Profession NPC: received non-valid skill ID");
		return false;
	}
	pPlayer->SetSkill(SkillInfo->id, MYMAXSKILL, MYMAXSKILL);
	LearnSkillRecipesHelper(pPlayer, SkillInfo->id);
	ChatHandler(pPlayer).PSendSysMessage(u8"所有 %s 配方已经学习完成", skill_name);
	return true;
}

void CompleteLearnProfession(Player *pPlayer, SkillType skill)
{
	if (pPlayer->GetFreePrimaryProfessionPoints() == 0 && !(skill == SKILL_COOKING || skill == SKILL_FIRST_AID || skill == SKILL_FISHING))
	{
		ChatHandler(pPlayer).PSendSysMessage(u8"[系统消息]:你已经拥有了|cffff00002项专业技能|h|r了.");
	}
	else
	{
		if (!LearnAllRecipesInProfession(pPlayer, skill))
			ChatHandler(pPlayer).PSendSysMessage(u8"系统错误.练习管理人员");
	}
}
bool GossipSelect_ProfessionNPC(Player* player, uint32 sender, const uint32 action)
{
	switch (action)
	{
	case 1:
		if (player->GetSkillValue(SKILL_ALCHEMY) < MYMAXSKILL)
			CompleteLearnProfession(player, SKILL_ALCHEMY);
		break;
	case 2:
		if (player->GetSkillValue(SKILL_BLACKSMITHING) < MYMAXSKILL)
			CompleteLearnProfession(player, SKILL_BLACKSMITHING);
		break;
	case 3:
		if (player->GetSkillValue(SKILL_LEATHERWORKING) < MYMAXSKILL)
			CompleteLearnProfession(player, SKILL_LEATHERWORKING);
		break;
	case 4:
		if (player->GetSkillValue(SKILL_TAILORING) < MYMAXSKILL)
			CompleteLearnProfession(player, SKILL_TAILORING);
		break;
	case 5:
		if (player->GetSkillValue(SKILL_ENGINEERING) < MYMAXSKILL)
			CompleteLearnProfession(player, SKILL_ENGINEERING);
		break;
	case 6:
		if (player->GetSkillValue(SKILL_ENCHANTING) < MYMAXSKILL)
			CompleteLearnProfession(player, SKILL_ENCHANTING);
		break;
	case 7:
		if (player->GetSkillValue(SKILL_JEWELCRAFTING) < MYMAXSKILL)
			CompleteLearnProfession(player, SKILL_JEWELCRAFTING);
		break;

	case 9:
		if (player->GetSkillValue(SKILL_HERBALISM) < MYMAXSKILL)
			CompleteLearnProfession(player, SKILL_HERBALISM);
		break;
	case 10:
		if (player->GetSkillValue(SKILL_SKINNING) < MYMAXSKILL)
			CompleteLearnProfession(player, SKILL_SKINNING);
		break;
	case 11:
		if (player->GetSkillValue(SKILL_MINING) < MYMAXSKILL)
			CompleteLearnProfession(player, SKILL_MINING);
		break;

	}

	player->CLOSE_GOSSIP_MENU();
	return true;
}
bool GossipMainMenu(Player *pPlayer, ObjectGuid guid, uint32 sender, const uint32 uiAction, char const* reStr) {
	if (uiAction < 1000 && uiAction>100) {
		sLog.outString("[pzx-select] (%s:%d) Select action   [%d]", pPlayer->GetName(), pPlayer->GetObjectGuid().GetCounter(), uiAction);
		if (uiAction == 101)
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

			if (pPlayer->GetSkillValue(SKILL_FIRST_AID) < MYMAXSKILL)
				CompleteLearnProfession(pPlayer, SKILL_FIRST_AID);
			if (pPlayer->GetSkillValue(SKILL_FISHING) < MYMAXSKILL)
				CompleteLearnProfession(pPlayer, SKILL_FISHING);
			if (pPlayer->GetSkillValue(SKILL_COOKING) < MYMAXSKILL)
				CompleteLearnProfession(pPlayer, SKILL_COOKING);

			if (sPzxConfig.GetIntDefault("initItemSet", 1) <= 2) {
				addItemSet(pPlayer, sPzxConfig.GetIntDefault("initItemSet", 0));//增加T1套装
			}
			pPlayer->CLOSE_GOSSIP_MENU();
			return true;
		}
		else if (uiAction == 103) {
			/*	if (!player->HasItemCount(sPzxConfig.GetIntDefault("vipItemID", 40003), 1, true)) {
			ChatHandler(pPlayer).PSendSysMessage( u8"[系统消息]:需要VIP认证卡才可以使用本功能，请联系GM获取");
			player->CLOSE_GOSSIP_MENU();
			return false;
			}*/
			sLog.outString(u8"[GetItembyID] (%s:%d) Input str: [%s]", pPlayer->GetName(), pPlayer->GetObjectGuid().GetCounter(), reStr);
			if (!reStr || strlen(reStr) > 12) {
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
					ChatHandler(pPlayer).PSendSysMessage(u8"[系统消息]:获取物品级别过高,需小于[%d]等级;如需要特殊物品，请联系管理园微信:solomon0728", sPzxConfig.GetIntDefault("item.level", 155));
				}
			}
			else
				ChatHandler(pPlayer).PSendSysMessage(u8"[系统消息]:物品未找到");
			//继续显示这个页面
			pPlayer->CLOSE_GOSSIP_MENU();
			return true;

		}
		else if (uiAction == 108) {
			resetIntance(pPlayer, 0);
			return true;
		}
		else if (uiAction == 105) {
			pPlayer->resetTalents(true);
			pPlayer->CastSpell(pPlayer, 14867, TRIGGERED_OLD_TRIGGERED);
			ChatHandler(pPlayer).PSendSysMessage(u8"[系统消息]:|cff00ff00 天赋已经重置.|h|r");
			return true;
		}
		else if (uiAction == 106) {
			pPlayer->SetAtLoginFlag(AT_LOGIN_RENAME);
			CharacterDatabase.PExecute("UPDATE characters SET at_login = at_login | '1' WHERE guid = '%u'", pPlayer->GetGUIDLow());
			ChatHandler(pPlayer).PSendSysMessage(u8"[系统消息]:|cff00ff00 请返回到人物界面后更改您的新角色名.|h|r");
			return true;
		}
		else if (uiAction == 400) {
			std::vector<uint32> ids = itemset(pPlayer);
			for (uint32 i = 0; i < ids.size(); i++)
			{
				//获取套装中文名
				ItemSetEntry const* set = sItemSetStore.LookupEntry(ids[i]);
				if (set)
				{
					int loc = pPlayer->GetSession()->GetSessionDbcLocale();
					std::string name = set->name[loc];
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, name.c_str(), GOSSIP_SENDER_MAIN, 400 + i + 1);
				}

			}
			pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, guid);
			return true;
		}

		else if (uiAction == 205) {
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

				}
			}
			else {
				ChatHandler(pPlayer).PSendSysMessage(u8"[系统消息]:请先|cffff0000 驯服或者召唤出|h|r一只要强化的宠物");

			}
			pPlayer->CLOSE_GOSSIP_MENU();
			return true;
		}
		else if (uiAction == 206) {
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
			return true;
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
			pPlayer->CLOSE_GOSSIP_MENU();
			return true;
		}
		else if (uiAction == 209) {
			pPlayer->CLOSE_GOSSIP_MENU();
			//pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, u8"创建--> 《伊利丹》进度", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 31);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, u8"创建--> 《布鲁塔鲁斯》斯进度", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 32);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, u8"创建--> 《菲米丝》进度", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 33);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, u8"创建--> 《艾瑞达双子》进度", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 34);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, u8"创建--> 《穆鲁》进度", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 35);
			pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, guid);
			return true;
		}
		else if (uiAction == 300) {
			pPlayer->CLOSE_GOSSIP_MENU();
			if (pPlayer->IsGameMaster()) {//GM 都可以传送
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送--> 奥格瑞玛", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送--> 幽暗城", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送--> 雷霆崖", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送--> 银月城", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送--> 铁炉堡", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送--> 暴风城", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送--> 达纳苏斯", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送--> 埃索达", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 10);
			}
			else if (pPlayer->GetTeam() == HORDE) {
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
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送区域--> 卡拉赞", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
			//5个区域本
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送区域--> 风暴要塞", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送区域--> 盘牙水库", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 12);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送区域--> 奥金顿", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 13);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送区域--> 地狱火堡垒", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 14);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送区域--> 时光之穴", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 15);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"传送区域--> 太阳井高地", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 16);

			pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, guid);
			return true;
		}
		else if (uiAction == 301) {
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

			pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, guid);
			return true;
		}
		else if (uiAction - 301 <= 14 && uiAction - 301 >= 1) {
			return GossipSelect_ProfessionNPC(pPlayer, sender, uiAction - 301);
		}
		else if ((uiAction - 400) <= 15 && (uiAction - 400) >= 1) {//401~415
			uint32 index = uiAction - 400 - 1;
			std::vector<uint32> ids = itemset(pPlayer);
			if (index <= ids.size() - 1) {
				addItemSet(pPlayer, ids[index]);
			}
			else {
				ChatHandler(pPlayer).PSendSysMessage(u8"[系统消息]:系统itemset配置异常");
			}
			pPlayer->CLOSE_GOSSIP_MENU();
			return true;
		}
		else if (uiAction - GOSSIP_ACTION_INFO_DEF > 0 && uiAction - GOSSIP_ACTION_INFO_DEF < 20) {//20个传送点
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
			case 16://太阳井高地
				pPlayer->CLOSE_GOSSIP_MENU();
				pPlayer->TeleportTo(530, 12582.3f, -6775.1f, 15.1f, 6.2f);
				break;
			default:
				break;
			}
			return true;
		}
		else if (uiAction - GOSSIP_ACTION_INFO_DEF > 0 && uiAction - GOSSIP_ACTION_INFO_DEF < 40) {//20个传送点
			if (pPlayer->GetGroup() || pPlayer->GetMapRef()->IsDungeon()) {
				ChatHandler(pPlayer).PSendSysMessage(u8"[系统消息]:|cffff0000您已经在队伍或者副本中|h|r，请先脱离队伍或离开副本区域.");
				pPlayer->CLOSE_GOSSIP_MENU();
				return false;
			}
			int index = uiAction - GOSSIP_ACTION_INFO_DEF-30;
			switch (index)
			{
				//case 1://伊利丹
				//	pPlayer->CLOSE_GOSSIP_MENU();
				//	pPlayer->TeleportTo(0, -11120.2f, -2015.27f, 47.1869f, 1.91823f);
				//	break;

			case 2://布胖
				pPlayer->CLOSE_GOSSIP_MENU();
				saveInstance(pPlayer, 1, "3 0 0 0 0 0");
				ChatHandler(pPlayer).PSendSysMessage(u8"[系统消息]:太阳井高地|cffff0000 布鲁塔鲁斯 |h|r进度已经创建");
				break;

			case 3://菲米丝
				pPlayer->CLOSE_GOSSIP_MENU();
				saveInstance(pPlayer, 2, "3 3 0 0 0 0");
				ChatHandler(pPlayer).PSendSysMessage(u8"[系统消息]:太阳井高地|cffff0000 菲米丝 |h|r进度已经创建");
				break;
			case 4://双子
				pPlayer->CLOSE_GOSSIP_MENU();
				saveInstance(pPlayer, 3, "3 3 3 0 0 0");
				ChatHandler(pPlayer).PSendSysMessage(u8"[系统消息]:太阳井高地|cffff0000 艾瑞达双子 |h|r进度已经创建");
				break;
			case 5://穆鲁
				pPlayer->CLOSE_GOSSIP_MENU();
				saveInstance(pPlayer, 4, "3 3 3 3 0 0");
				ChatHandler(pPlayer).PSendSysMessage(u8"[系统消息]:太阳井高地|cffff0000 穆鲁 |h|r进度已经创建");
				break;
			default:
				break;
			}
		}
	}
	pPlayer->CLOSE_GOSSIP_MENU();
	return true;
}