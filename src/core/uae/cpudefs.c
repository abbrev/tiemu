#include <stdlib.h>
#include "sysdeps.h"
#include "uconfig.h"
#include "options.h"
#include "readcpu.h"
struct instr_def defs68k[] = {
{ 60, 0, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 65535, 0, "ORSR.B  #1"},
{ 124, 0, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 65535, 2, "ORSR.W  #1"},
{ 0, 8, {15,15,11,11,11,12,12,12,0,0,0,0,0,0,0,0}, 65280, 0, "OR.z    #z,d[!Areg]"},
{ 572, 0, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 65535, 0, "ANDSR.B #1"},
{ 636, 0, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 65535, 2, "ANDSR.W #1"},
{ 512, 8, {15,15,11,11,11,12,12,12,0,0,0,0,0,0,0,0}, 65280, 0, "AND.z   #z,d[!Areg]"},
{ 1024, 8, {15,15,11,11,11,12,12,12,0,0,0,0,0,0,0,0}, 65280, 0, "SUB.z   #z,d[!Areg]"},
{ 1536, 8, {15,15,11,11,11,12,12,12,0,0,0,0,0,0,0,0}, 65280, 0, "ADD.z   #z,d[!Areg]"},
{ 2048, 6, {9,9,9,10,10,10,0,0,0,0,0,0,0,0,0,0}, 65472, 0, "BTST    #1,s[!Areg]"},
{ 2112, 6, {9,9,9,10,10,10,0,0,0,0,0,0,0,0,0,0}, 65472, 0, "BCHG    #1,s[!Areg,Immd]"},
{ 2176, 6, {9,9,9,10,10,10,0,0,0,0,0,0,0,0,0,0}, 65472, 0, "BCLR    #1,s[!Areg,Immd]"},
{ 2240, 6, {9,9,9,10,10,10,0,0,0,0,0,0,0,0,0,0}, 65472, 0, "BSET    #1,s[!Areg,Immd]"},
{ 2620, 0, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 65535, 0, "EORSR.B #1"},
{ 2684, 0, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 65535, 2, "EORSR.W #1"},
{ 2560, 8, {15,15,11,11,11,12,12,12,0,0,0,0,0,0,0,0}, 65280, 0, "EOR.z   #z,d[!Areg]"},
{ 3072, 8, {15,15,11,11,11,12,12,12,0,0,0,0,0,0,0,0}, 65280, 0, "CMP.z   #z,d[!Areg]"},
{ 256, 9, {13,13,13,11,11,11,12,12,12,0,0,0,0,0,0,0}, 61888, 0, "MVPMR.W d[Areg-Ad16],Dr"},
{ 320, 9, {13,13,13,11,11,11,12,12,12,0,0,0,0,0,0,0}, 61888, 0, "MVPMR.L d[Areg-Ad16],Dr"},
{ 384, 9, {13,13,13,11,11,11,12,12,12,0,0,0,0,0,0,0}, 61888, 0, "MVPRM.W Dr,d[Areg-Ad16]"},
{ 448, 9, {13,13,13,11,11,11,12,12,12,0,0,0,0,0,0,0}, 61888, 0, "MVPRM.L Dr,d[Areg-Ad16]"},
{ 256, 9, {13,13,13,9,9,9,10,10,10,0,0,0,0,0,0,0}, 61888, 0, "BTST    Dr,s[!Areg]"},
{ 320, 9, {13,13,13,9,9,9,10,10,10,0,0,0,0,0,0,0}, 61888, 0, "BCHG    Dr,s[!Areg,Immd]"},
{ 384, 9, {13,13,13,9,9,9,10,10,10,0,0,0,0,0,0,0}, 61888, 0, "BCLR    Dr,s[!Areg,Immd]"},
{ 448, 9, {13,13,13,9,9,9,10,10,10,0,0,0,0,0,0,0}, 61888, 0, "BSET    Dr,s[!Areg,Immd]"},
{ 4096, 12, {12,12,12,11,11,11,9,9,9,10,10,10,0,0,0,0}, 61440, 0, "MOVE.B  s,d[!Areg]"},
{ 8192, 12, {12,12,12,11,11,11,9,9,9,10,10,10,0,0,0,0}, 61440, 0, "MOVEA.L s,d[Areg]"},
{ 8192, 12, {12,12,12,11,11,11,9,9,9,10,10,10,0,0,0,0}, 61440, 0, "MOVE.L  s,d[!Areg]"},
{ 12288, 12, {12,12,12,11,11,11,9,9,9,10,10,10,0,0,0,0}, 61440, 0, "MOVEA.W s,d[Areg]"},
{ 12288, 12, {12,12,12,11,11,11,9,9,9,10,10,10,0,0,0,0}, 61440, 0, "MOVE.W  s,d[!Areg]"},
{ 16384, 8, {15,15,11,11,11,12,12,12,0,0,0,0,0,0,0,0}, 65280, 0, "NEGX.z  d[!Areg]"},
{ 16576, 6, {11,11,11,12,12,12,0,0,0,0,0,0,0,0,0,0}, 65472, 1, "MVSR2.W d[!Areg]"},
{ 16896, 8, {15,15,11,11,11,12,12,12,0,0,0,0,0,0,0,0}, 65280, 0, "CLR.z   d[!Areg]"},
{ 17408, 8, {15,15,11,11,11,12,12,12,0,0,0,0,0,0,0,0}, 65280, 0, "NEG.z   d[!Areg]"},
{ 17600, 6, {9,9,9,10,10,10,0,0,0,0,0,0,0,0,0,0}, 65472, 0, "MV2SR.B s[!Areg]"},
{ 17920, 8, {15,15,11,11,11,12,12,12,0,0,0,0,0,0,0,0}, 65280, 0, "NOT.z   d[!Areg]"},
{ 18112, 6, {9,9,9,10,10,10,0,0,0,0,0,0,0,0,0,0}, 65472, 2, "MV2SR.W s[!Areg]"},
{ 18432, 6, {11,11,11,12,12,12,0,0,0,0,0,0,0,0,0,0}, 65472, 0, "NBCD.B  d[!Areg]"},
{ 18496, 6, {9,9,9,10,10,10,0,0,0,0,0,0,0,0,0,0}, 65472, 0, "SWAP.L  s[Dreg]"},
{ 18496, 6, {9,9,9,10,10,10,0,0,0,0,0,0,0,0,0,0}, 65472, 0, "PEA.L   s[!Dreg,Areg,Immd]"},
{ 18560, 6, {11,11,11,12,12,12,0,0,0,0,0,0,0,0,0,0}, 65472, 0, "EXT.W   d[Dreg]"},
{ 18560, 6, {11,11,11,12,12,12,0,0,0,0,0,0,0,0,0,0}, 65472, 0, "MVMLE.W d[!Dreg,Areg]"},
{ 18624, 6, {11,11,11,12,12,12,0,0,0,0,0,0,0,0,0,0}, 65472, 0, "EXT.L   d[Dreg]"},
{ 18624, 6, {11,11,11,12,12,12,0,0,0,0,0,0,0,0,0,0}, 65472, 0, "MVMLE.L d[!Dreg,Areg]"},
{ 18944, 8, {15,15,11,11,11,12,12,12,0,0,0,0,0,0,0,0}, 65280, 0, "TST.z   d"},
{ 19584, 6, {9,9,9,10,10,10,0,0,0,0,0,0,0,0,0,0}, 65472, 0, "MVMEL.W s[!Dreg,Areg,Immd]"},
{ 19648, 6, {9,9,9,10,10,10,0,0,0,0,0,0,0,0,0,0}, 65472, 0, "MVMEL.L s[!Dreg,Areg,Immd]"},
{ 20032, 4, {8,8,8,8,0,0,0,0,0,0,0,0,0,0,0,0}, 65520, 0, "TRAP    #J"},
{ 20048, 3, {13,13,13,0,0,0,0,0,0,0,0,0,0,0,0,0}, 65528, 0, "LINK.L  Ar,#1"},
{ 20056, 3, {13,13,13,0,0,0,0,0,0,0,0,0,0,0,0,0}, 65528, 0, "UNLK.L  Ar"},
{ 20064, 3, {13,13,13,0,0,0,0,0,0,0,0,0,0,0,0,0}, 65528, 2, "MVR2USP.L Ar"},
{ 20072, 3, {13,13,13,0,0,0,0,0,0,0,0,0,0,0,0,0}, 65528, 2, "MVUSP2R.L Ar"},
{ 20080, 0, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 65535, 2, "RESET"},
{ 20081, 0, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 65535, 0, "NOP"},
{ 20082, 0, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 65535, 2, "STOP    #1"},
{ 20083, 0, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 65535, 2, "RTE"},
{ 20084, 0, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 65535, 0, "RTD     #1"},
{ 20085, 0, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 65535, 0, "RTS"},
{ 20086, 0, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 65535, 0, "TRAPV"},
{ 20087, 0, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 65535, 0, "RTR"},
{ 20096, 6, {9,9,9,10,10,10,0,0,0,0,0,0,0,0,0,0}, 65472, 0, "JSR.L   s[!Dreg,Areg,Immd]"},
{ 16768, 9, {13,13,13,9,9,9,10,10,10,0,0,0,0,0,0,0}, 61888, 0, "CHK.W   s,Dr"},
{ 20160, 6, {9,9,9,10,10,10,0,0,0,0,0,0,0,0,0,0}, 65472, 0, "JMP.L   s[!Dreg,Areg,Immd]"},
{ 16832, 9, {13,13,13,9,9,9,10,10,10,0,0,0,0,0,0,0}, 61888, 0, "LEA.L   s[!Dreg,Areg,Immd],Ar"},
{ 20480, 11, {7,7,7,15,15,11,11,11,12,12,12,0,0,0,0,0}, 61696, 0, "ADDA.z  #j,d[Areg]"},
{ 20480, 11, {7,7,7,15,15,11,11,11,12,12,12,0,0,0,0,0}, 61696, 0, "ADD.z   #j,d[!Areg]"},
{ 20736, 11, {7,7,7,15,15,11,11,11,12,12,12,0,0,0,0,0}, 61696, 0, "SUBA.z  #j,d[Areg]"},
{ 20736, 11, {7,7,7,15,15,11,11,11,12,12,12,0,0,0,0,0}, 61696, 0, "SUB.z   #j,d[!Areg]"},
{ 20680, 7, {2,2,2,2,13,13,13,0,0,0,0,0,0,0,0,0}, 61688, 0, "DBcc.W  Dr,#1"},
{ 20672, 10, {2,2,2,2,11,11,11,12,12,12,0,0,0,0,0,0}, 61632, 0, "Scc.B   d[!Areg]"},
{ 24832, 0, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 65535, 0, "BSR.W   #1"},
{ 24832, 8, {6,6,6,6,6,6,6,6,0,0,0,0,0,0,0,0}, 65280, 0, "BSR.B   #i"},
{ 24576, 4, {3,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0}, 61695, 0, "Bcc.W   #1"},
{ 24576, 12, {3,3,3,3,6,6,6,6,6,6,6,6,0,0,0,0}, 61440, 0, "Bcc.B   #i"},
{ 28672, 11, {13,13,13,5,5,5,5,5,5,5,5,0,0,0,0,0}, 61696, 0, "MOVE.L  #i,Dr"},
{ 32768, 11, {13,13,13,15,15,9,9,9,10,10,10,0,0,0,0,0}, 61696, 0, "OR.z    s[!Areg],Dr"},
{ 32960, 9, {13,13,13,9,9,9,10,10,10,0,0,0,0,0,0,0}, 61888, 0, "DIVU.W  s[!Areg],Dr"},
{ 33024, 9, {13,13,13,11,11,11,12,12,12,0,0,0,0,0,0,0}, 61888, 0, "SBCD.B  d[Dreg],Dr"},
{ 33024, 9, {13,13,13,11,11,11,12,12,12,0,0,0,0,0,0,0}, 61888, 0, "SBCD.B  d[Areg-Apdi],Arp"},
{ 33024, 9, {13,13,13,11,11,11,12,12,12,0,0,0,0,0,0,0}, 61888, 0, "OR.B    Dr,d[!Areg,Dreg]"},
{ 33088, 9, {13,13,13,11,11,11,12,12,12,0,0,0,0,0,0,0}, 61888, 0, "OR.W    Dr,d[!Areg,Dreg]"},
{ 33152, 9, {13,13,13,11,11,11,12,12,12,0,0,0,0,0,0,0}, 61888, 0, "OR.L    Dr,d[!Areg,Dreg]"},
{ 33216, 9, {13,13,13,9,9,9,10,10,10,0,0,0,0,0,0,0}, 61888, 0, "DIVS.W  s[!Areg],Dr"},
{ 36864, 11, {13,13,13,15,15,9,9,9,10,10,10,0,0,0,0,0}, 61696, 0, "SUB.z   s,Dr"},
{ 37056, 9, {13,13,13,9,9,9,10,10,10,0,0,0,0,0,0,0}, 61888, 0, "SUBA.W  s,Ar"},
{ 37120, 11, {13,13,13,15,15,11,11,11,12,12,12,0,0,0,0,0}, 61696, 0, "SUBX.z  d[Dreg],Dr"},
{ 37120, 11, {13,13,13,15,15,11,11,11,12,12,12,0,0,0,0,0}, 61696, 0, "SUBX.z  d[Areg-Apdi],Arp"},
{ 37120, 11, {13,13,13,15,15,11,11,11,12,12,12,0,0,0,0,0}, 61696, 0, "SUB.z   Dr,d[!Areg,Dreg]"},
{ 37312, 9, {13,13,13,9,9,9,10,10,10,0,0,0,0,0,0,0}, 61888, 0, "SUBA.L  s,Ar"},
{ 45056, 11, {13,13,13,15,15,9,9,9,10,10,10,0,0,0,0,0}, 61696, 0, "CMP.z   s,Dr"},
{ 45248, 9, {13,13,13,9,9,9,10,10,10,0,0,0,0,0,0,0}, 61888, 0, "CMPA.W  s,Ar"},
{ 45504, 9, {13,13,13,9,9,9,10,10,10,0,0,0,0,0,0,0}, 61888, 0, "CMPA.L  s,Ar"},
{ 45312, 11, {13,13,13,15,15,11,11,11,12,12,12,0,0,0,0,0}, 61696, 0, "CMPM.z  d[Areg-Aipi],ArP"},
{ 45312, 11, {13,13,13,15,15,11,11,11,12,12,12,0,0,0,0,0}, 61696, 0, "EOR.z   Dr,d[!Areg]"},
{ 49152, 11, {13,13,13,15,15,9,9,9,10,10,10,0,0,0,0,0}, 61696, 0, "AND.z   s[!Areg],Dr"},
{ 49344, 9, {13,13,13,9,9,9,10,10,10,0,0,0,0,0,0,0}, 61888, 0, "MULU.W  s[!Areg],Dr"},
{ 49408, 9, {13,13,13,11,11,11,12,12,12,0,0,0,0,0,0,0}, 61888, 0, "ABCD.B  d[Dreg],Dr"},
{ 49408, 9, {13,13,13,11,11,11,12,12,12,0,0,0,0,0,0,0}, 61888, 0, "ABCD.B  d[Areg-Apdi],Arp"},
{ 49408, 9, {13,13,13,11,11,11,12,12,12,0,0,0,0,0,0,0}, 61888, 0, "AND.B   Dr,d[!Areg,Dreg]"},
{ 49472, 9, {13,13,13,11,11,11,12,12,12,0,0,0,0,0,0,0}, 61888, 0, "EXG.L   Dr,d[Dreg]"},
{ 49472, 9, {13,13,13,11,11,11,12,12,12,0,0,0,0,0,0,0}, 61888, 0, "EXG.L   Ar,d[Areg]"},
{ 49472, 9, {13,13,13,11,11,11,12,12,12,0,0,0,0,0,0,0}, 61888, 0, "AND.W   Dr,d[!Areg,Dreg]"},
{ 49536, 9, {13,13,13,11,11,11,12,12,12,0,0,0,0,0,0,0}, 61888, 0, "EXG.L   Dr,d[Areg]"},
{ 49536, 9, {13,13,13,11,11,11,12,12,12,0,0,0,0,0,0,0}, 61888, 0, "AND.L   Dr,d[!Areg,Dreg]"},
{ 49600, 9, {13,13,13,9,9,9,10,10,10,0,0,0,0,0,0,0}, 61888, 0, "MULS.W  s[!Areg],Dr"},
{ 53248, 11, {13,13,13,15,15,9,9,9,10,10,10,0,0,0,0,0}, 61696, 0, "ADD.z   s,Dr"},
{ 53440, 9, {13,13,13,9,9,9,10,10,10,0,0,0,0,0,0,0}, 61888, 0, "ADDA.W  s,Ar"},
{ 53504, 11, {13,13,13,15,15,11,11,11,12,12,12,0,0,0,0,0}, 61696, 0, "ADDX.z  d[Dreg],Dr"},
{ 53504, 11, {13,13,13,15,15,11,11,11,12,12,12,0,0,0,0,0}, 61696, 0, "ADDX.z  d[Areg-Apdi],Arp"},
{ 53504, 11, {13,13,13,15,15,11,11,11,12,12,12,0,0,0,0,0}, 61696, 0, "ADD.z   Dr,d[!Areg,Dreg]"},
{ 53696, 9, {13,13,13,9,9,9,10,10,10,0,0,0,0,0,0,0}, 61888, 0, "ADDA.L  s,Ar"},
{ 57344, 9, {7,7,7,4,15,15,14,14,14,0,0,0,0,0,0,0}, 61496, 0, "ASf.z   #j,DR"},
{ 57352, 9, {7,7,7,4,15,15,14,14,14,0,0,0,0,0,0,0}, 61496, 0, "LSf.z   #j,DR"},
{ 57360, 9, {7,7,7,4,15,15,14,14,14,0,0,0,0,0,0,0}, 61496, 0, "ROXf.z  #j,DR"},
{ 57368, 9, {7,7,7,4,15,15,14,14,14,0,0,0,0,0,0,0}, 61496, 0, "ROf.z   #j,DR"},
{ 57376, 9, {13,13,13,4,15,15,14,14,14,0,0,0,0,0,0,0}, 61496, 0, "ASf.z   Dr,DR"},
{ 57384, 9, {13,13,13,4,15,15,14,14,14,0,0,0,0,0,0,0}, 61496, 0, "LSf.z   Dr,DR"},
{ 57392, 9, {13,13,13,4,15,15,14,14,14,0,0,0,0,0,0,0}, 61496, 0, "ROXf.z  Dr,DR"},
{ 57400, 9, {13,13,13,4,15,15,14,14,14,0,0,0,0,0,0,0}, 61496, 0, "ROf.z   Dr,DR"},
{ 57536, 7, {4,11,11,11,12,12,12,0,0,0,0,0,0,0,0,0}, 65216, 0, "ASfW.W  d[!Dreg,Areg,PC8r,PC16]"},
{ 58048, 7, {4,11,11,11,12,12,12,0,0,0,0,0,0,0,0,0}, 65216, 0, "LSfW.W  d[!Dreg,Areg,PC8r,PC16]"},
{ 58560, 7, {4,11,11,11,12,12,12,0,0,0,0,0,0,0,0,0}, 65216, 0, "ROXfW.W d[!Dreg,Areg,PC8r,PC16]"},
{ 59072, 7, {4,11,11,11,12,12,12,0,0,0,0,0,0,0,0,0}, 65216, 0, "ROfW.W  d[!Dreg,Areg,PC8r,PC16]"}};
int n_defs68k = 122;
