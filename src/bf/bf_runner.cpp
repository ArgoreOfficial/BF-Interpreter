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

	loop_stack_counter = 0;
	pointer = 0;

	std::memset( cell_memory, 0, sizeof( cell_memory ) );
	std::memset( loop_stack, 0, sizeof( loop_stack ) );

	bytecode.clear();
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

	while ( std::getline( file, line ) )
		source += line;

	run( source );
}

void brainfuck::vm::run( const std::string& _source )
{
	power_cycle();
	
	compiler bfc{ _source };
	compiler::error_code err = bfc.compile( bytecode );
	switch( err )
	{
		case compiler::error_code::missing_loop_begin: 
			printf( "Compile Error: Missing [ to match ] at X:Y\n" );
			return;

		case compiler::error_code::missing_loop_end: 
			printf( "Compile Error: Missing ] to match [ at X:Y\n" );
			return;
		
		case compiler::error_code::no_source_input: 
			printf( "No source input\n" );
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
	case opcode::Incr:  _here() += instr.count; break;
	case opcode::Decr:  _here() -= instr.count; break;

	case opcode::pOut: printf( "%c", (char)_here() ); break;

	case opcode::pIn: 
	{
		char in = _getch();

		switch ( in ) // special cases
		{
		case '\r': in = '\n'; break;
		case EOF:  in = '\0'; break;
		case 26:   in = '\0'; break;
		}

		_here() = in;
	} break;

	case opcode::LoopBegin: if ( _here() == 0 ) _jmp( instr.partner ); break;
	case opcode::LoopEnd:   if ( _here() != 0 ) _jmp( instr.partner ); break;

	}
}
