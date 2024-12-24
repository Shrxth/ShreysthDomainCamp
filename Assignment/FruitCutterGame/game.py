import pygame
import random
import os
import sys

# Initialize Pygame
pygame.init()

# Constants
WIDTH, HEIGHT = 1000, 800
FPS = 60
WHITE = (255, 255, 255)
RED = (255, 0, 0)
ASSET_DIR = "C:/Users/asus/OneDrive/Desktop/CODING/Python/FruitCutterGame/Assets"

# Initialize Screen
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Fruit Cutter Game")
clock = pygame.time.Clock()

# Fonts
font = pygame.font.Font(None, 36)

# Load Assets
def load_asset(file_name, scale=None):
    """Loads an image and optionally scales it."""
    path = os.path.join(ASSET_DIR, file_name)
    if not os.path.exists(path):
        print(f"Error: File {file_name} not found in {ASSET_DIR}")
        sys.exit()
    img = pygame.image.load(path)
    return pygame.transform.scale(img, scale) if scale else img

# Images
background = load_asset("background.jpg", scale=(WIDTH, HEIGHT))
fruit_images = [
    load_asset("apple.png", scale=(90, 90)),
    load_asset("orange.png", scale=(90, 90)),
    load_asset("banana.png", scale=(90, 90))
]

# Sounds
try:
    slice_sound = pygame.mixer.Sound(os.path.join(ASSET_DIR, "Slice.wav"))
except FileNotFoundError:
    slice_sound = None
    print("Slice sound effect not found. Proceeding without sound.")

# Load and play background music (only if the file exists)
try:
    pygame.mixer.music.load(os.path.join(ASSET_DIR, "background_music.mp3"))
    pygame.mixer.music.set_volume(0.5)
    pygame.mixer.music.play(-1)  # Loop the background music
except FileNotFoundError:
    print("Background music not found. Proceeding without music.")

# Game Variables
score = 0
time_limit = 60  # seconds
start_ticks = pygame.time.get_ticks()

class Fruit:
    """Represents a fruit in the game."""
    def __init__(self):
        self.image = random.choice(fruit_images)
        self.x = random.randint(50, WIDTH - 50)
        self.y = -50
        self.speed = random.randint(5, 10)
        self.cut = False
        self.slice_effect_timer = 0  # Timer for showing slice effect

    def draw(self):
        """Draws the fruit or the slice effect."""
        if self.cut:
            if self.slice_effect_timer > 0:
                # Draw a slice effect (e.g., a red flash)
                pygame.draw.circle(screen, RED, (self.x + 40, self.y + 40), 50, 5)
                self.slice_effect_timer -= 1  # Reduce timer for slice effect
        else:
            screen.blit(self.image, (self.x, self.y))

    def move(self):
        """Moves the fruit downwards."""
        if not self.cut:
            self.y += self.speed

    def is_sliced(self, x, y):
        """Checks if the fruit is sliced by the mouse."""
        rect = self.image.get_rect(topleft=(self.x, self.y))
        return rect.collidepoint(x, y)

# Game Loop
def main():
    global score
    fruits_on_screen = [Fruit() for _ in range(3)]
    running = True

    while running:
        screen.blit(background, (0, 0))

        # Event Handling
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False

        # Mouse Position
        mouse_x, mouse_y = pygame.mouse.get_pos()
        mouse_pressed = pygame.mouse.get_pressed()[0]

        # Update and Draw Fruits
        for fruit in fruits_on_screen:
            fruit.move()
            fruit.draw()

            # Check slicing
            if mouse_pressed and fruit.is_sliced(mouse_x, mouse_y):
                if not fruit.cut:
                    score += 1
                    if slice_sound:
                        slice_sound.play()
                    fruit.cut = True
                    fruit.slice_effect_timer = 10  # Display slice effect for 10 frames

            # Remove off-screen fruits
            if fruit.y > HEIGHT or fruit.cut:
                fruits_on_screen.remove(fruit)
                fruits_on_screen.append(Fruit())

        # Timer and Score Display
        elapsed_time = (pygame.time.get_ticks() - start_ticks) / 1000
        remaining_time = max(time_limit - int(elapsed_time), 0)
        score_text = font.render(f"Score: {score}", True, (0, 0, 0))
        time_text = font.render(f"Time: {remaining_time}", True, (0, 0, 0))
        screen.blit(score_text, (10, 10))
        screen.blit(time_text, (10, 50))

        # Game Over
        if remaining_time <= 0:
            game_over_text = font.render("Game Over! Press ESC to Quit", True, RED)
            screen.blit(game_over_text, (WIDTH // 2 - 150, HEIGHT // 2))
            pygame.display.update()
            wait_for_quit()
            running = False

        pygame.display.update()
        clock.tick(FPS)

    pygame.quit()

def wait_for_quit():
    """Waits for the user to press ESC to quit."""
    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT or (event.type == pygame.KEYDOWN and event.key == pygame.K_ESCAPE):
                pygame.quit()
                sys.exit()

# Run the Game
if __name__ == "__main__":
    main()
