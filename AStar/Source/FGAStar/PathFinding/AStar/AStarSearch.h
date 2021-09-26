#pragma once
#include "CoreMinimal.h"

class UTile;

class FGASTAR_API AStarSearch
{
public:
	static void AStar(UTile* Start, UTile* Target, TArray<FVector2D>& OutPath, bool const AcceptPartialPath = false);

private:
	static float Heuristic(FVector2D const A, FVector2D const B){ return FMath::Abs(A.X - B.X) + FMath::Abs(A.Y - B.Y); }
};
