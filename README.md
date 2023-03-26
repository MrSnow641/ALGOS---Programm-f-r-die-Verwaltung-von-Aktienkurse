# Uebung1_Hashtabellen #

Implementieren Sie ein Programm zur Verwaltung von Aktienkursen.
Um schnell nach einer bestimmten Aktie suchen zu können, sollen die Aktien in einer
Hashtabelle verwaltet werden. Pro Aktie sollen Name, Wertpapierkennnummer (WKN
als String) und Kürzel (auch ein beliebiger String) gespeichert werden. Weiters sind
pro Aktie die Kursdaten der vergangenen 30 Tage zu verwalten mit folgenden
Informationen:
(Date,Open,High,Low,Close,Volume,Adj Close)
Aktienkursdaten dieser Form können z.B. von
http://de.finance.yahoo.com/q/hp?s=<KÜRZEL>
z.B. für die Microsoft Aktie (Kürzel MSFT)
http://de.finance.yahoo.com/q/hp?s=MSFT
auch als csv Datei heruntergeladen werden.
Das Programm wird über folgende Menüpunkte gesteuert:
1. ADD: Eine Aktie mit Namen, WKN und Kürzel wird hinzugefügt.
2. DEL: Aktie wird gelöscht.
3. IMPORT: Kurswerte für eine Aktie werden aus einer csv Datei importiert
4. SEARCH: Eine Aktie wird in der Hashtabelle gesucht (Eingabe von Namen
oder Kürzel) und der aktuellste Kurseintrag
(Date,Open,High,Low,Close,Volume,Adj Close) wird ausgegeben.
5. PLOT: Die Schlusskurse der letzten 30 Tage einer Aktie werden als ASCII
Grafik ausgegeben, Format ist frei wählbar.
6. SAVE <filename>: Programm speichert die Hashtabelle in eine Datei ab
7. LOAD <filename>: Programm lädt die Hashtabelle aus einer Datei
8. QUIT: Programm wird beendet

## Hinweise Datenstruktur ##

Überlegen Sie sich eine geeignete Hashfunktion, die aus dem Namen der Aktie bzw.
dem Kürzel einen Hashwert generiert (Kriterien für gute Hashfunktionen siehe
Vorlesung).

Implementieren Sie als Kollisionsbehandlung für die Hashtabelle quadratische
Sondierung. Überlegen Sie sich außerdem ein effizientes Verfahren für das Löschen
aus der Hashtabelle!

Überlegen Sie sich weiters eine geeignete Größe für die Hashtabelle unter der
Annahme, dass maximal 1000 Aktien verwaltet werden sollen.
Definieren Sie auch eine geeignete Datenstruktur für die Speicherung der 30
Kursdaten pro Aktie, um diese effizient anzeigen zu können.

## Hinweise Fileformat ##

Für die Kursdaten ist das csv Fileformat von finanze.yahoo.com zu verwenden,
Beispiel siehe Moodle Kurs.
Überlegen Sie sich ein geeignetes Dateiformat, um die Daten der Hashtabelle
abzuspeichern. Recherchieren Sie im Internet den Begriff Serialisierung in diesem
Zusammenhang.
