#include "compiler.h"

static bool is_legal_char( char _c )
{
	switch( _c )
	{
	case '>': return true;
	case '<': return true;
	case '+': return true;
	case '-': return true;
	case '.': return true;
	case ',': return true;
	case '[': return true;
	case ']': return true;
	}

	return false;
}

brainfuck::compiler::error_code brainfuck::compiler::compile( std::vector<instruction>& _out_vec )
{
	_flush();

	if( m_source.empty() )
		return error_code::no_source_input;

	instruction current_instr{};

	while( m_pointer < m_source.size() )
	{
		char c = _here();
		_incr();

		if( !is_legal_char( c ) )
			continue;
		
		current_instr.count = 1;
		current_instr.type = static_cast<opcode>( c );

		switch( c )
		{
		case opcode::Right: current_instr.count += _count_sequence( opcode::Right ); break;
		case opcode::Left:  current_instr.count += _count_sequence( opcode::Left  ); break;
		case opcode::Incr:  current_instr.count += _count_sequence( opcode::Incr  ); break;
		case opcode::Decr:  current_instr.count += _count_sequence( opcode::Decr  ); break;
		
		case opcode::pOut:      break;
		case opcode::pIn:       break;
		case opcode::LoopBegin: break;
		case opcode::LoopEnd:   break;
		}

		m_compiled.push_back( current_instr );
		current_instr = {};
	}

	m_compiled.push_back( { opcode::NullTerm, 0 } );
	_out_vec = m_compiled;
	return error_code::success;
}

void brainfuck::compiler::_flush()
{
	m_compiled.clear();
	m_pointer = 0;
}

size_t brainfuck::compiler::_count_sequence( char _char )
{
	size_t n = 0;
	while( _here() == _char )
		n++, _incr();
	return n;
}
