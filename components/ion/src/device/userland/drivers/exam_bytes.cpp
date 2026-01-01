#include <assert.h>
#include <ion/exam_bytes.h>
#include <string.h>

#include "persisting_bytes.h"

namespace Ion {
namespace ExamBytes {
Int s_examBytes = 0;
Int read() { return s_examBytes; }
void write(Int examBytes) { s_examBytes = examBytes; }
/*
  Int read() {
  return *reinterpret_cast<Int*>(
      PersistingBytes::read(PersistingBytes::Entry::ExamBytes));
}

void write(Int value) {
  PersistingBytes::write(reinterpret_cast<uint8_t*>(&value), sizeof(Int),
                         PersistingBytes::Entry::ExamBytes);
}
*/
}  // namespace ExamBytes
}  // namespace Ion
