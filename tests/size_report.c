#ifndef NANOPRINTF_SIZE_REPORT
  #error NANOPRINTF_SIZE_REPORT must be defined
#endif

#if NANOPRINTF_SIZE_REPORT == 0 // Minimal
  #define NANOPRINTF_USE_FIELD_WIDTH_FORMAT_SPECIFIERS 0
  #define NANOPRINTF_USE_PRECISION_FORMAT_SPECIFIERS 0
  #define NANOPRINTF_USE_FLOAT_FORMAT_SPECIFIERS 0
  #define NANOPRINTF_USE_LARGE_FORMAT_SPECIFIERS 0
  #define NANOPRINTF_USE_BINARY_FORMAT_SPECIFIERS 0
  #define NANOPRINTF_USE_WRITEBACK_FORMAT_SPECIFIERS 0

#elif NANOPRINTF_SIZE_REPORT == 1 // binary
  #define NANOPRINTF_USE_FIELD_WIDTH_FORMAT_SPECIFIERS 0
  #define NANOPRINTF_USE_PRECISION_FORMAT_SPECIFIERS 0
  #define NANOPRINTF_USE_FLOAT_FORMAT_SPECIFIERS 0
  #define NANOPRINTF_USE_LARGE_FORMAT_SPECIFIERS 0
  #define NANOPRINTF_USE_BINARY_FORMAT_SPECIFIERS 1
  #define NANOPRINTF_USE_WRITEBACK_FORMAT_SPECIFIERS 0

#elif NANOPRINTF_SIZE_REPORT == 2 // field width + precision
  #define NANOPRINTF_USE_FIELD_WIDTH_FORMAT_SPECIFIERS 1
  #define NANOPRINTF_USE_PRECISION_FORMAT_SPECIFIERS 1
  #define NANOPRINTF_USE_FLOAT_FORMAT_SPECIFIERS 0
  #define NANOPRINTF_USE_LARGE_FORMAT_SPECIFIERS 0
  #define NANOPRINTF_USE_BINARY_FORMAT_SPECIFIERS 0
  #define NANOPRINTF_USE_WRITEBACK_FORMAT_SPECIFIERS 0

#elif NANOPRINTF_SIZE_REPORT == 3 // field width + precision + binary
  #define NANOPRINTF_USE_FIELD_WIDTH_FORMAT_SPECIFIERS 1
  #define NANOPRINTF_USE_PRECISION_FORMAT_SPECIFIERS 1
  #define NANOPRINTF_USE_FLOAT_FORMAT_SPECIFIERS 0
  #define NANOPRINTF_USE_LARGE_FORMAT_SPECIFIERS 0
  #define NANOPRINTF_USE_BINARY_FORMAT_SPECIFIERS 1
  #define NANOPRINTF_USE_WRITEBACK_FORMAT_SPECIFIERS 0

#elif NANOPRINTF_SIZE_REPORT == 4 // float
  #define NANOPRINTF_USE_FIELD_WIDTH_FORMAT_SPECIFIERS 0
  #define NANOPRINTF_USE_PRECISION_FORMAT_SPECIFIERS 1
  #define NANOPRINTF_USE_FLOAT_FORMAT_SPECIFIERS 1
  #define NANOPRINTF_USE_LARGE_FORMAT_SPECIFIERS 0
  #define NANOPRINTF_USE_BINARY_FORMAT_SPECIFIERS 0
  #define NANOPRINTF_USE_WRITEBACK_FORMAT_SPECIFIERS 0

#elif NANOPRINTF_SIZE_REPORT == 5 // everything
  #define NANOPRINTF_USE_FIELD_WIDTH_FORMAT_SPECIFIERS 1
  #define NANOPRINTF_USE_PRECISION_FORMAT_SPECIFIERS 1
  #define NANOPRINTF_USE_FLOAT_FORMAT_SPECIFIERS 1
  #define NANOPRINTF_USE_LARGE_FORMAT_SPECIFIERS 1
  #define NANOPRINTF_USE_BINARY_FORMAT_SPECIFIERS 1
  #define NANOPRINTF_USE_WRITEBACK_FORMAT_SPECIFIERS 1

#else
  #error NANOPRINTF_SIZE_REPORT unknown value
#endif

#define NANOPRINTF_IMPLEMENTATION

#include "../nanoprintf.h"
