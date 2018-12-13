cmake_minimum_required(VERSION 3.4)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

option(BUILD_COVERAGE "Build coverage" OFF)

include("tools/gate/cmake/HunterGate.cmake")

huntergate(
  URL "https://github.com/ruslo/hunter/archive/v0.23.34.tar.gz"
  SHA1 "70287b1ffa810ee4e952052a9adff9b4856d0d54"
)

project(wallet)
string(APPEND CMAKE_CXX_FLAGS " -pedantic -Werror -Wall -Wextra")
string(APPEND CMAKE_CXX_FLAGS " -Wno-unused-command-line-argument")
string(APPEND CMAKE_CXX_FLAGS " -Wshadow -Wnon-virtual-dtor")

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)

hunter_add_package(OpenSSL)
hunter_add_package(GTest)
hunter_add_package(Boost COMPONENTS thread system)
find_package(Boost CONFIG REQUIRED thread system)
find_package(GTest CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)

add_executable(wallet
  ${CMAKE_CURRENT_SOURCE_DIR}/src/CryptoUtils.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/JsonUtils.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Key.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/LoggingCategories.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/NetworkUtils.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/OfflineWallet.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Script.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Transaction.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/TxIn.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/TxOut.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/http_client_async_ssl.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/usb_monitor.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/offline_wallet/informationwindow.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/offline_wallet/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/offline_wallet/mainwindow.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/offline_wallet/passwordforwallet.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/offline_wallet/transactionwindow.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/online_wallet/informationwindow.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/online_wallet/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/online_wallet/mainwindow.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/online_wallet/mainwindow.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/online_wallet/maketransactionwindow.cpp
)

add_executable(tests
  ${CMAKE_CURRENT_SOURCE_DIR}/tests/test.cpp
)

target_include_directories(wallet
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/offline_wallet
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/online_wallet
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes
)
target_include_directories(tests 
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/offline_wallet
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/online_wallet
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes
)

qt5_use_modules(wallet 
  Widgets
  Network
  Core
  Gui)

target_link_libraries(wallet 
  PUBLIC Boost::thread Boost::system Boost::boost 
  PUBLIC OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(tests PUBLIC GTest::main wallet 
  PUBLIC Boost::thread Boost::system Boost::boost 
  PUBLIC OpenSSL::SSL OpenSSL::Crypto)

enable_testing()
add_test(NAME unit_tests COMMAND tests) 

if(BUILD_COVERAGE)
  set(ENABLE_COVERAGE ON CACHE BOOL "Enable coverage build." FORCE)
  list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
  find_package(codecov)
  # TODO: change template word to project name and delete this comment
  add_coverage(wallet)
  add_coverage(tests)
  list(APPEND LCOV_REMOVE_PATTERNS "'${PROJECT_SOURCE_DIR}/tests/*'")
  coverage_evaluate()
endif()
