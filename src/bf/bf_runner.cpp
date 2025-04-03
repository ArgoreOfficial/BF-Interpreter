#include "bf_runner.h"

#include <iostream>
#include <fstream>

#include <stdio.h>
#include <conio.h>

void brainfuck_vm::power_cycle()
{
	pc = 0;
	cmd = {};

	loop_stack_counter = 0;
	pointer = 0;

	std::memset( cell_memory, 0, sizeof( cell_memory ) );
	std::memset( loop_stack, 0, sizeof( loop_stack ) );

	bytecode.clear();
}

void brainfuck_vm::run_file( const std::string& _path )
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

void brainfuck_vm::run( const std::string& _source )
{
	power_cycle();
	
	brainfuck_compiler bfc{ _source };
	bfc.compile( bytecode );

	_incr_pc();

	while ( cmd.type != 0 )
	{
		interpret_cmd();
		_incr_pc();
	}
}

void brainfuck_vm::interpret_cmd()
{
	switch ( cmd.type )
	{
	case BFCmd::Right: { pointer += cmd.count; } break;
	case BFCmd::Left:  { pointer -= cmd.count; } break;
	case BFCmd::Incr:  { _here() += cmd.count; } break;
	case BFCmd::Decr:  { _here() -= cmd.count; } break;

	case BFCmd::pOut: { 
		printf( "%c", (char)_here() ); 
	} break;
	case BFCmd::pIn: 
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

	case BFCmd::LoopBegin:
	{
		if ( _here() == 0 )
		{
			uint32_t loop_counter = 0;
			do
			{
				if ( cmd.type == BFCmd::LoopBegin ) loop_counter++;
				else if ( cmd.type == BFCmd::LoopEnd ) loop_counter--;

				if ( loop_counter > 0 ) _incr_pc();
			} while ( loop_counter > 0 );
		}
		else
			_push_stack( pc - 1 );

	} break;

	case BFCmd::LoopEnd:
	{
		uint16_t jmp_point = _pop_stack();
		if ( _here() != 0 )
			_jmp( jmp_point );

	} break;

	}
}
