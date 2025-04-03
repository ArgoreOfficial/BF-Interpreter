#pragma once

#include <vector>
#include <stdint.h>
#include <string>
#include <stack>

namespace brainfuck {

enum opcode : uint8_t
{
	NullTerm  = 0x0,
	Right     = '>',
	Left      = '<',
	Incr      = '+',
	Decr      = '-',
	pOut      = '.',
	pIn       = ',',
	LoopBegin = '[',
	LoopEnd   = ']'
};

struct instruction
{
	opcode type;
	uint32_t count = 0;
	uint32_t partner = 0;
};

struct loop_range
{
	size_t begin;
};

struct compiler
{
	enum class error_code
	{
		success,
		no_source_input,
		missing_loop_begin,
		missing_loop_end
	};

	compiler( const std::string& _source ) :
		m_source{ _source }
	{ }

	error_code compile( std::vector<instruction>& _out_vec );

private:

	void _flush();
	char _here() { return m_source[ m_pointer ]; }
	void _incr() { m_pointer++; }

	size_t _count_sequence( char _char );

	std::string m_source;
	std::vector<instruction> m_compiled;
	std::stack<loop_range> loop_stack;
	size_t m_pointer = 0;
};

}
