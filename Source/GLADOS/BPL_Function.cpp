#include "BPL_Function.h"

bool UBPL_Function::RectToRectCollision(FVector2D Rect1Origin, FVector2D Rect1Extents, FVector2D Rect2Origin, FVector2D Rect2Extents)
{
	bool CollsionCheck;
	if (Rect1Origin.X + Rect1Extents.X < Rect2Origin.X - Rect2Extents.X) { CollsionCheck = false; }

	if (Rect1Origin.X - Rect1Extents.X < Rect2Origin.X + Rect2Extents.X) { CollsionCheck = false; }

	if (Rect1Origin.Y + Rect1Extents.Y < Rect2Origin.Y - Rect2Extents.Y) { CollsionCheck = false; }

	if (Rect1Origin.Y - Rect1Extents.Y < Rect2Origin.Y + Rect2Extents.Y) { CollsionCheck = false; }

	else { CollsionCheck = true; }
	return CollsionCheck;
}
