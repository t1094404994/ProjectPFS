// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSCharacter.h"
#include "Engine/Engine.h"

// Sets default values
AFPSCharacter::AFPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//����һ����һ�˳������
	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	//�������������ӵ����������
	FPSCameraComponent->SetupAttachment(GetCapsuleComponent());
	//��������������۾��Ϸ���Զ��
	FPSCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + 88.0f));
	//��pawn�����������ת
	FPSCameraComponent->bUsePawnControlRotation = true;
	//Ϊӵ����Ҵ���һ����һ�˳�ģ�����
	FPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	// ��ģ�ͽ���ӵ����ҿɼ���
	FPSMesh->SetOnlyOwnerSee(true);
	// �� FPS ģ����ӵ� FPS �������
	FPSMesh->SetupAttachment(FPSCameraComponent);
	// ���ò��ֻ�����Ӱ��������һģ�ʹ��ڵļ���
	FPSMesh->bCastDynamicShadow = false;
	FPSMesh->CastShadow = false;
	GetMesh()->SetOwnerNoSee(true);
}

// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (GEngine)
	{
		// ��ʾ������Ϣ���롣-1������ֵ���׸�������˵������������»�ˢ�´���Ϣ��
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("We are using FPSCharacter."));
	}
}

// Called every frame
void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// ���á��ƶ����󶨡�
	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);
	// ���á��鿴���󶨡�
	PlayerInputComponent->BindAxis("Turn", this, &AFPSCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AFPSCharacter::AddControllerPitchInput);
	// ���á��������󶨡�
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPSCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AFPSCharacter::StopJump);
	//���ÿ����
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::Fire);
}
void AFPSCharacter::MoveForward(float Value)
{
	// ��ȷ�ĸ������ǡ�ǰ����������¼�����ͼ��˷����ƶ���
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}
void AFPSCharacter::MoveRight(float Value)
{
	// ��ȷ�ĸ������ǡ����ҡ�������¼�����ͼ��˷����ƶ���
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}
void AFPSCharacter::StartJump()
{
	bPressedJump = true;
}

void AFPSCharacter::StopJump()
{
	bPressedJump = false;
}
void AFPSCharacter::Fire()
{
	//���Է�������
	if (ProjectileClass) {
		//��ȡ������任
		FVector CameraLocation;
		FRotator CameraRocation;
		GetActorEyesViewPoint(CameraLocation, CameraRocation);
		//��MuzzleOffset��������ռ�任������ռ�
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRocation).InverseTransformVector(MuzzleOffset);
		FRotator MuzzleRotaion = CameraRocation;
		//��׼����΢��̧
		MuzzleRotaion.Pitch += 10.0f;
		UWorld* World = GetWorld();
		if (World) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			//��ǹ�ڴ����ɷ�����
			AFPSProjectile* Projectile = World->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotaion, SpawnParams);
			if (Projectile) {
				//���÷�����ĳ�ʼ���
				FVector LaunchDirection = MuzzleRotaion.Vector();
				Projectile->FireInDirection(LaunchDirection);
			}
		}
	}
}

