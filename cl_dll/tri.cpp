//========= Copyright © 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================

// Triangle rendering, if any

#include "hud.h"
#include "cl_util.h"

// Triangle rendering apis are in gEngfuncs.pTriAPI

#include "const.h"
#include "entity_state.h"
#include "cl_entity.h"
#include "triangleapi.h"
#include "Exports.h"

#include "particleman.h"
#include "tri.h"
#include "com_model.h"
extern IParticleMan* g_pParticleMan;

/*
=================
HUD_DrawNormalTriangles

Non-transparent triangles-- add them here
=================
*/
void DLLEXPORT HUD_DrawNormalTriangles()
{
	//	RecClDrawNormalTriangles();

	gHUD.m_Spectator.DrawOverview();
}


/*
=================
HUD_DrawTransparentTriangles

Render any triangles with transparent rendermode needs here
=================
*/
void DLLEXPORT HUD_DrawTransparentTriangles()
{
	//	RecClDrawTransparentTriangles();


	if (g_pParticleMan)
		g_pParticleMan->Update();

	extern Vector v_origin, v_angles;

	static model_t* texture = 0;
	static float nextParticle = 0;

	if ( g_pParticleMan )
	{
		const float clTime = gEngfuncs.GetClientTime();

		if (texture == 0)
		{
			texture = (model_t*)gEngfuncs.GetSpritePointer(SPR_Load("sprites/hotglow.spr"));
		}

		if (texture && nextParticle <= clTime)
		{
			Vector forward, right, up;
			AngleVectors(v_angles, forward, right, up);

			const Vector particleOrigin = v_origin + forward * 64.0f + up * gEngfuncs.pfnRandomFloat(-1.0f, 1.0f) * 32.0f;
			CBaseParticle *particle = g_pParticleMan->CreateParticle(particleOrigin, Vector(0.0f, 0.0f, 0.0f), texture, 32.0f, 255.0f, "particle");

			particle->SetLightFlag(LIGHT_NONE);
			particle->SetCullFlag(CULL_PVS);
			particle->SetRenderFlag(RENDER_FACEPLAYER);

			particle->m_iRendermode = kRenderTransAdd;
			particle->m_vColor = Vector(255, 255, 255);

			particle->m_flDieTime = clTime + 1.0f;
			nextParticle = clTime + 0.2f;
		}
	}
}
