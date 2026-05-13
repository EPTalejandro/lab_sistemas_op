/*
 * rr.h  —  Round Robin Scheduler · CI3825 Sistemas de Operación I
 *
 * Este archivo está completo. NO lo modifiques.
 *
 * Define la estructura del nodo de la cola circular y los prototipos
 * de las funciones que debes implementar en src/rr.c
 */

#ifndef RR_H
#define RR_H

/* ── Estructura de un proceso en la cola ──────────────────────────────── */
typedef struct Nodo {
    int           pid;       /* identificador del proceso   */
    char          nombre[32];/* nombre del proceso          */
    int           rafagas;   /* ráfagas restantes de CPU    */
    struct Nodo  *siguiente; /* puntero al siguiente nodo   */
} Nodo;

/*
 * Cola circular simplemente enlazada.
 * 'ultimo' apunta al último nodo insertado.
 * El primer nodo listo para ejecutar es siempre ultimo->siguiente.
 * Si la cola está vacía, ultimo == NULL.
 */
typedef struct {
    Nodo *ultimo;  /* puntero al último nodo de la cola circular */
    int   size;    /* cantidad de procesos en la cola            */
} ColaRR;

/* ── Prototipos que debes implementar en src/rr.c ─────────────────────── */

/* Inicializa la cola vacía */
void rr_init(ColaRR *cola);

/*
 * Inserta un nuevo proceso al final de la cola circular.
 * Reserva memoria con malloc. Verifica que malloc no devuelva NULL.
 * Si devuelve NULL, imprime "Error: malloc falló\n" y termina con exit(1).
 */
void rr_encolar(ColaRR *cola, int pid, const char *nombre, int rafagas);

/*
 * Simula un tick de CPU para el proceso al frente de la cola.
 *
 * - Imprime: "Tick N: ejecutando [nombre] (PID pid) — rafagas restantes: R\n"
 *   donde N es el número de tick (empieza en 1), R es rafagas DESPUÉS del tick.
 * - Descuenta 1 ráfaga al proceso al frente.
 * - Si el proceso llega a 0 ráfagas, lo retira de la cola con free() e imprime:
 *   "  -> [nombre] (PID pid) terminó.\n"
 * - Si la cola está vacía al llamar esta función, imprime:
 *   "Cola vacía.\n"
 *   y no hace nada más.
 *
 * Devuelve 1 si queda al menos un proceso en la cola después del tick.
 * Devuelve 0 si la cola quedó vacía.
 */
int rr_tick(ColaRR *cola, int tick);

/*
 * Libera todos los nodos restantes de la cola.
 * Después de llamar esta función, cola->ultimo debe quedar en NULL
 * y cola->size en 0.
 */
void rr_liberar(ColaRR *cola);

/*
 * Imprime el estado actual de la cola, en orden desde el primero
 * que ejecutará hasta el último.
 * Formato por línea: "  PID pid [nombre] rafagas=R\n"
 * Si está vacía: "  (cola vacía)\n"
 */
void rr_imprimir(const ColaRR *cola);

#endif /* RR_H */
