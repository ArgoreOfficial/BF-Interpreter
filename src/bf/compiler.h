#pragma once

#include <vector>
#include <stdint.h>
#include <string>

enum BFCmd : uint8_t
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

struct BFBytecode
{
	BFCmd type;
	uint32_t count = 0;
};

struct brainfuck_compiler
{
	enum class error_code
	{
		success,
		no_source_input,
		missing_loop_begin,
		missing_loop_end
	};

	brainfuck_compiler( const std::string& _source ) :
		m_source{ _source }
	{ }

	int compile( std::vector<BFBytecode>& _out_vec );

private:

	void _flush();
	char _here() { return m_source[ m_pointer ]; }
	void _incr() { m_pointer++; }

	size_t _count_sequence( char _char );

	std::string m_source;
	std::vector<BFBytecode> m_compiled;
	size_t m_pointer = 0;
};
