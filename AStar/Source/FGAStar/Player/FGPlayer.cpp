#include "FGPlayer.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "FGAStar/PathFinding/GridActor.h"
#include "FGAStar/PathFinding/AStar/AStarAsyncTask.h"
#include "FGAStar/PathFinding/Grid/PathFindingGrid.h"
#include "FGAStar/PathFinding/Tile/Tile.h"
#include "Kismet/KismetMathLibrary.h"

AFGPlayer::AFGPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(Root);
}

void AFGPlayer::BeginPlay()
{
	Super::BeginPlay();

	SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, ZHeight));
	CameraComponent->SetRelativeRotation(CameraRotation);

	PlayerController = Cast<APlayerController>(GetController());

	if(PlayerController)
	{
		PlayerController->bShowMouseCursor = true;
		PlayerController->bEnableClickEvents = true;
		PlayerController->bEnableMouseOverEvents = true;
	}

	PointDistanceThresholdSq = FMath::Square(PointDistanceThreshold);
}

void AFGPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Horizontal", this, &AFGPlayer::HorizontalMovement);
	PlayerInputComponent->BindAxis("Vertical", this, &AFGPlayer::VerticalMovement);
	PlayerInputComponent->BindAxis("Up", this, &AFGPlayer::UpMovement);
	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &AFGPlayer::Click);
}

void AFGPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Input.Normalize();
	AddActorWorldOffset(Input * DeltaTime * MovementSpeed);

	for (FVector2D const Point : OutPath)
	{
		DrawDebugSphere(GetWorld(), FVector(Point.X, Point.Y, 0), 10, 5, FColor::White, false, -1, 0, 5);
	}

	MoveUnit(DeltaTime);
}

void AFGPlayer::HorizontalMovement(float const Value)
{
	Input.X = Value;
}

void AFGPlayer::VerticalMovement(float const Value)
{
	Input.Y = Value;
}

void AFGPlayer::UpMovement(float const Value)
{
	Input.Z = Value;
}

void AFGPlayer::Click()
{
	if(!GridActor)
	{
		return;
	}

	if(PlayerController)
	{
		FVector WorldLoc, WorldDir;
		if(PlayerController->DeprojectMousePositionToWorld(WorldLoc, WorldDir))
		{
			FVector const End = WorldLoc + (WorldDir * InteractionLength);

			FHitResult Hit;
			GetWorld()->LineTraceSingleByChannel(Hit, WorldLoc, End, ECC_Visibility);
			
			if(Hit.bBlockingHit)
			{
				EndTile = GridActor->MyGrid->GetNearestTileFromWorldLocation(Hit.Location);
			}
			else
			{
				EndTile = GridActor->MyGrid->GetNearestTileFromWorldLocation(End);
			}
			
			UpdateUnitPath(UnitActor->GetActorLocation());
		}
	}
}

void AFGPlayer::MoveUnit(float const DeltaTime)
{
	if(OutPath.Num() < 1 || !(CurrentIndex < OutPath.Num()) || !UnitActor)
	{
		return;
	}

	FVector const CurrentPoint = FVector(OutPath[CurrentIndex].X, OutPath[CurrentIndex].Y, 0);
	FVector const Direction = (CurrentPoint - UnitActor->GetActorLocation()).GetSafeNormal();
	FQuat const LookAt = UKismetMathLibrary::FindLookAtRotation(UnitActor->GetActorLocation(), CurrentPoint).Quaternion();

	RotAccumulator = FMath::Clamp(RotAccumulator + (RotationSpeed * DeltaTime), 0.0f,1.0f);

	UnitActor->AddActorWorldOffset(Direction * UnitMovementSpeed * DeltaTime);
	UnitActor->SetActorRotation(FQuat::Slerp(StartRot, LookAt, RotAccumulator));
	
	if(FVector::DistSquared(UnitActor->GetActorLocation(), CurrentPoint) <= PointDistanceThresholdSq)
	{
		StartRot = UnitActor->GetActorQuat();
		RotAccumulator = 0.0f;
		CurrentIndex++;
	}
}

void AFGPlayer::UpdateUnitPath(FVector const UnitLocation)
{
	CurrentIndex = 0;
	RotAccumulator = 0.0f;
	StartRot = UnitActor->GetActorQuat();

	StartTile = GridActor->MyGrid->GetNearestTileFromWorldLocation(UnitLocation);
	OutPath.Empty();
	(new FAutoDeleteAsyncTask<AStarAsyncTask>(StartTile, EndTile, OutPath))->StartBackgroundTask();
}
     