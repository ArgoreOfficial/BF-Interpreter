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
	
	ShowConsoleCursor( false );
	
	runner.run_file( "../scripts/hello_world.bf" );
	printf( "\n" );

	runner.run_file( "../scripts/size_test.bf" );
	printf( "\n" );
	
	runner.run_file( "../scripts/weird_test.bf" );
	printf( "\n" );
	
	runner.run_file( "../scripts/gol.bf" );
	printf( "\n" );

	ShowConsoleCursor( true );

	return 0;
}
