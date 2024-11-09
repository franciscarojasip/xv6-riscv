#ifndef SPINLOCK_H
#define SPINLOCK_H

// Estructura para el lock de exclusión mutua.
struct spinlock {
  uint locked;       // ¿Está el lock en uso?

  // Para depuración:
  char *name;        // Nombre del lock.
  struct cpu *cpu;   // La CPU que sostiene el lock.
};

#endif // SPINLOCK_H

