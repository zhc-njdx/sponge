#include "stream_reassembler.hh"

#include <assert.h>
#include <iostream>
#include <string.h>
#include <string>
// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

//#define NEW_PUSH

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) : _output(capacity), _capacity(capacity) {
    unassemble_buffer = new char[_capacity];
    occupied = new bool[_capacity];
    for (size_t i = 0; i < _capacity; i++) {
        occupied[i] = false;
    }
    begin = 0;
    end = _capacity - 1;
}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    //    DUMMY_CODE(data, index, eof);
    // std::cerr << index << ";" << eof << std::endl;
    // size_t data_len = strlen(data.c_str()); // the strlen function will end at '\0' if the '\0' appear in the middle
    // of data ... will be wrong
    size_t data_len = data.size();
    if (data_len == 0) {  // if data is empty return ( but need to judge the eof
        if (eof)
            _eof = true;
        if (_eof && empty())
            stream_out().end_input();
        return;
    }
    // the index of the first byte and of the last byte
    size_t begin_idx = index;
    size_t end_idx = index + data_len - 1;
    if (begin_idx > _next_idx + _capacity - 1 || end_idx < _next_idx)
        return;  // the data couldn't be stored in the buffer otherwise exceed the capacity or the total data has been
                 // pushed into ByteStream
    // the position that data stored in the buffer
    // 7.a big deal!!! forget to consider the condition that the current data contain the data that has already
    // reassembled
    size_t begin_pos;
    size_t j;  // indicating the index of data
               //	bool has_next;
    if (begin_idx > _next_idx) {
        begin_pos = (begin + (begin_idx - _next_idx)) % _capacity;  // 4. here also need to mod _capacity
        j = 0;
        // has_next = false; // lack of some bytes
    } else {
        begin_pos = begin;
        j = _next_idx - begin_idx;
        // has_next = true; // can build newly continuous data string
    }
    size_t i = begin_pos;  // indicating tbe idx of the buffer
    //	while( i != end && j < data_len ){ //3. here 'i' can be 'end', 'end' indicates the last byte in the buffer, i =
    //end is also right
    while (j < data_len) {
        if (occupied[i] == true) {  // the position has already the valid byte
                                    // pass
        } else {
            // when the last byte is the last byte of the entire stream
            // 2.here has some wrong understandings,
            // when 'eof' is 1, indicating the stream has been end and
            // there will be no more data
            // if(j == data_len - 1 && eof){
            //	_eof = true;
            //}
            unassemble_buffer[i] = data[j];
            occupied[i] = true;
            buffer_bytes++;
        }
        // when data is end but it can connect two substring!!!
        // if(has_next) _next_idx ++;
        i = (i + 1) % _capacity;
        if (i == begin)
            break;  // 3.solution
        j++;
    }

    // 2. the '_eof' should be set to true as long as the 'eof' is 1.
    if (eof)
        _eof = true;
    // write the newly continuous data into stream
    string written_data = "";
    size_t bgn = begin;
    while (occupied[bgn] == true) {
        written_data += unassemble_buffer[bgn];
        bgn = (bgn + 1) % _capacity;
        if (bgn == begin)
            break;  // 5.when the ptr back the begin, the loop should be over.
    }
    // tha actually written data length
    size_t written_bytes_len = stream_out().write(written_data);
    // update the buffer begin and end
    // update the buffer bytes number and 6.occupied array.
    // 7. update the _next_idx
    for (size_t m = 0; m < written_bytes_len; m++) {
        occupied[begin] = false;
        begin = (begin + 1) % _capacity;
        end = (end + 1) % _capacity;
        buffer_bytes--;
        _next_idx++;
    }
    // begin = (begin + written_bytes_len) % _capacity;
    // end = (end + written_bytes_len) % _capacity;
    // update the remaining bytes in the buffer
    // buffer_bytes -= written_bytes_len; // 1. after written into the stream, the number of bytes in the buffer need to
    // update
    // when all data have been reassembled and write into stream
    if (empty() && _eof) {
        stream_out().end_input();
    }
}

// 1.
#ifdef NEW_PUSH
void StreamReassembler::push_substring(const std::string &data, const uint64_t index, const bool eof) {
    size_t data_len = strlen(data.c_str());
    size_t i = begin;
    size_t idx = index;
    size_t j = 0;
    size_t buffer_idx = _next_idx;
    while (j < data_len) {
        if (_next_idx == idx) {
            if (occupied[i] == false) {
                unassemble_buffer[i] = data[j];
                occupied[i] = true;
                buffer_bytes++;
                i = (i + 1) % _capacity;
                j++;
                idx++;

            } else {
                i = (i + 1) % _capacity;
                j++;
                idx++;
            }
        } else if (_next_idx > idx) {
            idx++;
            j++;
        } else {  // _next_idx < idx
            i = (i + 1) % _capacity;
        }
    }
}
#endif

size_t StreamReassembler::unassembled_bytes() const { return buffer_bytes; }

bool StreamReassembler::empty() const { return buffer_bytes == 0; }
