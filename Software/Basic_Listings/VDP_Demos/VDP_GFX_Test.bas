10 REM Simple Graphics Tests for aZ80 VDP
15 REM
20 REM Reset GFX list
30 PRINT CHR$(27);"[99g";
40 REM Move cursor to absolute position
50 PRINT CHR$(27);"[1;100;100g";
60 REM Draw Rectangle
70 PRINT CHR$(27);"[10;25;25;123g";
80 REM Draw Rectangle filled
85 PRINT CHR$(27);"[1;150;100g";
90 PRINT CHR$(27);"[11;25;25;89g";
95 REM Move cursor to absolute position
96 PRINT CHR$(27);"[1;40;40g";
100 REM Draw Circle
105 PRINT CHR$(27);"[1;200;100g";
110 PRINT CHR$(27);"[20;25;102g";
120 REM Draw Circle filled
125 PRINT CHR$(27);"[1;250;100g";
130 PRINT CHR$(27);"[21;40;123g";

200 REM--- Regenbogen Linien ---
210 CLS
215 PRINT CHR$(27);"[99g"
220 FOR L=0 TO 239
230 :  PRINT CHR$(27);"[1;0;";L;"g";
240 :  PRINT CHR$(27);"[6;319;";L;";";L;"g";
250 NEXT L

800 REM--- LISSAJOUS-FIGUREN ---
805 CLS
810 X0 = 110: Y0 = 95: A = 80: B = 90
815 D = 77: N = 3: M = 2: T0 = 3.141592/180
820 PRINT CHR$(27);"[99g"
825 PRINT CHR$(27);"[1;";X0;";";Y0;"g";
830 FOR W=0 TO 180*D STEP D
835 :  T = W * T0
840 :  X = X0 + A*COS(N*T)
845 :  Y = Y0 + B*SIN(M*T)
850 :  IF T<>0 THEN PRINT CHR$(27);"[1;";INT(X1);";";INT(Y1);"g";
860 :  IF T<>0 THEN PRINT CHR$(27);"[7;";INT(X);";";INT(Y);";89g";
870 :  X1=X : Y1=Y
880 NEXT W
890 GOTO 800



