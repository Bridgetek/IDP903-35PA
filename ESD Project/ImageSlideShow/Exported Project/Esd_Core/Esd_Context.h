/**
 * This source code ("the Software") is provided by Bridgetek Pte Ltd
 * ("Bridgetek") subject to the licence terms set out
 *   http://brtchip.com/BRTSourceCodeLicenseAgreement/ ("the Licence Terms").
 * You must read the Licence Terms before downloading or using the Software.
 * By installing or using the Software you agree to the Licence Terms. If you
 * do not agree to the Licence Terms then do not download or use the Software.
 *
 * Without prejudice to the Licence Terms, here is a summary of some of the key
 * terms of the Licence Terms (and in the event of any conflict between this
 * summary and the Licence Terms then the text of the Licence Terms will
 * prevail).
 *
 * The Software is provided "as is".
 * There are no warranties (or similar) in relation to the quality of the
 * Software. You use it at your own risk.
 * The Software should not be used in, or for, any medical device, system or
 * appliance. There are exclusions of Bridgetek liability for certain types of loss
 * such as: special loss or damage; incidental loss or damage; indirect or
 * consequential loss or damage; loss of income; loss of business; loss of
 * profits; loss of revenue; loss of contracts; business interruption; loss of
 * the use of money or anticipated savings; loss of information; loss of
 * opportunity; loss of goodwill or reputation; and/or loss of, damage to or
 * corruption of data.
 * There is a monetary cap on Bridgetek's liability.
 * The Software may have subsequently been amended by another user and then
 * distributed by that other user ("Adapted Software").  If so that user may
 * have additional licence terms that apply to those amendments. However, Bridgetek
 * has no liability in relation to those amendments.
 */

#ifndef ESD_CONTEXT___H
#define ESD_CONTEXT___H

#include "Esd_Base.h"
#include "Esd_GpuAlloc.h"
#include "Esd_Scissor.h"
#include "Esd_BitmapHandle.h"
#include "Esd_TouchTag.h"

#ifdef ESD_MEMORYPOOL_ALLOCATOR
#include "Esd_MemoryPool.h"
#endif

#ifdef ESD_LITTLEFS_FLASH
#define ESD_LITTLEFS_READCACHE
#define ESD_LITTLEFS_READPENDING
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4146) //< Unary minus on unsigned type
#endif
#include <lfs.h>
#ifdef _MSC_VER
#pragma warning(pop)
#endif
#endif

#define ESD_MULTIGRADIENT_CACHE

#ifdef __cplusplus
extern "C" {
#endif

/* Runtime context of ESD */
typedef struct
{
	EVE_HalContext HalContext; //< Pointer to current s_Host
	Esd_GpuAlloc GpuAlloc; //< Pointer to current s_GAlloc
	uint32_t Millis; //< Time in milliseconds for current frame
	uint32_t DeltaMs; //< Delta time in milliseconds between frames
	uint32_t Frame; //< Number of times Render has been called
	esd_rgb32_t ClearColor; //< Screen clear color (default is 0x212121)
	uint8_t LoopState; //< Current state of loop

	bool RequestStop; //< Flag to stop the loop

	bool ShowLogo; //< True to pop up logo during next render
	bool SpinnerPopup;

	bool HasReset; //< True if the coprocessor reset during in the previous frame
	bool SwapIdled; //< True if idled during swap
	bool SpinnerPopped; //< Spinner is currently visible
	bool ShowingLogo; //< Logo is currently showing (animation already finished)
	void *CmdOwner; //< Owner of currently long-running coprocessor function (sketch, spinner, etc.)

	void *DeferredFree;

	Esd_HandleState HandleState;

	Esd_GpuHandle MultiGradientGpuHandle; //< Multi gradient rendering bitmap
	uint16_t MultiGradientCell; //< Next available cell in the bitmap
#ifdef ESD_MULTIGRADIENT_CACHE
	int16_t MultiGradientCacheIdx;
	uint64_t MultiGradientCache[8];
#endif

	uint32_t AnimationChannelsReserved; //< Reserved animation channels, bitfield
	uint32_t AnimationChannelsSetup; //< Channels which have been set up, may be reset due to coprocessor fault
	uint32_t AnimationChannelsActive; //< Channels which are active (from register REG_ANIM_ACTIVE)

	Esd_BitmapInfo *BgVideoInfo; //< Currently playing background video
	uint32_t BgVideoTransfered;
	bool bAllFramePlayed;

	Esd_GpuHandle MediaFifoHandle; //< Media fifo used for background video playback

#ifdef ESD_LITTLEFS_FLASH
	bool LfsMounted;
	struct lfs_config LfsConfig;
	lfs_t Lfs; //< LittleFS Context
	lfs_block_t LfsEraseBlock;
	Esd_GpuHandle LfsEraseHandle; //< Handle to 4kB EVE memory for erasing and programming a sector
	bool LfsUnflushed;
	bool LfsCmdFault;
#ifdef ESD_LITTLEFS_READPENDING
	bool LfsEraseFlushed;
#endif
#ifdef ESD_LITTLEFS_READCACHE
	lfs_block_t LfsReadBlock;
	Esd_GpuHandle LfsReadHandle; //< Handle to 4kB EVE memory for reading a sector, may be dropped by GC
	uint32_t LfsLastProg;
#endif
	uint8_t LfsReadBuffer[EVE_FLASH_WRITE_ALIGN]; //< Size of cache matches write align (not read align)
	uint8_t LfsProgBuffer[EVE_FLASH_WRITE_ALIGN]; //< Both caches have the same size
	uint8_t LfsLookaheadBuffer[32]; //< Allocation lookahead
#endif

	/* Callbacks called by Esd_Loop */
	Esd_Callback Start;
	Esd_Callback Update;
	Esd_Callback Render;
	Esd_Callback Idle;
	Esd_Callback End;
	void *UserContext;

} Esd_Context;


#ifdef ESD_FLASH_FILES
#define ESD_FLASH_BT815 (EVE3 - EVE3)
#define ESD_FLASH_BT816 (EVE3 - EVE3)
#define ESD_FLASH_NB 1
#endif

/// Parameters for initializing an ESD context
typedef struct
{
	/* Callbacks called by Esd_Loop */
	Esd_Callback Start;
	Esd_Callback Update;
	Esd_Callback Render;
	Esd_Callback Idle;
	Esd_Callback End;
	void *UserContext;

#ifdef ESD_FLASH_FILES
	/* Flash file path */
	eve_tchar_t FlashFilePaths[ESD_FLASH_NB][260];
#endif

#ifdef ESD_MEMORYPOOL_ALLOCATOR
	/* Memory pool settings */
	size_t MaxPoolMemory;
	size_t IdealPoolMemory;
#endif
} Esd_Parameters;

extern ESD_CORE_EXPORT Esd_Context *Esd_CurrentContext; //< Pointer to current ESD context
extern ESD_CORE_EXPORT EVE_HalContext *Esd_Host; //< Pointer to current EVE hal context
extern ESD_CORE_EXPORT Esd_GpuAlloc *Esd_GAlloc; //< Pointer to current allocator

#ifdef ESD_MEMORYPOOL_ALLOCATOR
extern ESD_CORE_EXPORT Esd_MemoryPool *Esd_MP; //< Pointer to current allocator
#endif

#define ESD_CO_SCRATCH_HANDLE EVE_CO_SCRATCH_HANDLE

ESD_CORE_EXPORT void Esd_SetCurrent(Esd_Context *ec);

ESD_CORE_EXPORT void Esd_Initialize();
ESD_CORE_EXPORT void Esd_Release();

ESD_CORE_EXPORT void Esd_Defaults(Esd_Parameters *ep);
ESD_CORE_EXPORT bool Esd_Open(Esd_Context *ec, Esd_Parameters *ep);
ESD_CORE_EXPORT void Esd_Close(Esd_Context *ec);

/* Main loop, calls Esd_Start, Esd_Update, Esd_WaitSwap, and Esd_Stop */
ESD_CORE_EXPORT void Esd_Loop(Esd_Context *ec);

ESD_CORE_EXPORT void Esd_Start(Esd_Context *ec);
ESD_CORE_EXPORT void Esd_Update(Esd_Context *ec);
ESD_CORE_EXPORT void Esd_Render(Esd_Context *ec);
ESD_CORE_EXPORT bool Esd_WaitSwap(Esd_Context *ec);
ESD_CORE_EXPORT void Esd_Stop(Esd_Context *ec);

/// A function to get milliseconds for current frame
ESD_FUNCTION(Esd_GetMillis, Type = uint32_t, DisplayName = "Get Milliseconds", Category = EsdUtilities, Inline, Include = "Esd_Core.h")
static inline uint32_t Esd_GetMillis() { return Esd_CurrentContext->Millis; }

/// A function to get the difference in milliseconds since last frame Update call
ESD_FUNCTION(Esd_GetDeltaMs, Type = uint32_t, DisplayName = "Get Delta Ms", Category = EsdUtilities, Inline, Include = "Esd_Core.h")
static inline uint32_t Esd_GetDeltaMs() { return Esd_CurrentContext->DeltaMs; }

/// A function to get the current HAL context data structure
ESD_FUNCTION(Esd_GetHost, Type = EVE_HalContext *, DisplayName = "Get EVE Host", Category = EsdUtilities, Inline, Include = "Esd_Core.h")
static inline EVE_HalContext *Esd_GetHost() { return Esd_Host; }

#ifdef ESD_MEMORYPOOL_ALLOCATOR
// ESD_FUNCTION(Esd_GetMemoryPool, Type = Esd_MemoryPool *, DisplayName = "Get EVE Memory Pool", Category = EsdUtilities, Inline, Include = "Esd_Core.h")
static inline Esd_MemoryPool *Esd_GetMemoryPool() { return Esd_MP; }
#endif

#ifdef __cplusplus
}
#endif

#endif /* #ifndef ESD_CONTEXT__H */

/* end of file */
