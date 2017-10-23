#include <bson.h>

#include "Types.h"
namespace BSON {

class Document {
public:
  Document();
  template <BSON::Type TypeCode, typename ValueType>
  void append(const std::string& key, const ValueType& value);

  template <typename ValueType>
  void append(const std::string& key, const ValueType & value);

};
}
