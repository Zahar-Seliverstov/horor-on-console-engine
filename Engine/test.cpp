//#include "Library.h"
//
//
//int main()
//{
//	// ���������� ������� MCI ��� ��������������� ����������
//	//LPCWSTR command = L"open sounds/game.wav type waveaudio alias gameSound";
//	//mciSendString(command, NULL, 0, 0);
//
//	// ������������� ����
//	mciSendString(L"play sounds/game.wav", NULL, 0, 0);
//
//	// ����, ���� ���� �� ����������
//	Sleep(1000); // ��������, 5 ������
//
//	// ��������� ���������� ���������������
//	mciSendString(L"close sounds/game.wav", NULL, 0, 0);
//	mciSendString(L"sounds/game.wav", NULL, 0, 0);
//	while (true)
//	{
//		if (GetAsyncKeyState('D') & 0x8000)
//		{
//			
//		}
//	}
//
//
//	return 0;
//}