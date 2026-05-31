/*
 * CI3825 Sistemas de Operación I · Laboratorio · Abr-Jul 2026
 * Ing. Néstor Bueno
 *
 * src/grep.c  —  Lógica del hilo buscador
 *
 * Implementa las tres funciones declaradas en include/grep.h.
 * Lee con cuidado los comentarios antes de escribir código.
 *
 * Restricciones:
 *   - No modifiques grep.h.
 *   - Puedes agregar funciones auxiliares estáticas si lo necesitas.
 *   - Compila con:  make
 *   - Prueba con:   make test
 */

#include "../include/grep.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

/* ================================================================== */
/* grep_init                                                         */
/* ================================================================== */
void grep_init(Resultado *res, int fd) {
    res->total_coincidencias = 0;
    res->pipe_escritura      = fd;
    res->mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
}

/* ================================================================== */
/* grep_buscar                                                       */
/* Función ejecutada por cada hilo.                                  */
/* ================================================================== */
void *grep_buscar(void *arg) {
    ArgsHilo *args = (ArgsHilo *)arg;

    FILE *f = fopen(args->ruta, "r");
    if (f == NULL) {
        fprintf(stderr, "[Hilo %d] Error: no se pudo abrir %s\n",
                args->id_hilo, args->ruta);
        return NULL;
    }

    char linea[MAX_LINEA];
    while (fgets(linea, sizeof(linea), f)) {

        if (strstr(linea, args->palabra) != NULL) {

            linea[strcspn(linea, "\n")] = '\0';

            pthread_mutex_lock(&args->res->mutex);
            args->res->total_coincidencias++;

            char msg[MAX_RUTA + MAX_LINEA + 64];
            int n = snprintf(msg, sizeof(msg),
                             "[Hilo %d] %s: %s\n",
                             args->id_hilo,
                             args->ruta,
                             linea);
            
            write(args->res->pipe_escritura, msg, n);

            pthread_mutex_unlock(&args->res->mutex);
        }
    }

    fclose(f);
    return NULL;
}

/* ================================================================== */
/* grep_destruir                                                     */
/* ================================================================== */
void grep_destruir(Resultado *res) {
    pthread_mutex_destroy(&res->mutex);
}