file(GLOB TS_FILES translations/*.ts)


if(FLAVOR STREQUAL "uuitk")
    add_definitions(-DTRANSLATION_FOLDER="./share/exphone/translations")
# FIXME else()
endif()

list(APPEND RESOURCES exphone.qrc)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/platform-${FLAVOR}.qrc")
    list(APPEND RESOURCES "platform-${FLAVOR}.qrc")
else()
    message(WARNING "Resource file 'platform-${FLAVOR}.qrc' not found. Proceeding without it.")
endif()

add_executable(exphone
  main.cpp
  ${RESOURCES}
  ${TS_FILES}
)

QT5_ADD_TRANSLATION(QM_FILES ${TS_FILES})
add_custom_target(translations DEPENDS ${QM_FILES})
add_dependencies(exphone translations)

QT5_CREATE_TRANSLATION(QT_FILES "${CMAKE_SOURCE_DIR}"
    OPTIONS "-no-obsolete")

install(FILES ${QM_FILES}
    DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/exphone/translations")

install(TARGETS exphone RUNTIME
        DESTINATION ${CMAKE_INSTALL_BINDIR})

target_compile_definitions(exphone
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(exphone
  PRIVATE exphone-lib libwatchfish Qt5::Core Qt5::Gui Qt5::Sql Qt5::Quick)

#install(FILES "exphone.desktop"
#    DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/applications")

install(FILES "icons/256x256/exphone.png"
    DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/256x256/apps")

install(FILES "icons/512x512/exphone.png"
    DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/512x512/apps")

# install(FILES "exphone.appdata.xml"
#    DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/metainfo")
