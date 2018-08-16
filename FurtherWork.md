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

Client:
  states: DISCONNECTED, CONNECTED, REGISTERED, [UNREGISTERED]
  events: connected, rejected(uuid taken, no free slot), disconnected(timeout, closed by remote, close by local)

Server:
  states: ACCEPTING/REGISTERING
  events: connected
Server by client:
  states: CONNECTED, REGISTERED, DISCONNECTED, [UNREGISTERED]
  events: registered, disconnected(timeout, close by remote, close by local)

Registration: uuid, [player/client info (name, color, ...)? -> gamepad version only]
