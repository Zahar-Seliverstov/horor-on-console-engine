#include "Library.h"
#include "IEngine.h"

int main()
{
	setlocale(LC_ALL, "rus");
	srand(time(NULL));

	Engine engine;	// �������� ������� ������ Engin ������� �������� �������� ��� ��������� � ������� 
	engine.Run();	// �������� � ����� ������� ����� Run ��� �� ��������� ���������� 
	
	return 0;
}