// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "MaroonedCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class ACraftable; 
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AMaroonedCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SwitchWeaponAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Marooned", meta = (AllowPrivateAccess = "true"))
	int CurrentWeaponIndex; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Marooned", meta = (AllowPrivateAccess = "true"))
	TArray<ACraftable*> Weapons; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Marooned", meta = (AllowPrivateAccess = "true"))
	float WalkSpeed = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Marooned", meta = (AllowPrivateAccess = "true"))
	float SprintSpeed = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Marooned", meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

    // Energy variable exposed to Blueprints
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Marooned", meta = (AllowPrivateAccess = "true"))
    float Energy;

    // Maximum energy
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Marooned", meta = (AllowPrivateAccess = "true"))
    float MaxEnergy = 100.f;

    // Energy drain rate while sprinting
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Marooned", meta = (AllowPrivateAccess = "true"))
    float EnergyDrainRate = 10.f;

    // Energy regeneration rate
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Marooned", meta = (AllowPrivateAccess = "true"))
    float EnergyRegenRate = 5.f;

	float energyTimerPeriod = 0.1f; // timers loop every 0.1 seconds

public:
	AMaroonedCharacter();
	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void SwitchWeapon(const FInputActionValue& Value); 

	void StartSprinting();
	void StopSprinting();

	FTimerHandle EnergyDrainTimerHandle;
	FTimerHandle EnergyRegenTimerHandle;
    void DrainEnergy();
    void RegenerateEnergy();

    // Blueprint extension points
    UFUNCTION(BlueprintImplementableEvent, Category = "Marooned")
    void OnDrainEnergy();

    UFUNCTION(BlueprintImplementableEvent, Category = "Marooned")
    void OnRegenerateEnergy();

protected:

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Marooned")
	void OnSwitchWeapon(int weaponIndex);
};

