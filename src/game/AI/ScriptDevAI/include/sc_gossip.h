/* This file is part of the ScriptDev2 Project. See AUTHORS file for Copyright information
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef SC_GOSSIP_H
#define SC_GOSSIP_H

#include "Entities/Player.h"
#include "Entities/GossipDef.h"
#include "Quests/QuestDef.h"

// Gossip Item Text
#define GOSSIP_TEXT_BROWSE_GOODS        "我想浏览一下你们的商品."
#define GOSSIP_TEXT_TRAIN               "训练我!"

#define GOSSIP_TEXT_BANK                "银行"
#define GOSSIP_TEXT_IRONFORGE_BANK      "铁炉堡银行"
#define GOSSIP_TEXT_STORMWIND_BANK      "暴风城银行"
#define GOSSIP_TEXT_WINDRIDER           "风骑士管理员"
#define GOSSIP_TEXT_GRYPHON             "狮鹫管理员"
#define GOSSIP_TEXT_BATHANDLER          "蝙蝠管理员"
#define GOSSIP_TEXT_HIPPOGRYPH          "角鹰兽管理员"
#define GOSSIP_TEXT_ZEPPLINMASTER       "飞艇管理员"
#define GOSSIP_TEXT_DEEPRUNTRAM         "矿道地铁"
#define GOSSIP_TEXT_FERRY               "鲁特拉渡口"
#define GOSSIP_TEXT_FLIGHTMASTER        "飞行管理员"
#define GOSSIP_TEXT_AUCTIONHOUSE        "拍卖行"
#define GOSSIP_TEXT_GUILDMASTER         "公会管理员"
#define GOSSIP_TEXT_INN                 "旅店"
#define GOSSIP_TEXT_MAILBOX             "邮箱"
#define GOSSIP_TEXT_STABLEMASTER        "兽栏"
#define GOSSIP_TEXT_WEAPONMASTER        "武器训练师"
#define GOSSIP_TEXT_OFFICERS            "休闲室"
#define GOSSIP_TEXT_BATTLEMASTER        "将领"
#define GOSSIP_TEXT_BARBER              "理发师"
#define GOSSIP_TEXT_CLASSTRAINER        "等级训练师"
#define GOSSIP_TEXT_PROFTRAINER         "专业训练师"
#define GOSSIP_TEXT_LEXICON             "力量宝典"

#define GOSSIP_TEXT_ALTERACVALLEY       "奥特呵科山谷"
#define GOSSIP_TEXT_ARATHIBASIN         "阿瑞斯盆地"
#define GOSSIP_TEXT_WARSONGULCH         "战歌峡谷"
#define GOSSIP_TEXT_ARENA               "竞技场"
#define GOSSIP_TEXT_EYEOFTHESTORM       "风暴之眼"
#define GOSSIP_TEXT_STRANDOFANCIENT     "远古海滩"

#define GOSSIP_TEXT_DEATH_KNIGHT        "死亡骑士"
#define GOSSIP_TEXT_DRUID               "德鲁伊"
#define GOSSIP_TEXT_HUNTER              "猎人"
#define GOSSIP_TEXT_PRIEST              "牧师"
#define GOSSIP_TEXT_ROGUE               "盗贼"
#define GOSSIP_TEXT_WARRIOR             "战士"
#define GOSSIP_TEXT_PALADIN             "骑士"
#define GOSSIP_TEXT_SHAMAN              "萨满"
#define GOSSIP_TEXT_MAGE                "法师"
#define GOSSIP_TEXT_WARLOCK             "术士"

#define GOSSIP_TEXT_ALCHEMY             "炼金"
#define GOSSIP_TEXT_BLACKSMITHING       "锻造"
#define GOSSIP_TEXT_COOKING             "烹饪"
#define GOSSIP_TEXT_ENCHANTING          "附魔"
#define GOSSIP_TEXT_ENGINEERING         "工程"
#define GOSSIP_TEXT_FIRSTAID            "急救"
#define GOSSIP_TEXT_HERBALISM           "药剂"
#define GOSSIP_TEXT_LEATHERWORKING      "制皮"
#define GOSSIP_TEXT_TAILORING           "裁缝"
#define GOSSIP_TEXT_MINING              "采矿"
#define GOSSIP_TEXT_FISHING             "钓鱼"
#define GOSSIP_TEXT_SKINNING            "剥皮"
#define GOSSIP_TEXT_JEWELCRAFTING       "珠宝"
#define GOSSIP_TEXT_INSCRIPTION         "铭文"

enum
{
    // Skill defines
    TRADESKILL_ALCHEMY             = 1,
    TRADESKILL_BLACKSMITHING       = 2,
    TRADESKILL_COOKING             = 3,
    TRADESKILL_ENCHANTING          = 4,
    TRADESKILL_ENGINEERING         = 5,
    TRADESKILL_FIRSTAID            = 6,
    TRADESKILL_HERBALISM           = 7,
    TRADESKILL_LEATHERWORKING      = 8,
    TRADESKILL_POISONS             = 9,
    TRADESKILL_TAILORING           = 10,
    TRADESKILL_MINING              = 11,
    TRADESKILL_FISHING             = 12,
    TRADESKILL_SKINNING            = 13,
    TRADESKILL_JEWLCRAFTING        = 14,
    TRADESKILL_INSCRIPTION         = 15,

    TRADESKILL_LEVEL_NONE          = 0,
    TRADESKILL_LEVEL_APPRENTICE    = 1,
    TRADESKILL_LEVEL_JOURNEYMAN    = 2,
    TRADESKILL_LEVEL_EXPERT        = 3,
    TRADESKILL_LEVEL_ARTISAN       = 4,
    TRADESKILL_LEVEL_MASTER        = 5,
    TRADESKILL_LEVEL_GRAND_MASTER  = 6,

    // Gossip defines
    GOSSIP_ACTION_TRADE            = 1,
    GOSSIP_ACTION_TRAIN            = 2,
    GOSSIP_ACTION_TAXI             = 3,
    GOSSIP_ACTION_GUILD            = 4,
    GOSSIP_ACTION_BATTLE           = 5,
    GOSSIP_ACTION_BANK             = 6,
    GOSSIP_ACTION_INN              = 7,
    GOSSIP_ACTION_HEAL             = 8,
    GOSSIP_ACTION_TABARD           = 9,
    GOSSIP_ACTION_AUCTION          = 10,
    GOSSIP_ACTION_INN_INFO         = 11,
    GOSSIP_ACTION_UNLEARN          = 12,
    GOSSIP_ACTION_INFO_DEF         = 1000,

    GOSSIP_SENDER_MAIN             = 1,
    GOSSIP_SENDER_INN_INFO         = 2,
    GOSSIP_SENDER_INFO             = 3,
    GOSSIP_SENDER_SEC_PROFTRAIN    = 4,
    GOSSIP_SENDER_SEC_CLASSTRAIN   = 5,
    GOSSIP_SENDER_SEC_BATTLEINFO   = 6,
    GOSSIP_SENDER_SEC_BANK         = 7,
    GOSSIP_SENDER_SEC_INN          = 8,
    GOSSIP_SENDER_SEC_MAILBOX      = 9,
    GOSSIP_SENDER_SEC_STABLEMASTER = 10
};

// Defined fuctions to use with player.

// This fuction add's a menu item,
// Icon Id
// Text
// Sender(this is to identify the current Menu with this item)
// Option id (identifies this Menu Item)
// Text to be displayed in pop up box
// Money value in pop up box
// Coded
#define ADD_GOSSIP_ITEM(icon, text, sender, optionId)   PlayerTalkClass->GetGossipMenu().AddMenuItem(icon, text, sender, optionId, "", 0)
#define ADD_GOSSIP_ITEM_ID(icon, textId, sender, optionId)   PlayerTalkClass->GetGossipMenu().AddMenuItem(icon, textId, sender, optionId, 0, 0)
#define ADD_GOSSIP_ITEM_EXTENDED(icon, text, sender, optionId, boxMessage, boxMoney, code)   PlayerTalkClass->GetGossipMenu().AddMenuItem(icon, text, sender, optionId, boxMessage, boxMoney, code)

// Sets proper 2.4+ gossip menu id
#define SET_GOSSIP_MENU_ID(id)      PlayerTalkClass->GetGossipMenu().SetMenuId(id)

// This fuction Sends the current menu to show to client
// uiTextId - NPCTEXTID (uint32)
// guid - npc guid (ObjectGuid)
#define SEND_GOSSIP_MENU(textId, guid)      PlayerTalkClass->SendGossipMenu(textId, guid)

// Closes the Menu
#define CLOSE_GOSSIP_MENU()        PlayerTalkClass->CloseGossip()

// Fuctions to send NPC lists
// guid - is always the npc guid (ObjectGuid)
#define SEND_VENDORLIST(guid)         GetSession()->SendListInventory(guid)
#define SEND_TRAINERLIST(guid)        GetSession()->SendTrainerList(guid)
#define SEND_BANKERLIST(guid)         GetSession()->SendShowBank(guid)
#define SEND_TABARDLIST(guid)         GetSession()->SendTabardVendorActivate(guid)
#define SEND_TAXILIST(guid)           GetSession()->SendTaxiStatus(guid)

#endif
