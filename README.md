# Validador de Archivos JSON

Este programa en C++ está diseñado para leer un archivo de entrada en formato JSON y verificar si está bien escrito de acuerdo al estándar JSON. Si se encuentra un error en el formato del archivo de entrada, el programa se detendrá y mostrará un mensaje por consola indicando el tipo de error y la ubicación en el archivo.

## Uso

1. Copiar un código JSON dentro del archivo `entrada.json`.
2. Ejecutar el archivo `main.cpp`.

## Resultados

Si el archivo de entrada es válido, el programa creará un archivo de salida llamado `salida.json` que contendrá el contenido del archivo de entrada sin espacios ni saltos de línea. Además, proporcionará un archivo llamado `buffers.txt` que diferenciará lo que se guardó en cada buffer dependiendo de las distintas expresiones del archivo JSON de entrada.

## Ejemplo de Archivo JSON de Entrada
```
{
  "nombre": "Juan",
  "edad": 30,
  "ciudad": "Ciudad de México",
  "intereses": ["programación", "música", "viajes"]
}
```

## Notas
- Este programa solo permite string dentro de una lista [].
- Se recomienda tener cuidado al proporcionar nombres de archivos de entrada para evitar errores de lectura.
- Si el programa finaliza con éxito, los archivos de `salida.json` y `buffers.txt` estarán disponibles en el directorio de ejecución del programa.
- Se recomienda utilizar entornos como Replit para ejecutar el programa, esto evitará posbiles errores al buscar la ruta del archivo de entrada.

___

¡Gracias por usar nuestro Validador de Archivos JSON en C++!