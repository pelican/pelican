#
# dependencies.cmake: Sets depencies for PELICAN.
#
# Required dependencies:
#   Qt4
#   Boost (program options)
#
# Optional dependencies:
#   CppUnit
#

# === Find Dependencies.
find_package(Boost COMPONENTS program_options REQUIRED)
find_package(Qt4 COMPONENTS QtCore QtNetwork QtXml REQUIRED)
find_package(CppUnit)

if (NOT CPPUNIT_FOUND)
    message("===================================================================")
    message("= WARNING: CppUnit library not found, unit tests will not be build!")
    message("===================================================================")
endif (NOT CPPUNIT_FOUND)








































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
