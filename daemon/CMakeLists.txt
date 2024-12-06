add_executable(exphone-daemon
  daemon.cpp
  blockcontroler.cpp
  blockcontroler.h
)

target_link_libraries(exphone-daemon PRIVATE exphone-lib)

install(TARGETS exphone-daemon RUNTIME
        DESTINATION ${CMAKE_INSTALL_BINDIR})

target_compile_definitions(exphone-daemon
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(exphone-daemon
  PRIVATE Qt5::Core Qt5::Gui Qt5::Sql Qt5::Quick Qt5::DBus)
