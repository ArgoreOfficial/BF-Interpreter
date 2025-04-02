#include <iostream>
#include <fstream>

#include <Windows.h>

#include <bf/bf_runner.h>

BrainFuckRunner runner{};

void ShowConsoleCursor( bool showFlag )
{
	HANDLE out = GetStdHandle( STD_OUTPUT_HANDLE );

	CONSOLE_CURSOR_INFO     cursorInfo;

	GetConsoleCursorInfo( out, &cursorInfo );
	cursorInfo.bVisible = showFlag; // set the cursor visibility
	SetConsoleCursorInfo( out, &cursorInfo );
}

int main()
{
	std::string source, line;
	std::ifstream file( "../hello_world.b" );
	while ( std::getline( file, line ) )
		source += line;
	
	ShowConsoleCursor( false );
	
	//runner.run( "++++[>++++++<-]>[>+++++>+++++++<<-]>>++++<[[>[[>>+<<-]<]>>>-]>-[>+>+<<-]>]++++ + [ > ++++++ + << ++ > -] > . << ." );
	runner.run( source );
	
	ShowConsoleCursor( true );

	return 0;
}
