
#ifndef CONF_H
#define CONF_H
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <lgpio.h>

extern int chip;
extern int velocidad;
extern volatile int salir;

void escribir(const char *ruta, const char *valor);
int init(void);
void duty_cycle(int porcentaje);
void detener(void);
void clean(void);
void manejar_ctrl_c(int senal);


#endif 