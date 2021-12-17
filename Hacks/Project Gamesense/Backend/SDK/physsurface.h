#pragma once
struct surfacephysicsparams_t
{
	// vphysics physical properties
	float			friction;
	float			elasticity;				// collision elasticity - used to compute coefficient of restitution
	float			density;				// physical density (in kg / m^3)
	float			thickness;				// material thickness if not solid (sheet materials) in inches
	float			dampening;
};

struct surfaceaudioparams_t
{
	// sounds / audio data
	float			reflectivity;		// like elasticity, but how much sound should be reflected by this surface
	float			hardnessFactor;	// like elasticity, but only affects impact sound choices
	float			roughnessFactor;	// like friction, but only affects scrape sound choices

	// audio thresholds
	float			roughThreshold;	// surface roughness > this causes xorstr_("rough") scrapes, < this causes xorstr_("smooth") scrapes
	float			hardThreshold;	// surface hardness > this causes xorstr_("hard") impacts, < this causes xorstr_("soft") impacts
	float			hardVelocityThreshold;	// collision velocity > this causes xorstr_("hard") impacts, < this causes xorstr_("soft") impacts
	// NOTE: Hard impacts must meet both hardnessFactor AND velocity thresholds
};

struct surfacesoundnames_t
{
	unsigned short	stepleft;
	unsigned short	stepright;

	unsigned short	impactSoft;
	unsigned short	impactHard;

	unsigned short	scrapeSmooth;
	unsigned short	scrapeRough;

	unsigned short	bulletImpact;
	unsigned short	rolling;

	unsigned short	breakSound;
	unsigned short	strainSound;
};

struct surfacegameprops_t
{
	public:
		float    maxSpeedFactor; //0x0000
		float    jumpFactor; //0x0004
		char    pad00[0x4]; //0x0008
		float    flPenetrationModifier; //0x000C
		float    flDamageModifier; //0x0010
		unsigned short    material; //0x0014
		char    pad01[0x3];

};//Size=0x0019

struct surfacedata_t
{
	surfacephysicsparams_t	physics;	// physics parameters
	surfaceaudioparams_t	audio;		// audio parameters
	surfacesoundnames_t		sounds;		// names of linked sounds
	surfacegameprops_t		game;		// Game data / properties
};

class IPhysicsSurfaceProps
{
	public:
		virtual ~IPhysicsSurfaceProps( void ) {}

		// parses a text file containing surface prop keys
		virtual int		ParseSurfaceData( const char* pFilename, const char* pTextfile ) = 0;
		// current number of entries in the database
		virtual int		SurfacePropCount( void ) = 0;

		virtual int		GetSurfaceIndex( const char* pSurfacePropName ) = 0;
		virtual void	GetPhysicsProperties( int surfaceDataIndex, float* density, float* thickness, float* friction, float* elasticity ) = 0;

		virtual surfacedata_t*	GetSurfaceData( int surfaceDataIndex ) = 0;
		virtual const char*		GetString( unsigned short stringTableIndex ) = 0;


		virtual const char*		GetPropName( int surfaceDataIndex ) = 0;

		// sets the global index table for world materials
		virtual void	SetWorldMaterialIndexTable( int* pMapArray, int mapSize ) = 0;

		// NOTE: Same as GetPhysicsProperties, but maybe more convenient
		virtual void	GetPhysicsParameters( int surfaceDataIndex, surfacephysicsparams_t* pParamsOut ) = 0;
};
