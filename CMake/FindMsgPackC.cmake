# - Find MsgPack-C

if(MSGPACK_C_INCLUDE_DIRS)
    set(MSGPACK_C_FOUND TRUE)

else(MSGPACK_C_INCLUDE_DIRS)
    find_path(MSGPACK_C_INCLUDE_DIRS msgpack.hpp
      /usr/include
      /usr/include/msgpack
      /usr/local/include/
      /usr/local/include/msgpack
      )

  if(MSGPACK_C_INCLUDE_DIRS)
      set(MSGPACK_C_FOUND TRUE)
      message(STATUS "Found msgpack-c: ${MSGPACK_C_INCLUDE_DIRS}")
  else(MSGPACK_C_INCLUDE_DIRS AND MSGPACK_C_LIBRARIES)
      set(MSGPACK_C_FOUND FALSE)
    message(STATUS "msgpack-c not found.")
  endif(MSGPACK_C_INCLUDE_DIRS)

  mark_as_advanced(MSGPACK_C_INCLUDE_DIRS)

endif(MSGPACK_C_INCLUDE_DIRS)
