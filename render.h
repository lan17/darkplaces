/*
Copyright (C) 1996-1997 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

// refresh.h -- public interface to refresh functions

#define	MAXCLIPPLANES	11

#define	TOP_RANGE		16			// soldier uniform colors
#define	BOTTOM_RANGE	96

//=============================================================================

typedef struct efrag_s
{
	struct mleaf_s		*leaf;
	struct efrag_s		*leafnext;
	struct entity_s		*entity;
	struct efrag_s		*entnext;
} efrag_t;

#define RENDER_STEP 1
#define RENDER_GLOWTRAIL 2
#define RENDER_VIEWMODEL 4

// LordHavoc: made this more compact, and added some more fields
typedef struct
{
	double	time; // time this state was updated
	vec3_t	origin;
	vec3_t	angles;
	int		effects;
	unsigned short modelindex;
	unsigned short frame;
	byte	colormap;
	byte	skin;
	byte	alpha;
	byte	scale;
	byte	glowsize;
	byte	glowcolor;
	byte	colormod;
	byte	flags;
	byte	active;
} entity_state_t;

typedef struct entity_s
{
	entity_state_t			state_baseline;	// baseline for entity
	entity_state_t			state_previous;	// previous state (interpolating from this)
	entity_state_t			state_current;	// current state (interpolating to this)

	struct
	{
		vec3_t					origin;
		vec3_t					angles;	

		// LordHavoc: added support for alpha transprency and other effects
		float					alpha;			// opacity (alpha) of the model
		float					colormod[3];	// color tint for model
		float					scale;			// size the model is shown
		float					glowsize;		// how big the glow is
		byte					glowcolor;		// color of glow and particle trail (paletted)
		byte					flags;			// render flags

		struct model_s			*model;			// NULL = no model
		int						frame;			// current desired frame (usually identical to frame2, but frame2 is not always used)
//		struct efrag_s			*efrag;			// linked list of efrags
		int						colormap;
		int						effects;		// light, particles, etc
		int						skinnum;		// for Alias models

		int						visframe;		// last frame this entity was found in an active leaf

		struct model_s			*lerp_model;	// lerp resets when model changes
		float					lerp_starttime;	// start of this transition
		int						frame1;			// frame that the model is interpolating from
		int						frame2;			// frame that the model is interpolating to
		double					framelerp;		// interpolation factor, usually computed from lerp_starttime
		double					frame1start;	// time frame1 began playing (for framegroup animations)
		double					frame2start;	// time frame2 began playing (for framegroup animations)
//		float					syncbase;		// for client-side animations

		int						dlightframe;	// dynamic lighting
		int						dlightbits[8];
		
		float					trail_time;
	// FIXME: could turn these into a union
//		int						trivial_accept;
//		struct mnode_s			*topnode;		// for bmodels, first world node that splits bmodel, or NULL if not split
	}
	render;
} entity_t;

typedef struct
{
	vrect_t		vrect;				// subwindow in video for refresh
									// FIXME: not need vrect next field here?
	/*
	vrect_t		aliasvrect;			// scaled Alias version
	int			vrectright, vrectbottom;	// right & bottom screen coords
	int			aliasvrectright, aliasvrectbottom;	// scaled Alias versions
	float		vrectrightedge;			// rightmost right edge we care about,
										//  for use in edge list
	float		fvrectx, fvrecty;		// for floating-point compares
	float		fvrectx_adj, fvrecty_adj; // left and top edges, for clamping
	int			vrect_x_adj_shift20;	// (vrect.x + 0.5 - epsilon) << 20
	int			vrectright_adj_shift20;	// (vrectright + 0.5 - epsilon) << 20
	float		fvrectright_adj, fvrectbottom_adj;
										// right and bottom edges, for clamping
	float		fvrectright;			// rightmost edge, for Alias clamping
	float		fvrectbottom;			// bottommost edge, for Alias clamping
	float		horizontalFieldOfView;	// at Z = 1.0, this many X is visible 
										// 2.0 = 90 degrees
	float		xOrigin;			// should probably always be 0.5
	float		yOrigin;			// between be around 0.3 to 0.5
	*/

	vec3_t		vieworg;
	vec3_t		viewangles;
	
	float		fov_x, fov_y;

//	int			ambientlight;
} refdef_t;


//
// refresh
//


extern	refdef_t	r_refdef;
extern vec3_t	r_origin, vpn, vright, vup;

void R_Init (void);
void R_RenderView (void); // must set r_refdef first
void R_ViewChanged (vrect_t *pvrect, int lineadj, float aspect); // called whenever r_refdef or vid change

// LordHavoc: changed this for sake of GLQuake
void R_InitSky (byte *src, int bytesperpixel); // called at level load

//void R_InitEfrags (void);
//void R_AddEfrags (entity_t *ent);
//void R_RemoveEfrags (entity_t *ent);
//void R_StoreEfrags (efrag_t **ppefrag);

int R_VisibleCullBox (vec3_t mins, vec3_t maxs);

void R_NewMap (void);

#include "r_decals.h"

void R_ParseParticleEffect (void);
void R_RunParticleEffect (vec3_t org, vec3_t dir, int color, int count);
void R_RocketTrail (vec3_t start, vec3_t end, int type, entity_t *ent);
void R_RocketTrail2 (vec3_t start, vec3_t end, int type, entity_t *ent);
void R_SparkShower (vec3_t org, vec3_t dir, int count);
void R_BloodPuff (vec3_t org, vec3_t vel, int count);
void R_FlameCube (vec3_t mins, vec3_t maxs, int count);
void R_Flames (vec3_t org, vec3_t vel, int count);

void R_EntityParticles (entity_t *ent);
void R_BlobExplosion (vec3_t org);
void R_ParticleExplosion (vec3_t org, int smoke);
void R_ParticleExplosion2 (vec3_t org, int colorStart, int colorLength);
void R_LavaSplash (vec3_t org);
void R_TeleportSplash (vec3_t org);

void R_NewExplosion(vec3_t org);

void R_PushDlights (void);
void R_DrawWorld (void);
//void R_RenderDlights (void);
void R_DrawParticles (void);
void R_MoveParticles (void);
void R_DrawExplosions (void);
void R_MoveExplosions (void);
