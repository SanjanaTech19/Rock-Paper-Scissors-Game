import random

options = ("Rock","Paper","Scissors")

computer = random.choice(options)

game_is_running = True

player = input("enter your choice (Rock , Paper , Scissors)")

while game_is_running:
    
    print(f"Player : {player}")
    print(f"Computer : {computer}")
    
    if player == computer:
        print("It's a tie")
    elif player == "Rock" and computer == "Scissors":
        print("You win")
    elif player == "Paper" and computer == "Rock":
        print("You win")
    elif player == "Scissors" and computer == "Paper":
        print("You win")
    elif player not in options:
        print("Invalid input entered by player")
    else:
        print("You lose")
    game_is_running = False

