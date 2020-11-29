# Install utils to system

# systemd temperature-server autostart
set(IN_FILE "temperature-server.service.in")
set(OUT_FILE "temperature-server.service")
configure_file(${IN_FILE} ${OUT_FILE} @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${OUT_FILE}
    DESTINATION ${CMAKE_INSTALL_DATADIR}/${CMAKE_PROJECT_NAME})

# systemd temperature-to-cloud autostart
set(IN_FILE "temperature-to-cloud.service.in")
set(OUT_FILE "temperature-to-cloud.service")
configure_file(${IN_FILE} ${OUT_FILE} @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${OUT_FILE}
    DESTINATION ${CMAKE_INSTALL_DATADIR}/${CMAKE_PROJECT_NAME})

# logrotate
install(FILES "temperature-monitor.logrotate"
    DESTINATION ${CMAKE_INSTALL_DATADIR}/${CMAKE_PROJECT_NAME})