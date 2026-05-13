/*
 * main.c  —  Driver del Round Robin Scheduler
 *
 * Este archivo está completo. NO lo modifiques.
 *
 * Lee comandos de la entrada estándar (stdin) y ejecuta la simulación.
 *
 * Comandos reconocidos:
 *   ENCOLAR pid nombre rafagas   — agrega un proceso a la cola
 *   TICK                         — ejecuta un tick de CPU
 *   IMPRIMIR                     — muestra el estado de la cola
 *   FIN                          — termina la simulación y libera memoria
 *
 * El número de tick se lleva globalmente y se incrementa con cada TICK.
 */

#include <stdio.h>
#include <string.h>
#include "rr.h"

int main(void) {
    ColaRR cola;
    rr_init(&cola);

    char  cmd[16];
    int   tick_global = 1;

    while (scanf("%15s", cmd) == 1) {

        if (strcmp(cmd, "ENCOLAR") == 0) {
            int  pid, rafagas;
            char nombre[32];
            scanf("%d %31s %d", &pid, nombre, &rafagas);
            rr_encolar(&cola, pid, nombre, rafagas);

        } else if (strcmp(cmd, "TICK") == 0) {
            rr_tick(&cola, tick_global);
            tick_global++;

        } else if (strcmp(cmd, "IMPRIMIR") == 0) {
            rr_imprimir(&cola);

        } else if (strcmp(cmd, "FIN") == 0) {
            rr_liberar(&cola);
            break;
        }
    }

    return 0;
}
