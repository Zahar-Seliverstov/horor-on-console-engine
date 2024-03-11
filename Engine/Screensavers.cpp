#include "Library.h"

void initialScreensaver()
{
	int key = 0;
	printf("\x1b[0m\x1b[0;0H   \x1b[90m ƒÀﬂ  ŒÃ‘Œ–“ÕŒ… »√–€,\x1b[0m –¿—“≈Õ»“≈ Œ ÕŒ\x1b[90m ƒŒ Õ”∆ÕŒ√Œ –¿«Ã≈–¿ . . .\n\n\x1b[91m ~ \x1b[90m Õ¿◊¿“‹ »√–” <\x1b[0m Enter \x1b[90m>\n\x1b[91m ~ \x1b[90m ¬€’Œƒ »« »√–€ <\x1b[0m Escape \x1b[90m>");
	while (key != 13)
	{
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