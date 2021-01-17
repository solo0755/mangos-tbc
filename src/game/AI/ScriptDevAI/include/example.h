#include "AI/ScriptDevAI/include/sc_common.h"
#include "Config/PzxConfig.h"
const uint32 factionID[2][53] =
{

	{ 947,942,1011,935,989 },//H
	{ 946,942,1011,935,989 }//B
};

//0.5~2.5,
const uint32 IDS[13][3] =
{
	{ 0,0, 0 },
	{ 654,656, 673 },//CLASS_WARRIOR
	{ 624, 628, 679 },//CLASS_PALADIN
	{ 651, 652, 669 },//CLASS_HUNTER
	{ 621,622, 668 },//CLASS_ROGUE
	{ 664,665, 675 },//CLASS_PRIEST
	{ 0,0, 0 },
	{ 631,636, 683 },//CLASS_SHAMAN
	{ 648, 649, 671 },//CLASS_MAGE
	{ 645, 646, 670 },//CLASS_WARLOCK
	{ 0,0, 0 },
	{ 638,641, 677 },//CLASS_DRUID
};
struct initClazz
{
	uint8 clazz;
	std::string menuName;
	uint32 checkSpells[6];
	uint32 checkItems[5];
	std::string talenNames[3];
};


const initClazz all[] = {
	{ 0, u8"未定义",{},{} ,{} },
	{ CLASS_WARRIOR,u8"学习 |cff6247c8狂暴姿态、防御姿态|h|r",{ 2458, 71, 355, 7386, 20617 },{},{ u8"武器",u8"狂怒",u8"防护" } },
	{ CLASS_PALADIN,u8"学习 |cff6247c8救赎|h|r 职业技能",{ 7328 },{} ,{ u8"神圣",u8"惩戒",u8"防护" } },
	{ CLASS_HUNTER,u8"学习 |cff6247c8驯服野兽|h|r 职业技能",{ 1515, 5149, 883, 2641, 6991, 982 },{} ,{ u8"野兽控制",u8"射击",u8"生存" } },
	{ CLASS_ROGUE,u8"学习 |cff6247c8制毒|h|r 职业技能",{ 2842 },{} ,{ u8"刺杀",u8"战斗",u8"敏锐" } },
	{ CLASS_PRIEST,u8"未定义",{},{} ,{ u8"戒律",u8"神圣",u8"暗影" } },
	{ 0, u8"未定义",{},{} ,{ u8"",u8"",u8"" } },
	{ CLASS_SHAMAN,u8"获取 |cff6247c8风火水地图腾|h|r",{},{ 5175 ,5176,5177,5178 } ,{ u8"元素",u8"增强",u8"恢复" } },
	{ CLASS_MAGE,u8"未定义",{},{} ,{ u8"奥术",u8"火焰",u8"冰霜" } },
	{ CLASS_WARLOCK,u8"学习 |cff6247c8召唤小鬼|h|r 职业技能",{ 688, 712,691 },{} ,{ u8"痛苦",u8"恶魔学识",u8"毁灭" } },
	{ 0, u8"未定义",{},{} ,{ u8"",u8"",u8"" } },
	{ CLASS_DRUID,u8"学习 |cff6247c8熊形态|h|r 职业技能",{ 5487,9634 },{} ,{ u8"平衡",u8"野性战斗",u8"恢复" } }
};

bool check(Player *player, bool modify);
bool addRep(Player *player, bool modify);
void addOneItemToPlayer(uint32 itemid, Player* player);
void addItemSet(Player *player, uint8 itemindex);
bool GossipSelect_ProfessionNPC(Player* player, uint32 sender, const uint32 action);
