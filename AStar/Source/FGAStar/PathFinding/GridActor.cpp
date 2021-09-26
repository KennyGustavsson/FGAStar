#include "GridActor.h"
#include "DrawDebugHelpers.h"
#include "Grid/PathFindingGrid.h"
#include "Tile/Tile.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Obstacle/GridObstacle.h"

// #include "AStar/AStarAsyncTask.h" // Async
// #include "AStar/AStarSearch.h" // NonAsync

AGridActor::AGridActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGridActor::BeginPlay()
{
	Super::BeginPlay();
	
	UKismetSystemLibrary::FlushPersistentDebugLines(this);
	
	// Create Grid and Generate it
	UPathFindingGrid* Grid = NewObject<UPathFindingGrid>();
	MyGrid = Grid;

	MyGrid->Owner = this;

	// TODO Temp, this will get all the obstacles in the scene
	TArray<AActor*> GridObstacles;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGridObstacle::StaticClass(), GridObstacles);
	// --

	for (AActor* const GridObstacle : GridObstacles)
	{
		AGridObstacle* Obstacle = Cast<AGridObstacle>(GridObstacle);

		if(Obstacle)
		{
			FObstacleInfo Info;
		
			Info.Cost = Obstacle->Cost;
			Info.bIsWall = Obstacle->bIsWall;
			Info.Location = Obstacle->Mesh->GetComponentLocation();
			Info.Extent = Obstacle->Mesh->GetStaticMesh()->ExtendedBounds.BoxExtent * Obstacle->Mesh->GetComponentScale();
			Info.Rotation = Obstacle->Mesh->GetComponentRotation();

			MyGrid->AddGridObstacle(Info);
		}
	}
	
	// Generate grid
	MyGrid->Generate(FVector2DInt(Width, Height), GridScale, FVector2DInt(
		FGenericPlatformMath::RoundToInt(GetActorLocation().X), FGenericPlatformMath::RoundToInt(GetActorLocation().Y)));

	// Non Async
	// AStarSearch::AStar(Start, End, OutPath);

	// Async
	//(new FAutoDeleteAsyncTask<AStarAsyncTask>(Start, End, OutPath))->StartBackgroundTask();
}

void AGridActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	if(bDebugVisuals)
	{
		UKismetSystemLibrary::FlushPersistentDebugLines(this);
	
		for (int x = 0; x < Width; x++)
		{
			for (int y = 0; y < Height; y++)
			{
				float const IndexX = x;
				float const IndexY = y;
				
				FVector Loc = FVector(IndexX, IndexY, 0) * GridScale;
				Loc.X += FGenericPlatformMath::RoundToInt(GetActorLocation().X);
				Loc.Y += FGenericPlatformMath::RoundToInt(GetActorLocation().Y);
				
				FVector Extent = (FVector::OneVector * GridScale * 0.5f - 1.0f) * 0.9f;
				Extent.Z = 0;
				FColor Color = FColor::Blue;
			
				DrawDebugBox(GetWorld(), Loc, Extent, Color, true, -1, 0, 10);
			}
		}	
	}
}
