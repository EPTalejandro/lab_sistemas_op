#!/bin/bash
# ─────────────────────────────────────────────────────────────────────────────
# evaluar.sh  —  Corrector automático · Proyecto 0 · CI3825
# ─────────────────────────────────────────────────────────────────────────────
# Uso:  bash tests/evaluar.sh          (o simplemente: make test)
#
# Ejecuta los 5 casos de prueba y compara la salida del programa contra
# la salida esperada. Imprime un resumen y la nota sobre 4 puntos.
#
# Puntuación:  0.8 pts por caso correcto  (5 casos × 0.8 = 4 pts)
# ─────────────────────────────────────────────────────────────────────────────

BINARIO="./rr_sim"
TESTS_DIR="tests"
TOTAL=5
CORRECTOS=0

# Colores (se desactivan si la terminal no los soporta)
if [ -t 1 ]; then
    VERDE="\033[0;32m"
    ROJO="\033[0;31m"
    AMARILLO="\033[0;33m"
    NEGRITA="\033[1m"
    RESET="\033[0m"
else
    VERDE="" ROJO="" AMARILLO="" NEGRITA="" RESET=""
fi

echo ""
echo -e "${NEGRITA}╔══════════════════════════════════════════════════╗${RESET}"
echo -e "${NEGRITA}║   CI3825 · Proyecto 0 · Corrector automático     ║${RESET}"
echo -e "${NEGRITA}╚══════════════════════════════════════════════════╝${RESET}"
echo ""

# Verificar que el binario existe
if [ ! -f "$BINARIO" ]; then
    echo -e "${ROJO}Error: no se encontró '$BINARIO'.${RESET}"
    echo "       Ejecuta 'make' primero."
    exit 1
fi

# Ejecutar cada caso
for i in $(seq -f "%02g" 1 $TOTAL); do
    ENTRADA="$TESTS_DIR/caso${i}.txt"
    ESPERADA="$TESTS_DIR/caso${i}.expected"
    OBTENIDA=$(mktemp)

    # Verificar que los archivos existen
    if [ ! -f "$ENTRADA" ] || [ ! -f "$ESPERADA" ]; then
        echo -e "  Caso $i: ${AMARILLO}SKIP${RESET} (archivos de prueba no encontrados)"
        rm -f "$OBTENIDA"
        continue
    fi

    # Ejecutar con timeout de 5 segundos
    timeout 5s "$BINARIO" < "$ENTRADA" > "$OBTENIDA" 2>/dev/null
    EXIT_CODE=$?

    if [ $EXIT_CODE -eq 124 ]; then
        echo -e "  Caso $i: ${ROJO}TIMEOUT${RESET} — el programa tardó más de 5 s (¿bucle infinito?)"
        rm -f "$OBTENIDA"
        continue
    fi

    # Comparar salidas (diff silencioso)
    if diff -q "$ESPERADA" "$OBTENIDA" > /dev/null 2>&1; then
        echo -e "  Caso $i: ${VERDE}✓ CORRECTO${RESET}"
        CORRECTOS=$((CORRECTOS + 1))
    else
        echo -e "  Caso $i: ${ROJO}✗ INCORRECTO${RESET}"
        echo -e "    ${AMARILLO}── Diferencias (esperado vs obtenido) ──${RESET}"
        diff --label "esperado" --label "obtenido" \
             -u "$ESPERADA" "$OBTENIDA" \
             | tail -n +3 \
             | sed 's/^/    /'
    fi

    rm -f "$OBTENIDA"
done

# ── Resumen ────────────────────────────────────────────────────────────────
echo ""
echo -e "${NEGRITA}──────────────────────────────────────────────────${RESET}"

NOTA=$(echo "scale=1; $CORRECTOS * 0.8" | bc)

if [ "$CORRECTOS" -eq "$TOTAL" ]; then
    COLOR=$VERDE
elif [ "$CORRECTOS" -ge 3 ]; then
    COLOR=$AMARILLO
else
    COLOR=$ROJO
fi

echo -e "  Casos correctos : ${COLOR}${NEGRITA}${CORRECTOS} / ${TOTAL}${RESET}"
echo -e "  Nota automática : ${COLOR}${NEGRITA}${NOTA} / 4.0 pts${RESET}"
echo -e "  Informe técnico :       / 1.0 pt  (evaluado por el profesor)"
echo ""
echo -e "  ${AMARILLO}Recuerda incluir la Declaración Jurada de Autoría.${RESET}"
echo ""
