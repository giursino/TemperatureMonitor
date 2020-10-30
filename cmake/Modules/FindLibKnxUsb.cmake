include(GNUInstallDirs)

find_library(
  LIBKNXUSB_LIBRARY
  NAMES knxusb
)

find_path(
  LIBKNXUSB_INCLUDE_DIR
  NAMES libknxusb.h
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(LibKnxUsb DEFAULT_MSG
  LIBKNXUSB_LIBRARY
  LIBKNXUSB_INCLUDE_DIR
)

mark_as_advanced(LIBKNXUSB_LIBRARY LIBKNXUSB_INCLUDE_DIR)

if(LIBKNXUSB_FOUND AND NOT TARGET libKnxUsb::libKnxUsb)
  add_library(libKnxUsb:libKnxUsb SHARED IMPORTED)
  set_target_properties(
    libKnxUsb:libKnxUsb
    PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${LIBKNXUSB_INCLUDE_DIR}"
    IMPORTED_LOCATION ${LIBKNXUSB_LIBRARY})
endif()
