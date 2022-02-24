cmake_minimum_required(VERSION 3.0)

project(Cashdesk)
add_executable(Cashdesk main.cpp Add.cpp CashDesk.cpp Command.cpp Data.cpp Order.cpp Item.cpp Remove.cpp Show.cpp)


SET(CMAKE_CXX_FLAGS "-std=gnu++17")
file(COPY db DESTINATION .)
