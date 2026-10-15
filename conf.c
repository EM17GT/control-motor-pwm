#include "conf.h"
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <lgpio.h>


#define PWM_DIR    "/sys/class/pwm/pwmchip0"
#define PERIODO_NS 1000000   // 1 kHz (en nanosegundos)

// ---------- VARIABLES ----------
 int chip = 0;
 int velocidad = 0;        // 0 a 100 %
 volatile int salir = 0;   // se pone en 1 con Ctrl+C

void escribir(const char *ruta, const char *valor)
{
    FILE *f = fopen(ruta, "w");
    if (f == NULL)
        return;
    fprintf(f, "%s", valor);
    fclose(f);
}

int init(void)
{
    chip = lgGpiochipOpen(0);
    if (chip < 0)
        return -1;

    escribir(PWM_DIR "/export", "0");    // crea la carpeta pwm0
    usleep(200000);                      // espera 200 ms

    // Comprobar que el PWM quedó disponible
    if (access(PWM_DIR "/pwm0/period", W_OK) != 0)
        return -2;

    escribir(PWM_DIR "/pwm0/period", "1000000");
    escribir(PWM_DIR "/pwm0/duty_cycle", "0");
    escribir(PWM_DIR "/pwm0/enable", "1");
    return 0;
}
void duty_cycle(int porcentaje)
{
if (porcentaje < 0)   porcentaje = 0;
    if (porcentaje > 100) porcentaje = 100;

    velocidad = porcentaje;

    char texto[32];
    snprintf(texto, sizeof(texto), "%d", velocidad * PERIODO_NS / 100);
    escribir(PWM_DIR "/pwm0/duty_cycle", texto);
}
void detener(void)
{
  duty_cycle(0);
}
void clean(void)
{
    duty_cycle(0);
    escribir(PWM_DIR "/pwm0/enable", "0");
    escribir(PWM_DIR "/unexport", "0");
    lgGpiochipClose(chip);
}
void manejar_ctrl_c(int senal)
{
    salir = 1;
}