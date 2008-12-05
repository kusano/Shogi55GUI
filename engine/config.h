
#pragma once


#define		MATE						100000000	//	詰みの評価値

#define		USE_NULLMOVE				1		//	NullMove枝刈りの使用
#define		NOT_GENERATE_MEANINGLESS	1		//	角・飛の成らずを生成しない


//	通常
#define		INITIALIZE_RANDOM			1			//	乱数の初期化
#define		POSITION_TABLE_SIZE			0x400000	//	局面表のサイズ（通常時）
#define		QPOSITION_TABLE_SIZE		0x400000	//	静止探索用局面表のサイズ
#define		SEQUENCE_IN_POSITION_TABLE	0			//	局面表に末端までの手順を記録（学習時必須）
#define		UPDATE_SEARCH_TREE			1			//	探索木の更新

/*
//	学習用
#define		INITIALIZE_RANDOM			0			//	乱数の初期化
#define		POSITION_TABLE_SIZE			0x100		//	局面表のサイズ（通常時）
#define		QPOSITION_TABLE_SIZE		0x100000	//	静止探索用局面表のサイズ
#define		SEQUENCE_IN_POSITION_TABLE	1			//	局面表に末端までの手順を記録（学習時必須）
*/