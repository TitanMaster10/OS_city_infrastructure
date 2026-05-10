AI Usage - Phases 1 & 2

Tool folosit: ChatGPT (GPT-4o)

Phase 1

parse_condition() si match_condition() - cerut o functie care sparge "field:op:value" in 3 parti. Prima varianta cu strtok nu mergea (input const char*, strtok modifica stringul), rescrisa cu while loop. match_condition() primita ca baza, adaugat fprintf pe stderr cand campul nu exista.

Structura proiectului:intrebat cum sa impart fisierele. Luat ce avea sens, main.c cu parsarea argumentelor scrisa separat.

Permisiuni si fisiere binare: intrebat diferenta open/ fopen, ramas pe open() conform cerintei. chmod dupa open necesar pt ca mode-ul se aplica doar la creare.

lseek si ftruncate in remove_report, nu stiam cum se sterge un record din mijlocul unui fisier binar,am primit ideea cu shift records + ftruncate, loop de shift scris dupa ce am inteles logica.

Phase 2

remove_district(): intrebat pattern-ul fork+exec pt rm -rf. Verificarile pe path puse dupa ce am citit in cerinta ca trebuie atentie sa nu stergi alte chestii pe calculator.

monitor_reports.c : stiam ca trebuie sigaction nu signal dar nu stiam cum se completeaza struct-ul. Cerut exemplu, restul facut pe baza lui. write() in handler in loc de printf pt ca printf nu e async-signal-safe.

notify_monitor(): scris singur. Deschide .monitor_pid, citeste pid, kill cu SIGUSR1. Daca crapa returnez -1 si in main se logheaza ca nu a mers.

Makefile: intrebat cum bag monitor_reports ca target separat. Bug enervant cu $ in loc de $< la pattern rule, facut de 2 ori cumva.