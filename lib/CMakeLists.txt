add_library(exphone-lib
  blockitem.cpp
  blockitem.h
  blockmodel.cpp
  blockmodel.h
  exphoneconfig.cpp
  exphoneconfig.h
  pushclient.cpp
  pushclient.h
)

target_include_directories(exphone-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(exphone-lib PUBLIC Qt5::Core Qt5::Gui Qt5::Sql Qt5::DBus)

