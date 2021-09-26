#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PathFindingGrid.generated.h"

class UTile;

USTRUCT()
struct FTileArray // Wrapping in FStruct to enable normal garbage collection
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient)
	TArray<UTile*> Tile;

	UTile* operator[](const int& Value){ return Tile[Value]; }
};

USTRUCT(BlueprintType)
struct FVector2DInt // 2D vectors with ints
{
	GENERATED_BODY()

public:
// Functions
	FVector2DInt(){ X = 0; Y = 0; }
	FVector2DInt(int x, int y) { X = x; Y = y; }
	
// Properties
	UPROPERTY(Transient)
	int X = 0;
	UPROPERTY(Transient)
	int Y = 0;
};

#pragma region Obstacles
USTRUCT(BlueprintType)
struct FObstacleInfo // For Obstacles
{
	GENERATED_BODY()

public:
	FVector Location;
	FVector Extent;
	FRotator Rotation;
	int Cost = 0;
	bool bIsWall = false;
};

USTRUCT()
struct FTriangle
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<FVector2D> Points;
};

USTRUCT()
struct FObstacleQuad
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<FTriangle> Tris;
	UPROPERTY()
	FObstacleInfo ObstacleInfo;
};
#pragma endregion Obstacles

UCLASS()
class FGASTAR_API UPathFindingGrid : public UObject
{
	GENERATED_BODY()

public:
// Functions
	static float Sign(FVector2D const A, FVector2D const B, FVector2D const C);
	static bool PointInTriangle(FVector2D const Point, FTriangle T);
	
	UFUNCTION()
	void Generate(const FVector2DInt GridSize, const int TileScale, const FVector2DInt Offset);
	void GenerateGrid(const FVector2DInt Offset);
	void GetObstacleVertices();
	void GetObstacleInfosToGrid();
	
	UFUNCTION(BlueprintCallable)
	UTile* GetNearestTileFromWorldLocation(const FVector Location);
	UFUNCTION(BlueprintCallable)
	UTile* GetTileFromWorldLocation(const FVector Location);
	
	void AddGridObstacle(FObstacleInfo const ObstacleInfo);

// Properties
	UPROPERTY(Transient)
	TArray<FTileArray> Grid;
	UPROPERTY(EditAnywhere)
	TArray<FObstacleInfo> Obstacles;
	UPROPERTY(Transient)
	TArray<FObstacleQuad> ObstacleQuads;
	UPROPERTY(Transient)
	FVector2DInt Size;
	UPROPERTY(Transient)
	FVector2DInt GridOffset;

	// TODO Temp, for visualizing obstacles
	UPROPERTY(Transient)
	AActor* Owner;
	
	UPROPERTY(Transient)
	int Scale = 1;
};
