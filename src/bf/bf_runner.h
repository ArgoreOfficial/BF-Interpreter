#pragma once

#include <stdio.h>
#include <string>
#include <conio.h>
#include <vector>

enum BFCmd
{
	NullTerm = 0x0,
	Right = 0x1,
	Left = 0x2,
	Incr = 0x3,
	Decr = 0x4,
	pOut = 0x5,
	pIn = 0x6,
	LoopBegin = 0x7,
	LoopEnd = 0x8
};

struct BFBytecode
{
	BFCmd type;
	uint32_t count = 0;
};

struct BrainFuckRunner
{
	std::string src = "";
	std::string out_buffer = "";
	std::vector<BFBytecode> bytecode;

	uint16_t pc = 0;
	BFBytecode cmd = {};

	int32_t pointer = 0;
	uint8_t cell_memory[ UINT16_MAX ];

	uint16_t loop_stack[ 256 ] = { 0 };
	uint8_t loop_stack_counter = 0;

	void power_cycle();
	void preprocessor( const std::string& _source );
	void parse_bytecode( std::string _source );
	
	void push_stack( uint8_t _v ) {
		loop_stack[ loop_stack_counter++ ] = _v;
	}

	uint8_t pop_stack() {
		return loop_stack[ --loop_stack_counter ];
	}

	void incr_pc() {
		cmd = bytecode[ pc++ ];
	}

	void jmp( uint16_t _i ) {
		pc = _i;
		cmd = bytecode[ _i ];
	}

	uint8_t& here() { 
		return cell_memory[ pointer ]; 
	}

	void run( std::string _source );
	void interpret_cmd();

};