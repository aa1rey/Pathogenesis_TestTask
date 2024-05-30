// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIState.h"
#include "AICEnemy.generated.h"

struct FAIStimulus;
class UBlackboardComponent;

UCLASS()
class PATHOGENESISTESTTASK_API AAICEnemy : public AAIController
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI|Actor Components", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTreeComponent* BehaviourTreeComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Actor Components") class UAIPerceptionComponent* AIPerceptionComponent;
	UBlackboardComponent* BlackboardComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Sense") class UAISenseConfig_Sight* AIConfigSight;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Sense") class UAISenseConfig_Hearing* AIConfigHearing;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Sense") class UAISenseConfig_Damage* AIConfigDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))	class UBehaviorTree* BehaviorTree;
	UPROPERTY(EditDefaultsOnly, Category = "AI") EAIState CurrentState;
	UPROPERTY(EditDefaultsOnly, Category = "AI|Target") float TargetLostTime;
	class ABaseEnemy* ControlledPawn;
	UPROPERTY(EditDefaultsOnly) FName TargetEnemyTag;
	UPROPERTY(BlueprintReadOnly) AActor* TargetEnemy;
	FTimerHandle TargetLostHandle;

public:
	AAICEnemy(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	UBlackboardComponent* GetBlackboard() const { return BlackboardComp; }
	UFUNCTION(BlueprintCallable) void SetNewState(EAIState State);

protected:
	virtual void OnPossess(APawn* const PossessedPawn) override;
	UFUNCTION() void SightSenseDetection(AActor* Actor, FAIStimulus Stimulus);
	UFUNCTION() void HearingSenseDetection(AActor* Actor, FAIStimulus Stimulus);
	UFUNCTION() void DamageSenseDetection(AActor* Actor, FAIStimulus Stimulus);
	void SetTargetEnemy(AActor* Target);
	void ClearTargetEnemy();

	UFUNCTION() void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
};
