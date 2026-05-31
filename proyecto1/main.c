/*
 * CI3825 Sistemas de Operación I · Laboratorio · Abr-Jul 2026
 * Ing. Néstor Bueno
 *
 * main.c  —  Orquestador del mini-grep multihilo
 *
 * DEBES COMPLETAR LOS BLOQUES TODO DE ESTE ARCHIVO.
 * También debes completar src/grep.c.
 *
 * Uso esperado al final:
 *   ./grep <palabra> <archivo1> [archivo2 ... archivoN]
 *
 * Flujo que debes implementar:
 *   1. Validar argumentos.
 *   2. Crear el pipe.
 *   3. Hacer fork() para crear el proceso monitor (hijo).
 *   4. El hijo lee del pipe e imprime hasta EOF, luego termina.
 *   5. El padre inicializa Resultado, lanza N hilos (uno por archivo),
 *      espera a que terminen, cierra el pipe y espera al hijo.
 *   6. Imprimir el total de coincidencias y liberar recursos.
 */

#include "include/grep.h"

/* ================================================================== */
/*  ejecutar_monitor                                                    */
/*  Llamada SOLO por el proceso hijo.                                  */
/*  Lee mensajes del pipe y los imprime en stdout hasta recibir EOF.   */
/*                                                                      */
/*  Parámetro: fd_lectura — extremo de lectura del pipe.               */
/* ================================================================== */
static void ejecutar_monitor(int fd_lectura) {

    /* TODO-M (Monitor)
     * ----------------
     * En un bucle, lee bloques del pipe con read() y escríbelos
     * directamente a STDOUT_FILENO con write().
     * El bucle termina cuando read() retorna 0 (EOF) o negativo (error).
     * Al salir del bucle, cierra fd_lectura.
     *
     * Pista:
     *   char buf[MAX_RUTA + MAX_LINEA + 64];
     *   ssize_t n;
     *   while ((n = read(fd_lectura, buf, sizeof(buf))) > 0)
     *       write(STDOUT_FILENO, buf, n);
     *
     * ~5 líneas de código.
     */

}

/* ================================================================== */
/*  main                                                                */
/* ================================================================== */
int main(int argc, char *argv[]) {

    if (argc < 3){
        printf("Uso: ./grep <palabra> <archivo1> [archivo2 ...]\n");
        return 1;
    }
    const char *palabra = argv[1];
    int         n_arch  = argc - 2;
    if (n_arch > MAX_HILOS){
        printf("error\n");
        return 1;
    }

    // le puse mario al pipe por que mario es un fontanero xd 
    int mario[2];
    if (pipe(mario) == -1){
        perror("pipe");
        return 1;
    }

    pid_t id;
    id = fork();
    if(id == -1){
        perror("fork");
        return -1;
    }
    else if (id == 0){
        close(mario[1]);
        ejecutar_monitor(mario[0]);
        exit(0);
    }
    else{
        close(mario[0]);
    }
    
    Resultado res;
    grep_init(&res,mario[1]);
    pthread_t hilos[MAX_HILOS];
    ArgsHilo  args[MAX_HILOS];
    for(int i=0;i<n_arch;i++){
        args[i].id_hilo = i;
        strncpy(args[i].ruta,argv[i+2],MAX_RUTA-1);
        strncpy(args[i].palabra,palabra,MAX_LINEA-1);
        args[i].res= &res;
        if(pthread_create(&hilos[i], NULL, grep_buscar, &args[i])!=0){
            perror("pthread_create");
        }
    }

    for(int i=0;i<n_arch;i++){
        pthread_join(hilos[i],NULL);
    }

    close(mario[1]);
    wait(NULL);

    printf("---\nTotal de coincidencias: %d\n", res.total_coincidencias);
    grep_destruir(&res);
    return(0);

}
