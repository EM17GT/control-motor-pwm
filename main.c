#include "conf.h"
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <lgpio.h>

int main(void)
{
    // 1. Inicializar el sistema y el PWM
    int r = init();
    if (r == -1)
    {
        printf("Error al abrir el chip GPIO\n");
        return 1;
    }
    if (r == -2)
    {
        printf("No hay acceso al PWM. Revisa 'ls /sys/class/pwm/' o prueba con sudo\n");
        return 1;
    }

    // Capturar Ctrl+C para salir de forma segura
    signal(SIGINT, manejar_ctrl_c);

    printf("Sistema iniciado. El motor comenzará a variar de velocidad.\n");
    printf("Presiona Ctrl+C para salir.\n");

    // 2. Bucle principal sin botones (ejemplo de prueba automática de velocidad)
    while (!salir)
    {
        // Aumentar velocidad gradualmente del 0% al 100%
        for (int v = 0; v <= 100 && !salir; v += 10)
        {
            duty_cycle(v);
            printf("Velocidad actual: %d%%\n", velocidad);
            sleep(1); // Espera 1 segundo en cada nivel
        }

        // Disminuir velocidad gradualmente del 100% al 0%
        for (int v = 100; v >= 0 && !salir; v -= 10)
        {
            duty_cycle(v);
            printf("Velocidad actual: %d%%\n", velocidad);
            sleep(1); // Espera 1 segundo en cada nivel
        }
    }

    // 3. Limpieza y apagado seguro al salir
    printf("\nSaliendo y deteniendo motor...\n");
    clean();
    return 0;
}