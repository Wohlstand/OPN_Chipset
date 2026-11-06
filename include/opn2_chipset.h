/*
 * Interfaces over Yamaha OPN2 (YM2612) chip emulators
 *
 * Copyright (c) 2017-2026 Vitaly Novichkov (Wohlstand)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef OPN2_CHIPSET_H
#define OPN2_CHIPSET_H

#ifdef __cplusplus
extern "C" {
#endif

#define OPN_OPN2_SAMPLE_RATE        53267
#define OPN_OPNA_SAMPLE_RATE        55466

#include <stddef.h>

#ifndef OPNMIDI_H
#   if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
#       include <stdint.h>
typedef uint8_t         OPN2_UInt8;
typedef uint16_t        OPN2_UInt16;
typedef uint32_t        OPN2_UInt32;
typedef int8_t          OPN2_SInt8;
typedef int16_t         OPN2_SInt16;
#   else
typedef unsigned char   OPN2_UInt8;
typedef unsigned short  OPN2_UInt16;
typedef unsigned int    OPN2_UInt32;
typedef char            OPN2_SInt8;
typedef short           OPN2_SInt16;
typedef int             OPN2_SInt32;
#   endif

/**
 * @brief List of available OPN2 emulators (IT MUST MATCH WITH libOPNMIDI!!!)
 */
enum Opn2_Emulator
{
    /*! Mame YM2612 */
    OPNMIDI_EMU_MAME = 0,
    /*! Nuked OPN2 (YM3438 mode) */
    OPNMIDI_EMU_NUKED_YM3438,
    OPNMIDI_EMU_NUKED = OPNMIDI_EMU_NUKED_YM3438,
    /*! GENS */
    OPNMIDI_EMU_GENS,
    /*! YMFM OPN2 */
    OPNMIDI_EMU_YMFM_OPN2,
    /*! Neko Project II OPNA */
    OPNMIDI_EMU_NP2,
    /*! Mame YM2608 */
    OPNMIDI_EMU_MAME_2608,
    /*! YMFM OPNA */
    OPNMIDI_EMU_YMFM_OPNA,
    /*! VGM file dumper (required for MIDI2VGM) */
    OPNMIDI_VGM_DUMPER,
    /*! Nuked OPN2 (YM2612 mode) */
    OPNMIDI_EMU_NUKED_YM2612,
    /*! Very low-level variant of Nuked OPN2 (YM2612) */
    OPNMIDI_EMU_NUKED_YM2612_LLE,
    /*! Very low-level variant of Nuked OPNA (YM2608) */
    OPNMIDI_EMU_NUKED_YM2608_LLE,
    /*! Very low-level variant of Nuked OPN2 (YM3438) */
    OPNMIDI_EMU_NUKED_YM3438_LLE,
    /*! Very low-level variant of Nuked OPN2 (YMF276) */
    OPNMIDI_EMU_NUKED_YMF276_LLE,

    /* DEPRECATED AND REMOVED EMULATORS (must be ALWAYS at the TAIL!) */
    /*! Genesis Plus GX (a fork of Mame YM2612): Somewhere broken and unstable */
    OPNMIDI_EMU_GX,
    /*! PMDWin OPNA: Very old and very glitchy */
    OPNMIDI_EMU_PMDWIN,
    
    /*! Count instrument on the level */
    OPNMIDI_EMU_end,
};

#endif /* OPNMIDI_H */


#ifdef OPNCHIPSET_BUILD
#   ifndef OPNCHIPSET_DECLSPEC
#       if defined (_WIN32) && defined(OPNCHIPSET_BUILD_DLL)
#           define OPNCHIPSET_DECLSPEC __declspec(dllexport)
#       else
#           define OPNCHIPSET_DECLSPEC
#       endif
#   endif
#else
#   define OPNCHIPSET_DECLSPEC
#endif



typedef struct OPN_EMU_ChipsetCtx_t OPN_EMU_ChipsetCtx;

enum OPN_Chipset_ChipFamily
{
    OPNChipType_OPN2 = 0,
    OPNChipType_OPNA = 1,
    OPNChipType_Count
};

/**
 * @brief Initialise the instance of the single chip emulator (to emulate multiple chips, create multiple instances using same function!)
 * @param emu Emulator type to initialise
 * @param family Tell emulator to work as the chip of family (it will affect the frequency only)
 * @return Instance of the chip emulator
 */
extern OPNCHIPSET_DECLSPEC OPN_EMU_ChipsetCtx *opn_chipset_init(enum Opn2_Emulator emu, enum OPN_Chipset_ChipFamily family);

/**
 * @brief Destroy the previously initialised chip emulator (free memory)
 * @param ctx Instance of the chip emulator
 */
extern OPNCHIPSET_DECLSPEC void opn_chipset_free(OPN_EMU_ChipsetCtx *ctx);

/**
 * @brief Get the current clock rate of the chip
 * @param ctx Instance of the chip emulator
 * @return Value of the current clock rate
 */
extern OPNCHIPSET_DECLSPEC OPN2_UInt32 opn_chipset_clockRate(OPN_EMU_ChipsetCtx *ctx);

/**
 * @brief Get the native clock rate of the chip
 * @param ctx Instance of the chip emulator
 * @return Value of the native clock rate
 */
extern OPNCHIPSET_DECLSPEC OPN2_UInt32 opn_chipset_nativeClockRate(OPN_EMU_ChipsetCtx *ctx);

/**
 * @brief Can this emulator run at PCM rate to reduce the CPU load?
 * @param ctx Instance of the chip emulator
 * @return 1 - Can, 0 - can't
 */
extern OPNCHIPSET_DECLSPEC int opn_chipset_canRunAtPcmRate(OPN_EMU_ChipsetCtx *ctx);

/**
 * @brief Does this emulator works at PCM rate?
 * @param ctx Instance of the chip emulator
 * @return 1 - Enabled, 0 - Disabled
 */
extern OPNCHIPSET_DECLSPEC int opn_chipset_isRunningAtPcmRate(OPN_EMU_ChipsetCtx *ctx);

/**
 * @brief Tell to chip emulator work at PCM rate. This reduces CPU load, however, may cause sound distortion while playing some instruments.
 * @param ctx Instance of the chip emulator
 * @param r Enable state: 1 - enable, 0 - disable
 */
extern OPNCHIPSET_DECLSPEC void opn_chipset_setRunningAtPcmRate(OPN_EMU_ChipsetCtx *ctx, int r);

/**
 * @brief Assign the chip ID (any number from 0 to N) to disguess each chip from the pointer
 * @param ctx Instance of the chip emulator
 * @param id ID value (a number from 0 to N)
 */
extern OPNCHIPSET_DECLSPEC void opn_chipset_setChipId(OPN_EMU_ChipsetCtx *ctx, OPN2_UInt32 id);

/**
 * @brief Get the previously assigned chip ID value
 * @param ctx Instance of the chip emulator
 * @return ID value
 */
extern OPNCHIPSET_DECLSPEC OPN2_UInt32 opn_chipset_chipId(OPN_EMU_ChipsetCtx *ctx);

/**
 * @brief Configure the output sample rate (a PCM rate that will be used to generate the stream), it will be resampled internally
 * @param ctx Instance of the chip emulator
 * @param rate PCM sample rate for the output generation
 */
extern OPNCHIPSET_DECLSPEC void opn_chipset_setOutputSampleRate(OPN_EMU_ChipsetCtx *ctx, OPN2_UInt32 rate);

/**
 * @brief Current sample rate that is generated by chip emulator before resampler
 * @param ctx Instance of the chip emulator
 * @return PCM sample rate value
 */
extern OPNCHIPSET_DECLSPEC OPN2_UInt32 opn_chipset_effectiveRate(OPN_EMU_ChipsetCtx *ctx);

/**
 * @brief Current native chip's sample rate (depends on the family)
 * @param ctx Instance of the chip emulator
 * @return PCM sample rate
 */
extern OPNCHIPSET_DECLSPEC OPN2_UInt32 opn_chipset_nativeRate(OPN_EMU_ChipsetCtx *ctx);

/**
 * @brief Re-initialise the chip and reset its states
 * @param ctx Instance of the chip emulator
 */
extern OPNCHIPSET_DECLSPEC void opn_chipset_reset(OPN_EMU_ChipsetCtx *ctx);

/**
 * @brief Write chip's registry command
 * @param ctx Instance of the chip emulator
 * @param port Port value
 * @param addr Register address
 * @param data Data to write
 */
extern OPNCHIPSET_DECLSPEC void opn_chipset_writeReg(OPN_EMU_ChipsetCtx *ctx, OPN2_UInt32 port, OPN2_UInt16 addr, OPN2_UInt8 data);

/**
 * @brief Does this chip emulator supports a soft-panning hack?
 * @param ctx Instance of the chip emulator
 * @return 1 - soft-pan supported, 2 - soft-pan is not supported
 */
extern OPNCHIPSET_DECLSPEC int opn_chipset_hasFullPanning(OPN_EMU_ChipsetCtx *ctx);

/* Soft-Panning hack! (Only supported emulators, some emulators doesn't have soft-panning) */
/**
 * @brief Soft-panning per FM channel (a hack that works at few emulators only)
 * @param ctx Instance of the chip emulator
 * @param addr FM channel number from 0 to 5
 * @param data MIDI Panning value from 0 to 127, 64 is centre
 */
extern OPNCHIPSET_DECLSPEC void opn_chipset_writePan(OPN_EMU_ChipsetCtx *ctx, OPN2_UInt16 addr, OPN2_UInt8 data);


/* Generate one single frame per call (two samples - one per channel) */

/**
 * @brief Get one single PCM stereo frame in chip native sample rate
 * @param ctx Instance of the chip emulator
 * @param outputFrame Two 16-bit signed integers written into array by pointer
 */
extern OPNCHIPSET_DECLSPEC void opn_chipset_nativeGenerate(OPN_EMU_ChipsetCtx *ctx, OPN2_SInt16 *outputFrame);

/**
 * @brief Get one single PCM stereo frame in requested sample rate
 * @param ctx Instance of the chip emulator
 * @param outputFrame Two 16-bit signed integers written into array by pointer
 */
extern OPNCHIPSET_DECLSPEC void opn_chipset_resampledGenerate(OPN_EMU_ChipsetCtx *ctx, OPN2_SInt32 *outputFrame);


/* Generate stream of stereo samples of int16 format, inside native or int32 container for further mixing and converting */

/**
 * @brief Generate the output stream in requested sample rate after resampler and override the output array
 * @param ctx Instance of the chip emulator
 * @param output Array of stereo audio frames, signed 16-bit integers
 * @param frames Number of frames (one frame contains two samples)
 */
extern OPNCHIPSET_DECLSPEC void opn_chipset_generate(OPN_EMU_ChipsetCtx *ctx, OPN2_SInt16 *output, size_t frames);

/**
 * @brief Generate the output stream in requested sample rate after resampler and mix with the existing the output array
 * @param ctx Instance of the chip emulator
 * @param output Array of stereo audio frames, signed 16-bit integers
 * @param frames Number of frames (one frame contains two samples)
 *
 * Suitable for multi-chip playback. Please assign array by zeroes before filling it by data to avoid the garbage result.
 */
extern OPNCHIPSET_DECLSPEC void opn_chipset_generateAndMix(OPN_EMU_ChipsetCtx *ctx, OPN2_SInt16 *output, size_t frames);

/**
 * @brief Generate the output stream in requested sample rate after resampler and override the output array
 * @param ctx Instance of the chip emulator
 * @param output Array of stereo audio frames, signed 16-bit integers stored into 32-bit signed integer containers
 * @param frames Number of frames (one frame contains two samples)
 *
 * Unlike normal variant, here are 16-bit samples stored into 32-bit integer containers that allows safe mixing of
 * many inputs and applying of further conversion into normal formats.
 */
extern OPNCHIPSET_DECLSPEC void opn_chipset_generate32(OPN_EMU_ChipsetCtx *ctx, OPN2_SInt32 *output, size_t frames);

/**
 * @brief Generate the output stream in requested sample rate after resampler and mix with the existing the output array
 * @param ctx Instance of the chip emulator
 * @param output Array of stereo audio frames, signed 16-bit integers stored into 32-bit signed integer containers
 * @param frames Number of frames (one frame contains two samples)
 *
 * Suitable for multi-chip playback. Please assign array by zeroes before filling it by data to avoid the garbage result.
 *
 * Unlike normal variant, here are 16-bit samples stored into 32-bit integer containers that allows safe mixing of
 * many inputs and applying of further conversion into normal formats.
 */
extern OPNCHIPSET_DECLSPEC void opn_chipset_generateAndMix32(OPN_EMU_ChipsetCtx *ctx, OPN2_SInt32 *output, size_t frames);


#ifdef __cplusplus
}
#endif

#endif /* OPN2_CHIPSET_H */

