import pygame

# Configuración de pantalla
WIDTH, HEIGHT = 1280, 720
GRID_SIZE = 10

# Colores
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
PLAYER1_COLOR = (0, 20, 255)
PLAYER2_COLOR = (255, 0, 0)

# Configuración de jugadores
PLAYER_SPEED = GRID_SIZE

pygame.init()
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Tron 2D")

# Estado de jugadores
players = {
    "player1": {"pos": [WIDTH // 4, HEIGHT // 2], "color": PLAYER1_COLOR, "direction": "RIGHT", "trail": []},
    "player2": {"pos": [3 * WIDTH // 4, HEIGHT // 2], "color": PLAYER2_COLOR, "direction": "LEFT", "trail": []}
}

def handle_input():
    keys = pygame.key.get_pressed()
    # Controles para el jugador 1
    if keys[pygame.K_w]:
        players["player1"]["direction"] = "UP"
    elif keys[pygame.K_s]:
        players["player1"]["direction"] = "DOWN"
    elif keys[pygame.K_a]:
        players["player1"]["direction"] = "LEFT"
    elif keys[pygame.K_d]:
        players["player1"]["direction"] = "RIGHT"

    # Controles para el jugador 2
    if keys[pygame.K_UP]:
        players["player2"]["direction"] = "UP"
    elif keys[pygame.K_DOWN]:
        players["player2"]["direction"] = "DOWN"
    elif keys[pygame.K_LEFT]:
        players["player2"]["direction"] = "LEFT"
    elif keys[pygame.K_RIGHT]:
        players["player2"]["direction"] = "RIGHT"


def move_player(player):
    direction = players[player]["direction"]
    x, y = players[player]["pos"]
    players[player]["trail"].append((x, y))

    if direction == "UP":
        y -= PLAYER_SPEED
    elif direction == "DOWN":
        y += PLAYER_SPEED
    elif direction == "LEFT":
        x -= PLAYER_SPEED
    elif direction == "RIGHT":
        x += PLAYER_SPEED

    players[player]["pos"] = [x, y]



def check_collisions():
    for player in players:
        x, y = players[player]["pos"]
        # Colisión con los bordes
        if x < 0 or x >= WIDTH or y < 0 or y >= HEIGHT:
            return True
        # Colisión con la propia línea o la del otro jugador
        for trail in players["player1"]["trail"] + players["player2"]["trail"]:
            if (x, y) == trail:
                return True
    return False


# Bucle principal del juego
running = True
clock = pygame.time.Clock()
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
    screen.fill(BLACK)
    handle_input()

    if check_collisions():
        print("¡Colisión! El juego ha terminado.")
        break

    # Mueve a cada jugador y revisa colisiones
    for player in players:
        move_player(player)

    # Dibuja las trayectorias
    for player in players.values():
        for trail in player["trail"]:
            pygame.draw.rect(screen, player["color"], (*trail, GRID_SIZE, GRID_SIZE))

    pygame.display.flip()
    clock.tick(15)  # Limita el FPS a 15 para un control más preciso

pygame.quit()
