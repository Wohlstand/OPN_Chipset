# OPN2/OPNA Chipset for C++

A bundle of various OPN2 and OPNA emulators wrapped as unified C++ interface
with the polymorphism and resampling.

Primarily used with the libOPNMIDI and OPN2 Bank Editor projects.

This module supports C++98 and higher. However, the use of YMFM emualtors
requires the support for C++14 standard by compilers.

# How to use
- Include the desired emulator's header (or multiple) into your project (for example "nuked_opn2.h")
- Have the pointer of type `OPNChipBase` that will hold the initialized emulator's instance.
- Use emulator through the API at the `OPNChipBase` pointer:
  - Set the output sample rate.
  - Set properties like enable or disable simulated full-panning stereo.
  - Write data to register addresses.
  - Generate the output using one of available methods: single sample, or stream.
- Delete chip by deleting the heap object via previous pointer. It's fine to use smart pointers.
- You can use different chip emulators through the same interface.
- Among emulators, you will find various interfaces over hardware outputs, they require additional setup like hardware address or COM port setup.

# License
- The API itself is licensed under LGPLv2.1+.
- Included chip emulators has various licenses:
  - **GPLv2+**:
    - Nuked LLE OPNA 2608, LLE OPN2 YM2612, LLE OPN2 YMF276 are licensed under GPLv2+.
    - GENS OPN2 is licensed under GPLv2+.
    - MAME OPN2 is licensed under GPLv2+.
    - MAME ONPA is licensed under GPLv2+.
  - **LGPLv2.1+**:
    - Nuked OPN2 is licensed under LGPLv2.1+.
    - VGM Dumper pseudo chip is licensed under LGPLv2.1+.
  - **Permissive**:
    - Neko Project II Kai OPNA is licensed under MIT license.
    - PMDWin OPNA has BSD-2-Clause license.
    - YMFM emulators has BSD-3-Clause license.

