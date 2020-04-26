/* This file is part of the ScriptDev2 Project. See AUTHORS file for Copyright information
* This program is free software licensed under GPL version 2
* Please see the included DOCS/LICENSE.TXT for more information */

#ifndef SC_COMMON_H
#define SC_COMMON_H

#include "AI/ScriptDevAI/include/sc_creature.h"
#include "AI/ScriptDevAI/include/sc_gossip.h"
#include "AI/ScriptDevAI/include/sc_grid_searchers.h"
#include "AI/ScriptDevAI/include/sc_instance.h"
#include "Config/PzxConfig.h"


//0.5~2.5,勇士、督军 
const uint32 IDS[13][7] =
{
	{ 0,0, 0, 0, 0, 0 ,0 },
	{ 189,511, 209, 218,496,474 ,523 },//CLASS_WARRIOR
	{ 188, 516, 208, 217,505,475 ,528 },//CLASS_PALADIN
	{ 186, 515, 206, 215,509,477,530 },//CLASS_HUNTER
	{ 184,512, 204, 213,497,478,524 },//CLASS_ROGUE
	{ 182,514, 202, 211,507,480,525 },//CLASS_PRIEST
	{ 0,0, 0, 0, 0, 0 ,0 },
	{ 187,519, 207, 216,501,476,527 },//CLASS_SHAMAN
	{ 181, 517, 201, 210 ,503,482,526 },//CLASS_MAGE
	{ 183, 518, 203, 212 ,499,481,529 },//CLASS_WARLOCK
	{ 0,0, 0, 0 , 0, 0,0 },
	{ 185,513, 205, 214 ,493,479,521 },//CLASS_DRUID
};
struct initClazz
{
	uint8 clazz;
	char * menuName;
	uint32 checkSpells[6];
	uint32 checkItems[5];
	char * talenNames[3];
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
#endif
