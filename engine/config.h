
#pragma once


#define		MATE						100000000	//	�l�݂̕]���l

#define		USE_NULLMOVE				1		//	NullMove�}����̎g�p
#define		NOT_GENERATE_MEANINGLESS	1		//	�p�E��̐��炸�𐶐����Ȃ�


//	�ʏ�
#define		INITIALIZE_RANDOM			1			//	�����̏�����
#define		POSITION_TABLE_SIZE			0x400000	//	�ǖʕ\�̃T�C�Y�i�ʏ펞�j
#define		QPOSITION_TABLE_SIZE		0x400000	//	�Î~�T���p�ǖʕ\�̃T�C�Y
#define		SEQUENCE_IN_POSITION_TABLE	0			//	�ǖʕ\�ɖ��[�܂ł̎菇���L�^�i�w�K���K�{�j
#define		UPDATE_SEARCH_TREE			1			//	�T���؂̍X�V

/*
//	�w�K�p
#define		INITIALIZE_RANDOM			0			//	�����̏�����
#define		POSITION_TABLE_SIZE			0x100		//	�ǖʕ\�̃T�C�Y�i�ʏ펞�j
#define		QPOSITION_TABLE_SIZE		0x100000	//	�Î~�T���p�ǖʕ\�̃T�C�Y
#define		SEQUENCE_IN_POSITION_TABLE	1			//	�ǖʕ\�ɖ��[�܂ł̎菇���L�^�i�w�K���K�{�j
*/