#ifndef FSYNTH_H
#define FSYNTH_H

//Frequency Generator
#include "RUtil/RUtil.h"
#include "DSPInclude.h"
#include "CSynth.h"
#include "Type/FDFrame.h"

AutoClass
{
    CSynth SubSynth;
    float SynthFreq;
} EndClass(FSynth);

typedef struct
{
    int PSOLAFrameHopSize;
    int BeforeVOT;
} FSynthSendback;

extern void FSynth_SetSymbol(FSynth* Dest, String* Symbol);
extern void FSynth_SetConsonantRatio(FSynth* Dest, float CRatio);
extern void FSynth_SetVowelRatio(FSynth* Dest, float VRatio);
extern void FSynth_Reset(FSynth* Dest);
extern void FSynth_SetFrequency(FSynth* Dest, float Freq);

extern FSynthSendback FSynth_Synthesis(FSynth* Dest, FDFrame* Output);
extern float FSynth_GetVOT(FSynth* Dest);

#endif