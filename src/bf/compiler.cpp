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

int brainfuck_compiler::compile( std::vector<BFBytecode>& _out_vec )
{
	_flush();

	if( m_source.empty() )
		return 1;

	BFBytecode current_bc{};

	while( m_pointer < m_source.size() )
	{
		char c = _here();
		_incr();

		if( !is_legal_char( c ) )
			continue;
		
		current_bc.count = 1;

		switch( c )
		{
		case '>':
		{
			current_bc.type = BFCmd::Right;
			current_bc.count += _count_sequence( '>' );
		}; break;
		case '<':
		{
			current_bc.type = BFCmd::Left;
			current_bc.count += _count_sequence( '<' );
		}; break;
		case '+':
		{
			current_bc.type = BFCmd::Incr;
			current_bc.count += _count_sequence( '+' );
		}; break;
		case '-':
		{
			current_bc.type = BFCmd::Decr;
			current_bc.count += _count_sequence( '-' );
		}; break;
		case '.': { current_bc.type = BFCmd::pOut; }; break;
		case ',': { current_bc.type = BFCmd::pIn; }; break;
		case '[': { current_bc.type = BFCmd::LoopBegin; }; break;
		case ']': { current_bc.type = BFCmd::LoopEnd; }; break;
		}

		m_compiled.push_back( current_bc );
		current_bc = {};
	}

	m_compiled.push_back( { BFCmd::NullTerm, 0 } );
	_out_vec = m_compiled;
	return 0;
}

void brainfuck_compiler::_flush()
{

}

size_t brainfuck_compiler::_count_sequence( char _char )
{
	size_t n = 0;
	while( _here() == _char )
		n++, _incr();
	return n;
}
