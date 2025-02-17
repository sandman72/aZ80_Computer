# aZ80 Hardware

<img width="640px" src="../Docs/aZ80_System.jpg" alt="Das aZ80 System" />

#### Wie funktioniert der aZ80 Retro Computer?

Wenn man sich die Teile dieses Computers, wie Bus-Platine, Taktgenerator, CPU, RAM, ROM, Serielle Schnittstelle usw., als Teile eines menschlichen Körpers vorstellt, dann wäre z.B. die Bus-Platine das Rückgrat, welche die anderen Komponenten verbindet. An der Busplatine hängt dann die Taktplatine, die den Herzschlag an allen Platinen liefert und diese damit synchronisiert. Die CPU als Hirn hängt genauso am Bus und verarbeitet alle Sinneseindrücke und gibt die Befehle. Die ROM und RAM Platinen hängen dort auch wie unser Gedächtnis mit am Bus. Und die Serielle Schnittstelle arbeitet wie unser Mund und Ohren und nimmt die Verständigung mit der Umwelt auf.

Alle Teile werden benötigt um ein funktionierenden Computer zu bekommen und wenn ein Teil nicht richtig funktioniert, kann sich das aZ80 System im übertragenen Sinne z.B. nichts merken, oder auch nicht richtig sprechen.

#### Aus welchen Komponenten besteht das aZ80 System?

Über die zentrale Verbindungsplatine (aZ80 Bus) werden die Prozessorplatine (aZ80 CPU),
der Festwertspeicher (aZ80 ROM), der Zugriffspeicher (aZ80 RAM), die Taktplatine (aZ80 Clock),
der serielle Controller (aZ80 Serial), die Ein- und Ausgabeplatine (aZ80 DIO) und weitere Platinen,
miteinander verbunden. Der 40 Pin breite Bus des aZ80 enthält die wichtigsten Signale um diese 
Komponenten miteinander zu verbinden. Weitere Infos und Produktionsdaten findet man im jeweiligen
Unterordner.

## Hier weitere Infos zur Hardware:

### Pin-Belegung der aZ80 Busses:

|   Pin   |  Signal  |
|--------:|:---------|
|  1 - 16 | A15-A0 	Adress-Bus | 
|      17 | GND		Masse |
|      18 | VCC +5V	Versorgungsspannung |
|      19 | M1		Steuerungssignal |
|      20 | RESET		Zurücksetzen des Systems |
|      21 | CLOCK		Taktsignal 3,68MHz |
|      22 | INT		Interrupt, Unterbrechungsanforderung |
|      23 | MREQ		Memory Request, Speicheranfrage |
|      24 | WR		Write, Daten schreiben |
|      25 | RD		Read, Daten lesen |
|      26 | IORQ		I/O Request, I/O-Anfrage |
| 27 - 34 | D0-D7		Daten-Bus |
|      35 | TX		Serielle Daten Senden |
|      36 | RX		Serielle Daten Empfangen |
| 37 - 40 | USR1-USR4	Benutzerdefinierte Signale |  


### Speicher und I/O Adressen

#### ROM:  $0000 - $3FFF
|  Addresse  | Verwendung |
| ----------:|:-----------|
| $0000 | RST Vectors und System Funktionen |
| $0070 | BIOS |
| $0200 | BASIC |
| $2000 | MONITOR |
|   |   |
| $4000 - $7FFF	| Expansion ROM or VideoRAM 16K (preliminary) |

#### RAM:  $8000 - $FFFF
|  Addresse  | Verwendung |
| ----------:|:-----------|
| $8000 - $80FF | System Variables (Zero Page) |
|   $8000 | Serial Buffer  size $3F |
|   $8040 | Serial In Pointer  size 2 |
|   $8042 | Serial Rd Pointer  size 2 |
|   $8044 | Serial Buffer Used	size 2 |
|   $8046 | Basic Started?  size 1 |
| Init: | | 
|   $80ED | Temp Stack |			
| Basic: | |
|   $8047 | Workspace Start (see Basic) |
|   $81A3 | Basic Memory Start |
| Monitor: | |
|   $8047 | Workspace Start (see Monitor) |
| | |
| $8100 - $FFFF	| Stack + User Memory |


#### I/O Map
|  I/O Addresse  | Verwendung |
| -------------:|:-----------|
| $00-$0F	| Digital I/O 00000000 - 0000xx11  (preliminary) |
| $10-$1F	| IDE/CF  I/O 0001xx00 - 0001xx11  (preliminary) |
| $80-$81	| Serial  I/O 10xxxxx0 - 10xxxxx1  (addr decoder verbrät viele Addressen) |
| $C0-$CF	| Video   I/O 1100xx00 - 1100xx11  (preliminary) |
| $D0-$DF	| Audio   I/O 1101xx00 - 1101xx11  (preliminary) |

#### RST Vectors
|  Reset Vector | Verwendung |
| -------------:|:-----------|
| RST00		| $0000	Reset Vector |
| RST08		| $0008	TX Char - Sende Zeichen über seriellen Port |
| RST10		| $0010	RX Char - Empfange Zeichen über seriellen Port | 
| RST18		| $0018	Check Char - Überprüfe ob Zeichen eingegangen sind |
| RST20		| $0020	- |
| RST28		| $0028	- |
| RST30		| $0030	- |
| RST38		| $0038	INT Vector - |
