import random

cant_datos = 50000 

datos = [str(random.randint(1, 1000000000)) for _ in range(cant_datos)]

with open("datos_generados.csv", "w") as f:
    f.write(",".join(datos))

print(f"[+] Archivo 'datos_generados.csv' generado con {cant_datos} números.")