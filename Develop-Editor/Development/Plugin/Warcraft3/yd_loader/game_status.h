#pragma once

#include <Windows.h>

namespace auto_enter { namespace game_status {

	enum WARCRAFT3_STATUS
	{
		WARCRAFT3_NOWINDOW_STATUS = -1,     // �ͻ��˻�û�д�
		WARCRAFT3_NONE_STATUS     = 0,		// ��û�н���ˢ����״̬��WAR3�����Ѿ������ˣ�
		WARCRAFT3_WAIT0_STATUS    = 1,		// �Ѿ�����ˢ����״̬,��û��ˢ������
		WARCRAFT3_WAIT1_STATUS    = 2,		// �Ѿ�ˢ������,��û�м���
		WARCRAFT3_JOIN_STATUS     = 3,		// �Ѿ���������״̬
		WARCRAFT3_CREATE_STATUS   = 4,		// �Ѿ���������
		WARCRAFT3_JOIN1_STATUS    = 5,		// �û��Ѿ�������Ϸ
		WARCRAFT3_GAMEING_STATUS  = 6,		// �Ѿ�������Ϸ״̬
		WARCRAFT3_GAME_CLOSED     = 7,		// ��Ϸ�Ѿ�������
		WARCRAFT3_GAME_OVER       = 8,
	};

	extern bool             open_map_flag;
	extern bool             enable_auto_enter;
	extern WARCRAFT3_STATUS status;

	void initialize(HMODULE gamedll);
}}
