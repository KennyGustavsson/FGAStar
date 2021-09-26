#pragma once
#include "CoreMinimal.h"
#include "FGAStar/PathFinding/Grid/PathFindingGrid.h"
#include "GameFramework/Actor.h"
#include "GridObstacle.generated.h"

UCLASS()
class FGASTAR_API AGridObstacle : public AActor
{
	GENERATED_BODY()

// Components
public:
	UPROPERTY(EditDefaultsOnly)
	USceneComponent* Root;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;
	
// Functions
	AGridObstacle();

// Properties
	FObstacleInfo ObstacleInfo;
	UPROPERTY(EditAnywhere)
	int Cost = 0;
	UPROPERTY(EditAnywhere)
	bool bIsWall = false;
};
