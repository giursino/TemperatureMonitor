add_mocked_test(temperature-server
  #SOURCES ${CMAKE_SOURCE_DIR}/src/temperature-client.c
  MOCKS
    LKU_ReceiveLBusmonMessage
    write
  LINK_LIBRARIES Threads::Threads ${LIBKNXUSB_LIBRARY}
)
