#pragma once

#include <vector>
#include <stdint.h>
#include <string>

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

struct brainfuck_compiler
{
	brainfuck_compiler( const std::string& _source ) :
		m_source{ _source }
	{ }

	int compile( std::vector<BFBytecode>& _out_vec );

private:

	void _flush();
	char _here() { return m_source[ m_pointer ]; }
	void _incr() { m_pointer++; }

	std::string m_source;
	std::vector<BFBytecode> m_compiled;
	size_t m_pointer = 0;
};
