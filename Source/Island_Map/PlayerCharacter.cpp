// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
APlayerCharacter::APlayerCharacter()
{
    //Calls Tick() every frame.
    PrimaryActorTick.bCanEverTick = true;

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
    Camera->SetupAttachment(RootComponent);
    Camera->bUsePawnControlRotation = true;

    // Initialize stamina
    MaxStamina = 100.0f;
    StaminaDrainRate = 20.0f;
    StaminaRegenRate = 10.0f;
    CurrentStamina = MaxStamina;

}

void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
    CurrentStamina = MaxStamina;
}

void APlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Drain or regenerate stamina based on running status
    if (bIsRunning)
    {
        CurrentStamina = FMath::Max(0.0f, CurrentStamina - StaminaDrainRate * DeltaTime);
        if (CurrentStamina <= 0.1 * MaxStamina) // Player becomes exhausted at 10% stamina
        {
            bIsRunning = false;
            UE_LOG(LogTemp, Warning, TEXT("Player Exhausted"));
        }
    }
    else
    {
        CurrentStamina = FMath::Min(MaxStamina, CurrentStamina + StaminaRegenRate * DeltaTime);
    }

    // Display stamina on screen
    FString StaminaString = FString::Printf(TEXT("Stamina: %.2f"), CurrentStamina);
    GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Yellow, StaminaString);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
    PlayerInputComponent->BindAxis("TurnCamera", this, &APlayerCharacter::Turn);
    PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::LookUp);

    // Bind functions for running
    PlayerInputComponent->BindAction("Run", IE_Pressed, this, &APlayerCharacter::StartRunning);
    PlayerInputComponent->BindAction("Run", IE_Released, this, &APlayerCharacter::StopRunning);
}

void APlayerCharacter::MoveForward(float InputValue)
{
    if (CurrentStamina <= 0)
    {
        GetCharacterMovement()->MaxWalkSpeed = 100; // Exhausted
    }
    else if (bIsRunning)
    {
        GetCharacterMovement()->MaxWalkSpeed = 750; // Running
    }
    else
    {
        GetCharacterMovement()->MaxWalkSpeed = 350; // Walking
    }

    FVector ForwardDirection = GetActorForwardVector();
    AddMovementInput(ForwardDirection, InputValue);

    // Display speed on screen
    FString SpeedString = FString::Printf(TEXT("Speed: %.2f"), GetCharacterMovement()->Velocity.Size());
    GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Yellow, SpeedString);
}

void APlayerCharacter::MoveRight(float InputValue)
{
    FVector RightDirection = GetActorRightVector();
    AddMovementInput(RightDirection, InputValue);
}

void APlayerCharacter::Turn(float InputValue)
{
    AddControllerYawInput(InputValue);
}

void APlayerCharacter::LookUp(float InputValue)
{
    AddControllerPitchInput(InputValue);
}

void APlayerCharacter::StartRunning()
{
    if (CurrentStamina > 60.0f)
    {
        bIsRunning = true;
        UE_LOG(LogTemp, Warning, TEXT("Started Running"));
    }
}

void APlayerCharacter::StopRunning()
{
    bIsRunning = false;
    UE_LOG(LogTemp, Warning, TEXT("Stopped Running"));
}


