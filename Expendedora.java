import java.util.Scanner;
public class Expendedora {

    // ---------- Catalogo (arreglos paralelos) ----------
    static int[] codigos = {1, 2, 3, 4};
    static String[] nombres = {"Agua", "Soda", "Papas", "Chocolate"};
    static double[] precios = {10.0, 15.0, 12.5, 18.0};
    static int[] stocks = {5, 5, 5, 5};

    // Denominaciones de moneda aceptadas por la maquina (enteras)
    static int[] denominaciones = {10, 5, 1};

    // ---------- Metodos ----------

    static void mostrarCatalogo() {
        System.out.println("\n--- CATALOGO ---");
        for (int i = 0; i < codigos.length; i++) {
            System.out.printf("[%d] %-10s L. %.2f  (stock: %d)%n",
                    codigos[i], nombres[i], precios[i], stocks[i]);
        }
    }

    // Recibe el saldo actual (double) y el monto a insertar (double), devuelve el nuevo saldo
    static double insertarDinero(double saldoActual, double monto) {
        if (monto <= 0) {
            System.out.println("Monto invalido.");
            return saldoActual;
        }
        return saldoActual + monto;
    }

    // Inserta una moneda por denominacion (int). Java convierte
    // automaticamente el int a double al sumarlo con el saldo decimal
    // (coercion implicita real, no explicada, solo aplicada).
    static double insertarMoneda(double saldoActual, int denominacion) {
        return saldoActual + denominacion;
    }

    // Busca un producto por codigo, devuelve el indice o -1 si no existe
    static int buscarProducto(int codigo) {
        for (int i = 0; i < codigos.length; i++) {
            if (codigos[i] == codigo) {
                return i;
            }
        }
        return -1;
    }

    // Verifica si el saldo alcanza para el precio dado (boolean nativo)
    static boolean validarSaldo(double precio, double saldo) {
        return saldo >= precio;
    }

    // Calcula el vuelto de forma RECURSIVA usando las denominaciones disponibles
    static void calcularVuelto(double monto, int idx) {
        if (monto <= 0 || idx >= denominaciones.length) {
            return;
        }

        int denom = denominaciones[idx];

        if (monto >= denom) {
            System.out.println("  -> L. " + denom);
            calcularVuelto(monto - denom, idx);
        } else {
            calcularVuelto(monto, idx + 1);
        }
    }

    // Entrega el producto: descuenta stock y muestra el vuelto
    static double seleccionarProducto(int codigo, double saldo) {
        int idx = buscarProducto(codigo);

        if (idx == -1) {
            System.out.println("Producto no encontrado.");
            return saldo;
        }

        if (stocks[idx] <= 0) {
            System.out.println("Producto agotado.");
            return saldo;
        }

        if (!validarSaldo(precios[idx], saldo)) {
            System.out.printf("Saldo insuficiente. Faltan L. %.2f%n", precios[idx] - saldo);
            return saldo;
        }

        stocks[idx]--;
        System.out.println("Entregando: " + nombres[idx]);

        double vuelto = saldo - precios[idx];
        if (vuelto > 0) {
            System.out.printf("Su vuelto es L. %.2f:%n", vuelto);
            calcularVuelto(vuelto, 0);
        }

        return 0.0;
    }

    // Cancela la transaccion y devuelve el dinero insertado
    static double cancelarTransaccion(double saldo) {
        if (saldo > 0) {
            System.out.printf("Transaccion cancelada. Se devuelven L. %.2f%n", saldo);
        } else {
            System.out.println("No hay saldo que devolver.");
        }
        return 0.0;
    }

    // Lee un entero de forma segura. Si el usuario escribe algo que no es
    // un numero, Scanner.hasNextInt() lo detecta antes de leerlo, se
    // descarta el token invalido y se vuelve a pedir el dato: validacion
    // de tipo aplicada de verdad, no solo explicada.
    static int leerEntero(Scanner sc, String mensaje) {
        while (true) {
            System.out.print(mensaje);
            if (sc.hasNextInt()) {
                return sc.nextInt();
            }
            System.out.println("Entrada invalida, se esperaba un numero entero.");
            sc.next(); // descartar el token invalido
        }
    }

    // Lee un decimal de forma segura, mismo principio que leerEntero.
    static double leerDecimal(Scanner sc, String mensaje) {
        while (true) {
            System.out.print(mensaje);
            if (sc.hasNextDouble()) {
                return sc.nextDouble();
            }
            System.out.println("Entrada invalida, se esperaba un numero decimal.");
            sc.next();
        }
    }

    // ---------- Programa principal ----------
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        double saldo = 0.0;
        int opcion;

        do {
            System.out.println("\n===== MAQUINA EXPENDEDORA =====");
            System.out.printf("Saldo actual: L. %.2f%n", saldo);
            System.out.println("1. Ver catalogo");
            System.out.println("2. Insertar dinero (monto libre)");
            System.out.println("3. Seleccionar producto");
            System.out.println("4. Cancelar / pedir vuelto");
            System.out.println("5. Insertar moneda por denominacion (10/5/1)");
            System.out.println("0. Salir");
            opcion = leerEntero(sc, "Opcion: ");

            switch (opcion) {
                case 1:
                    mostrarCatalogo();
                    break;
                case 2: {
                    double monto = leerDecimal(sc, "Ingrese monto a insertar: ");
                    saldo = insertarDinero(saldo, monto);
                    break;
                }
                case 3: {
                    int codigo = leerEntero(sc, "Ingrese codigo de producto: ");
                    saldo = seleccionarProducto(codigo, saldo);
                    break;
                }
                case 4:
                    saldo = cancelarTransaccion(saldo);
                    break;
                case 5: {
                    int denom = leerEntero(sc, "Ingrese denominacion (10, 5 o 1): ");
                    saldo = insertarMoneda(saldo, denom);
                    System.out.printf("Saldo actualizado: L. %.2f%n", saldo);
                    break;
                }
                case 0:
                    System.out.println("Gracias por su compra.");
                    break;
                default:
                    System.out.println("Opcion invalida.");
            }
        } while (opcion != 0);

        sc.close();
    }
}
