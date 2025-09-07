# 03_BullCowGame
This is a text game that I programmed. The assets came from GameDev.tv, a site that helps users code unreal engine games and Unity games also.

The game is a word guessing game. The objective is to guess the word within the allotted amount of guesses. The aren't just any words, they are isograms, which are words that have no repeating letters. These words are decided from a list of 1000 random words and a function in BullCowCartridge.cpp will take that list of words and sort which words are isograms which become eligible to use within the game.

Once those words are sorted, the game will choose a word at random and the user will then need to figure out the word to be successful. The attempts are based on how long or short the word is.