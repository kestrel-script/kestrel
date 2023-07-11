#include <iostream>
#include <cassert>

#include "instruction_array.hpp"

int main(int argc, char** argv) {
    kestrel::InstructionArray arr;

    // Write some test values to the array
    arr.writeByte(0x12, 0);
    arr.writeShort(0x3456, 1);
    arr.writeInt(0x78CDEF, 3);

    // Check that the values were written correctly
    assert(arr.readByte(0) == 0x12);
    assert(arr.readShort(1)== 0x3456);
    assert(arr.readInt(3) == 0x78CDEF);
}