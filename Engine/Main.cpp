#include "Library.h"
#include "IEngine.h"
#include "IScreensavers.h"

int main()
{
	setlocale(LC_ALL, "rus");
	srand(time(NULL));

	Engine engine;	// Создание обьекта класса Engin который является основным для отрисовки в консоль 
	engine.Run();	// Вызываем у этого обьекта метод Run что бы запустить вычисления 
	/*for (size_t i = 0; i < 256; i++)
	{
		cout << i << " : " << char(i) << endl; 
	}*/
	return 0;
}