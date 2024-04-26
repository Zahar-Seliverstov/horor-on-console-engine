#include "Library.h"
#include <fstream>
#pragma comment(lib, "winmm.lib")

void initialScreensaver()
{
	int key = 0;
	printf("\n\x1b[90m=========================| \x1b[0mSTART MENU\x1b[90m |=========================\x1b[0m");
	printf("\n\n\x1b[90m # \x1b[94mFOR A COMFORTABLE GAME, EXPAND THE WINDOW TO THE DESIRED SIZE\x1b[0m");
	printf("\n\x1b[90m  \x1b[94m NEXT, THE SCREEN SIZE CAN BE CHANGED IN TINCTURES. . .\x1b[0m");
	printf("\n\n\x1b[90m   ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~\x1b[0m");
	printf("\n\n\x1b[93m  |\x1b[0m POSSIBLE ACTIONS\x1b[0m");
	printf("\n\x1b[90m  __________________\x1b[0m");
	printf("\n\n\x1b[91m  ~ \x1b[90mSTART THE GAME < \x1b[93mENTER\x1b[90m >");
	printf("\n\x1b[91m  ~ \x1b[90mEXITING THE GAME < \x1b[93mESCAPE\x1b[90m >");
	printf("\n\n\x1b[90m________________________________________________________________\n\x1b[0m");
	printf("\n\n\x1b[93m  |\x1b[0m CONTROL OF THE GAME");
	printf("\n\x1b[90m  _____________________\x1b[0m");
	printf("\n\n\x1b[90m   # \x1b[94mWALKING");
	printf("\n\n\t\x1b[90mForward - \x1b[0mKEY \x1b[90m<\x1b[93m W \x1b[90m>");
	printf("\n\n\t\x1b[90mBackwards - \x1b[0mKEY \x1b[90m<\x1b[93m S \x1b[90m>");
	printf("\n\n\t\x1b[90mLeft - \x1b[0mKEY \x1b[90m<\x1b[93m A \x1b[90m>");
	printf("\n\n\t\x1b[90mRight - \x1b[0mKEY \x1b[90m<\x1b[93m D \x1b[90m>");
	printf("\n\n\t\x1b[90mRun - \x1b[0mKEY \x1b[90m<\x1b[93m SHIFT \x1b[90m>");
	printf("\n\n\x1b[90m   # \x1b[94mSETTINGS");
	printf("\n\n\t\x1b[90mOpen - \x1b[0mKEY \x1b[90m<\x1b[93m Esc \x1b[90m>");
	printf("\n\n\t\x1b[90mMenu management - \x1b[0mKEY \x1b[90m< \x1b[93mARROW UP\x1b[90m,\x1b[93m DOWN\x1b[90m,\x1b[93m LEFT\x1b[90m,\x1b[93m RIGTH\x1b[90m >");
	printf("\n\n\x1b[90m   # \x1b[94mMANAGEMENT INFORMATION DISPLAY");
	printf("\n\n\t\x1b[90mFPS, COORDINATES - \x1b[0mKEY \x1b[90m<\x1b[93m F \x1b[90m>");
	printf("\n\n\t\x1b[90mMAP - \x1b[0mKEY \x1b[90m<\x1b[93m M \x1b[90m>");
	printf("\n\n\x1b[90m===============================================================\n\n");

	while (key != 13) {
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
		"\x1b[90m            /   #   /##   #     /##   /#\\   #\\ /#   ###      ###   /#\\               ",
		"\x1b[90m            # * #   #=    #     #     [ ]   # * #   #=        #    [ ]              ",
		"\x1b[90m            \\/ \\/   ###   \\##   \\##   \\#/   #   #   ###       #    \\#/              ",
		"\x1b[90m                                                                                   ",
		"\x1b[31;48m                                                                                   ",
		"    ___ ___  ___    ____  ______    __         _    ____________      ________     ",
		"   /- /   -_ _ /|  / - / /  =_ /|  /_/|      / /|  / _ ==  _ --/|   _/  _ -= /|   ",
		"   #### ####### |  #### ###  ## |  ## |      ## |  ## ##### ### |  / ###  ### /|   ",
		"   \\#### ######/   ## #  ###### |  ## |      ## |  ###  ###__##/  ####_ ##_## |   ",
		"        ## |        # |       # |   #/       ## |  #/ |_________   ## |_  __ #/    ",
		"        ##/        ## |      ## |   #/|       # |  ##/_  -     /|  ##/ / _= ##/    ",
		"        ##/|       ## |      ##/|   # |      ## |   ## ####  ## |  ##  ###### |    ",
		"         # |       ## |      # /|  ## | __ _ # /   ####_/#####/    ###  ##_##/     ",
		"        ## |        # |______## |  ## |/_  /\\ #/|  ## |________     # |   ##\\      ",
		"        ## |       ##/  - _   # |  ##/ #_##  ## |  ##/_  -_= - /|   # |   \\/ \\     ",
		"    \x1b[90m =- \x1b[31;48m## |\x1b[90m--=-__- \x1b[31;48m## #####/## |\x1b[90m_- \x1b[31;48m###\x1b[90m--__\x1b[31;48m#### |\x1b[90m=-\x1b[31;48m##########/# |\x1b[90m--\x1b[31;48m## |\x1b[90m=--_\x1b[31;48m\\# \\\x1b[90m__--",
		"   \x1b[90m-__-= \x1b[31;48m#/ \x1b[90m__-==--\x1b[31;48m####  ###/##/\x1b[90m --\x1b[31;48m##\x1b[90m-_==-_--\x1b[31;48m##/ \x1b[90m=-\x1b[31;48m### #  #####/ \x1b[90m--\x1b[31;48m##/\x1b[90m-==-_-\x1b[31;48m\\#/ \x1b[90m--  ",
		"\x1b[90m ----_---__----=-=-=--_--___---------==_-----------__--=--==-____----=--_--=-----"
	};
	int startX = screenWidth / 2 - 45;
	int startY = screenHeight / 2 - 9;
	printf("\x1b[31;48m");
	this_thread::sleep_for(chrono::milliseconds(500));

	thread([]() {PlaySound(L"sounds/menu.wav", NULL, SND_ASYNC); }).detach();

	this_thread::sleep_for(chrono::milliseconds(200));
	for (int i = 0; i <= 17; i++)
	{
		printf("\x1b[%d;%dH", startY + i, startX);
		this_thread::sleep_for(chrono::milliseconds(10));
		cout << "\t" << nameGame[i] << endl;
	}
	this_thread::sleep_for(chrono::milliseconds(3100));
	system("cls");
}
void DownloadScreensaver(int& screenWidth, int& screenHeight)
{
	string path = "animationPortal.txt";
	ifstream file(path);

	vector<vector<string>> arr;
	if (file.is_open())
	{
		mciSendString(L"play sounds/teleport.wav", NULL, 0, NULL);
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
		throw std::exception("Error to opening file...");
}