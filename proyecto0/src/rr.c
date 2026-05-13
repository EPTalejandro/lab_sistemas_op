/*
 * rr.c  —  Implementación del Round Robin Scheduler
 *
 * CI3825 Sistemas de Operación I · Proyecto 0
 * Integrante 1: Carlos Bisogno Carnet: 2210041
 * Integrante 2: Alejandro Villamizar Carnet: 2210439
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rr.h"

void rr_init(ColaRR *cola) {
    cola -> ultimo = NULL;
    cola -> size = 0;
}

void rr_encolar(ColaRR *cola, int pid, const char *nombre, int rafagas) {
    Nodo *nuevo = malloc(sizeof(Nodo));
    if (nuevo == NULL) {
        printf("Error: malloc falló\n");
        exit(1); 
    }
    nuevo -> pid =pid;
    nuevo -> rafagas = rafagas;   
    strncpy(nuevo->nombre, nombre, 31);
    nuevo->nombre[31] = '\0';
    if(cola -> ultimo == NULL){
        cola -> ultimo = nuevo;
        nuevo -> siguiente = nuevo;
    }
    else{
        nuevo -> siguiente = cola -> ultimo ->siguiente;
        cola -> ultimo -> siguiente = nuevo;
        cola -> ultimo = nuevo;
    }
    cola -> size++;
}

int rr_tick(ColaRR *cola, int tick) {
    if(cola -> ultimo == NULL){
        printf("Cola vacía.\n");
        return 0;
    }
    Nodo *actual = cola->ultimo->siguiente;
    printf("Tick %d: ejecutando %s (PID %d) — rafagas restantes: %d\n", tick,actual->nombre,actual->pid,actual->rafagas-1);
    actual -> rafagas--;
    if (actual->rafagas==0){
        printf("  -> %s (PID %d) terminó.\n",actual->nombre,actual->pid);
        if (actual == cola->ultimo){
            free(actual);
            cola->ultimo = NULL;
            cola->size = 0;
            return 0;
        }
        cola->ultimo->siguiente = actual->siguiente;
        free(actual);
        cola ->size--;
    }
    else{
        cola->ultimo = cola ->ultimo ->siguiente;
    }
    return 1;
}

void rr_liberar(ColaRR *cola) {
    if(cola -> ultimo == NULL) return;

    Nodo *actual = cola ->ultimo->siguiente;
    cola -> ultimo -> siguiente = NULL;
    while (actual != NULL){
        Nodo *temporal = actual -> siguiente;
        free(actual);
        actual = temporal;
    }
    cola -> ultimo = NULL;
    cola -> size = 0;
    
}

void rr_imprimir(const ColaRR *cola) {
    if(cola -> ultimo == NULL){
        printf("  (cola vacía)\n");
    }
    Nodo *actual = cola -> ultimo -> siguiente;
    while (actual != cola -> ultimo){
        printf("  PID %d [%s] rafagas=%d\n",actual->pid,actual->nombre,actual->rafagas);
        actual = actual ->siguiente;
    }
    printf("PID %d %s %d\n",actual->pid,actual->nombre,actual->rafagas);   
}
