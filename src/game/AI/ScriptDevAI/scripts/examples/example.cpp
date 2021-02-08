#include "AI/ScriptDevAI/include/example.h"

std::vector<uint32> itemset(Player *player) {
	uint8 clazz=player->getClass();
	std::string classIds("itemset_"+ std::to_string(clazz) );
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
	for (uint32 id = 0; id < 5; id++) {
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
		addOneItemToPlayer(30637, player);//破损大厅
		addOneItemToPlayer(27991, player);//安逸迷宫
		addOneItemToPlayer(30623, player);//水库钥匙
		
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

void CompleteLearnProfession(Player *pPlayer,  SkillType skill)
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
bool GossipSelect_ProfessionNPC(Player* player,  uint32 sender, const uint32 action)
{
	switch (action)
	{
	case 1:
		if (player->GetSkillValue(SKILL_ALCHEMY)<MYMAXSKILL)
			CompleteLearnProfession(player, SKILL_ALCHEMY);
		break;
	case 2:
		if (player->GetSkillValue(SKILL_BLACKSMITHING)<MYMAXSKILL)
			CompleteLearnProfession(player,  SKILL_BLACKSMITHING);
		break;
	case 3:
		if (player->GetSkillValue(SKILL_LEATHERWORKING)<MYMAXSKILL)
			CompleteLearnProfession(player,  SKILL_LEATHERWORKING);
		break;
	case 4:
		if (player->GetSkillValue(SKILL_TAILORING)<MYMAXSKILL)
			CompleteLearnProfession(player,  SKILL_TAILORING);
		break;
	case 5:
		if (player->GetSkillValue(SKILL_ENGINEERING)<MYMAXSKILL)
			CompleteLearnProfession(player,  SKILL_ENGINEERING);
		break;
	case 6:
		if (player->GetSkillValue(SKILL_ENCHANTING)<MYMAXSKILL)
			CompleteLearnProfession(player,  SKILL_ENCHANTING);
		break;
	case 7:
		if (player->GetSkillValue(SKILL_JEWELCRAFTING)<MYMAXSKILL)
			CompleteLearnProfession(player,  SKILL_JEWELCRAFTING);
		break;

	case 9:
		if (player->GetSkillValue(SKILL_HERBALISM)<MYMAXSKILL)
			CompleteLearnProfession(player,  SKILL_HERBALISM);
		break;
	case 10:
		if (player->GetSkillValue(SKILL_SKINNING)<MYMAXSKILL)
			CompleteLearnProfession(player,  SKILL_SKINNING);
		break;
	case 11:
		if (player->GetSkillValue(SKILL_MINING)<MYMAXSKILL)
			CompleteLearnProfession(player,  SKILL_MINING);
		break;

	}

	player->CLOSE_GOSSIP_MENU();
	return true;
}