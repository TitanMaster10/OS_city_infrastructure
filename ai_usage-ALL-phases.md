AI Usage - All Phases

Tool folosit: ChatGPT (GPT-4o)

Phase 1

parse_condition() si match_condition() - cerut o functie care sparge "field:op:value" in 3 parti. Prima varianta cu strtok nu mergea (input const char*, strtok modifica stringul), rescrisa cu while loop. match_condition() primita ca baza, adaugat fprintf pe stderr cand campul nu exista.

Structura proiectului - intrebat cum sa impart fisierele. Luat ce avea sens, main.c cu parsarea argumentelor scrisa separat.

Permisiuni si fisiere binare - intrebat diferenta open vs fopen, ramas pe open() conform cerintei. chmod dupa open necesar pt ca mode-ul se aplica doar la creare.

lseek si ftruncate in remove_report - nu stiam cum se sterge un record din mijlocul unui fisier binar. Primit ideea cu shift records + ftruncate, bucla de shift scrisa dupa ce am inteles logica.

Phase 2

remove_district() - intrebat pattern-ul fork+exec pt rm -rf. Verificarile pe path puse dupa ce am citit in cerinta ca trebuie atentie sa nu stergi chestii de pe calculator.

monitor_reports.c - stiam ca trebuie sigaction nu signal dar nu stiam cum se completeaza struct-ul. Cerut exemplu, restul facut pe baza lui. write() in handler in loc de printf pt ca printf nu e async-signal-safe.

notify_monitor() - scrisa singur. Deschide .monitor_pid, citeste pid, kill cu SIGUSR1. Daca crapa returnez -1 si in main se logheaza ca nu a mers.

Makefile - intrebat cum bag monitor_reports ca target separat. Bug enervant cu $ in loc de $< la pattern rule, facut de 2 ori cumva.

Phase 3

scorer.c -  am intrebat cum sa structurez un program care citeste rapoarte si calculeaza scor per inspector. Primit ideea cu un array de structuri si cautare liniara. Apoi am scris open, read Report, printf output.

city_hub.c -  interactive loop am facut cu fgets + strcmp/strncmp pe comenzi. Am intrebat cum sa parsez argumentele de la calculate_scores  - cu strtok pe spatii.

calculate_scores cu pipe+dup2 - am intrebat cum functioneaza pipe si dup2 si exec ca sa redirectez stdout-ul scorer-ului intr-un pipe pe care il citeste parintele. Primit exemplu cu secventa: pipe(), fork(), in copil close+dup2+close+exec, in parinte close+read+waitpid.

monitor_reports modificat - intrebat cum verific daca un PID e activ fara sa trimit semnal real.M-am informat despre kill(). Adaugat check la startup, mesaje formatate cu prefix ERROR;SIGUSR1;SIGINT;INFO.

start_monitor cu hub_mon - cea mai complexa parte. Intrebat de ce trebuie proces intermediar (ca city_hub sa nu se blocheze in read).structura cu city_hub fork hub_mon, hub_mon face pipe+fork+exec monitor, hub_mon citeste pipe linie cu linie. Scris citirea byte cu byte pana la newline dupa ce am inteles de ce read nu garanteaza linii complete.
