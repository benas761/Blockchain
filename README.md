# Hash-function

Maišos funkcijos paaiškinimas:

1. Perskaito 32 simbolius
	1.1. Jeigu gale nesusidaro 32 simboliai, pridedama tiek 0, kiek reikia norint pasiekti 32 simbolius.
2. Šie simboliai yra perskaitomi 8 kartus, kur 1-as skaitymas pradeda nuo pirmo simbolio, 2 nuo antro ir t.t.
	2.1. Pasiekus galą vėl pradedama skaityti nuo pradžių, kol visi simboliai yra perskaitomi.
3. Kiekvienas perskaitymas yra paverčiamas į skaičių, kuris:
	3.1. Padauginamas iš 97, 71, 67, 37, 19, 31, 59 ir 23 (Pirmas perskaitymas iš 97, antras iš 71 ir t.t.)
	3.2. Keliamas kvadratu
	3.3. Iš kvadrato išimami viduriniai 13 skaičių.
4. Perskaitomi tolimesni 32 simboliai ir einama į pirmą žingsnį kol baigiasi failas.
5. Gauti skaičiai iš eilės rašomi į failą hash.txt ir rašymo metu verčiami į šešioliktainę sistemą, apsiribojant 8 simboliais.

Analizė:

1. Įvairių įvesčių testavimas:
	1.1. Įvestis: "a". Išvestis: "afc0e2358d4248c30bce5bafc07d405da99a38a260fa262ea7cde7e16e115f47".
	Įvestis: "b". Išvestis: "56d3ce84a1f35158d37847075544632ccbb89de4835551c813623b613bb2f237".
	1.2. 1000 atsitiktinių simbolių failo nr. 1 išvestis: "2bfa20bad29f340f83e89d736207bfe5e587a948d640c2be1b9405c7aeb031a1".
	1000 atsitiktinių simbolių failo nr. 2 išvestis: "23bf7664940beb19a630aa709edec15f43ae5437b69a1481b2d454bca96140c5".
	1.3. 1000 atsitiktinių simbolių failo nr. 3, kuris skiriasi nuo failo nr. 2 vienu simboliu, išvestis: "8f6ee12d3f23a7d346309c685e510a7f83b97f4d6bc6f4c2ec8470c5359cd7e7".
	1.4. Tuščio failo išvestis: "6679ecdd5821cc46d9a4ad1cc231bc386845f94c64da6de39fe7449a6e909435".
2. Išvestys visos buvo 64 simbolių, nesikartojo ir įvedus tą pačią įvesti antrą kartą išliko tos pačios.
3. Konstitucija.txt buvo eilutėmis "suhashinta" per 0.032099s.
4. Sugeneruotos poros kaip buvo nurodyta.
5. Testavimui parašyta programa pairTest.cpp. Aptikta 0 vienodų porų.