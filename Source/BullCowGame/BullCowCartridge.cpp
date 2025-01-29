// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    SetupGame();
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    //ProcessGuess(Input);
    IsogramCheck(Input);
}

void UBullCowCartridge::SetupGame()
{
    ClearScreen();

    HiddenWord = "about";
    Lives = HiddenWord.Len();
    bGameOver = false;

    PrintLine(TEXT("Welcome to Bull Cows!"));
    PrintLine(TEXT("You have %i lives to guess the hidden word."), Lives);
    PrintLine(TEXT("Input your guess and press enter!"));

    const TCHAR HW[] = TEXT("about");
    PrintLine(TEXT("The first character of the hidden word is %c"), HW[0]);
}

void UBullCowCartridge::Endgame()
{
    bGameOver = true;
}

void UBullCowCartridge::ProcessGuess(FString Guess)
{
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
        return;
    }

    if (Guess == HiddenWord)
    {
        PrintLine(TEXT("Well done, You have guessed correctly!\nPress enter your play again..."));
        Endgame();
        return;
    }

    --Lives;

    if (Lives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("You have lost all lives!"));
        PrintLine(TEXT("Press enter to reload the game..."));
        Endgame();
        return;
    }

    if (Guess != HiddenWord && Guess.Len() == HiddenWord.Len())
    {
        PrintLine(TEXT("You have guessed incorrectly, you now have %i lives remaining!"), Lives);
        return;
    }

    if (Guess != HiddenWord && Guess.Len() != HiddenWord.Len())
    {
        
        PrintLine(TEXT("Your word was %i characters long.\nThe Hidden word is %i long.."), Guess.Len(), HiddenWord.Len());
        PrintLine(TEXT("You have guessed incorrectly, you now have %i lives remaining!"), Lives);
        return;   
    }
}

bool UBullCowCartridge::IsogramCheck(FString Word) const
{
    for (int32 i = 0; i < Word.Len(); i++)
    {
        PrintLine(TEXT("%c"), Word[i]);
    }

    return true;
}