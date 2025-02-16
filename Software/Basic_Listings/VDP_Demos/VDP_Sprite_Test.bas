10 REM Setup Sprites
20 FOR I = 0 TO 47
30 A$=CHR$(27)+"[81;"+RIGHT$(STR$(I),2)+";"
32 A$=A$+STR$(INT(20+RND(5)*300))+";"
34 A$=A$+STR$(INT(20+RND(5)*200))+"m"
36 PRINT A$;
40 A$=CHR$(27)+"[82;"+RIGHT$(STR$(I),2)+";"
42 A$=A$+STR$(INT(2-RND(5)*5))+";"
44 A$=A$+STR$(INT(2-RND(5)*5))+"m"
46 PRINT A$;
100 NEXT I
110 FOR I = 0 TO 47
120 A$=CHR$(27)+"[80;"+RIGHT$(STR$(I),2)+";0m"
125 PRINT A$;
130 NEXT I
140 FOR I = 0 TO 47
150 A$=CHR$(27)+"[80;"+RIGHT$(STR$(I),2)+";1m"
155 PRINT A$;
160 NEXT I
170 GOTO 110
