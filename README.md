# Conway's Game of Life 🎮

![Game of Life Screenshot](./screenshot.png)

Implementación del autómata celular clásico de John Conway en **C++** y **Python**.

## ¿Qué es?

Un "juego" basado en 4 reglas simples que generan patrones complejos:
- Una célula viva con 2-3 vecinos sobrevive
- Una célula muerta con exactamente 3 vecinos nace
- En cualquier otro caso, muere o permanece muerta

## C++ (Versión interactiva)

Implementación completa con controles en tiempo real.

### Compilación y uso:
```bash
make
./conway <cols> <rows>
# Ejemplo: ./conway 100 40
```

### Controles:
- `+/-` → Velocidad
- `p` → Pausa
- `w` → Activar bordes envolventes
- `r` → Reiniciar
- `q` → Salir

## Python (Versión educativa)

Código simplificado **sin controles interactivos** para entender la lógica sin complejidad de threading/entrada de datos. Perfecto para aprender los algoritmos base.

## Propósito académico

Comparar implementaciones y entender cómo los mismos algoritmos se adaptan según el lenguaje y requisitos.
