// The user must define CLA_C in the project linker settings if using the
// CLA C compiler
// Project Properties -> C2000 Linker -> Advanced Options -> Command File
// Preprocessing -> --define
#ifdef CLA_C
// Define a size for the CLA scratchpad area that will be used
// by the CLA compiler for local symbols and temps
// Also force references to the special symbols that mark the
// scratchpad are.
CLA_SCRATCHPAD_SIZE = 0x100;
--undef_sym=__cla_scratchpad_end
--undef_sym=__cla_scratchpad_start
#endif //CLA_C

MEMORY
{
PAGE 0 :
   /* BEGIN is used for the "boot to SARAM" bootloader mode   */

	BEGIN				: origin = 0x000000,	length = 0x000002
	BOOT_RSVD			: origin = 0x000002,	length = 0x000121     /* Part of M0, BOOT rom will use this for stack */
	RAMM0				: origin = 0x000123,	length = 0x0002DD
	RAMM1				: origin = 0x000400,	length = 0x0003F8	/* on-chip RAM block M1 */
//   RAMM1_RSVD      : origin = 0x0007F8, length = 0x000008		/* Reserve and do not use for code as per the errata advisory "Memory: Prefetching Beyond Valid Memory" */

	RAMD0				: origin = 0x00B000,	length = 0x000800
	RAMD1				: origin = 0x00B800,	length = 0x000800

	RESET           	: origin = 0x3FFFC0,	length = 0x000002

	RAMLS0          	: origin = 0x008000,	length = 0x000800
	RAMLS1          	: origin = 0x008800,	length = 0x000800
	RAMLS2      		: origin = 0x009000,	length = 0x000800
	RAMLS3      		: origin = 0x009800,	length = 0x000800
	RAMLS4      	    : origin = 0x00A000,	length = 0x000800
	RAMLS5				: origin = 0x00A800,	length = 0x000800

	// RAMGS0				: origin = 0x00C000,	length = 0x001000
	GAINVALS			: origin = 0x00C000,	length = 0x000100
	SAMPLE_TO_CPU2		: origin = 0x00C100,	length = 0x000100

	// RAMGS1				: origin = 0x00D000,	length = 0x001000
	SAMPLE_FROM_CPU2	: origin = 0x00D000,	length = 0x000100

	// Used for CPU1 .ebss
	RAMGS2_3			: origin = 0x00E000,	length = 0x002000

	// RAMGS2				: origin = 0x00E000,	length = 0x001000
	// RAMGS3				: origin = 0x00F000,	length = 0x001000

	RAMGS4				: origin = 0x010000,	length = 0x001000
	RAMGS5           	: origin = 0x011000,	length = 0x001000
	RAMGS6           	: origin = 0x012000,	length = 0x001000
	RAMGS7           	: origin = 0x013000,	length = 0x001000
	RAMGS8           	: origin = 0x014000,	length = 0x001000
	RAMGS9           	: origin = 0x015000,	length = 0x001000
	RAMGS10          	: origin = 0x016000,	length = 0x001000
	RAMGS11          	: origin = 0x017000,	length = 0x001000     /* Only Available on F28379D, F28377D, F28375D devices. Remove line on other devices. */
	RAMGS12          	: origin = 0x018000,	length = 0x001000     /* Only Available on F28379D, F28377D, F28375D devices. Remove line on other devices. */
	RAMGS13          	: origin = 0x019000,	length = 0x001000     /* Only Available on F28379D, F28377D, F28375D devices. Remove line on other devices. */
	RAMGS14          	: origin = 0x01A000,	length = 0x001000     /* Only Available on F28379D, F28377D, F28375D devices. Remove line on other devices. */
	RAMGS15          	: origin = 0x01B000,	length = 0x000FF8     /* Only Available on F28379D, F28377D, F28375D devices. Remove line on other devices. */

//   RAMGS11     : origin = 0x017000, length = 0x000FF8   /* Uncomment for F28374D, F28376D devices */

//   RAMGS11_RSVD : origin = 0x017FF8, length = 0x000008    /* Reserve and do not use for code as per the errata advisory "Memory: Prefetching Beyond Valid Memory" */

//   RAMGS15_RSVD : origin = 0x01BFF8, length = 0x000008    /* Reserve and do not use for code as per the errata advisory "Memory: Prefetching Beyond Valid Memory" */
                                                            /* Only on F28379D, F28377D, F28375D devices. Remove line on other devices. */

	CLA1_MSGRAMLOW		: origin = 0x001480,	length = 0x000080
	CLA1_MSGRAMHIGH		: origin = 0x001500,	length = 0x000080

	CPU2TOCPU1RAM		: origin = 0x03F800,	length = 0x000400
    CPU1TOCPU2RAM		: origin = 0x03FC00,	length = 0x000400
}

SECTIONS
{
	codestart			: > BEGIN,      PAGE = 0
	.text				: >> RAMD0 | RAMD1 | RAMLS0 | RAMLS1 | RAMLS2 | RAMLS3,    PAGE = 0
	.cinit				: > RAMLS3,      PAGE = 0
	.switch				: > RAMM0,      PAGE = 0
	.reset				: > RESET,      PAGE = 0, TYPE = DSECT /* not used, */
	.stack				: > RAMM1,      PAGE = 0

	.pinit				: > RAMM0,     PAGE = 0
	.ebss				: > RAMGS2_3,    PAGE = 0
	.econst				: > RAMLS3,    PAGE = 0
	.esysmem			: > RAMLS3,    PAGE = 0

	GainVals			: > GAINVALS,	PAGE = 0
	SampleToCPU2		: > SAMPLE_TO_CPU2, PAGE = 0
	SampleFromCPU2		: > SAMPLE_FROM_CPU2, PAGE = 0

    /* CLA specific sections */
	CLAData				: > RAMLS4, PAGE=0
	Cla1Prog			: > RAMLS5, PAGE=0

	Cla1ToCpuMsgRAM		: > CLA1_MSGRAMLOW,   PAGE = 0
	CpuToCla1MsgRAM		: > CLA1_MSGRAMHIGH,  PAGE = 0

#ifdef __TI_COMPILER_VERSION__
   #if __TI_COMPILER_VERSION__ >= 15009000
    .TI.ramfunc : {} > RAMM0,      PAGE = 0
   #else
    ramfuncs    : > RAMM0      PAGE = 0   
   #endif
#endif

	/* The following section definitions are required when using the IPC API Drivers */
	GROUP : > CPU1TOCPU2RAM, PAGE = 0
	{
	    PUTBUFFER
	    PUTWRITEIDX
	    GETREADIDX
	}

	GROUP : > CPU2TOCPU1RAM, PAGE = 0
	{
	    GETBUFFER :    TYPE = DSECT
	    GETWRITEIDX :  TYPE = DSECT
	    PUTREADIDX :   TYPE = DSECT
	}

	Cpu1ToCpu2RAM	: > CPU1TOCPU2RAM,   PAGE = 0
	Cpu2ToCpu1RAM	: > CPU2TOCPU1RAM,   PAGE = 0

#ifdef CLA_C
   /* CLA C compiler sections */
   //
   // Must be allocated to memory the CLA has write access to
   //
	CLAscratch		:
                     { *.obj(CLAscratch)
                     . += CLA_SCRATCHPAD_SIZE;
                     *.obj(CLAscratch_end) } >  RAMLS4,  PAGE = 0

	.scratchpad		: > RAMLS4,       PAGE = 0
	.bss_cla		: > RAMLS4,       PAGE = 0
	.const_cla	    : > RAMLS4,       PAGE = 0
#endif //CLA_C
}

/*
//===========================================================================
// End of file.
//===========================================================================
*/
