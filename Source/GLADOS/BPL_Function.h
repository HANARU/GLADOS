#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BPL_Function.generated.h"

UCLASS()
class GLADOS_API UBPL_Function : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	bool RectToRectCollision(FVector2D Rect1Origin, FVector2D Rect1Extents, FVector2D Rect2Origin, FVector2D Rect2Extents);
	
};
