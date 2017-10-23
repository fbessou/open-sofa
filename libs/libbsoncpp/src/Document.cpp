#include "Document.h"

namespace BSON {

Document::Document()
{
}

template <Type t = Timestamp, double>
    void Document::append < (const key, T value)
}

template <typename ValueType = uint8_t>
void Document::append(const string& key, uint8_t& value)
{
  bson_append_uint8(document_, key, value)
}

template <BSONType TypeCode = UINT8, typename ValueType = uint8_t>
void Document::append<TypeCode>(const string& key, uint8_t& value)
{
}
