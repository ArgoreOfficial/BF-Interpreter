#pragma once

#include <vector>
#include <stdint.h>
#include <string>
#include <stack>
#include <format>

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

struct alignas( 8 ) instruction
{
	opcode type;
	uint16_t count = 0;
	uint16_t partner = 0;
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

	compiler( const std::string& _source );

	error_code compile( const std::string& _file_name, std::vector<instruction>& _out_vec );

	std::string get_error_message();

private:

	void _flush();
	char _here() { return m_source_lines[ m_line ][ m_pointer ]; }
	void _incr() { m_pointer++; }

	size_t _count_sequence( char _char );

	std::vector<std::string> m_source_lines;
	std::vector<instruction> m_compiled;
	std::stack<loop_range> loop_stack;
	size_t m_pointer = 0;
	size_t m_line = 0;

	size_t m_error_point = 0;
	size_t m_error_line = 0;
	std::string m_file_name = "";
	std::string m_error_msg = "";
};

}
