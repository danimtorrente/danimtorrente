import random as random
import matplotlib.pyplot as plt
import numpy as np

#Jugador 1 y 2 tienen 7 elementos
def buscar_ficha(Jugador_1, Jugador_2, ficha):
    for i in range(7):
        if ficha == Jugador_1[i]: return 1
        if ficha == Jugador_2[i]: return 2
    return -1

def buscar_ficha_rapido(Jugador_1, Jugador_2, ficha):
    if ficha in Jugador_1: return 1
    if ficha in Jugador_2: return 2
    return -1

fichas = []
for i in range(7):
    for j in range(7):
        fichas.append((i,j))

ficha = random.choice(fichas)

Jugador_1 = []
Jugador_2 = []

for i in range(7):
    Jugador_1.append(random.choice(fichas))
    Jugador_2.append(random.choice(fichas))

print(Jugador_1)
print(Jugador_2)

result = buscar_ficha(Jugador_1, Jugador_2, (6,6))

print("El doble 6 es de ")

if result == 1: print("Jugador_1")
elif result == 2: print("Jugador_2")
else: print("nadie")

