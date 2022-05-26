#include "byte_stream.hh"

#include <string.h>
#include <string>
// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity) {
    //	DUMMY_CODE(capacity);
    _capacity = capacity;
    _buffer = new char[capacity];
}

size_t ByteStream::write(const string &data) {
    //    DUMMY_CODE(data);
    //    return {};
    size_t len = 0;
    // 4. need to consider the len of data!!!
    // 5. from: lab1 strlen function will be invalid when the data has '\0'
    // size_t data_len = strlen(data.c_str());
    size_t data_len = data.size();
    while (len < data_len && remaining_capacity() > 0) {  // still has data to write and still has space for new bytes
        _buffer[buffer_write_ptr] = data[len++];          // write the one byte
        write_bytes_num++;                                // total written bytes add
        bytes_in_buffer++;                                // the bytes in buffer add
        buffer_write_ptr = (buffer_write_ptr + 1) % _capacity;
    }
    return len;  // return the written bytes num
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    //    DUMMY_CODE(len);
    //    return {};

    string res = "";
    int can_read_len = bytes_in_buffer > len ? len : bytes_in_buffer;
    size_t ptr = buffer_read_ptr;
    while (can_read_len-- > 0) {
        res += _buffer[ptr];
        ptr = (ptr + 1) % _capacity;  // 3. here also consider the circle data structure
    }
    return res;
}

//! \param[in] len bytes will be removed from the output side of the buffer

// 1.pop also means read!!!
void ByteStream::pop_output(const size_t len) {
    //	DUMMY_CODE(len);
    int can_read_len = bytes_in_buffer > len ? len : bytes_in_buffer;
    while (can_read_len-- > 0) {
        buffer_read_ptr = (buffer_read_ptr + 1) % _capacity;
        bytes_in_buffer--;
        read_bytes_num++;
    }
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    //    DUMMY_CODE(len);
    //    return {};
    string res = peek_output(len);
    pop_output(len);
    return res;
}

void ByteStream::end_input() { _eof = true; }

bool ByteStream::input_ended() const { return _eof; }

// the num of bytes that can read from the stream into buffer
size_t ByteStream::buffer_size() const {
    //	return {};
    return bytes_in_buffer;
}

bool ByteStream::buffer_empty() const {
    //	return {};
    return bytes_in_buffer == 0;
}

bool ByteStream::eof() const {
    // 2.only the writer end the input, output can reach the ending
    // _eof is precondition !!!
    return (_eof && write_bytes_num == read_bytes_num);
    //	return false;
}

size_t ByteStream::bytes_written() const {
    //	return {};
    return write_bytes_num;
}

size_t ByteStream::bytes_read() const {
    //	return {};
    return read_bytes_num;
}

// the one byte was read from stream in reader
// the one more byte can be written into stream in writer
size_t ByteStream::remaining_capacity() const {
    //	return {};
    // the space in buffer - the bytes flying in the stream
    return _capacity - bytes_in_buffer;
}
