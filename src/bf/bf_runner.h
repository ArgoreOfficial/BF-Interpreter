#pragma once

#include <string>
#include <vector>

#include "compiler.h"

struct brainfuck_vm
{
	void power_cycle();

	void run_file( const std::string& _path );
	void run( const std::string& _source );
	void interpret_cmd();

protected:
	void _push_stack( uint8_t _v ) { 
		loop_stack[ loop_stack_counter++ ] = _v; 
	}
	
	uint8_t _pop_stack() { 
		return loop_stack[ --loop_stack_counter ]; 
	}
	
	void _incr_pc() { 
		cmd = bytecode[ pc++ ]; 
	}
	
	void _jmp( uint16_t _i ) {
		pc = _i;
		cmd = bytecode[ _i ];
	}

	uint8_t& _here() { return cell_memory[ pointer ]; }

	std::vector<BFBytecode> bytecode;

	uint16_t pc = 0;
	BFBytecode cmd = {};

	int32_t pointer = 0;
	uint8_t cell_memory[ UINT16_MAX ];

	uint16_t loop_stack[ 256 ] = { 0 };
	uint8_t loop_stack_counter = 0;
};