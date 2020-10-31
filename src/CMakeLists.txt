add_executable(temperature-server temperature-server.c)
include_directories(${LIBKNXUSB_INCLUDE_DIR})
target_link_libraries(temperature-server PRIVATE Threads::Threads libKnxUsb::libKnxUsb)


add_executable(temperature-client temperature-client.c)
