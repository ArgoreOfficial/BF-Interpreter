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
		{
			continue;
		}

		current_bc.count = 1;

		switch( c )
		{
		case '>':
		{
			current_bc.type = BFCmd::Right;
			while( _here() == '>' ) current_bc.count++, _incr();
		}; break;
		case '<':
		{
			current_bc.type = BFCmd::Left;
			while( _here() == '<' ) current_bc.count++, _incr();
		}; break;
		case '+':
		{
			current_bc.type = BFCmd::Incr;
			while( _here() == '+' ) current_bc.count++, _incr();
		}; break;
		case '-':
		{
			current_bc.type = BFCmd::Decr;
			while( _here() == '-' ) current_bc.count++, _incr();
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
