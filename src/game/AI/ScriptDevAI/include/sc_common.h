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


//0.5~2.5,��ʿ������ 
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
#endif
