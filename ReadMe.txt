========================================================================
    CONSOLE APPLICATION : morse Project Overview
=======================================================================

This application was implemented to solve the problem(s) presented in the document 'Hidden Message - Handout.docx'.
This programming assignment was in regards to the Aethna interview.

To build:

	mac: clang++ -std=c++0x -stdlib=libc++ morse.cpp -o morse
	win: cl morse.cpp /EHsc /O2 /Oi
	win (alternative): Load and build from solution ( morse.sln ) NOTE: debug version is built without optimizations, so 
	                   it will not meet the time restrictions of the problem.

To run:
    //    " H   E   L   L   O      W   O   R   L    D " " H   E  L    P"
	morse "**** * *-** *-** ---   *-- --- *-* *-** -**" "**** * *-** *--*"       ( problem 1)

	//    " H   E   L   L   O      W   O   R   L    D " " H   E  L    P"
	morse "-_**** *   *** - *- *-*   *-- *- *-* ***   *** *- --* *-" "**** * *-** *--*"       ( problem 2)

	//         " The Star Wars Saga "                                      " Yoda"          "Leia"
	morse "- **** *   *** - *- *-*   *-- *- *-* ***   *** *- --* *-" "-*-- --- -** *-" "*-** * ** *-"
