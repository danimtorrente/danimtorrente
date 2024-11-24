import pygame

# Configuración de pantalla
WIDTH, HEIGHT = 700, 700
GRID_SIZE = 2

# Colores
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
PLAYER1_COLOR = (255, 255, 255)
PLAYER2_COLOR = (255, 127, 0)


# Configuración de jugadores
PLAYER_SPEED = GRID_SIZE

pygame.init()
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Tron 2D")
font = pygame.font.Font(None, 36)  # Fuente predeterminada, tamaño 36

pygame.mixer.init()
# Cargar la música de fondo
pygame.mixer.music.load("SOUNDTRACK.mp3")

# Estado de jugadores
players = {
    "player1": {"pos": [WIDTH // 4, HEIGHT // 2], "color": PLAYER1_COLOR, "direction": "RIGHT", "trail": []},
    "player2": {"pos": [3 * WIDTH // 4, HEIGHT // 2], "color": PLAYER2_COLOR, "direction": "LEFT", "trail": []}
}

def handle_input():
    global finish
    keys = pygame.key.get_pressed()
    if keys[pygame.K_c]: finish = True

    # Controles para el jugador 1
    if keys[pygame.K_w]:
        if players["player1"]["direction"] != "DOWN": players["player1"]["direction"] = "UP"
    elif keys[pygame.K_s]:
        if players["player1"]["direction"] != "UP": players["player1"]["direction"] = "DOWN"
    elif keys[pygame.K_a]:
        if players["player1"]["direction"] != "RIGHT":players["player1"]["direction"] = "LEFT"
    elif keys[pygame.K_d]:
        if players["player1"]["direction"] != "LEFT":players["player1"]["direction"] = "RIGHT"

    # Controles para el jugador 2
    if keys[pygame.K_UP]:
        if players["player2"]["direction"] != "DOWN":players["player2"]["direction"] = "UP"
    elif keys[pygame.K_DOWN]:
        if players["player2"]["direction"] != "UP":players["player2"]["direction"] = "DOWN"
    elif keys[pygame.K_LEFT]:
        if players["player2"]["direction"] != "RIGHT":players["player2"]["direction"] = "LEFT"
    elif keys[pygame.K_RIGHT]:
        if players["player2"]["direction"] != "LEFT":players["player2"]["direction"] = "RIGHT"

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


def reset():
    global text_surface1
    global text_surface2
    players["player1"] = {"pos": [WIDTH // 4, HEIGHT // 2], "color": PLAYER1_COLOR, "direction": "RIGHT", "trail": []}
    players["player2"] = {"pos": [3 * WIDTH // 4, HEIGHT // 2], "color": PLAYER2_COLOR, "direction": "LEFT", "trail": []}
    text_surface1 = font.render(f"Puntuación Jugador1: {puntos_j1}", True, (255, 255, 255))  # Color blanco
    text_surface2 = font.render(f"Puntuación Jugador2: {puntos_j2}", True, (255, 255, 255))  # Color blanco

def check_collisions():
    global puntos_j1
    global puntos_j2
    for player in players:
        x, y = players[player]["pos"]
        # Colisión con los bordes
        if x < 0 or x >= WIDTH or y < 0 or y >= HEIGHT:
            if player == "player1": puntos_j1 -= 1
            else : puntos_j2 -= 1
            return True
        # Colisión con la propia línea o la del otro jugador
        for trail in players["player1"]["trail"] + players["player2"]["trail"]:
            if (x, y) == trail:
                if player == "player1": puntos_j1 -= 1
                else : puntos_j2 -= 1
                return True
    return False

def init():
    global running
    pygame.mixer.music.play(-1)
    pygame.mixer.music.set_volume(1)
    text_surface = font.render("¡Bienvenido a tron! Pulse n para continuar", True, (255, 255, 255))  # Color blanco
    text_rect = text_surface.get_rect(center=(WIDTH/2, HEIGHT/2))  # Centrar en pantalla
    screen.fill((0, 0, 0))  # Fondo negro
    screen.blit(text_surface, text_rect)  # Mostrar el texto en la pantalla
    pygame.display.flip()  # Actualizar la pantalla
    go_on = False
    while not go_on:
        keys = pygame.key.get_pressed()
        if keys[pygame.K_n]: go_on = True
        for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    running = False
                    pygame.quit()

def finish_screen():
    global running
    text_surface = font.render("Pulse n para salir", True, (255, 255, 255))  # Color blanco
    text_rect = text_surface.get_rect(center=(WIDTH/2, HEIGHT/2))  # Centrar en pantalla
    text_rect3 = text_surface.get_rect(center=(WIDTH/2, HEIGHT/2))  # Centrar en pantalla
    screen.fill((0, 0, 0))  # Fondo negro
    screen.blit(text_surface, text_rect)  # Mostrar el texto en la pantalla
    screen.blit(text_surface3, text_rect3)
    pygame.display.flip()  # Actualizar la pantalla
    go_on = False
    while not go_on:
        keys = pygame.key.get_pressed()
        if keys[pygame.K_n]: go_on = True
        for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    running = False
                    pygame.quit()


finish = False
running = True
puntos_j1 = 10
puntos_j2 = 10
clock = pygame.time.Clock()
text_surface1 = font.render(f"Puntuación Jugador1: {puntos_j1}", True, (255, 255, 255))  # Color blanco
text_surface2 = font.render(f"Puntuación Jugador2: {puntos_j2}", True, (255, 255, 255))  # Color blanco
text_surface3 = font.render("Player 1 wins!!!", True, (255, 255, 255))  # Color blanco
text_rect1 = text_surface1.get_rect(center=(150, 20))  # Ajusta la posición en la pantalla para la primera línea
text_rect2 = text_surface2.get_rect(center=(150, 50))  # Ajusta la posición en la pantalla para la segunda línea

init()

# Bucle principal del juego
while finish == False:
    reset()
    if puntos_j1 == 0 or puntos_j2 == 0:
        if puntos_j1 == 0: text_surface3 = font.render("Player 2 wins!!!", True, (255, 255, 255))  # Color blanco
        finish_screen()
        running = False
        pygame.quit()
        break

    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
                pygame.quit()
        screen.fill(BLACK)  # Limpiar la pantalla
        screen.blit(text_surface1, text_rect1)  # Mostrar la primera línea
        screen.blit(text_surface2, text_rect2)  # Mostrar la segunda línea
        handle_input()
        if finish: 
            running = False
            pygame.quit()
            break

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
        clock.tick(60)  # Limita el FPS a 60 para un control más preciso
