#include "Library.h"

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
void DownloadScreensaver()
{
	string screenSaver = "";
	for (int i = 0; i < 10; i++)
	{
		this_thread::sleep_for(chrono::milliseconds(900));
		screenSaver += ">{ }<";
		system("cls");
		cout << screenSaver;
	}
}