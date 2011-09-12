#
# dependencies.cmake: Sets depencies for PELICAN.
#
# Required dependencies:
#   Qt4
#   Boost (program options)
#   CppUnit
#
# Sets the following variables:
#   PROJECT_LIBARRIES: Needed for linking with the libpelican.a
#

# === Find Dependencies.
find_package(Boost COMPONENTS program_options REQUIRED)
find_package(Qt4 COMPONENTS QtCore QtNetwork QtXml REQUIRED)
find_package(CppUnit REQUIRED)

# Global include directories set for all sub-packages
include_directories(
    #${CMAKE_CURRENT_SOURCE_DIR}
    ${QT_INCLUDE_DIR}
    ${QT_QTCORE_INCLUDE_DIR}
    ${CPPUNIT_INCLUDE_DIR}
)













macro(_PELICAN_PIC)
if(printed_macro_printed)
    return()
endif(printed_macro_printed)
set(printed_macro_printed ON)
message("

                            `.---...__
                           /```.__    ``.
                           |```/  `'--.._`.
                           |``|        (o).)
                           \\`` \\    _,-'   `.
                            \\```\\  ( ( ` .   `.
                             `.```. `.` . `    `.
                               `.``\\  `.__   `.  `.
                              ___\\``), )\\ `-.  `.  `.
                    __    _,-'   \\,'  /  \\   `-.  `. `.
                 ,-' '`,-'  '  ''| '   ' |      `-. `. `.
              ,-''_,-' '  ' '  ' |   '  '|         `-. `.`.
           ,-'_,-'   '   '  ''   | '  '  |            `-.`.`.
        ,-',-'  ''  ,'   |   |   |'   ' /                `-..`.
      ,' ,'  ' '     |  ,' | ,' /    ' '|                   `-.)
     // /  '   |    ,'    ,'   /   '  '/
     | || ,'  ,' |    ,' |   ,'   '   '|
     ||||   |   ,' ,'   ,' ,' ' '     /
     |  | |,'  '     |'  ,'  '   '  '/
     | ||,'   ,' |  ,' ,' '    \\   '/
     ||||  |  , ,'  ,-'  /  ' '| ','
     | /  ,' '   ,-' '   |'    |,'
     | | ' ,' ,-' '  ' ' |    '|
     |||,' ,-'  '  '   '_|'  '/
     |,|,-' /'___,..--''  \\ ' |
     / // ,'-' |' |        \\  |
    ///,-'      \\ |         \\'|
   '--'          \\'\\        | |
               __ ) \\___  __| |_
 ____,...----''   ||`-- <_.--._ -`--. __
                  ''            `-`     `'''''''-----......_____
")
_PELICAN()
endmacro(_PELICAN_PIC)



macro(_PELICAN)
message(""
"    _ __  ___  _   ____     ___    _    __   _           \n"
"   | '_ \\/  _|| |  \\_ _\\   / __\\  /  \\  | \\ | |    \n"
"   | |_) | |_ | |   | |   / /    / /_\\\\ |  \\| |       \n"
"   | .__/| |_ | |_ _| |_ / /___ /  __  \\| |\\  |        \n"
"   |_|   \\___||___||_ _/ |____/ \\_/  \\_/|_| \\_|      \n"
" _________________________________________________       \n"
" -------------------------------------------------       \n"
)
endmacro(_PELICAN)
