AI Usage
Tool: ChatGPT (GPT-4o)

parse_condition()

Am cerut o functie care sparge un string "field:op:value" in 3 parti. Mi-a dat ceva cu strtok dar nu mergea pt ca input-ul e const char* si strtok modifica stringul. Am rescris manual cu while loop. Am adaugat si bounds checking ca lipsea.

match_condition()

Am descris struct-ul Report si am cerut o functie care compara un camp cu o valoare folosind operatorul dat. A iesit ok, foloseste un cmp (-1/0/1) si aplica operatorul la sfarsit. Am adaugat un fprintf pe stderr cand campul nu exista ca altfel nu stii ca ai scris gresit.

cmd_filter()

Scrisa de mine. Citesc din reports.dat, parsez conditiile o singura data inainte de loop, verific fiecare record cu AND pe toate conditiile.