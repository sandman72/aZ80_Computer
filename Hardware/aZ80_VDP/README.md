# aZ80 VDP Platine

<img width="640px" src="aZ80_VDP.jpg" alt="Die aZ80 VDP Platine" />

Die Video-Display-Processor VDP Platine ist eine Grafik- und I/O-Karte mit eingebautem VT100+ Terminal Emulator. Über die serielle Schnittstelle kann sie Daten vom aZ80 System
entgegennehmen und anzeigen, kann aber auch über den USB Port Eingaben über eine USB-Tastatur an das aZ80 System weitergeben. 
Auch andere USB-Geräte, wie Maus, Joystick oder Speicherstick können abgefragt werden, wobei die Unterstützung noch work in progress ist.

Der VDP untertützt die normale Anzeige von Text, der über die seriellen Schnittstelle eingeht. Darüber hinaus werden auch ANSI Kontrollzeichen 
unterstützt, welche eine erweiterte Kontrolle der Textattribute, wie Farbe, Blinken, Position usw. erlaubt, und darüber hinaus auch eine Grafikausgaben
und die Kontrolle von Sprites, Maus-Cursor und den Buzzer erlaubt. Weiteres auf der <a href="./ANSI.md">ANSI Seite</a>.

Über die USB-Tastatur kann mit der Tastenkombination WIN+ESC ein Debug-Fenster eingeblendet werden, das den akutellen Zustand des VDP anzeigt.

Der Reset Taster SW1 bietet es an den VDP zurückzusetzen und vereinfacht auch das installieren einer neuen Firmware an einem PC.

Der Umschalter SW2 bietet zwei Betriebsmodi an:
| SW1 | Modus |
|:---:|:------|
| PC  | PC-Betrieb - Die serielle Schnittstelle auf der Bus-Platine hat Vorrang |
| VDP | Standalone-Betrieb - Die USB Schnittstelle am VDP hat Vorrang |

#### Technische Daten

Der VDP bietet eine Grafikauflösung von 320x240 Pixel mit 256 Farben. Die Ausgabe erfolgt hochskaliert als 640x480 Pixel Modus mit 60Hz über eine (not) HDMI Schnittstelle.

 - Arduino Software auf einem Raspberry PI Pico 2 - RP2350
 - 320x240 Pixel Auflösung
 - 40x30 Zeichen Textmodus mit 16 bzw. 256 Farben
 - 256 Farben Palette mit 16bit Farbtiefe, 8 Farben in automatischer Farbrotation
 - 64 32x32 Pixel Sprites mit 256 Farben und Maskenkanal
 - einen Mauscursor
 - einfache Text- und Grafik-Kommandos
 - Double-Buffered Text-, Grafik und Sprite-Aufbau für flickerfreie Anzeige mit 60Hz

#### Update der Firmware

Zur Installation einer neuen Firmware auf den Pico 2 wird dieser mit einem Micro USB Kabel an einen PC angeschloßen.
Dann drückt man erst die Reset Taste SW1, nun den BOOT Taster, hält diesen fest und lässt die Reset Taste SW1 los.
Auf dem PC sollte nun ein weiteres Laufwerk RPI angezeigt werden. Auf dieses kopiert man die aktualisierte UF2 Datei.
