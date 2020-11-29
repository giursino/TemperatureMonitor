# Install utils to system

# systemd temperature-server autostart
set(IN_FILE "temperature-server.service.in")
set(OUT_FILE "temperature-server.service")
configure_file(${IN_FILE} ${OUT_FILE} @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${OUT_FILE}
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/systemd/system)

# systemd temperature-to-cloud autostart
set(IN_FILE "temperature-to-cloud.service.in")
set(OUT_FILE "temperature-to-cloud.service")
configure_file(${IN_FILE} ${OUT_FILE} @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${OUT_FILE}
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/systemd/system)

# logrotate
set(IN_FILE "temperature-monitor.logrotate")
set(OUT_FILE "temperature-monitor.conf")
configure_file(${IN_FILE} ${OUT_FILE} @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${OUT_FILE}
    DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/logrotate.d)