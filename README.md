Per compilare tutte le tappe dalla root del progetto:

cmake -B build -D CMAKE_BUILD_TYPE=Release
cmake --build build


Per lanciare gli eseguibili dalla root del progetto:

./build/TappaXX

Sostituire XX con il numero della tappa da lanciare (ad esempio, ./build/Tappa01).


Comandi dell'interfaccia utente:

Tappa01:
nessun comando

Tappa02-04:
Tasti direzionali:  per muoversi

Tappa05-06:
Tasti direzionali:  per muoversi
H:                  per attivare/disattivare le hitbox

Tappa07:
Tasti direzionali:  per muoversi
H:                  per attivare/disattivare le hitbox
X:                  per attaccare

Tappa08:
Tasti direzionali:  per muoversi
H:                  per attivare/disattivare le hitbox
X:                  per attaccare
Enter:              nella schermata di Game Over, per ricominciare da capo

Tappa09:
Tasti direzionali:  per muoversi
H:                  per attivare/disattivare le hitbox
X:                  per attaccare
Z:                  per interagire con i forzieri e per mandare avanti i dialoghi
Enter:              nella schermata di Game Over o di Vittoria, per ricominciare da capo

Tappa10-11:
Su/giù:             nel menù, per andare su e giù
Tasti direzionali:  in partita, per muoversi
H:                  in partita, per attivare/disattivare le hitbox
X:                  in partita, per attaccare
Z:                  in partita, per interagire con i forzieri e per mandare avanti i dialoghi,
                    nel menù, per selezionare un'opzione
Enter:              nella schermata di Game Over o di Vittoria, per ricominciare la partita da capo
Escape:             nella schermata di Game Over o di Vittoria, per tornare al menù