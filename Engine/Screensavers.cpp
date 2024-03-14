#include "Library.h"
#include <fstream>

void initialScreensaver()
{
	int key = 0;
	printf("\n\x1b[0m_____________________\x1b[4m\x1b[90mСТАРТОВОЕ МЕНЮ\x1b[0m______________________\x1b[0m");
	printf("\n\n\x1b[90m # \x1b[94mДЛЯ КОМФОРТНОЙ ИГРЫ, РАСТЕНИТЕ ОКНО ДО НУЖНОГО РАЗМЕРА\x1b[0m");
	printf("\n\x1b[90m  \x1b[94m РАЗМЕР ЭКРАНА МОЖНО ПОМЕНЯТЬ В НАСТОЙКАХ. . .\x1b[0m");
	printf("\n\n\x1b[0m>\x1b[90m ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ \x1b[0m<\x1b[0m");
	printf("\n\n\x1b[93m  |\x1b[0m ВОЗМОЖНЫЕ ДЕЙСТВИЯ\x1b[0m");
	printf("\n\x1b[90m  ____________________\x1b[0m");
	printf("\n\n\x1b[91m  ~ \x1b[90mНАЧАТЬ ИГРУ < \x1b[93menter\x1b[90m >");
	printf("\n\x1b[91m  ~ \x1b[90mВЫХОД ИЗ ИГРЫ < \x1b[93mescape\x1b[90m >");
	printf("\n\x1b[0m_________________________________________________________\n\x1b[0m");
	while (key != 13){
		if (key == 27) { exit(0); }
		key = _getch();
	}
}
void PrintGameTitle(int& screenWidth, int& screenHeight)
{
	system("cls");
	string line = "";
	string nameGame[18] =
	{
		"            #   #   ###   #     ###   ###   #\\ /#   ###      ###   ###               ",
		"            # * #   #=    #     #     [ ]   # * #   #=        #    [ ]              ",
		"            #/ \\#   ###   ###   ###   ###   #   #   ###       #    ###              ",
		"                                                                                   ",
		"                                                                                   ",
		"    ____________    ____________    __        __    ____________    __________     ",
		"   /           /|  /           /|  / /|      / /|  /           /|  /         /|    ",
		"   ############ |  ############ |  ## |      ## |  ############ |  ########## /|   ",
		"   ############/   ############ |  ## |      ## |  ############/   ########### |   ",
		"        ## |       ## |      ## |  ## |      ## |  ## |_________   ##       ## |   ",
		"        ## |       ## |      ## |  ## |      ## |  ##/         /|  ##       ##/    ",
		"        ## |       ## |      ## |  ## |      ## |  ############ |  ########## |    ",
		"        ## |       ## |      ## |  ## | ____ ## |  ############/   ##########/     ",
		"        ## |       ## |______## |  ## |/   /\\## |  ## |________    ## |  ## \\      ",
		"        ## |       ##/       ## |  ##/ ####  ## |  ##/         /|  ## |   ## \\     ",
		"        ## |       ############ |  ####    #### |  ############ |  ## |    ## \\    ",
		"        ##/        ############/   ##        ##/   ############/   ##/      ##/    ",
		"                                                                                   "
	};
	int startX = screenWidth / 2 - 45;
	int startY = screenHeight / 2 - 9;
	printf("\x1b[31;48m");

	std::thread gg([]() {PlaySound(L"C:\\Languch\\c++\\Engin\\ConsoleEngine\\Engine\\zvuk-avtomobilnoy-avarii-tormojenie-zatem-udar-34123.mp3", NULL, SND_ASYNC); });
	gg.detach();

	this_thread::sleep_for(chrono::milliseconds(200));
	for (int i = 0; i < 17; i++)
	{
		printf("\x1b[%d;%dH", startY + i, startX);
		
		if(i == 5 && i > 4){this_thread::sleep_for(chrono::milliseconds(500));}
		else{this_thread::sleep_for(chrono::milliseconds(30));}
		cout << "\t" << nameGame[i] << endl;
	}
	this_thread::sleep_for(chrono::milliseconds(1000));
}
void DownloadScreensaver(int& screenWidth, int& screenHeight)
{
	string path = "animationPortal.txt";
	ifstream file(path);

	vector<vector<string>> arr;

	if (file.is_open())
	{
		string str;
		// Считываем весь файл в одну строку
		getline(file, str, '\0');
		file.close();

		// Разделяем строку на кадры
		size_t pos = 0;
		string delimiter1 = "\\f";
		while ((pos = str.find(delimiter1)) != string::npos)
		{
			string frame = str.substr(0, pos);
			str.erase(0, (pos + delimiter1.length() + 1));

			vector<string> lines;

			// Разделяем кадр на строки
			pos = 0;
			string delimiter2 = "\n";
			while ((pos = frame.find(delimiter2)) != string::npos)
			{
				string line = frame.substr(0, pos);
				frame.erase(0, (pos + delimiter2.length()));
				lines.push_back(line);
				line.erase();
			}

			arr.push_back(lines);
			lines.clear();
		}

		screenWidth = (screenWidth / 2) - (arr[0][0].length() / 2);
		screenHeight = (screenHeight / 2) - (arr[0].size() / 2);
		// Выводим кадры
		for (int i = 0; i < arr.size(); i++)
		{
			printf("\x1b[H"); // Перемещаем курсор в начало экрана
			for (int j = 0; j < arr[i].size(); j++)
			{
				printf("\x1b[%d;%dH", screenHeight + j, screenWidth);
				cout << arr[i][j] << endl;
			}
			Sleep(100);
		}
	}
	else
		throw std::exception("Error open file");
}