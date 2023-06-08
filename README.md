# Stacja-pogodowa-do-auta
Stary projekt (z 2021 r.) prostej stacji pogodowej na bazie Arduino UNO do samochodu.

Wyświetlacz znajduje się nad deską rodzielczą i połączony jest kablem z bazą, która znajduje się pod siedzeniem kierowcy. W bazie znajdują się czujniki:
- temperatury DHT22 oraz zapasowy
- ciśnienia
- wilgotności DHT22

Stacja mierzy również autonomicznie czas za pomocą zegara elektronicznego DS1307 zasilanego baterią 3,3 V.

Wyświetlacz pokazuje stale temperaturą oraz jej zmiany w sposób następujący:
-> oznacza, że temperatura ZWIĘKSZYŁA SIĘ o przynajmniej 0,2 stopnie celsjusza w ciągu 30 minut
~ oznacza, że temperatura się waha nieznacznie lub jest stała
<- oznacza, że temperatura ZMALAŁA o przynajmniej 0,2 stopnie celsjusza w ciągu 30 minut

Ponadto stale wyświetla się aktualny czas.
Do tego co minutę pojawiają się zamiennie następujące informacje (ze względu na ograniczoną ilość znaków na wyświetlaczu):
- Obecny dzień, miesiąc, rok oraz dzień tygodnia
- Ciśnienie oraz wilgotność
