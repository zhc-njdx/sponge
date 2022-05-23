#include "stream_reassembler.hh"
#include <string.h>
// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) : _output(capacity), _capacity(capacity) {
	unassemble_buffer = new char[_capacity];
	occupied = new bool[_capacity];
	begin = 0;
	end = _capacity - 1;
}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
//    DUMMY_CODE(data, index, eof);
	size_t data_len = strlen(data.c_str());
	// the index of the first byte and of the last byte
	size_t begin_idx = index; size_t end_idx = index + data_len - 1;
	if( begin_idx > _next_idx + _capacity - 1 || 
			end_idx < _next_idx ) return; // the data couldn't be stored in the buffer otherwise exceed the capacity
	// the position that data stored in the buffer
	size_t begin_pos = begin + (begin_idx - _next_idx);
	size_t i = begin_pos; // indicating tbe idx of the buffer
	size_t j = 0; // indicating the idx of the data
	while( i != end && j < data_len ){
		if( occupied[i] == true ){ // the position has already the valid byte
			// pass
		} else {
			// when the last byte is the last byte of the entire stream
			if(j == data_len - 1 && eof){
				_eof = true;
			}
			unassemble_buffer[i] = data[j];
			occupied[i] = true;
			buffer_bytes ++;
		}
		_next_idx ++;
		i = (i + 1) % _capacity;
		j ++;
	}
	// write the newly continuous data into stream
	string written_data = "";
	int bgn = begin;
	while(occupied[bgn] == true){
		written_data += unassemble_buffer[bgn];
		bgn = (bgn + 1) % _capacity;
	}
	// tha actually written data length
	size_t written_bytes_len = stream_out().write(written_data);
	// update the buffer begin and end
	begin = (begin + written_bytes_len) % _capacity;
	end = (end + written_bytes_len) % _capacity;
	// when all data have been reassembled and write into stream
	if(unassembled_bytes() == 0 && empty() && _eof){
		stream_out().end_input();
	}
}

size_t StreamReassembler::unassembled_bytes() const { 
	return buffer_bytes;
}

bool StreamReassembler::empty() const { 
	return buffer_bytes == 0;
}
