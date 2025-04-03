#pragma once

#include <string>
#include <vector>

#include "compiler.h"

namespace brainfuck {

struct vm
{
	void power_cycle();

	void run_file( const std::string& _path );
	void run( const std::string& _source );
	void interpret_instr();

protected:
	void _incr_pc() { 
		instr = bytecode[ pc++ ]; 
	}
	
	void _jmp( uint16_t _i ) {
		pc = _i;
		instr = bytecode[ _i ];
	}

	uint8_t& _here() { return cell_memory[ pointer ]; }
	char _getinput();

	std::string m_file_name = "EMBED";
	std::vector<instruction> bytecode;

	uint16_t pc = 0;
	instruction instr = {};

	int32_t pointer = 0;
	uint8_t cell_memory[ UINT16_MAX ];
};

}
