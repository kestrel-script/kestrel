
#include "instruction_array.hpp"

namespace kestrel {

class InstructionArrayDetail {
public:
  std::vector<uint8_t> elements;
};

InstructionArray::InstructionArray()
    : detail(std::make_unique<InstructionArrayDetail>()) {}

InstructionArray::InstructionArray(const InstructionArray& other)
    : detail(std::make_unique<InstructionArrayDetail>(*other.detail)) {}

InstructionArray::InstructionArray(InstructionArray&& other) noexcept
    : detail(std::move(other.detail)) {}

InstructionArray& InstructionArray::operator=(const InstructionArray& other) {
  if (this != &other) {
    *detail = *other.detail;
  }
  return *this;
}

InstructionArray& InstructionArray::operator=(InstructionArray&& other) noexcept {
  if (this != &other) {
    detail = std::move(other.detail);
  }
  return *this;
}

InstructionArray::~InstructionArray() = default;

void InstructionArray::writeByte(uint8_t value, size_t index) {
  if (index >= detail->elements.size()) {
    detail->elements.resize(index * 1.5 + 8);
  }
  detail->elements[index] = value;
}

void InstructionArray::writeBoolean(bool value, size_t index) {
  writeByte(value ? 1 : 0, index);
}

void InstructionArray::writeShort(int16_t value, size_t index) {
  writeByte(value & 0xff, index);
  writeByte(value >> 8, index + 1);
}

void InstructionArray::writeInt(int32_t value, size_t index) {
  writeShort(value & 0xffff, index);
  writeShort(value >> 16, index + 2);
}

void InstructionArray::writeLong(int64_t value, size_t index) {
  writeInt(static_cast<int32_t>(value & 0xffffffff), index);
  writeInt(static_cast<int32_t>(value >> 32), index + 4);
}

size_t InstructionArray::appendByte(uint8_t value) {
  detail->elements.push_back(value);
  return detail->elements.size() - 1;
}

size_t InstructionArray::appendBoolean(bool value) {
  detail->elements.push_back(value ? 1 : 0);
  return detail->elements.size() - 1;
}

size_t InstructionArray::appendShort(int16_t value) {
  size_t index = detail->elements.size();
  detail->elements.resize(index + 2);
  writeShort(value, index);
  return index;
}

size_t InstructionArray::appendInt(int32_t value) {
  size_t index = detail->elements.size();
  detail->elements.resize(index + 4);
  writeInt(value, index);
  return index;
}

size_t InstructionArray::appendLong(int64_t value) {
  size_t index = detail->elements.size();
  detail->elements.resize(index + 8);
  writeLong(value, index);
  return index;
}

uint8_t InstructionArray::readByte(size_t index) const {
  return detail->elements[index];
}

bool InstructionArray::readBoolean(size_t index) const {
  return readByte(index) != 0;
}

int16_t InstructionArray::readShort(size_t index) const {
  int16_t result = (static_cast<uint8_t>(detail->elements[index + 1]) << 8) |
                    static_cast<uint8_t>(detail->elements[index]);
  return result;
}

int32_t InstructionArray::readInt(size_t index) const {
  int32_t result = (static_cast<uint8_t>(detail->elements[index + 3]) << 24) |
                   (static_cast<uint8_t>(detail->elements[index + 2]) << 16) |
                   (static_cast<uint8_t>(detail->elements[index + 1]) << 8) |
                   static_cast<uint8_t>(detail->elements[index]);
  return result;
}

int64_t InstructionArray::readLong(size_t index) const {
  int64_t result = (static_cast<int64_t>(readInt(index + 4)) << 32) | readInt(index);
  return result;
}

size_t InstructionArray::size() const {
  return detail->elements.size();
}
}