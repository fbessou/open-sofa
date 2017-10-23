#include <bson.h>

namespace BSON {
  enum class Type {
    EOD = 0x00,
    DOUBLE = 0x01,
    UTF8 = 0x02,
    DOCUMENT = 0x03,
    ARRAY = 0x04,
    BINARY = 0x05,
    UNDEFINED = 0x06,
    OID = 0x07,
    BOOL = 0x08,
    DATE_TIME = 0x09,
    NULL = 0x0A,
    REGEX = 0x0B,
    DBPOINTER = 0x0C,
    CODE = 0x0D,
    SYMBOL = 0x0E,
    CODEWSCOPE = 0x0F,
    INT32 = 0x10,
    TIMESTAMP = 0x11,
    INT64 = 0x12,
    DECIMAL128 = 0x13,
    MAXKEY = 0x7F,
    MINKEY = 0xFF,
  };

  enum class Subtype {
   
typedef enum {
   BINARY = 0x00,
   FUNCTION = 0x01,
   BINARY_DEPRECATED = 0x02,
   UUID_DEPRECATED = 0x03,
   UUID = 0x04,
   MD5 = 0x05,
   USER = 0x80,
  };
}
