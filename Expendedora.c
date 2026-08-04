#include <stdio.h>

#define MAX_PRODUCTOS 4
#define MAX_DENOMINACIONES 3

/* ---------- Catalogo (arreglos paralelos) ---------- */
int    codigos[MAX_PRODUCTOS]  = {1, 2, 3, 4};
char   nombres[MAX_PRODUCTOS][20] = {"Agua", "Soda", "Papas", "Chocolate"};
float  precios[MAX_PRODUCTOS]  = {10.0, 15.0, 12.5, 18.0};
int    stocks[MAX_PRODUCTOS]   = {5, 5, 5, 5};

/* Denominaciones de moneda aceptadas por la maquina (enteras) */
int denominaciones[MAX_DENOMINACIONES] = {10, 5, 1};

/* ---------- Funciones ---------- */

void mostrarCatalogo() {
    printf("\n--- CATALOGO ---\n");
    for (int i = 0; i < MAX_PRODUCTOS; i++) {
        printf("[%d] %-10s L. %.2f  (stock: %d)\n",
               codigos[i], nombres[i], precios[i], stocks[i]);
    }
}

/* Recibe el saldo actual (float) y el monto a insertar (float),
   devuelve el nuevo saldo. */
float insertarDinero(float saldoActual, float monto) {
    if (monto <= 0) {
        printf("Monto invalido.\n");
        return saldoActual;
    }
    return saldoActual + monto;
}

/* Inserta una moneda por denominacion. La denominacion es ENTERA
   pero el saldo es DECIMAL: al sumarlas, C convierte automaticamente
   el entero a float antes de la suma (coercion implicita real,
   no explicada, solo aplicada). */
float insertarMoneda(float saldoActual, int denominacion) {
    return saldoActual + denominacion;
}

/* Busca un producto por codigo, devuelve el indice o -1 si no existe */
int buscarProducto(int codigo) {
    for (int i = 0; i < MAX_PRODUCTOS; i++) {
        if (codigos[i] == codigo) {
            return i;
        }
    }
    return -1;
}

/* Verifica si el saldo alcanza para el precio dado (booleano simulado
   con int: 1 = verdadero, 0 = falso) */
int validarSaldo(float precio, float saldo) {
    if (saldo >= precio) {
        return 1;
    }
    return 0;
}

/* Calcula el vuelto de forma RECURSIVA usando las denominaciones disponibles. */
void calcularVuelto(float monto, int idx) {
    if (monto <= 0 || idx >= MAX_DENOMINACIONES) {
        return;
    }

    int denom = denominaciones[idx];

    if (monto >= denom) {
        printf("  -> L. %d\n", denom);
        calcularVuelto(monto - denom, idx);
    } else {
        calcularVuelto(monto, idx + 1);
    }
}

/* Entrega el producto: descuenta stock y muestra el vuelto */
float seleccionarProducto(int codigo, float saldo) {
    int idx = buscarProducto(codigo);

    if (idx == -1) {
        printf("Producto no encontrado.\n");
        return saldo;
    }

    if (stocks[idx] <= 0) {
        printf("Producto agotado.\n");
        return saldo;
    }

    if (!validarSaldo(precios[idx], saldo)) {
        printf("Saldo insuficiente. Faltan L. %.2f\n", precios[idx] - saldo);
        return saldo;
    }

    stocks[idx] = stocks[idx] - 1;
    printf("Entregando: %s\n", nombres[idx]);

    float vuelto = saldo - precios[idx];
    if (vuelto > 0) {
        printf("Su vuelto es L. %.2f:\n", vuelto);
        calcularVuelto(vuelto, 0);
    }

    return 0.0;
}

/* Cancela la transaccion y devuelve el dinero insertado */
float cancelarTransaccion(float saldo) {
    if (saldo > 0) {
        printf("Transaccion cancelada. Se devuelven L. %.2f\n", saldo);
    } else {
        printf("No hay saldo que devolver.\n");
    }
    return 0.0;
}

/* Lee un entero de forma segura. Si el usuario escribe algo que no es
   un numero, scanf falla (retorna 0), se limpia el buffer y se vuelve
   a pedir el dato: validacion de tipo aplicada de verdad. */
int leerEntero(const char *mensaje) {
    int valor;
    while (1) {
        printf("%s", mensaje);
        if (scanf("%d", &valor) == 1) {
            return valor;
        }
        printf("Entrada invalida, se esperaba un numero entero.\n");
        while (getchar() != '\n'); /* limpiar buffer */
    }
}

/* Lee un decimal de forma segura, mismo principio que leerEntero. */
float leerDecimal(const char *mensaje) {
    float valor;
    while (1) {
        printf("%s", mensaje);
        if (scanf("%f", &valor) == 1) {
            return valor;
        }
        printf("Entrada invalida, se esperaba un numero decimal.\n");
        while (getchar() != '\n');
    }
}

/* ---------- Programa principal ---------- */
int main() {
    float saldo = 0.0;
    int opcion;

    do {
        printf("\n===== MAQUINA EXPENDEDORA =====\n");
        printf("Saldo actual: L. %.2f\n", saldo);
        printf("1. Ver catalogo\n");
        printf("2. Insertar dinero (monto libre)\n");
        printf("3. Seleccionar producto\n");
        printf("4. Cancelar / pedir vuelto\n");
        printf("5. Insertar moneda por denominacion (10/5/1)\n");
        printf("0. Salir\n");
        opcion = leerEntero("Opcion: ");

        switch (opcion) {
            case 1:
                mostrarCatalogo();
                break;
            case 2: {
                float monto = leerDecimal("Ingrese monto a insertar: ");
                saldo = insertarDinero(saldo, monto);
                break;
            }
            case 3: {
                int codigo = leerEntero("Ingrese codigo de producto: ");
                saldo = seleccionarProducto(codigo, saldo);
                break;
            }
            case 4:
                saldo = cancelarTransaccion(saldo);
                break;
            case 5: {
                int denom = leerEntero("Ingrese denominacion (10, 5 o 1): ");
                saldo = insertarMoneda(saldo, denom);
                printf("Saldo actualizado: L. %.2f\n", saldo);
                break;
            }
            case 0:
                printf("Gracias por su compra.\n");
                break;
            default:
                printf("Opcion invalida.\n");
        }
    } while (opcion != 0);

    return 0;
}
