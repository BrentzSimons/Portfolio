Video Demonstration:
  https://youtu.be/al8quUOyeUQ


Instructions:

Compiling and Running the game:
  -Type "make" in the terminal to compile the game. Then type "make run" or "./connectFour" in order to play the game.

Main Menu:
  -First, enter how many columns you want the board to have. Please only enter integers that are greater than zero.
  -Next, enter how many rows you want the board to have. Again, please only enter integers that are greater than zero.
  -Next, select if you would like to play with another player or if you want to play against the AI. Enter a 0 to play against the AI and a 1 to play against another player.
  -If you select AI, then you will enter the level of difficulty you want to AI to be. Enter 1 for easy, 2 for medium, and 3 for hard. All difficulties are beatable.

Game:
  -The turns will go back and forth between black and white.
  -If you are playing against the AI, you will be white and the AI is black.
  -Enter which column you want to play and your piece will be placed and it'll be the next players turn.
  -First player to get four in a row (diagonal, horizontal, or vertical) wins!
  -After each game, you are asked if you want to play again. Enter 1 to play again or 0 to end the game.
  -If you play again, whoever lost the last round will start.
  -If you decide to end the game, you will be shown how many times black won, how many times white won, and how many times you tied.

Code Design:
  -The easy AI mode selects a random column and plays in that column as long as it's not already full.
  -The medium AI mode calculates a score for every column and picks the column with the best score. If all of the columns give the same score, it picks a random column.
  -The hard AI mode uses the same scoring method as medium, but also uses the minimax with alpha-beta pruning algorithm to look at future moves and finds the best move. In order to keep runtime low, the depth for the minimax decreases (from 7 down to 2) as the amount of columns increase.
