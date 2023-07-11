#pragma once

#include <cstdint>
#include <vector>
#include <memory>

namespace kestrel {

class InstructionArrayDetail;

class InstructionArray {
public:
  InstructionArray();
  InstructionArray(const InstructionArray& other);
  InstructionArray(InstructionArray&& other) noexcept;
  InstructionArray& operator=(const InstructionArray& other);
  InstructionArray& operator=(InstructionArray&& other) noexcept;
  ~InstructionArray();

  void writeByte(uint8_t value, size_t index);
  void writeBoolean(bool value, size_t index);
  void writeShort(int16_t value, size_t index);
  void writeInt(int32_t value, size_t index);
  void writeLong(int64_t value, size_t index);

  size_t appendByte(uint8_t value);
  size_t appendBoolean(bool value);
  size_t appendShort(int16_t value);
  size_t appendInt(int32_t value);
  size_t appendLong(int64_t value);

  uint8_t readByte(size_t index) const;
  bool readBoolean(size_t index) const;
  int16_t readShort(size_t index) const;
  int32_t readInt(size_t index) const;
  int64_t readLong(size_t index) const;

  size_t size() const;

private:
  std::unique_ptr<InstructionArrayDetail> detail;
};

}