# temperature-server
add_executable(temperature-server "temperature-server.c")
include_directories(${LIBKNXUSB_INCLUDE_DIR})
target_link_libraries(temperature-server PRIVATE Threads::Threads libKnxUsb::libKnxUsb)

install (TARGETS temperature-server DESTINATION bin)

# temperature-client
add_executable(temperature-client "temperature-client.c")

# temperature-to-cloud
install (FILES "temperature-to-cloud.py" DESTINATION bin)

# temperature-to-file
install (FILES "temperature-to-file.py" DESTINATION bin)

