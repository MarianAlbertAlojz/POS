
#include "terminal_display.h"
#include "server.h"
#include "client.h"

/*
 * Tu  bude logika od Andreja,teda pohyb hore,dolu,vpravo,vlavo
 * premysliet:
 *           -ze hrac je aj server aj hrac teda ze prvy hrac bude server aj hrac
 *
 * */
/*
 * Tu  bude riesenie vlakien na ktorom budeme robit pravdepodobne obaja
 * premysliet:
 *           -este nevieme :(
 * Ten co je prvy bude aj hrac aj server
 * ostatni budu len hraci
 *
 *  spravanie ked spustam ako server:
 *      server hrac nastavi
 *          svoje meno
 *          pocet maximalnych pohybov
 *          cas jednej hry
 *      client hrac nastavi
 *          svoje meno
 *testtest
 *
 * thready pri server aj klient
 *      prvy: vlakno pre bezanie servru
 *      prvy: dat tu vlakno na timer? premysliet
 *      prvy: vlakno pre bezanie klienta
 *      druhy: jedno vlakno klienta
 *
 * zobrazovanie : kazdy klient si zobrazi svoju tabulku to znamena ze sa budu posielat
 * data o jednotlivych polickach,case,tahoch,score kazdemu clientovi
 * klient posle data po kazdom tahu ( vzdy po tahu klienta posle info servru klient--> server)
 * info o tabulkach sa bude aktualizovat po kazdej sekunde( server--> klienti)
 * */

/*
 * Tu  bude riesenie socketov na ktorom budeme robit pravdepodobne obaja
 * premysliet:
 *           -este nevieme :(
 *
 * */


/*
 * Tu  budu ostatne riesenia na ktorych budeme robit pravdepodobne obaja
 * premysliet:
 *           -este nevieme :(
 *
 * */

int main(int argc, char * argv[]) {
   if (strcmp(argv[1],"server") == 0){
        server();
   }else {
        client();
   }
    return 0;
}

