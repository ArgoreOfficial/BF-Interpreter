
#include <bf/bf_runner.h>

#include <Windows.h>

brainfuck::vm bf_vm{};

void set_console_cursor_visible( bool showFlag )
{
	HANDLE out = GetStdHandle( STD_OUTPUT_HANDLE );

	CONSOLE_CURSOR_INFO     cursorInfo;

	GetConsoleCursorInfo( out, &cursorInfo );
	cursorInfo.bVisible = showFlag; // set the cursor visibility
	SetConsoleCursorInfo( out, &cursorInfo );
}

int main()
{
	set_console_cursor_visible( false );
	
	bf_vm.run_file( "../scripts/hello_world.bf" );
	bf_vm.run_file( "../scripts/size_test.bf" );
	bf_vm.run_file( "../scripts/weird_test.bf" );
	bf_vm.run_file( "../scripts/broken.bf" );
	bf_vm.run( "++[-.]++++.[" );

	set_console_cursor_visible( true );
	
	return 0;
}
