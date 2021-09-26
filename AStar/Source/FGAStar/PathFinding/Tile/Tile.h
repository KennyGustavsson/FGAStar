#pragma once

#include "CoreMinimal.h"
#include "FGAStar/PathFinding/Grid/PathFindingGrid.h"
#include "UObject/Object.h"
#include "Tile.generated.h"

class UPathFindingGrid;

UCLASS()
class FGASTAR_API UTile : public UObject
{
	GENERATED_BODY()

// Functions
public:
	UFUNCTION()
	void InitializeTile(FVector2D const TileIndex, FVector2DInt const TileOffset, int const TileCost, bool const TileIsWall, UPathFindingGrid* TileGrid);
	UFUNCTION()
	void GetNeighbors();
	UFUNCTION()
	bool IsUpLimit() const { return Index.Y == Grid->Size.Y - 1; }
	UFUNCTION()
	bool IsDownLimit() const { return Index.Y == 0; }
	UFUNCTION()
	bool IsRightLimit() const { return Index.X == Grid->Size.X - 1; }
	UFUNCTION()
	bool IsLeftLimit() const { return Index.X == 0; }

// Properties
	UPROPERTY(Transient)
	int Cost = 1;
	UPROPERTY(Transient)
	FVector2D Index = FVector2D::ZeroVector;
	UPROPERTY(Transient)
	bool IsWall = false;
	UPROPERTY(Transient)
	TArray<UTile*> Neighbors;
	UPROPERTY(Transient)
	UPathFindingGrid* Grid;
	UPROPERTY(Transient)
	FVector2D Position;
};
