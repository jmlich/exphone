file(GLOB TS_FILES translation/*.ts)

add_executable(exphone
  main.cpp
  exphone.qrc
  ${TS_FILES}
)

QT5_ADD_TRANSLATION(QM_FILES ${TS_FILES})
add_custom_target(translations DEPENDS ${QM_FILES})
add_dependencies(exphone translations)

QT5_CREATE_TRANSLATION(QT_FILES "${CMAKE_SOURCE_DIR}"
    OPTIONS "-no-obsolete")

install(TARGETS exphone RUNTIME
        DESTINATION ${CMAKE_INSTALL_BINDIR})

target_compile_definitions(exphone
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(exphone
  PRIVATE exphone-lib libwatchfish Qt5::Core Qt5::Gui Qt5::Sql Qt5::Quick)


install(FILES ${QM_FILES}
    DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/exphone/translation")

#install(FILES "exphone.desktop"
#    DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/applications")

# install(FILES "icons/exphone.png"
#     DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/86x86/apps"
#     RENAME "exphone.png")

# install(FILES "icons/exphone128.png"
#    DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/128x128/apps"
#    RENAME "exphone.png")

# install(FILES "exphone.appdata.xml"
#    DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/metainfo")

install(TARGETS exphone RUNTIME
        DESTINATION ${CMAKE_INSTALL_BINDIR})
