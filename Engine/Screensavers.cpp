#include "Library.h"

void initialScreensaver()
{
	int key = 0;
	printf("\x1b[0m\x1b[0;0H   \x1b[90m ��� ���������� ����,\x1b[0m ��������� ����\x1b[90m �� ������� ������� . . .\n\n\x1b[91m ~ \x1b[90m ������ ���� <\x1b[0m Enter \x1b[90m>\n\x1b[91m ~ \x1b[90m ����� �� ���� <\x1b[0m Escape \x1b[90m>");
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
	this_thread::sleep_for(chrono::milliseconds(200));
	for (int i = 0; i < 17; i++)
	{
		printf("\x1b[%d;%dH", startY + i, startX);
		if (i < 5){this_thread::sleep_for(chrono::milliseconds(70));}
		else if(i == 5){this_thread::sleep_for(chrono::milliseconds(600));}
		else{this_thread::sleep_for(chrono::milliseconds(50));}
		cout << "\t" << nameGame[i] << endl;
	}
	this_thread::sleep_for(chrono::milliseconds(2000));
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