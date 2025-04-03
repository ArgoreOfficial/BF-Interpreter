
#include <bf/bf_runner.h>

#include <Windows.h>

brainfuck::vm bf_vm{};

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
	
	bf_vm.run_file( "../scripts/hello_world.bf" );
	bf_vm.run_file( "../scripts/size_test.bf" );
	bf_vm.run_file( "../scripts/weird_test.bf" );
	bf_vm.run( "++[-.]]" );

	ShowConsoleCursor( true );

	return 0;
}
