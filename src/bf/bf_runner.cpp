#include "bf_runner.h"

#include <iostream>
#include <fstream>

#include <stdio.h>
#include <conio.h>
#include <chrono>

void brainfuck::vm::power_cycle()
{
	pc = 0;
	instr = {};
	pointer = 0;

	std::memset( cell_memory, 0, sizeof( cell_memory ) );	
	bytecode.clear();

	m_file_name = "EMBED";
}

void brainfuck::vm::run_file( const std::string& _path )
{
	std::string source, line;
	std::ifstream file( _path );
	if ( !file.is_open() )
	{
		printf( "Failed to open file %s\n", _path.c_str() );
		return;
	}

	m_file_name = _path.substr( _path.find_last_of( "/\\" ) + 1 );

	while ( std::getline( file, line ) )
		source += line + "\n";

	run( source );

	power_cycle();
}

void brainfuck::vm::run( const std::string& _source )
{
	compiler bfc{ _source };
	compiler::error_code err = bfc.compile( m_file_name, bytecode );

	if( err != compiler::error_code::success )
	{
		printf( "%s\n", bfc.get_error_message().c_str() );
		return;
	}

	_incr_pc();
	auto start = std::chrono::system_clock::now();

	while ( instr.type != 0 )
	{
		interpret_instr();
		_incr_pc();
	}

	auto end = std::chrono::system_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>( end - start );
	double milliseconds = static_cast<double>( elapsed.count() ) / 1000.0;
	printf( "Executed in %.03fms\n", milliseconds );
}

void brainfuck::vm::interpret_instr()
{
	switch ( instr.type )
	{
	case opcode::Right: pointer += instr.count; break;
	case opcode::Left:  pointer -= instr.count; break;
	
	case opcode::Incr: _here() += instr.count; break;
	case opcode::Decr: _here() -= instr.count; break;
	
	case opcode::pOut: printf( "%c", (char)_here() ); break;
	case opcode::pIn:  _here() = _getinput();         break;

	case opcode::LoopBegin: if ( _here() == 0 ) _jmp( instr.partner ); break;
	case opcode::LoopEnd:   if ( _here() != 0 ) _jmp( instr.partner ); break;
	}
}

char brainfuck::vm::_getinput()
{
	char in = _getch();

	switch( in ) // special cases
	{
	case '\r': in = '\n'; break;
	case EOF:  in = '\0'; break;
	case 26:   in = '\0'; break;
	}

	return in;
}
