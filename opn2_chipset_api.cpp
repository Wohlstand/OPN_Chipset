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

#include "opn2_chipset.h"

// Nuked OPN2 emulator, Most accurate, but requires the powerful CPU
#include "nuked_opn2.h"

// MAME YM2612 emulator, Well-accurate and fast
#include "mame_opn2.h"

// GENS 2.10 emulator, very outdated and inaccurate, but gives the best performance
#include "gens_opn2.h"

// Genesis Plus GX emulator, Variant of MAME with enhancements
#include "gx_opn2.h"

// Neko Project II OPNA emulator
#include "np2_opna.h"

// MAME YM2608 emulator
#include "mame_opna.h"

// PMDWin OPNA emulator
#include "pmdwin_opna.h"

// YMFM emulators
#ifdef OPNCHIPSET_ENABLE_YMFM
#   include "ymfm_opn2.h"
#   include "ymfm_opna.h"
#endif

// LLE variant of Nuked OPN2 Emulator
#include "ym2612_lle.h"
#include "ymf276_lle.h"

// LLE variant of Nuked OPNA Emulator
#include "ym2608_lle.h"




// Setup compiler defines useful for exporting required public API symbols in gme.cpp
#ifndef OPNCHIPSET_EXPORT
#   if defined (_WIN32) && defined(OPNCHIPSET_BUILD_DLL)
#       define OPNCHIPSET_EXPORT __declspec(dllexport)
#   elif defined (OPNCHIPSET_VISIBILITY) && defined (__GNUC__)
#       define OPNCHIPSET_EXPORT __attribute__((visibility ("default")))
#   else
#       define OPNCHIPSET_EXPORT
#   endif
#endif



OPNCHIPSET_EXPORT OPN_EMU_ChipsetCtx *opn_chipset_init(enum Opn2_Emulator emu, enum OPN_Chipset_ChipFamily family_in)
{
    OPNChipBase *chip = NULL;
    OPNFamily family = static_cast<OPNFamily>(family_in);

    switch(emu)
    {
    default:
    case OPNMIDI_EMU_MAME:
        chip = new MameOPN2(family);
        break;
    case OPNMIDI_EMU_NUKED_YM3438:
        chip = new NukedOPN2(family, true);
        break;
    case OPNMIDI_EMU_NUKED_YM2612:
        chip = new NukedOPN2(family, false);
        break;
    case OPNMIDI_EMU_GENS:
        chip = new GensOPN2(family);
        break;
#ifndef OPNMIDI_DISABLE_YMFM_EMULATOR
    case OPNMIDI_EMU_YMFM_OPN2:
        chip = new YmFmOPN2(family);
        break;
#endif
    case OPNMIDI_EMU_GX:
        chip = new GXOPN2(family);
        break;
    case OPNMIDI_EMU_NP2:
        chip = new NP2OPNA<>(family);
        break;
    case OPNMIDI_EMU_MAME_2608:
        chip = new MameOPNA(family);
        break;
#ifdef OPNCHIPSET_ENABLE_YMFM
    case OPNMIDI_EMU_YMFM_OPNA:
        chip = new YmFmOPNA(family);
        break;
#endif
    case OPNMIDI_EMU_NUKED_YM2612_LLE:
        chip = new Ym2612LLEOPN2(family);
        break;
    case OPNMIDI_EMU_NUKED_YM3438_LLE:
        chip = new Ymf276LLEOPN2(family, false);
        break;
    case OPNMIDI_EMU_NUKED_YMF276_LLE:
        chip = new Ymf276LLEOPN2(family, true);
        break;
    case OPNMIDI_EMU_NUKED_YM2608_LLE:
        chip = new Ym2608LLEOPNA(family);
        break;
    case OPNMIDI_EMU_PMDWIN:
        chip = new PMDWinOPNA(family);
        break;
    }

    return reinterpret_cast<OPN_EMU_ChipsetCtx*>(chip);
}

OPNCHIPSET_EXPORT void opn_chipset_free(OPN_EMU_ChipsetCtx *ctx)
{
    OPNChipBase *chip = reinterpret_cast<OPNChipBase*>(ctx);
    delete chip;
}


OPNCHIPSET_EXPORT OPN2_UInt32 opn_chipset_clockRate(OPN_EMU_ChipsetCtx *ctx)
{
    OPNChipBase *chip = reinterpret_cast<OPNChipBase*>(ctx);

    if(chip)
        return chip->clockRate();
        
    return 0;
}

OPNCHIPSET_EXPORT OPN2_UInt32 opn_chipset_nativeClockRate(OPN_EMU_ChipsetCtx *ctx)
{
    OPNChipBase *chip = reinterpret_cast<OPNChipBase*>(ctx);

    if(chip)
        return chip->nativeClockRate();
        
    return 0;
}

OPNCHIPSET_EXPORT int opn_chipset_canRunAtPcmRate(OPN_EMU_ChipsetCtx *ctx)
{
    OPNChipBase *chip = reinterpret_cast<OPNChipBase*>(ctx);

    if(chip)
        return chip->canRunAtPcmRate();
        
    return 0;
}

OPNCHIPSET_EXPORT int opn_chipset_isRunningAtPcmRate(OPN_EMU_ChipsetCtx *ctx)
{
    OPNChipBase *chip = reinterpret_cast<OPNChipBase*>(ctx);

    if(chip)
        return chip->isRunningAtPcmRate();
        
    return 0;
}

OPNCHIPSET_EXPORT void opn_chipset_setRunningAtPcmRate(OPN_EMU_ChipsetCtx *ctx, int r)
{
    OPNChipBase *chip = reinterpret_cast<OPNChipBase*>(ctx);

    if(chip)
        chip->setRunningAtPcmRate(r > 0);
}



OPNCHIPSET_EXPORT void opn_chipset_setChipId(OPN_EMU_ChipsetCtx *ctx, OPN2_UInt32 id)
{
    OPNChipBase *chip = reinterpret_cast<OPNChipBase*>(ctx);

    if(chip)
        chip->setChipId(id);
}

OPNCHIPSET_DECLSPEC OPN2_UInt32 opn_chipset_chipId(OPN_EMU_ChipsetCtx *ctx)
{
    OPNChipBase *chip = reinterpret_cast<OPNChipBase*>(ctx);

    if(chip)
        return chip->chipId();

    return 0;
}

OPNCHIPSET_EXPORT void opn_chipset_setOutputSampleRate(OPN_EMU_ChipsetCtx *ctx, OPN2_UInt32 rate)
{
    OPNChipBase *chip = reinterpret_cast<OPNChipBase*>(ctx);

    if(chip)
        chip->setRate(rate, chip->nativeClockRate());
}

OPNCHIPSET_EXPORT OPN2_UInt32 opn_chipset_effectiveRate(OPN_EMU_ChipsetCtx *ctx)
{
    OPNChipBase *chip = reinterpret_cast<OPNChipBase*>(ctx);

    if(chip)
        return chip->effectiveRate();
        
    return 0;
}

OPNCHIPSET_EXPORT OPN2_UInt32 opn_chipset_nativeRate(OPN_EMU_ChipsetCtx *ctx)
{
    OPNChipBase *chip = reinterpret_cast<OPNChipBase*>(ctx);

    if(chip)
        return chip->nativeRate();
        
    return 0;
}


OPNCHIPSET_EXPORT void opn_chipset_reset(OPN_EMU_ChipsetCtx *ctx)
{
    OPNChipBase *chip = reinterpret_cast<OPNChipBase*>(ctx);

    if(chip)
        chip->reset();
}

OPNCHIPSET_EXPORT void opn_chipset_writeReg(OPN_EMU_ChipsetCtx *ctx, OPN2_UInt32 port, OPN2_UInt16 addr, OPN2_UInt8 data)
{
    OPNChipBase *chip = reinterpret_cast<OPNChipBase*>(ctx);

    if(chip)
        chip->writeReg(port, addr, data);
}

OPNCHIPSET_EXPORT int opn_chipset_hasFullPanning(OPN_EMU_ChipsetCtx *ctx)
{
    OPNChipBase *chip = reinterpret_cast<OPNChipBase*>(ctx);

    if(chip)
        return chip->hasFullPanning();
        
    return 0;
}

OPNCHIPSET_EXPORT void opn_chipset_writePan(OPN_EMU_ChipsetCtx *ctx, OPN2_UInt16 addr, OPN2_UInt8 data)
{
    OPNChipBase *chip = reinterpret_cast<OPNChipBase*>(ctx);

    if(chip)
        chip->writePan(addr, data);
}


OPNCHIPSET_EXPORT void opn_chipset_nativeGenerate(OPN_EMU_ChipsetCtx *ctx, OPN2_SInt16 *outputFrame)
{
    OPNChipBase *chip = reinterpret_cast<OPNChipBase*>(ctx);

    if(chip)
        chip->nativeGenerate(outputFrame);
}

OPNCHIPSET_EXPORT void opn_chipset_resampledGenerate(OPN_EMU_ChipsetCtx *ctx, OPN2_SInt32 *outputFrame)
{
    OPNChipBase *chip = reinterpret_cast<OPNChipBase*>(ctx);

    if(chip)
        chip->resampledGenerate(outputFrame);
}

OPNCHIPSET_EXPORT void opn_chipset_generate(OPN_EMU_ChipsetCtx *ctx, OPN2_SInt16 *output, size_t frames)
{
    OPNChipBase *chip = reinterpret_cast<OPNChipBase*>(ctx);

    if(chip)
        chip->generate(output, frames);
}

OPNCHIPSET_EXPORT void opn_chipset_generateAndMix(OPN_EMU_ChipsetCtx *ctx, OPN2_SInt16 *output, size_t frames)
{
    OPNChipBase *chip = reinterpret_cast<OPNChipBase*>(ctx);

    if(chip)
        chip->generateAndMix(output, frames);
}

OPNCHIPSET_EXPORT void opn_chipset_generate32(OPN_EMU_ChipsetCtx *ctx, OPN2_SInt32 *output, size_t frames)
{
    OPNChipBase *chip = reinterpret_cast<OPNChipBase*>(ctx);

    if(chip)
        chip->generate32(output, frames);
}

OPNCHIPSET_EXPORT void opn_chipset_generateAndMix32(OPN_EMU_ChipsetCtx *ctx, OPN2_SInt32 *output, size_t frames)
{
    OPNChipBase *chip = reinterpret_cast<OPNChipBase*>(ctx);

    if(chip)
        chip->generateAndMix32(output, frames);
}


