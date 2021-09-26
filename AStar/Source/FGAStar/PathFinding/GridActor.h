#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grid/PathFindingGrid.h"
#include "GridActor.generated.h"

UCLASS()
class FGASTAR_API AGridActor : public AActor
{
	GENERATED_BODY()

// Functions
public:
	AGridActor();
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

// Properties
	UPROPERTY(Transient)
	class UPathFindingGrid* MyGrid;
	UPROPERTY(EditInstanceOnly)
	int Width = 10;
	UPROPERTY(EditInstanceOnly)
	int Height = 10;
	UPROPERTY(EditInstanceOnly)
	int GridScale = 100.0f;
	UPROPERTY(EditAnywhere)
	bool bDebugVisuals = true;
};
