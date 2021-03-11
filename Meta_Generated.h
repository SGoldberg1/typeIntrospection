#ifndef _ENGINE_META_GENERATED_H
#define _ENGINE_META_GENERATED_H

//This program was computer generated
//See MetaGen.cpp for details
//Generated: Wed Jan 27 00:58:01 2021


enum meta_type
{
	MetaType_v2,
	MetaType_char,
	MetaType_type_flag,
	MetaType_int,
	MetaType_float,
};

struct meta_struct
{
	int IsConstant;
	int IsPointer;
	int Offset;
	meta_type Type;
	const char* MemberName;
};

meta_struct MetaMembersOf_test_struct[] = 
{
	{ 0, 0, (unsigned int)&((test_struct*)0)->Position, MetaType_v2, "Position" },
	{ 0, 0, (unsigned int)&((test_struct*)0)->Character, MetaType_char, "Character" },
	{ 0, 0, (unsigned int)&((test_struct*)0)->Type, MetaType_type_flag, "Type" },
	{ 0, 0, (unsigned int)&((test_struct*)0)->Flag, MetaType_int, "Flag" },
};

meta_struct MetaMembersOf_v2[] = 
{
	{ 0, 0, (unsigned int)&((v2*)0)->Y, MetaType_float, "Y" },
	{ 0, 0, (unsigned int)&((v2*)0)->X, MetaType_float, "X" },
};

#endif //_ENGINE_META_GENERATED_H
