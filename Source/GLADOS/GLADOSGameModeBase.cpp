// Copyright Epic Games, Inc. All Rights Reserved.


#include "GLADOSGameModeBase.h"

AGLADOSGameModeBase::AGLADOSGameModeBase() : Super()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/2_BP/BP_SinglePlayer"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
