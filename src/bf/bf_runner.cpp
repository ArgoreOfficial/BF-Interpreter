#include "bf_runner.h"

static bool is_legal_char( char _c )
{
	switch ( _c )
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

void BrainFuckRunner::power_cycle()
{
	out_buffer = "";
	src = "";

	pc = 0;
	cmd = {};

	loop_stack_counter = 0;
	pointer = 0;

	std::memset( cell_memory, 0, sizeof( cell_memory ) );
	std::memset( loop_stack, 0, sizeof( loop_stack ) );

	bytecode.clear();
}


void BrainFuckRunner::preprocessor( const std::string& _source )
{
	const size_t size = _source.size();
	char* parsed = new char[ size + 1 ];

	size_t parsed_size = 0;
	for ( size_t i = 0; i < size; i++ )
	{
		char c = _source[ i ];
		if ( !is_legal_char( c ) )
			continue;

		parsed[ parsed_size ] = c;
		parsed_size++;
	}

	parsed[ parsed_size ] = 0;

	src = std::string( parsed );
	delete[] parsed;
}

void BrainFuckRunner::parse_bytecode( std::string _source )
{
	const size_t size = _source.size();
	char* pChar = _source.data();

	BFBytecode current_bc{};

	while ( *pChar != 0 )
	{
		char c = *pChar;
		
		switch ( c )
		{
		case '>': {
			current_bc.type = BFCmd::Right;
			while ( *pChar == '>' )
			{
				current_bc.count++;
				pChar++;
			}
		}; break;
		case '<':
		{
			current_bc.type = BFCmd::Left;
			while ( *pChar == '<' )
			{
				current_bc.count++;
				pChar++;
			}
		}; break;
		case '+':
		{
			current_bc.type = BFCmd::Incr;
			while ( *pChar == '+' )
			{
				current_bc.count++;
				pChar++;
			}
		}; break;
		case '-':
		{
			current_bc.type = BFCmd::Decr;
			while ( *pChar == '-' )
			{
				current_bc.count++;
				pChar++;
			}
		}; break;
		case '.': { pChar++; current_bc.type = BFCmd::pOut; }; break;
		case ',': { pChar++; current_bc.type = BFCmd::pIn; }; break;
		case '[': { pChar++; current_bc.type = BFCmd::LoopBegin; }; break;
		case ']': { pChar++; current_bc.type = BFCmd::LoopEnd; }; break;
		}

		bytecode.push_back( current_bc );
		current_bc = {};
	}

}

void BrainFuckRunner::run( std::string _source )
{
	power_cycle();
	preprocessor( _source );
	parse_bytecode( src );

	incr_pc();

	while ( cmd.type != 0 )
	{
		printf( "\033[%d;%dH", 1, 1 );

		for ( size_t i = 0; i < 12; i++ )
			printf( "[%03i]", cell_memory[ i ] );
		printf( "\n" );

		interpret_cmd();
		incr_pc();

		printf( "%s\n", out_buffer.c_str() );
	}
}

void BrainFuckRunner::interpret_cmd()
{
	switch ( cmd.type )
	{
	case BFCmd::Right: { pointer += cmd.count; } break;
	case BFCmd::Left:  { pointer -= cmd.count; } break;
	case BFCmd::Incr:  { here() += cmd.count; } break;
	case BFCmd::Decr:  { here() -= cmd.count; } break;

	case BFCmd::pOut: { out_buffer += (char)here(); } break;
	case BFCmd::pIn:
	{
		char in = _getch();

		switch ( in ) // special cases
		{
		case '\r': in = '\n'; break;
		case EOF:  in = '\0'; break;
		case 26:   in = '\0'; break;
		}

		printf( "%i\n", in );
		here() = in;
	} break;

	case BFCmd::LoopBegin:
	{
		if ( here() == 0 )
		{
			uint32_t loop_counter = 0;
			do
			{
				if ( cmd.type == BFCmd::LoopBegin ) loop_counter++;
				else if ( cmd.type == BFCmd::LoopEnd ) loop_counter--;

				if ( loop_counter > 0 ) incr_pc();
			} while ( loop_counter > 0 );
		}
		else
			push_stack( pc - 1 );

	} break;

	case BFCmd::LoopEnd:
	{
		uint16_t jmp_point = pop_stack();
		if ( here() != 0 )
			jmp( jmp_point );

	} break;

	}
}
