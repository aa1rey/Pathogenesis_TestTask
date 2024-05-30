// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AICEnemy.h"
#include <Perception/AIPerceptionComponent.h>
#include "Perception/AIPerceptionTypes.h"
#include <Perception/AISenseConfig_Sight.h>
#include <Perception/AISenseConfig_Hearing.h>
#include <Perception/AISenseConfig_Damage.h>
#include "Perception/AIPerceptionSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeTypes.h"

#include "UObject/ConstructorHelpers.h"

using enum EAIState;

AAICEnemy::AAICEnemy(const FObjectInitializer& ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT_Obj(
		TEXT("/Script/AIModule.BehaviorTree'/Game/Blueprints/AI/MovingEnemy/BT_MovingEnemy.BT_MovingEnemy'"));
	if (BT_Obj.Succeeded())
		BehaviorTree = BT_Obj.Object;

	TargetEnemyTag = "AITargetEnemy";

	BehaviourTreeComponent = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("Behavior Component"));
	BlackboardComp = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("Blackboard Component"));

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	AIConfigSight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AIConfigSight"));
	AIConfigHearing = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("AIConfigHearing"));
	AIConfigDamage = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("AIConfigDamage"));

	if (AIPerceptionComponent)
	{
		// Sight
		AIConfigSight->SightRadius = 1000.0f;
		AIConfigSight->LoseSightRadius = 1200.0f;
		AIConfigSight->PeripheralVisionAngleDegrees = 60.0f;
		AIConfigSight->DetectionByAffiliation.bDetectNeutrals = true;
		AIConfigSight->DetectionByAffiliation.bDetectEnemies = true;
		AIConfigSight->DetectionByAffiliation.bDetectFriendlies = true;
		AIConfigSight->PointOfViewBackwardOffset = 150.f;
		AIConfigSight->SetMaxAge(20.f);
		AIPerceptionComponent->ConfigureSense(*AIConfigSight);

		//Hearing
		AIConfigHearing->HearingRange = 3000.0f;
		AIConfigHearing->DetectionByAffiliation.bDetectEnemies = true;
		AIConfigHearing->DetectionByAffiliation.bDetectNeutrals = true;
		AIConfigHearing->DetectionByAffiliation.bDetectFriendlies = true;
		AIConfigHearing->SetMaxAge(10.0f);
		AIPerceptionComponent->ConfigureSense(*AIConfigHearing);

		// Damage
		AIConfigDamage->SetMaxAge(10.f);
		AIPerceptionComponent->ConfigureSense(*AIConfigDamage);

		AIPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());

		AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAICEnemy::SightSenseDetection);
		AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAICEnemy::HearingSenseDetection);
		AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAICEnemy::DamageSenseDetection);
	}

	CurrentState = EAIS_Patrolling;
	TargetLostTime = 10.f;
}

void AAICEnemy::SetNewState(EAIState State)
{
	CurrentState = State;
	GetBlackboard()->SetValueAsEnum(BBK::State, (uint8)CurrentState);
	//ControlledPawn->UpdateMovementSpeed(State);
}

void AAICEnemy::OnPossess(APawn* const PossessedPawn)
{
	Super::OnPossess(PossessedPawn);

	/*ControlledPawn = Cast<ABaseEnemy>(PossessedPawn);
	if (!ControlledPawn) return;
	ControlledPawn->OnTakeAnyDamage.AddDynamic(this, &AAICEnemy::OnTakeAnyDamage);*/

	if (BlackboardComp) BlackboardComp->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	RunBehaviorTree(BehaviorTree);
	BehaviourTreeComponent->StartTree(*BehaviorTree);
	SetNewState(CurrentState);
}

void AAICEnemy::SightSenseDetection(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.Type.Name != "Default__AISense_Sight") return;
	
	if (Actor->ActorHasTag(TargetEnemyTag) && Stimulus.WasSuccessfullySensed()) SetTargetEnemy(Actor);
	
	if (Actor == TargetEnemy && !Stimulus.WasSuccessfullySensed())
		GetWorldTimerManager().SetTimer(TargetLostHandle, this, &AAICEnemy::ClearTargetEnemy, TargetLostTime);
}

void AAICEnemy::HearingSenseDetection(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.Type.Name != "Default__AISense_Hearing" || TargetEnemy) return;
	GetBlackboard()->SetValueAsVector(BBK::TargetLocation, Stimulus.StimulusLocation);

	if (CurrentState == EAIS_Idle || CurrentState == EAIS_Patrolling)
		SetNewState(EAIS_Investigating);
}

void AAICEnemy::DamageSenseDetection(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.Type.Name != "Default__AISense_Damage" || TargetEnemy) return;
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.f, FColor::Red, TEXT("DamageSenseDetection"));
	if (CurrentState == EAIS_Idle || CurrentState == EAIS_Patrolling || CurrentState == EAIS_Investigating)
		SetTargetEnemy(Actor);
}

void AAICEnemy::SetTargetEnemy(AActor* Target)
{
	if (TargetLostHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(TargetLostHandle);
		TargetLostHandle.Invalidate();
	}

	TargetEnemy = Target;
	GetBlackboard()->SetValueAsObject(BBK::TargetEnemy, TargetEnemy);

	SetNewState(EAIS_Attacking);
}

void AAICEnemy::ClearTargetEnemy()
{
	if (!TargetEnemy) return;
	GetBlackboard()->SetValueAsVector(BBK::TargetLocation, TargetEnemy->GetActorLocation());
	TargetEnemy = nullptr;
	GetBlackboard()->SetValueAsObject(BBK::TargetEnemy, nullptr);

	//if (CurrentState == EAIS_Attacking || CurrentState == EAIS_Investigating)
		SetNewState(EAIS_Patrolling);
}

void AAICEnemy::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{

}