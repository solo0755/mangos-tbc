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
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, u8"������70��+��ʼ��װ+����������", GOSSIP_SENDER_MAIN, 122);
	if (pPlayer->getLevel() >= 60) {
		if (!addRep(pPlayer, false)) {
			pPlayer->ADD_GOSSIP_ITEM(3, u8"����������򸱱�����ֵ��� ", GOSSIP_SENDER_MAIN, 206);
		}
		else {
			pPlayer->ADD_GOSSIP_ITEM(3, u8"�л� [ռ����/�¶���] ����ֵ���", GOSSIP_SENDER_MAIN, 206);
		}
		if (!check(pPlayer, false)) {//�ݶ�60������ѧϰ
			const char* getmenu = all[pPlayer->getClass()].menuName.c_str();
			pPlayer->ADD_GOSSIP_ITEM(3, getmenu, GOSSIP_SENDER_MAIN, 201);//  ְҵ�˵�
		}
	}
	if (sPzxConfig.GetIntDefault("openT", 1)) {
		pPlayer->ADD_GOSSIP_ITEM(7, u8"������һ��|cff6247c8ְҵ��װ|h|r", GOSSIP_SENDER_MAIN, 107);
	}
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, u8"ѧϰ-|cff6247c8��ҵ����|h|r", GOSSIP_SENDER_MAIN, 301);


	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"����--> ɳ��˹�ǣ����ֽӴ���", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"����--> ������ �ŶӸ���", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
	if (pPlayer->GetTeam() == HORDE) {
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"����--> �¸�����", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"����--> �İ���", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"����--> ������", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"����--> ���³�", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9);
	}
	else {
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"����--> ��¯��", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"����--> �����", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"����--> ������˹", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, u8"����--> ������", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 10);
	}

	if (pPlayer->getClass() == CLASS_HUNTER) {
		pPlayer->ADD_GOSSIP_ITEM(3, u8"���� �ҵĳ���|cff6247c8�ҳ϶Ⱥ͵ȼ�|h|r ", GOSSIP_SENDER_MAIN, 205);
	}
	if (sPzxConfig.GetIntDefault("show.additem", 1)) {
		pPlayer->ADD_GOSSIP_ITEM_EXTENDED(6, u8"|cFF990066|TInterface\\ICONS\\Achievement_PVP_G_12.blp:20|t|r����ID|cff0070dd��ȡ��Ʒ|r,���޲�����Ʒ", GOSSIP_SENDER_MAIN, 777, u8"�ڵ�����������ƷID��� ����\n ��:|cFF00F0ff��Ҫ4���޵װ�|r��������:|cFFF0FF0014156 4|r", 0, true);
	}
	if (pPlayer->IsGameMaster()) {
		pPlayer->ADD_GOSSIP_ITEM(3, u8"���¼���ϵͳ����", GOSSIP_SENDER_MAIN, 778);
	}
	// pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
	pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _item->GetObjectGuid());
	return false;
}

bool GossipSelect_ItemPzx(Player *pPlayer, Item *_item, uint32 sender, const uint32 uiAction, char const* reStr)
{

	if (uiAction == 107) {
		if (sPzxConfig.GetIntDefault("openT3", 1)) {
			pPlayer->ADD_GOSSIP_ITEM(0, u8"������|cffe31bd2T4��װ|h|r", GOSSIP_SENDER_MAIN, 109);
		}
		if (sPzxConfig.GetIntDefault("openT4", 1)) {
			pPlayer->ADD_GOSSIP_ITEM(0, u8"������|cffe31bd2T5��װ|h|r", GOSSIP_SENDER_MAIN, 110);
		}
		if (sPzxConfig.GetIntDefault("openT5", 1)) {
			pPlayer->ADD_GOSSIP_ITEM(0, u8"������|cffe31bd2T6��װ|h|r", GOSSIP_SENDER_MAIN, 111);
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
		//pPlayer->learnSpell(33392, false);//�м�����
		pPlayer->learnSpell(34093, false);//ר�Ҽ�������

		pPlayer->SetUInt32Value(PLAYER_XP, 0);
		pPlayer->UpdateSkillsForLevel(true);
		if (sPzxConfig.GetIntDefault("initItemSet", 1) <= 2) {
			addItemSet(pPlayer, sPzxConfig.GetIntDefault("initItemSet", 0));//����T1��װ
		}
	}

	if (uiAction == 201) {
		check(pPlayer, true); //ѧϰְҵ����
	}

	if (uiAction == 205) {

		if (pPlayer->GetPet() && pPlayer->GetPet()->getPetType() == HUNTER_PET) {
			uint32 maxlevel = 70;
			Pet* HunterPet = pPlayer->GetPet();
			if (HunterPet->getLevel() < maxlevel || HunterPet->GetLoyaltyLevel() < LoyaltyLevel(BEST_FRIEND)) {

				//player->ADD_GOSSIP_ITEM(3, u8"���� �ҵĳ����ҳ϶Ⱥ͵ȼ� ", GOSSIP_SENDER_MAIN, 205);
				pPlayer->GetPet()->GivePetXP(99999999);
				pPlayer->GetPet()->ModifyLoyalty(1000000.0);
			}
			else {
				ChatHandler(pPlayer).PSendSysMessage(u8"[ϵͳ��Ϣ]:|cff0000ff ���ĳ����Ѿ�ǿ�����!|h|r");
			}
		}
		else {
			ChatHandler(pPlayer).PSendSysMessage(u8"[ϵͳ��Ϣ]:����|cffff0000 ѱ�������ٻ���|h|rһֻҪǿ���ĳ���");
		}
	}
	if (uiAction == 206) {
		addRep(pPlayer, true);
		FactionEntry const* factionEntry1 = sFactionStore.LookupEntry<FactionEntry>(932);//�¶���
		FactionEntry const* factionEntry2 = sFactionStore.LookupEntry<FactionEntry>(934);//ռ����
		if (pPlayer->GetReputationMgr().GetReputation(factionEntry1) < sPzxConfig.GetIntDefault("rep.init", 42001)) {
			pPlayer->GetReputationMgr().SetReputation(factionEntry1, sPzxConfig.GetIntDefault("rep.init", 42001));//����ֵ
																												  //pPlayer->GetReputationMgr().SetReputation(factionEntry2, 0);//����ֵ
		}
		else {
			//pPlayer->GetReputationMgr().SetReputation(factionEntry1, 0);//����ֵ
			pPlayer->GetReputationMgr().SetReputation(factionEntry2, sPzxConfig.GetIntDefault("rep.init", 42001));//����ֵ
		}

	}
	if (uiAction == 301) {
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"������", GOSSIP_SENDER_MAIN, 301 + 1);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"|cFF990066|TInterface\\ICONS\\Trade_BlackSmithing.blp:20|t|r ����", GOSSIP_SENDER_MAIN, 301 + 2);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"��Ƥ", GOSSIP_SENDER_MAIN, 301 + 3);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"�÷�", GOSSIP_SENDER_MAIN, 301 + 4);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"|cFF990066|TInterface\\ICONS\\Trade_Engineering.blp:20|t|r ����", GOSSIP_SENDER_MAIN, 301 + 5);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"|cFF990066|TInterface\\ICONS\\Trade_Engraving.blp:20|t|r ��ħ", GOSSIP_SENDER_MAIN, 301 + 6);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"|cFF990066|TInterface\\ICONS\\INV_Misc_Gem_02.blp:20|t|r �鱦", GOSSIP_SENDER_MAIN, 301 + 7);
		//pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"����",        GOSSIP_SENDER_MAIN, 301+8);773
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"��ҩ", GOSSIP_SENDER_MAIN, 301 + 9);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"��Ƥ", GOSSIP_SENDER_MAIN, 301 + 10);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"�ɿ�", GOSSIP_SENDER_MAIN, 301 + 11);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"����", GOSSIP_SENDER_MAIN, 301 + 12);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"����", GOSSIP_SENDER_MAIN, 301 + 13);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, u8"���", GOSSIP_SENDER_MAIN, 301 + 14);

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
		case 5://�İ�
			pPlayer->CLOSE_GOSSIP_MENU();
			pPlayer->TeleportTo(0, 1633.75f, 240.167f, -43.1034f, 6.26128f);
			break;
		case 7://������
			pPlayer->CLOSE_GOSSIP_MENU();
			pPlayer->TeleportTo(1, -1274.45f, 71.8601f, 128.159f, 2.80623f);
			break;

		case 9://���³�
			pPlayer->CLOSE_GOSSIP_MENU();
			pPlayer->TeleportTo(530, 9738.28f, -7454.19f, 13.5605f, 0.043914f);
			break;

		case 4://IRONforge
			pPlayer->CLOSE_GOSSIP_MENU();
			pPlayer->TeleportTo(0, -4917.0f, -955.0f, 502.0f, 0.0f);
			break;

		case 6://�����
			pPlayer->CLOSE_GOSSIP_MENU();
			pPlayer->TeleportTo(0, -8842.09f, 626.358f, 94.0867f, 3.61363f);
			break;
		case 8://������˹
			pPlayer->CLOSE_GOSSIP_MENU();
			pPlayer->TeleportTo(1, 9869.91f, 2493.58f, 1315.88f, 2.78897f);
			break;

		case 10://������
			pPlayer->CLOSE_GOSSIP_MENU();
			pPlayer->TeleportTo(530, -3864.92f, -11643.7f, -137.644f, 5.50862f);
			break;


		default:
			break;
		}
		return true;
	}
	else if (uiAction - 301 <= 14 && uiAction - 301 >= 1) {
		return GossipSelect_ProfessionNPC(pPlayer, sender, uiAction - 301);
	}

	if (uiAction == 777) {
		/*	if (!player->HasItemCount(sPzxConfig.GetIntDefault("vipItemID", 40003), 1, true)) {
		ChatHandler(pPlayer).PSendSysMessage( u8"[ϵͳ��Ϣ]:��ҪVIP��֤���ſ���ʹ�ñ����ܣ�����ϵGM��ȡ");
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
				ChatHandler(pPlayer).PSendSysMessage(u8"[ϵͳ��Ϣ]:��������ȷ����ƷID");
				pPlayer->CLOSE_GOSSIP_MENU();

				return false;
			}
		}
		catch (...) {
			ChatHandler(pPlayer).PSendSysMessage(u8"[ϵͳ��Ϣ]:��������ȷ����ƷID");
			pPlayer->CLOSE_GOSSIP_MENU();
			return false;
		}


		ItemPrototype const *pProto = sItemStorage.LookupEntry<ItemPrototype>(ssitem[0]);
		if (pProto) {

			if (pProto->Quality < sPzxConfig.GetIntDefault("item.quality", ITEM_QUALITY_LEGENDARY) && pProto->ItemLevel < sPzxConfig.GetIntDefault("item.level", 155))
			{
				if (pPlayer->HasItemCount(pProto->ItemId, 1, true)) {//�Ѿ���һ����
					ChatHandler(pPlayer).PSendSysMessage(u8"[ϵͳ��Ϣ]:����ƷΨһ");
				}
				ItemPosCountVec dest;
				InventoryResult msg = pPlayer->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, pProto->ItemId, ssitem[1]);
				if (msg == EQUIP_ERR_OK)
				{
					Item* item = pPlayer->StoreNewItem(dest, pProto->ItemId, true);
					pPlayer->SendNewItem(item, ssitem[1], true, false);
					ChatHandler(pPlayer).PSendSysMessage(u8"[ϵͳ��Ϣ]:%s �Ѿ���ӵ������", item->GetProto()->Name1);
				}
				else
				{
					pPlayer->SendEquipError(msg, nullptr, nullptr, pProto->ItemId);
					ChatHandler(pPlayer).PSendSysMessage(u8"[ϵͳ��Ϣ]:�뱣�ְ������㹻�ռ�");
				}
			}
			else {
				ChatHandler(pPlayer).PSendSysMessage(u8"[ϵͳ��Ϣ]:��ȡ��Ʒ�ȼ�����,��С��[%d]������ϵ����Ա", sPzxConfig.GetIntDefault("item.level", 155));
			}
		}
		else
			ChatHandler(pPlayer).PSendSysMessage(u8"[ϵͳ��Ϣ]:��Ʒδ�ҵ�");

		pPlayer->CLOSE_GOSSIP_MENU();
		return false;

	}
	if (uiAction == 778) {
		sPzxConfig.Reload();
		ChatHandler(pPlayer).PSendSysMessage(u8"[ϵͳ��Ϣ]:config�ļ����سɹ�");
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
