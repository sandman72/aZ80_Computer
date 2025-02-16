# aZ80_Computer

<img width="640px" src="./Docs/aZ80_System.jpg" alt="Das aZ80 System" />

#### Was ist der aZ80 Retro Computer?

Die Idee war: Wir bauen einen kleinen bezahlbaren Retro Computer für Workshops im Attraktor e.V.

 - Mit Steckkarten, damit man ihn erweitern kann. 
 - Beim Aufbau jeder Karte gibt es einen Einblick, was diese im Detail macht und wie sie funktioniert
 - Eine Einführung zur Computertechnik der 70er Jahre und die Gedankengänge und Evolutionen, die es zu der Zeit gab.
 - Wir programmieren auf dem aZ80 mit MS-BASIC und Assembler, z.B. kleine Spiele und Programme.

Unter Docs findet sich eine erste Präsentation über das Projekt.

#### Hardware

<img width="640px" src="./Docs/aZ80_Boardset.jpg" alt="Die aZ80 Boards" />

Die Entscheidung was für ein System ich "neu" entwickeln wollte, fiel auf einen Bauvorschlag von <a href="http://searle.wales/">Grant Searle</a>, 
der auch bei einigen anderen Retro-Computern (RC2014) zum Einsatz kam. Damit gibt es eine gewisse 
Kompatibilität zum RC2014 System und den Steckkarten, aber der aZ80 sollte als Open-Hardware und -Software verfügbar 
sein und nicht hinter einer Paywall liegen, teure Kits oder Zusatz Hardware benötigen, z.B. wie der RC2014.

#### Software

Für den aZ80 wurden eine alte Version von MS Basic (Nascom Basic 4.7) und der Efex Monitor (Efex V4 1.0) mit einem eigenen BIOS kombiniert.
Das BIOS checkt, ob die wichtigsten Karten des Systems funktionieren und initialisiert diese. Danach erscheint über die serielle Schnittstelle
die Auswahlmöglichkeit zwischen dem Basic und dem Monitor Programm.

Nascom Basic Verweis: <a href="https://gitlab.com/feilipu/NASCOM_BASIC"> https://gitlab.com/feilipu/NASCOM_BASIC </a>

Efex V4 1.0 Verweis: <a href="https://mkpeker.wixsite.com/efex"> https://mkpeker.wixsite.com/efex </a>
