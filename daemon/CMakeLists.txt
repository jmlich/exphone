file(GLOB TS_FILES translations/*.ts)

add_executable(exphoned
  daemon.cpp
  blockcontroler.cpp
  blockcontroler.h
  ${TS_FILES}
)


if(FLAVOR STREQUAL "uuitk")
    add_definitions(-DTRANSLATION_FOLDER="../share/exphone/translations")
else()
    add_definitions(-DTRANSLATION_FOLDER=".")
endif()

QT5_ADD_TRANSLATION(QM_FILES ${TS_FILES})
add_custom_target(exphoned_translations DEPENDS ${QM_FILES})
add_dependencies(exphoned exphoned_translations)

QT5_CREATE_TRANSLATION(QT_FILES "${CMAKE_SOURCE_DIR}"
    OPTIONS "-no-obsolete")

install(FILES ${QM_FILES}
    DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/exphone/translations")

install(TARGETS exphoned RUNTIME
        DESTINATION ${CMAKE_INSTALL_BINDIR})

target_compile_definitions(exphoned
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(exphoned
  PRIVATE libwatchfish exphone-lib Qt5::Core Qt5::Gui Qt5::Sql Qt5::Quick Qt5::DBus)
