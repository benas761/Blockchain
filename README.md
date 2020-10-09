# Hash-function

Maišos funkcijos paaiškinimas:

1. Perskaito 32 simbolius
	- Jeigu gale nesusidaro 32 simboliai, pridedama tiek 0, kiek reikia norint pasiekti 32 simbolius.
2. Šie simboliai yra perskaitomi 8 kartus, kur 1-as skaitymas pradeda nuo pirmo simbolio, 2 nuo antro ir t.t.
	- Pasiekus galą vėl pradedama skaityti nuo pradžių, kol visi simboliai yra perskaitomi.
3. Kiekvienas perskaitymas yra paverčiamas į skaičių, kuris:
	- Padauginamas iš 97, 71, 67, 37, 19, 31, 59 ir 23 (Pirmas perskaitymas iš 97, antras iš 71 ir t.t.)
	- Keliamas kvadratu
	- Iš kvadrato išimami viduriniai 13 skaičių.
4. Perskaitomi tolimesni 32 simboliai ir einama į pirmą žingsnį kol baigiasi failas.
5. Gauti skaičiai iš eilės rašomi į failą hash.txt ir rašymo metu verčiami į šešioliktainę sistemą, apsiribojant 8 simboliais.

Analizė:

1. Įvairių įvesčių testavimas:
	- Įvestis: "a". Išvestis: "afc0e2358d4248c30bce5bafc07d405da99a38a260fa262ea7cde7e16e115f47".
	Įvestis: "b". Išvestis: "56d3ce84a1f35158d37847075544632ccbb89de4835551c813623b613bb2f237".
	- 1000 atsitiktinių simbolių failo nr. 1 išvestis: "2bfa20bad29f340f83e89d736207bfe5e587a948d640c2be1b9405c7aeb031a1".
	1000 atsitiktinių simbolių failo nr. 2 išvestis: "23bf7664940beb19a630aa709edec15f43ae5437b69a1481b2d454bca96140c5".
	- 1000 atsitiktinių simbolių failo nr. 3, kuris skiriasi nuo failo nr. 2 vienu simboliu, išvestis: "8f6ee12d3f23a7d346309c685e510a7f83b97f4d6bc6f4c2ec8470c5359cd7e7".
	- Tuščio failo išvestis: "6679ecdd5821cc46d9a4ad1cc231bc386845f94c64da6de39fe7449a6e909435".
2. Išvestys visos buvo 64 simbolių, nesikartojo ir įvedus tą pačią įvesti antrą kartą išliko tos pačios.
3. Konstitucija.txt buvo eilutėmis "suhashinta" per 0.032099s.
	- Norint pamatyti skirtum1 tarp SHA-256 ir mano funkcijos buvo sukurtas 1GB teksto failas pilnas atsitiktinių simbolių. Į 7-zip integruota SHA-256 hash funkcija failą apdorojo per ~4.8s, kol mano funkcijai prireikė 232.132s. Ji buvo beveik 50 kartų lėtesnė.
4. Sugeneruotos poros kaip buvo nurodyta.
5. Testavimui parašyta programa pairTest.cpp. Aptikta 0 vienodų porų.
6. Testuota su pairTest.cpp. Skaičiuota kiek vienodų simbolių arba bitų buvo kiekviename žodyje palyginus su kitu:
	- Atsitiktinių porų hex panašumas: vidutinis: 66.999931%, minimalus: 58.007812%, maksimalus: 76.171875%.
	Panašių porų bit panašumas: vidutinis: 66.996513%, minimalus: 57.226562%, maksimalus: 77.148438%.
	- Atsitiktinių porų panašumas: vidutinis: 6.243469%, minimalus: 0.000000%, maksimalus: 21.875000%.
	Panašių porų hex panašumas: vidutinis - 6.24433%, minimalus - 0%, maksimalus - 21.875%.
	Kaip supratau, hex panašumui testuoti reikia lyginti kiekvieną hash'o raidę. Jei ne, tada aš ne taip padariau.
7. Išvados: Kol hash'uojamas failas yra mažas, ši funkcija neblogai realizuoja avelanche efektą ir vengia kolizijos. Tačiau, palyginus su egzistuojančiomis hash funkcijomis ji yra apgailėtina laiko atžvilgiu.
