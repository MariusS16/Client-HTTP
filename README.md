Client HTTP pentru interacțiunea cu un REST API

Acesta tema implementeaza un client HTTP în C care interactioneaza cu un REST API. Clientul nostru funcționează ca o interfață în linia de comandă (CLI) și permite utilizatorilor să trimită comenzi către server și să primească răspunsuri în format JSON.
Aceasta tema are la baza Laboratorul 9 de la Protocoale de Comunitatii, peste care am implementat totul.

Clientul acceptă următoarele comenzi:

- `register`: Înregistrarea unui utilizator nou
- `login`: Autentificarea unui utilizator existent
- `enter_library`: Accesarea bibliotecii virtuale
- `get_book`: Obținerea detaliilor unei cărți specificate prin ID
- `add_book`: Adăugarea unei cărți noi în bibliotecă
- `delete_book`: Ștergerea unei cărți specificate
- `logout`: Deconectarea utilizatorului curent

Fiecare comanda specificata mai sus este implementata in fisierul Commands, iar Client.c: se ocupa de detectarea comenzilor date de utilizator si de apelarea celei care trebuie.

Proiectul este structurat în mai multe fișiere pentru a facilita modularizarea codului:

- client.c: Logica principală a clientului CLI, inclusiv citirea comenzilor de la tastatură și gestionarea sesiunii
- commands.c și commands.h: Implementarea comenzilor disponibile în CLI și trimiterea cererilor HTTP corespunzătoare
- helpers.c și helpers.h: Funcții auxiliare pentru diverse operațiuni, precum trimiterea cererilor HTTP către server
- parson.c și parson.h: Biblioteca `parson` pentru manipularea obiectelor JSON
- requests.c și requests.h: Construirea cererilor HTTP către server

Libraria utilizata pentu a parsa JSON este parson, care este inclusa.
