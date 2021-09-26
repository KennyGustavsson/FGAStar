#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FGPlayer.generated.h"

class UCameraComponent;

UCLASS(BlueprintType, Blueprintable)
class FGASTAR_API AFGPlayer : public APawn
{
	GENERATED_BODY()

// Functions
public:
	AFGPlayer();
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void HorizontalMovement(float const Value);
	UFUNCTION()
	void VerticalMovement(float const Value);
	UFUNCTION()
	void UpMovement(float const Value);	
	UFUNCTION()
	void Click();

	// Unit Piece
	UFUNCTION()
	void MoveUnit(float const DeltaTime);
	UFUNCTION()
	void UpdateUnitPath(FVector const UnitLocation);

// Components
	UPROPERTY(EditDefaultsOnly)
	USceneComponent* Root;
	UPROPERTY(EditDefaultsOnly)
	UCameraComponent* CameraComponent;

// Properties
private:
	UPROPERTY(Transient)
	FVector Input;
	UPROPERTY(EditDefaultsOnly)
	FRotator CameraRotation = FRotator::ZeroRotator;
	UPROPERTY(Transient)
	class UTile* StartTile;
	UPROPERTY(Transient)
	class UTile* EndTile;
	UPROPERTY(Transient)
	APlayerController* PlayerController;
	UPROPERTY(EditInstanceOnly)
	class AGridActor* GridActor;
	UPROPERTY(EditDefaultsOnly)
	float ZHeight = 500.0f;
	UPROPERTY(EditDefaultsOnly)
	float MovementSpeed = 100.0f;
	UPROPERTY(EditDefaultsOnly)
	float InteractionLength = 10000.0f;

	// Unit Piece
	UPROPERTY(Transient)
	TArray<FVector2D> OutPath;
	UPROPERTY(Transient)
	FQuat StartRot;
	UPROPERTY(EditInstanceOnly)
	AActor* UnitActor;
	UPROPERTY(EditDefaultsOnly)
	float UnitMovementSpeed = 100.0f;
	UPROPERTY(EditDefaultsOnly)
	float RotationSpeed = 3.0f;
	UPROPERTY(EditDefaultsOnly)
	float PointDistanceThreshold = 2.0f;
	UPROPERTY(Transient)
	float PointDistanceThresholdSq;
	UPROPERTY(Transient)
	float RotAccumulator;
	UPROPERTY(Transient)
	int CurrentIndex = 0;
};
