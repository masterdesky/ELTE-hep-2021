Installálás

ROOT környezet betöltése:
source /path_to_install_or_build_dir/bin/thisroot.sh 

Fordítás:
mkdir exe/
mkdir exe/object/
mkdir exe/object/dependencies/
make exe/analyzetree.exe
(van egy m32 kapcsoló a Makefile-ban, ha a rendszeretek és a ROOT-otok is 64 bites, akkor ezt törölni kell)

Futtatáshoz szükséges adatok:
data.root fájl letöltése/átnevezése (ugyanabba a mappába)
https://www.phenix.bnl.gov/phenix/WWW/publish/csanad/analyzetree/data.root

Futtatás:
exe/analyzetree.exe <adatfile neve> <kimenet neve> <vizsgálandó események száma>
pl:
exe/analyzetree.exe data.root analyzetree.root 1000

Ábrázolás
root.exe -b -q Plot_analyzetree.C\(\"analyzetree.root\",\"figs\") 

Eredmény:
ábrák a figs könyvtárban

Esemény-jellemzők:
- Zvertex: az adott ütközés helye a nyalábon belül, azaz az ütközési ponttól +- néhány cm, érdemes megnézni ennek eloszlását
- Centrality: az adott ütközés centralitása, azaz hogy mennyire középen találták el egymást a magok; 0% a teljesen centrális, 100% az egymást éppenhogy súroló eset. A % azt jelenti, hogy az események hány százaléka centrálisabb az adott eseménynél. Érdemes megnézned az eloszlását.
- ReactionPlane: az adott esemény reakciósíkjának iránya, azaz hogy a két mag helyét és sebességvektorát tartalmazó sík dőlésszöge a laborrendszerbeli x-z síkhoz képest. Ennek eloszlását is érdemes megnézned.
- Ntracks: hány track volt az adott eseményben. Ennek eloszlását is érdemes megnézned.

Részecske-jellemzők (ezek közül is mindnek érdemes megnézned az eloszlását):
- px,py,pz,E: négyesimpulzus (ebből számolható pl pt=sqrt(px^2+py^2) vagy m=sqrt(E^2-px^2-py^2-pz^2))
- ch: töltés
- Mch: matching változó, erről később
- isPi: PID változó, erről később
- detp: a részecskeazonosító detektorrétegbe való beérkezés helyének phi koordinátája
- detz: a részecskeazonosító detektorrétegbe való beérkezés helyének z koordinátája

Na és akkor egy kis magyarázat.

A mérésben használt detektorrendszer így néz ki:
https://www.phenix.bnl.gov/WWW/run/drawing/Phenix_2012.jpg
Láthatod, hogy ilyen hagymahéj-szerkezete van. Íme erről egy régi animáció:
http://www.phenix.bnl.gov/WWW/software/luxor/ani/PHENIXRun2/phenixRun2Large.mpg
Amúgy ez az adatfelvétel logikája:
https://www.phenix.bnl.gov/phenix/WWW/intro/dataflow/index.html

Valójában tehát beütéseink vannak a különböző detektorrétegekben, amelyeket egy "tracking" (nyomkövető) algoritmus válogat össze, illeszt rájuk egy nyomvonalat (amely nem pontosan a pontokon megy át, csak közelítőleg, hiszen a pontok helye nem tökéletesen pontosan van megmérve sem), ezt hívják rekonstrukciónak, és ebből adódnak a megfigyelt trackek.

Ezen trackek azonban különböző minőségűek, azaz különböző mértékben vagyunk biztosak benne, hogy a tracking algoritmus jól rakta őket össze. Hogy mennyire jól, azt pl onnan becsülhetjük meg, hogy a track projekció (azaz a részecske képzeletbeli nyomvonala) egy adott detektorrétegben milyen messze van a legközelebbi beütéstől. Ezen távolságok eloszlása kb Gauss alakú lesz, sigma szélességgel. Megtehetjük, hogy minden trackre megadjuk, hogy hány sigmára van tőle a legközelebbi tényleges beütés - na ezt hívjük matching változónak, amely minden detektorrétegre definiálható.

A részecskeazonosítás is valamelyest hasonló. Konkrétan a PHENIX-ben TimeOfFlight módszerrel zajlik a részecskeazonosítás, amelynek lényege, hogy
idő(TOF) = út/sebesség
és akkor a sebesség és az impulzus között a tömeg létesít kapcsolatot, amel relativisztikusan m^2 = p^2 * (1-v^2)/v^2). Ez alapján készülhet egy ilyen TOF vs impulzus ábra:
https://www.phenix.bnl.gov/WWW/run/drawing/tofpid_0800_02.gif
amelyből akkor a tömeg vs impulzus így néz ki:
https://www.phenix.bnl.gov/WWW/run/drawing/mass2tof.gif
Na ezen szépen látszik a pi,K,p részecskék helye, illetve hogy Gauss-jellegű eloszlása van a tömegeknek az elméleti érték körül. Ezt is beparaméterezhetjük, és megadhatjuk a sigma(p) impulzusfüggő szélességet. Egy adott részecskére akkor megmondhatjuk, hogy hány sigmára van a nominális tömegtől. Ez az isPi változó (amely itt jelentősen kerekítve van, egész vagy félegész értékekre). 
