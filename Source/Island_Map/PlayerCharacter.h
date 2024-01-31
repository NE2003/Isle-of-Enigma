// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class ISLAND_MAP_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
    // Sets default values for this character's properties
    APlayerCharacter();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

    UPROPERTY(EditAnywhere)
    class UCameraComponent* Camera;

    // Stamina properties
    UPROPERTY(EditAnywhere, Category = "Stamina")
    float MaxStamina;

    UPROPERTY(EditAnywhere, Category = "Stamina")
    float CurrentStamina;

    UPROPERTY(EditAnywhere, Category = "Stamina")
    float StaminaDrainRate;

    UPROPERTY(EditAnywhere, Category = "Stamina")
    float StaminaRegenRate;

    // Movement speed properties
    UPROPERTY(EditAnywhere, Category = "Speed")
    float MaxWalkingSpeed;

    UPROPERTY(EditAnywhere, Category = "Speed")
    float AdjustedSpeed;

    bool bIsRunning;

    void MoveForward(float InputValue);
    void MoveRight(float InputValue);

    void Turn(float InputValue);
    void LookUp(float InputValue);

    // Custom function for handling running with stamina
    void StartRunning();
    void StopRunning();
};
