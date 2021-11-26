//Copyrighted by XLebeL Studio. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Engine/Canvas.h" 
#include "FPSHUD.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEST_API AFPSHUD : public AHUD
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* CrosshairTexture;

public:
	//Primary call for draw Hud
	virtual void DrawHUD() override;
	
};
