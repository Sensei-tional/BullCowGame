// ©Kester McPherson. All rights reserved.


#include "Terminal.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Framework/Application/SlateApplication.h"

#include "Cartridge.h"

constexpr TCHAR GPrompt[4] = TEXT("$> ");

// Called when the game starts
void UTerminal::BeginPlay()
{
	Super::BeginPlay();
	UpdateText();
}


void UTerminal::ActivateTerminal()
{
    FInputKeyBinding PressedBinding(EKeys::AnyKey, EInputEvent::IE_Pressed);
    PressedBinding.KeyDelegate.BindDelegate(this, &UTerminal::OnKeyDown);

    FInputKeyBinding RepeatBinding(EKeys::AnyKey, EInputEvent::IE_Repeat);
    RepeatBinding.KeyDelegate.BindDelegate(this, &UTerminal::OnKeyDown);

	if (GetOwner()->InputComponent == nullptr) return;

    PressedBindingIndex = GetOwner()->InputComponent->KeyBindings.Emplace(MoveTemp(PressedBinding));
    RepeatBindingIndex = GetOwner()->InputComponent->KeyBindings.Emplace(MoveTemp(RepeatBinding));
}

void UTerminal::DeactivateTerminal() const
{
	if (GetOwner()->InputComponent == nullptr) return;
	
	// Must do in this order as RepeatBindingIndex > PressedBindingIndex so would change when first is removed
	GetOwner()->InputComponent->KeyBindings.RemoveAt(RepeatBindingIndex);
	GetOwner()->InputComponent->KeyBindings.RemoveAt(PressedBindingIndex);
}

void UTerminal::PrintLine(const FString& Line)
{
	FString Input = Line;
	FString Left, Right;
	while (Input.Split(TEXT("\n"), &Left, &Right))
	{
		Buffer.Emplace(Left);
		Input = Right;
	} 
	Buffer.Emplace(Input);
	UpdateText();
}

//Clears terminal screen
void UTerminal::ClearScreen()
{
	Buffer.Empty();
	UpdateText();
}

FString UTerminal::GetScreenText() const
{
	TArray<FString> FullTerminal = Buffer;
	FullTerminal.Add(GPrompt + InputLine);

	// WrapLines
	TArray<FString> WrappedLines(WrapLines(FullTerminal));
	Truncate(WrappedLines);

	return JoinWithNewline(WrappedLines);
}

TArray<FString> UTerminal::WrapLines(const TArray<FString>& Lines) const
{
	TArray<FString> WrappedLines;
	for (auto &&Line : Lines)
	{
		FString CurrentLine = Line;
		do
		{
			WrappedLines.Add(CurrentLine.Left(MaxColumns));
			CurrentLine = CurrentLine.RightChop(MaxColumns);
		}
		while (CurrentLine.Len() > 0);
	}
	return WrappedLines;
}

//Removes the lines at the top one a max number of lines have been reached
void UTerminal::Truncate(TArray<FString>& Lines) const
{
	while (Lines.Num() > MaxLines)
	{
		Lines.RemoveAt(0);
	}
}

//Adds text
FString UTerminal::JoinWithNewline(const TArray<FString>& Lines) const
{
	FString Result;
	for (auto &&Line : Lines)
	{
		Result += Line + TEXT(" <br>");
	}
	return Result;
}

//What happens when a key is pressed
void UTerminal::OnKeyDown(FKey Key)
{
	if (Key == EKeys::Enter)
	{
		AcceptInputLine();
	}

	if (Key == EKeys::BackSpace)
	{
		Backspace();
	}

    //Checks for upper and lower case modifier
	const FString KeyString = GetKeyString(Key);
    const FModifierKeysState KeyState = FSlateApplication::Get().GetModifierKeys();
	if (KeyState.IsShiftDown() || KeyState.AreCapsLocked())
	{
		InputLine += KeyString.ToUpper();
	}
	else
	{
		InputLine += KeyString.ToLower();
	}

	UpdateText();
}

//Commits what is in the terminal for users guess
void UTerminal::AcceptInputLine()
{
	Buffer.Emplace(GPrompt + InputLine);
	auto Cartridge = GetOwner()->FindComponentByClass<UCartridge>();
	if (Cartridge != nullptr)
	{
		Cartridge->OnInput(InputLine);
	}
	InputLine = TEXT("");

}

//Allows user to remove a previously typed letter.
void UTerminal::Backspace()
{
	if (InputLine.Len() > 0)
	{
		InputLine.RemoveAt(InputLine.Len()-1);
	}
}

//Takes keypress and converts into character
FString UTerminal::GetKeyString(FKey Key) const
{
	const uint32* KeyCode = nullptr;
	const uint32* CharCode = nullptr;
	FInputKeyManager::Get().GetCodesFromKey(Key, KeyCode, CharCode);
	if (CharCode != nullptr)
	{
		ANSICHAR Char[2] = {static_cast<ANSICHAR>(*CharCode), '\0'};
		return ANSI_TO_TCHAR(Char);
	}

	return TEXT("");
}

//Updates the text on the terminal
void UTerminal::UpdateText()
{
	TextUpdated.Broadcast(GetScreenText());
}