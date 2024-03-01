#include "Library.h"
#include "IEngine.h"

int main()
{
	setlocale(LC_ALL, "rus");
	srand(time(NULL));

	Engine engine;	// Создание обьекта класса Engin который является основным для отрисовки в консоль 
	engine.Run();	// Вызываем у этого обьекта метод Run что бы запустить вычисления 
	
	return 0;
}