//Copyrighted by XLebeL Studio. All rights reserved.


#include "FPSHUD.h"

void AFPSHUD::DrawHUD()
{
	Super::DrawHUD();


	if(CrosshairTexture)
	{
		//Find the center of the our canvas
		FVector2D Center(Canvas->ClipX * .5f, Canvas->ClipY * .5f);

		// Offset by half of the texture's dimensions so that the center of the texture aligns with the center of the Canvas.
		FVector2D CrossHairDrawPoisiton(Center.X - (CrosshairTexture->GetSurfaceWidth()* .5f),
			Center.Y - (CrosshairTexture->GetSurfaceHeight()*.5f));

		//Draw cross to the center
		
		FCanvasTileItem TileItem(CrossHairDrawPoisiton, CrosshairTexture->Resource, FLinearColor::White);
		TileItem.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(TileItem);
		
	}
}
