//#include "Library.h"
//
//
//int main()
//{
//	// Определяем команду MCI для воспроизведения аудиофайла
//	//LPCWSTR command = L"open sounds/game.wav type waveaudio alias gameSound";
//	//mciSendString(command, NULL, 0, 0);
//
//	// Воспроизводим звук
//	mciSendString(L"play sounds/game.wav", NULL, 0, 0);
//
//	// Ждем, пока звук не завершится
//	Sleep(1000); // Например, 5 секунд
//
//	// Закрываем устройство воспроизведения
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