#include "Library.h"
#include "IEngine.h"
#include "IScreensavers.h"

int main()
{
	setlocale(LC_ALL, "rus");
	srand(time(NULL));

	Engine engine;	// �������� ������� ������ Engin ������� �������� �������� ��� ��������� � ������� 
	engine.Run();	// �������� � ����� ������� ����� Run ��� �� ��������� ���������� 
	/*for (size_t i = 0; i < 256; i++)
	{
		cout << i << " : " << char(i) << endl; 
	}*/
	return 0;
}