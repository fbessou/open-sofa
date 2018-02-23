# Further Work

Passing C++ 11 functions by const/reference as function parameter.

2 modes for efficient memory management:
  small msg (mainly primitive types) but high frequency (game/gamepad)
    -> buffer: try to avoid to alloc/free mem, circular buffer or equivalent
    -> msgpack: unppack not referenced
  big msg with STR and BIN:
    -> buffer: avoid copy of buffer, care of segmented data
    -> msgpack: unpack referenced

Bloking / non-blocking stream
