#include "compiler.h"
#include "sstream"

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

brainfuck::compiler::compiler( const std::string& _source )
{
	std::istringstream f( _source );
	std::string line;
	while( std::getline( f, line ) )
		m_source_lines.push_back( line );
}

brainfuck::compiler::error_code brainfuck::compiler::compile( const std::string& _file_name, std::vector<instruction>& _out_vec )
{
	_flush();
	m_file_name = _file_name;
	
	if( m_source_lines.empty() )
	{
		m_error_msg = " No source input";
		return error_code::no_source_input;
	}

	instruction current_instr{};

	for( size_t i = 0; i < m_source_lines.size(); i++ )
	{
		m_line = i;
		m_error_line = m_line;
		m_pointer = 0;

		while( m_pointer < m_source_lines[ m_line ].size() )
		{
			char c = _here();
			_incr();

			if( !is_legal_char( c ) )
				continue;
		
			current_instr.count = 1;
			current_instr.type = static_cast<opcode>( c );
			size_t current_index = m_compiled.size();
			m_error_point = m_pointer;

			switch( c )
			{
			case opcode::Right: current_instr.count += _count_sequence( opcode::Right ); break;
			case opcode::Left:  current_instr.count += _count_sequence( opcode::Left  ); break;
			case opcode::Incr:  current_instr.count += _count_sequence( opcode::Incr  ); break;
			case opcode::Decr:  current_instr.count += _count_sequence( opcode::Decr  ); break;
		
			case opcode::pOut:      break;
			case opcode::pIn:       break;
			case opcode::LoopBegin:
			{
				loop_stack.push( { current_index } );
			} break;
			case opcode::LoopEnd:
			{
				if( loop_stack.empty() )
				{
					m_error_msg = "Missing [";
					return error_code::missing_loop_begin;
				}

				loop_range range = loop_stack.top();
				loop_stack.pop();
				m_compiled[ range.begin ].partner = current_index;
				current_instr.partner = range.begin;
			} break;
			}

			m_compiled.push_back( current_instr );
			current_instr = {};
		}
	}

	if( !loop_stack.empty() )
	{
		m_error_msg = "Missing ]";
		return error_code::missing_loop_end;
	}

	m_compiled.push_back( { opcode::NullTerm, 0 } );
	_out_vec = m_compiled;
	return error_code::success;
}

std::string brainfuck::compiler::get_error_message()
{
	std::string msg = std::format( "{}:{}:{}: error: {}", m_file_name, m_error_line, m_error_point, m_error_msg );
	std::string dsc = "";
	std::string where = "";
	std::string error_line = m_source_lines[ m_error_line ];

	int32_t begin = std::max<int32_t>( 0, (int32_t)m_error_point - 9 );
	int32_t end = std::min<int32_t>( (int32_t)m_error_point + 9, error_line.size());

	for( size_t i = begin; i < end; i++ )
	{
		dsc += error_line[ i ];
		where += (i == m_error_point-1) ? '^' : ' ';
	}

	return msg + "\n" + dsc + "\n" + where;
}

void brainfuck::compiler::_flush()
{
	m_file_name = "";
	m_error_msg = "";
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
