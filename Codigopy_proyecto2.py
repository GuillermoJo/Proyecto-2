from Adafruit_IO import Client
import serial
import serial.tools.list_ports
import time

# ─── Configuración Adafruit IO ────────────────────────────────
USUARIO_AIO  = "JoseGJo"
CLAVE_AIO    = "aio_XNYF31hwNkCCTuVUzz09l3V1CAnB"

FEED_S1 = "servo1"
FEED_S2 = "servo2"
FEED_S3 = "servo3"
FEED_S4 = "servo4"

FEEDS = [FEED_S1, FEED_S2, FEED_S3, FEED_S4]

# ─── Configuración serial ─────────────────────────────────────
PUERTO      = "COM4"
BAUDRATE    = 9600
TIMEOUT_S   = 1

# ─── Parámetros de comportamiento ────────────────────────────
ANGULO_MIN      = 0
ANGULO_MAX      = 180
ANGULO_DEFAULT  = 90
RETARDO_LOOP    = 0.5     # segundos entre lecturas
RETARDO_INICIO  = 2.0     # espera tras abrir el puerto
RETARDO_MODO    = 1.0     # espera tras seleccionar modo UART
RETARDO_LECTURA = 0.2     # espera antes de leer respuesta Arduino


# ════════════════════════════════════════════════════════════════
#  UTILIDADES
# ════════════════════════════════════════════════════════════════

def listar_puertos():
    """Muestra en consola todos los puertos seriales disponibles."""
    print("Puertos seriales disponibles:")
    for puerto in serial.tools.list_ports.comports():
        print(f"  {puerto.device} - {puerto.description}")


def limitar_angulo(valor):
    """
    Convierte 'valor' a entero y lo limita al rango válido de servo.
    Si la conversión falla devuelve ANGULO_DEFAULT.
    """
    try:
        valor = int(float(valor))
    except (ValueError, TypeError):
        return ANGULO_DEFAULT

    return max(ANGULO_MIN, min(ANGULO_MAX, valor))


# ════════════════════════════════════════════════════════════════
#  ADAFRUIT IO
# ════════════════════════════════════════════════════════════════

def leer_feed(aio, feed):
    """
    Lee el valor más reciente de un feed y lo devuelve como ángulo.
    En caso de error devuelve ANGULO_DEFAULT.
    """
    try:
        dato = aio.receive(feed)
        return limitar_angulo(dato.value)
    except Exception as error:
        print(f"  [AIO] Error en feed '{feed}': {error}")
        return ANGULO_DEFAULT


def leer_todos_los_feeds(aio):
    """Lee los cuatro feeds y devuelve una lista de ángulos [s1, s2, s3, s4]."""
    return [leer_feed(aio, feed) for feed in FEEDS]


# ════════════════════════════════════════════════════════════════
#  COMUNICACIÓN SERIAL
# ════════════════════════════════════════════════════════════════

def leer_respuestas(arduino):
    """Lee y muestra todas las líneas pendientes del Arduino."""
    time.sleep(RETARDO_LECTURA)
    while arduino.in_waiting > 0:
        linea = arduino.readline().decode("utf-8", errors="ignore").strip()
        if linea:
            print(f"  [Arduino] {linea}")


def enviar_comando(arduino, comando):
    """Envía un comando al Arduino y muestra su respuesta."""
    arduino.write(comando.encode("utf-8"))
    print(f"  [TX] {comando}")
    leer_respuestas(arduino)


def construir_comando(angulos):
    """
    Construye el string de movimiento a partir de una lista de ángulos.
    Formato esperado por el firmware: 'M g0 g1 g2 g3#'
    """
    return "M {} {} {} {}#".format(*angulos)


# ════════════════════════════════════════════════════════════════
#  PROGRAMA PRINCIPAL
# ════════════════════════════════════════════════════════════════

listar_puertos()

aio = Client(USUARIO_AIO, CLAVE_AIO)

try:
    arduino = serial.Serial(PUERTO, BAUDRATE, timeout=TIMEOUT_S)
    time.sleep(RETARDO_INICIO)

    print("\nConectado al Arduino.")
    leer_respuestas(arduino)

    # Seleccionar modo UART / Adafruit en el firmware
    print("\nEntrando al modo UART / Adafruit...")
    arduino.write(b"3")
    time.sleep(RETARDO_MODO)
    leer_respuestas(arduino)

    print("\nConectado a Adafruit IO.")
    print("Enviando valores de los sliders al Arduino...\n")

    ultimo_comando = ""

    while True:
        angulos  = leer_todos_los_feeds(aio)
        comando  = construir_comando(angulos)

        # Enviar solo si el comando cambió para evitar tráfico innecesario
        if comando != ultimo_comando:
            enviar_comando(arduino, comando)
            ultimo_comando = comando

        time.sleep(RETARDO_LOOP)

except serial.SerialException as error:
    print("\nNo se pudo abrir el puerto serial:")
    print(f"  {error}")
    print("\nVerifique que:")
    print("  1. El puerto COM sea el correcto.")
    print("  2. Ningún otro programa esté usando el mismo puerto.")
    print("  3. El Arduino esté conectado por USB.")

except KeyboardInterrupt:
    print("\nPrograma detenido por el usuario.")

finally:
    try:
        arduino.close()
        print("Puerto serial cerrado.")
    except Exception:
        pass