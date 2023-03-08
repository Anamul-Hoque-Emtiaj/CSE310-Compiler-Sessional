echo 'Started'
yacc -d -Wcounterexamples -Wother -Wconflicts-sr 1905113.y
echo 'Generated the parser C file as well the header file'
flex 1905113.l
echo 'Generated the scanner C file'
g++ -w -g lex.yy.c y.tab.c -o out
echo 'Linked lex.yy.c and y.tab.c files, now running'
./out input.c
echo 'ended'