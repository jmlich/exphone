add_executable(exphoned
  daemon.cpp
  blockcontroler.cpp
  blockcontroler.h
)

install(TARGETS exphoned RUNTIME
        DESTINATION ${CMAKE_INSTALL_BINDIR})

target_compile_definitions(exphoned
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(exphoned
  PRIVATE libwatchfish exphone-lib Qt5::Core Qt5::Gui Qt5::Sql Qt5::Quick Qt5::DBus)
