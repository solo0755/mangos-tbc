#include "AI/ScriptDevAI/include/sc_creature.h"
#include "AI/ScriptDevAI/include/sc_gossip.h"
#include "AI/ScriptDevAI/include/sc_grid_searchers.h"
#include "AI/ScriptDevAI/include/sc_instance.h"
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
	{ 0, u8"δ����",{},{} ,{} },
	{ CLASS_WARRIOR,u8"ѧϰ |cff6247c8����̬��������̬|h|r",{ 2458, 71, 355, 7386, 20617 },{},{ u8"����",u8"��ŭ",u8"����" } },
	{ CLASS_PALADIN,u8"ѧϰ |cff6247c8����|h|r ְҵ����",{ 7328 },{} ,{ u8"��ʥ",u8"�ͽ�",u8"����" } },
	{ CLASS_HUNTER,u8"ѧϰ |cff6247c8ѱ��Ұ��|h|r ְҵ����",{ 1515, 5149, 883, 2641, 6991, 982 },{} ,{ u8"Ұ�޿���",u8"���",u8"����" } },
	{ CLASS_ROGUE,u8"ѧϰ |cff6247c8�ƶ�|h|r ְҵ����",{ 2842 },{} ,{ u8"��ɱ",u8"ս��",u8"����" } },
	{ CLASS_PRIEST,u8"δ����",{},{} ,{ u8"����",u8"��ʥ",u8"��Ӱ" } },
	{ 0, u8"δ����",{},{} ,{ u8"",u8"",u8"" } },
	{ CLASS_SHAMAN,u8"��ȡ |cff6247c8���ˮ��ͼ��|h|r",{},{ 5175 ,5176,5177,5178 } ,{ u8"Ԫ��",u8"��ǿ",u8"�ָ�" } },
	{ CLASS_MAGE,u8"δ����",{},{} ,{ u8"����",u8"����",u8"��˪" } },
	{ CLASS_WARLOCK,u8"ѧϰ |cff6247c8�ٻ�С��|h|r ְҵ����",{ 688, 712,691 },{} ,{ u8"ʹ��",u8"��ħѧʶ",u8"����" } },
	{ 0, u8"δ����",{},{} ,{ u8"",u8"",u8"" } },
	{ CLASS_DRUID,u8"ѧϰ |cff6247c8����̬|h|r ְҵ����",{ 5487,9634 },{} ,{ u8"ƽ��",u8"Ұ��ս��",u8"�ָ�" } }
};

bool check(Player *player, bool modify);
bool addRep(Player *player, bool modify);
void addOneItemToPlayer(uint32 itemid, Player* player);
void addItemSet(Player *player, uint8 itemindex);
bool GossipSelect_ProfessionNPC(Player* player, uint32 sender, const uint32 action);
