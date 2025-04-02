#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include <conio.h>

bool is_legal_char( char _c )
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

struct BrainFuckRunner
{
	std::string src        = "";
	std::string out_buffer = "";

	uint16_t pc = 0;
	char cmd = 0;

	int32_t pointer = 0;
	uint8_t cell_memory[ UINT16_MAX ];

	uint16_t loop_stack[ 256 ] = { 0 };
	uint8_t loop_stack_counter = 0;

	std::vector<char> ran_chars{};

	void preprocessor( const std::string _source )
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

	void power_cycle() {
		out_buffer = "";
		src        = "";

		pc = 0;
		cmd = 0;
		
		loop_stack_counter = 0;
		pointer = 0;

		std::memset( cell_memory, 0, sizeof( cell_memory ) );
		std::memset( loop_stack, 0, sizeof( loop_stack ) );

		ran_chars.clear();
	}

	void push_stack( uint8_t _v ) {
		loop_stack[ loop_stack_counter++ ] = _v;
	}

	uint8_t pop_stack() {
		return loop_stack[ --loop_stack_counter ];
	}

	void incr_pc() {
		cmd = src[ pc++ ];
	}

	void jmp( uint16_t _i ) {
		pc = _i;
		cmd = src[ _i ];
	}

	uint8_t& here() { return cell_memory[ pointer ]; }

	void interpret_cmd()
	{
		switch ( cmd )
		{
		case '>': { pointer++; } break;
		case '<': { 
			pointer--; 
		} break;

		case '+': { here()++; } break;
		case '-': { here()--; } break;

		case '.': { out_buffer += (char)here(); } break;
		case ',': { 
			char c = 13;
			char in = 0;
			do
			{
				in = _getch();
				if( in != 13 )
					c = in;
			} while ( in != 13 );

			here() = c == 13 ? 10 : c;
		} break;

		case '[':
		{
			if ( here() == 0 )
			{
				uint32_t loop_counter = 0;
				do
				{
					if      ( cmd == '[' ) loop_counter++;
					else if ( cmd == ']' ) loop_counter--;

					if ( loop_counter > 0 ) incr_pc();
				} while ( loop_counter > 0 );
			}
			else
				push_stack( pc - 1 );

		} break;

		case ']':
		{
			uint16_t jmp_point = pop_stack();
			if ( here() != 0 )
				jmp( jmp_point );

		} break;

		}
	}

	void run( std::string _source )
	{
		power_cycle();
		incr_pc();
		preprocessor( _source );

		while ( cmd != 0 )
		{
			printf( "\033[%d;%dH", 1, 1 );

			for ( size_t i = 0; i < 12; i++ )
				printf( "[%03i]", cell_memory[ i ] );
			printf( "\n" );
			
			ran_chars.push_back( cmd );
			interpret_cmd();
			incr_pc();

			printf( "%s\n", out_buffer.c_str() );
		}
	}
};

BrainFuckRunner runner{};

int main()
{
	std::string source, line;
	std::ifstream file( "../hello_world.b" );
	while ( std::getline( file, line ) )
		source += line;
	
	runner.run( source );

	return 0;
}
