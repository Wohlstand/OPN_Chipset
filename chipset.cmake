set(CHIPS_SOURCES
    "${CMAKE_CURRENT_LIST_DIR}/np2_opna.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/np2/fmgen_file.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/np2/fmgen_fmgen.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/np2/fmgen_fmtimer.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/np2/fmgen_opna.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/np2/fmgen_psg.cpp"

    "${CMAKE_CURRENT_LIST_DIR}/gens/Ym2612.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/gens_opn2.cpp"

    "${CMAKE_CURRENT_LIST_DIR}/nuked_opn2.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/nuked/ym3438.c"

    "${CMAKE_CURRENT_LIST_DIR}/gx_opn2.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/gx/gx_ym2612.c"

    "${CMAKE_CURRENT_LIST_DIR}/mame_opn2.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/mame/mame_ym2612fm.c"

    "${CMAKE_CURRENT_LIST_DIR}/mame_opna.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/mamefm/emu2149.c"
    "${CMAKE_CURRENT_LIST_DIR}/mamefm/fm.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/mamefm/resampler.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/mamefm/ymdeltat.cpp"

    "${CMAKE_CURRENT_LIST_DIR}/pmdwin_opna.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/pmdwin/opna.c"
    "${CMAKE_CURRENT_LIST_DIR}/pmdwin/psg.c"
    "${CMAKE_CURRENT_LIST_DIR}/pmdwin/rhythmdata.c"

    # YM2612
    "${CMAKE_CURRENT_LIST_DIR}/ym2612_lle.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/ym2612_lle.h"
    "${CMAKE_CURRENT_LIST_DIR}/nuked_lle/nopn2.c"
    "${CMAKE_CURRENT_LIST_DIR}/nuked_lle/nopn2.h"
    "${CMAKE_CURRENT_LIST_DIR}/nuked_lle/fmopna_2612.c"
    "${CMAKE_CURRENT_LIST_DIR}/nuked_lle/fmopna_2612.h"

    # YMF276 + YM3438
    "${CMAKE_CURRENT_LIST_DIR}/ymf276_lle.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/ymf276_lle.h"
    "${CMAKE_CURRENT_LIST_DIR}/nuked_lle/nopn2f.c"
    "${CMAKE_CURRENT_LIST_DIR}/nuked_lle/nopn2f.h"
    "${CMAKE_CURRENT_LIST_DIR}/nuked_lle/fmopn2.c"
    "${CMAKE_CURRENT_LIST_DIR}/nuked_lle/fmopn2.h"

    # YM2608
    "${CMAKE_CURRENT_LIST_DIR}/ym2608_lle.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/ym2608_lle.h"
    "${CMAKE_CURRENT_LIST_DIR}/nuked_lle/nopna.c"
    "${CMAKE_CURRENT_LIST_DIR}/nuked_lle/nopna.h"
    "${CMAKE_CURRENT_LIST_DIR}/nuked_lle/fmopna_2608.c"
    "${CMAKE_CURRENT_LIST_DIR}/nuked_lle/fmopna_2608.h"

    # Common files of LLE
    "${CMAKE_CURRENT_LIST_DIR}/nuked_lle/fmopna_impl.h"
    "${CMAKE_CURRENT_LIST_DIR}/nuked_lle/fmopna_impl_c.h"
    "${CMAKE_CURRENT_LIST_DIR}/nuked_lle/fmopna_rom.h"
)

if(COMPILER_SUPPORTS_CXX14) # YMFM can be built in only condition when C++14 and newer were available
    set(YMFM_SOURCES
        "${CMAKE_CURRENT_LIST_DIR}/ymfm_opn2.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/ymfm_opna.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/ymfm/ymfm_opn.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/ymfm/ymfm_misc.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/ymfm/ymfm_pcm.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/ymfm/ymfm_adpcm.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/ymfm/ymfm_ssg.cpp"
    )

    if(DEFINED FLAG_CPP14)
        set_source_files_properties(${YMFM_SOURCES} COMPILE_FLAGS ${FLAG_CPP14})
    endif()

    list(APPEND CHIPS_SOURCES ${YMFM_SOURCES})
endif()

