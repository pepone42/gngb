#include "../global.h"
#include "../memory.h"
#include "op.h"

static int aff_op0(char *format,UINT16 pc,char *ret);
static int aff_op1(char *format,UINT16 pc,char *ret);
static int aff_op2(char *format,UINT16 pc,char *ret);
static int aff_op_cb(char *format,UINT16 pc,char *ret);
 
struct gbcpu_op {
  char *format;
  int (*aff_op)(char *format,UINT16 pc,char *ret);
  char nb_byte;
};

struct gbcpu_op tab_op[]={
  {"NOP               ",aff_op0,1},
  {"LD__BC,%04x       ",aff_op2,3},
  {"LD__(BC),A        ",aff_op0,1},
  {"INC__BC           ",aff_op0,1},
  {"INC__B            ",aff_op0,1},
  {"DEC__B            ",aff_op0,1},
  {"LD__B,%02x        ",aff_op1,2},
  {"RLCA              ",aff_op0,1},
  {"LD__(%04x),SP     ",aff_op2,3},
  {"ADD__HL,BC        ",aff_op0,1},
  {"LD__A,(BC)        ",aff_op0,1},
  {"DEC__BC           ",aff_op0,1},
  {"INC__C            ",aff_op0,1},
  {"DEC__C            ",aff_op0,1},
  {"LD__C,%02x        ",aff_op1,2},
  {"RRCA              ",aff_op0,1},
  {"STOP              ",aff_op0,1},
  {"LD__DE,%04x       ",aff_op2,3},
  {"LD__(DE),A        ",aff_op0,1},
  {"INC__DE           ",aff_op0,1},
  {"INC__D            ",aff_op0,1},
  {"DEC__D            ",aff_op0,1},
  {"LD__D,%02x        ",aff_op1,2},
  {"RLA               ",aff_op0,1},
  {"JR__%02x          ",aff_op1,2},
  {"ADD__HL,DE        ",aff_op0,1},
  {"LD__A,(DE)        ",aff_op0,1},
  {"DEC__DE           ",aff_op0,1},
  {"INC__E            ",aff_op0,1},
  {"DEC__E            ",aff_op0,1},
  {"LD__E,%02x        ",aff_op1,2},
  {"RRA               ",aff_op0,1},
  {"JR__NZ,%02x       ",aff_op1,2},
  {"LD__HL,%04x       ",aff_op2,3},
  {"LDI__(HL),A       ",aff_op0,1},
  {"INC__HL           ",aff_op0,1},
  {"INC__H            ",aff_op0,1},
  {"DEC__H            ",aff_op0,1},
  {"LD__H,%02x        ",aff_op1,2},
  {"DAA               ",aff_op0,1},
  {"JR__Z,%02x        ",aff_op1,2},
  {"ADD__HL,HL        ",aff_op0,1},
  {"LDI__A,(HL)       ",aff_op0,1},
  {"DEC__HL           ",aff_op0,1},
  {"INC__L            ",aff_op0,1},
  {"DEC__L            ",aff_op0,1},
  {"LD__L,%02x        ",aff_op1,2},
  {"CPL               ",aff_op0,1},
  {"JR__NC,%02x       ",aff_op1,2},
  {"LD__SP,%04x       ",aff_op2,3},
  {"LDD__(HL),A       ",aff_op0,1},
  {"INC__SP           ",aff_op0,1},
  {"INC__(HL)         ",aff_op0,1},
  {"DEC__(HL)         ",aff_op0,1},
  {"LD__(HL),%02x     ",aff_op1,2},
  {"SCF               ",aff_op0,1},
  {"JR__C,%02x        ",aff_op1,2},
  {"ADD__HL,SP        ",aff_op0,1},
  {"LDD__A,(HL)       ",aff_op0,1},
  {"DEC__SP           ",aff_op0,1},
  {"INC__A            ",aff_op0,1},
  {"DEC__A            ",aff_op0,1},
  {"LD__A,%02x        ",aff_op1,2},
  {"CCF               ",aff_op0,1},
  {"LD__B,B           ",aff_op0,1},
  {"LD__B,C           ",aff_op0,1},
  {"LD__B,D           ",aff_op0,1},
  {"LD__B,E           ",aff_op0,1},
  {"LD__B,H           ",aff_op0,1},
  {"LD__B,L           ",aff_op0,1},
  {"LD__B,(HL)        ",aff_op0,1},
  {"LD__B,A           ",aff_op0,1},
  {"LD__C,B           ",aff_op0,1},
  {"LD__C,C           ",aff_op0,1},
  {"LD__C,D           ",aff_op0,1},
  {"LD__C,E           ",aff_op0,1},
  {"LD__C,H           ",aff_op0,1},
  {"LD__C,L           ",aff_op0,1},
  {"LD__C,(HL)        ",aff_op0,1},
  {"LD__C,A           ",aff_op0,1},
  {"LD__D,B           ",aff_op0,1},
  {"LD__D,C           ",aff_op0,1},
  {"LD__D,D           ",aff_op0,1},
  {"LD__D,E           ",aff_op0,1},
  {"LD__D,H           ",aff_op0,1},
  {"LD__D,L           ",aff_op0,1},
  {"LD__D,(HL)        ",aff_op0,1},
  {"LD__D,A           ",aff_op0,1},
  {"LD__E,B           ",aff_op0,1},
  {"LD__E,C           ",aff_op0,1},
  {"LD__E,D           ",aff_op0,1},
  {"LD__E,E           ",aff_op0,1},
  {"LD__E,H           ",aff_op0,1},
  {"LD__E,L           ",aff_op0,1},
  {"LD__E,(HL)        ",aff_op0,1},
  {"LD__E,A           ",aff_op0,1},
  {"LD__H,B           ",aff_op0,1},
  {"LD__H,C           ",aff_op0,1},
  {"LD__H,D           ",aff_op0,1},
  {"LD__H,E           ",aff_op0,1},
  {"LD__H,H           ",aff_op0,1},
  {"LD__H,L           ",aff_op0,1},
  {"LD__H,(HL)        ",aff_op0,1},
  {"LD__H,A           ",aff_op0,1},
  {"LD__L,B           ",aff_op0,1},
  {"LD__L,C           ",aff_op0,1},
  {"LD__L,D           ",aff_op0,1},
  {"LD__L,E           ",aff_op0,1},
  {"LD__L,H           ",aff_op0,1},
  {"LD__L,L           ",aff_op0,1},
  {"LD__L,(HL)        ",aff_op0,1},
  {"LD__L,A           ",aff_op0,1},
  {"LD__(HL),B        ",aff_op0,1},
  {"LD__(HL),C        ",aff_op0,1},
  {"LD__(HL),D        ",aff_op0,1},
  {"LD__(HL),E        ",aff_op0,1},
  {"LD__(HL),H        ",aff_op0,1},
  {"LD__(HL),L        ",aff_op0,1},
  {"HALT              ",aff_op0,1},
  {"LD__(HL),A        ",aff_op0,1},
  {"LD__A,B           ",aff_op0,1},
  {"LD__A,C           ",aff_op0,1},
  {"LD__A,D           ",aff_op0,1},
  {"LD__A,E           ",aff_op0,1},
  {"LD__A,H           ",aff_op0,1},
  {"LD__A,L           ",aff_op0,1},
  {"LD__A,(HL)        ",aff_op0,1},
  {"LD__A,A           ",aff_op0,1},
  {"ADD__A,B          ",aff_op0,1},
  {"ADD__A,C          ",aff_op0,1},
  {"ADD__A,D          ",aff_op0,1},
  {"ADD__A,E          ",aff_op0,1},
  {"ADD__A,H          ",aff_op0,1},
  {"ADD__A,L          ",aff_op0,1},
  {"ADD__A,(HL)       ",aff_op0,1},
  {"ADD__A,A          ",aff_op0,1},
  {"ADC__A,B          ",aff_op0,1},
  {"ADC__A,C          ",aff_op0,1},
  {"ADC__A,D          ",aff_op0,1},
  {"ADC__A,E          ",aff_op0,1},
  {"ADC__A,H          ",aff_op0,1},
  {"ADC__A,L          ",aff_op0,1},
  {"ADC__A,(HL)       ",aff_op0,1},
  {"ADC__A,A          ",aff_op0,1},
  {"SUB__B            ",aff_op0,1},
  {"SUB__C            ",aff_op0,1},
  {"SUB__D            ",aff_op0,1},
  {"SUB__E            ",aff_op0,1},
  {"SUB__H            ",aff_op0,1},
  {"SUB__L            ",aff_op0,1},
  {"SUB__(HL)         ",aff_op0,1},
  {"SUB__A            ",aff_op0,1},
  {"SBC__A,B          ",aff_op0,1},
  {"SBC__A,C          ",aff_op0,1},
  {"SBC__A,D          ",aff_op0,1},
  {"SBC__A,E          ",aff_op0,1},
  {"SBC__A,H          ",aff_op0,1},
  {"SBC__A,L          ",aff_op0,1},
  {"SBC__A,(HL)       ",aff_op0,1},
  {"SBC__A,A          ",aff_op0,1},
  {"AND__B            ",aff_op0,1},
  {"AND__C            ",aff_op0,1},
  {"AND__D            ",aff_op0,1},
  {"AND__E            ",aff_op0,1},
  {"AND__H            ",aff_op0,1},
  {"AND__L            ",aff_op0,1},
  {"AND__(HL)         ",aff_op0,1},
  {"AND__A            ",aff_op0,1},
  {"XOR__B            ",aff_op0,1},
  {"XOR__C            ",aff_op0,1},
  {"XOR__D            ",aff_op0,1},
  {"XOR__E            ",aff_op0,1},
  {"XOR__H            ",aff_op0,1},
  {"XOR__L            ",aff_op0,1},
  {"XOR__(HL)         ",aff_op0,1},
  {"XOR__A            ",aff_op0,1},
  {"OR__B             ",aff_op0,1},
  {"OR__C             ",aff_op0,1},
  {"OR__D             ",aff_op0,1},
  {"OR__E             ",aff_op0,1},
  {"OR__H             ",aff_op0,1},
  {"OR__L             ",aff_op0,1},
  {"OR__(HL)          ",aff_op0,1},
  {"OR__A             ",aff_op0,1},
  {"CP__B             ",aff_op0,1},
  {"CP__C             ",aff_op0,1},
  {"CP__D             ",aff_op0,1},
  {"CP__E             ",aff_op0,1},
  {"CP__H             ",aff_op0,1},
  {"CP__L             ",aff_op0,1},
  {"CP__(HL)          ",aff_op0,1},
  {"CP__A             ",aff_op0,1},
  {"RET__NZ           ",aff_op0,1},
  {"POP__BC           ",aff_op0,1},
  {"JP__NZ,%04x       ",aff_op2,3},
  {"JP__%04x          ",aff_op2,3},
  {"CALL__NZ,%04x     ",aff_op2,3},
  {"PUSH__BC          ",aff_op0,1},
  {"ADD__A,%02x       ",aff_op1,2},
  {"RST__00H          ",aff_op0,1},
  {"RET__Z            ",aff_op0,1},
  {"RET               ",aff_op0,1},
  {"JP__Z,%04x        ",aff_op2,3},
  {"------------      ",aff_op_cb,2},
  {"CALL__Z,%04x      ",aff_op2,3},
  {"CALL__%04x        ",aff_op2,3},
  {"ADC__A,%02x       ",aff_op1,2},
  {"RST__8            ",aff_op0,1},
  {"RET__NC           ",aff_op0,1},
  {"POP__DE           ",aff_op0,1},
  {"JP__NC,%04x       ",aff_op2,3},
  {"----              ",aff_op0,1},
  {"CALL__NC,%04x     ",aff_op2,3},
  {"PUSH__DE          ",aff_op0,1},
  {"SUB__%02x         ",aff_op1,2},
  {"RST__10H          ",aff_op0,1},
  {"RET__C            ",aff_op0,1},
  {"RETI              ",aff_op0,1},
  {"JP__C,%04x        ",aff_op2,3},
  {"------------      ",aff_op0,1},
  {"CALL__C,%04x      ",aff_op2,3},
  {"------------      ",aff_op0,1},
  {"SBC__A,%02x       ",aff_op1,2},
  {"RST__18H          ",aff_op0,1},
  {"LD__($FF00+%02x),A",aff_op1,2},
  {"POP__HL           ",aff_op0,1},
  {"LD__($FF00+C),A   ",aff_op0,1},
  {"------------      ",aff_op0,1},
  {"------------      ",aff_op0,1},
  {"PUSH__HL          ",aff_op0,1},
  {"AND__%02x         ",aff_op1,2},
  {"RST__20H          ",aff_op0,1},
  {"ADD__SP,%02x      ",aff_op1,2},
  {"JP__(HL)          ",aff_op0,1},
  {"LD__(%04x),A      ",aff_op2,3},
  {"-----------       ",aff_op0,1},
  {"-----------       ",aff_op0,1},
  {"-----------       ",aff_op0,1},
  {"XOR__%02x         ",aff_op1,2},
  {"RST__28H          ",aff_op0,1},
  {"LD__A,($FF00+%02x)",aff_op1,2},
  {"POP__AF           ",aff_op0,1},
  {"LD__A,(C)         ",aff_op0,1},
  {"DI                ",aff_op0,1},
  {"-----------       ",aff_op0,1},
  {"PUSH__AF          ",aff_op0,1},
  {"OR__%02x          ",aff_op1,2},
  {"RST__30H          ",aff_op0,1},
  {"LD__HL,SP+%02x    ",aff_op1,2},
  {"LD__SP,HL         ",aff_op0,1},
  {"LD__A,(%04x)      ",aff_op2,3},
  {"EI                ",aff_op0,1},
  {"-----------       ",aff_op0,1},
  {"-----------       ",aff_op0,1},
  {"CP__%02x          ",aff_op1,2},
  {"RST__38H          ",aff_op0,1},
  {"RLC_B             ",aff_op0,1},
  {"RLC_C             ",aff_op0,1},
  {"RLC_D             ",aff_op0,1},
  {"RLC_E             ",aff_op0,1},
  {"RLC_H             ",aff_op0,1},
  {"RLC_L             ",aff_op0,1},
  {"RLC_MEM_HL        ",aff_op0,1},
  {"RLC_A             ",aff_op0,1},
  {"RRC_B             ",aff_op0,1},
  {"RRC_C             ",aff_op0,1},
  {"RRC_D             ",aff_op0,1},
  {"RRC_E             ",aff_op0,1},
  {"RRC_H             ",aff_op0,1},
  {"RRC_L             ",aff_op0,1},
  {"RRC_MEM_HL        ",aff_op0,1},
  {"RRC_A             ",aff_op0,1},
  {"RL_B              ",aff_op0,1},
  {"RL_C              ",aff_op0,1},
  {"RL_D              ",aff_op0,1},
  {"RL_E              ",aff_op0,1},
  {"RL_H              ",aff_op0,1},
  {"RL_L              ",aff_op0,1},
  {"RL_MEM_HL         ",aff_op0,1},
  {"RL_A              ",aff_op0,1},
  {"RR_B              ",aff_op0,1},
  {"RR_C              ",aff_op0,1},
  {"RR_D              ",aff_op0,1},
  {"RR_E              ",aff_op0,1},
  {"RR_H              ",aff_op0,1},
  {"RR_L              ",aff_op0,1},
  {"RR_MEM_HL         ",aff_op0,1},
  {"RR_A              ",aff_op0,1},
  {"SLA_B             ",aff_op0,1},
  {"SLA_C             ",aff_op0,1},
  {"SLA_D             ",aff_op0,1},
  {"SLA_E             ",aff_op0,1},
  {"SLA_H             ",aff_op0,1},
  {"SLA_L             ",aff_op0,1},
  {"SLA_MEM_HL        ",aff_op0,1},
  {"SLA_A             ",aff_op0,1},
  {"SRA_B             ",aff_op0,1},
  {"SRA_C             ",aff_op0,1},
  {"SRA_D             ",aff_op0,1},
  {"SRA_E             ",aff_op0,1},
  {"SRA_H             ",aff_op0,1},
  {"SRA_L             ",aff_op0,1},
  {"SRA_MEM_HL        ",aff_op0,1},
  {"SRA_A             ",aff_op0,1},
  {"SWAP_B            ",aff_op0,1},
  {"SWAP_C            ",aff_op0,1},
  {"SWAP_D            ",aff_op0,1},
  {"SWAP_E            ",aff_op0,1},
  {"SWAP_H            ",aff_op0,1},
  {"SWAP_L            ",aff_op0,1},
  {"SWAP_MEM_HL       ",aff_op0,1},
  {"SWAP_A            ",aff_op0,1},
  {"SRL_B             ",aff_op0,1},
  {"SRL_C             ",aff_op0,1},
  {"SRL_D             ",aff_op0,1},
  {"SRL_E             ",aff_op0,1},
  {"SRL_H             ",aff_op0,1},
  {"SRL_L             ",aff_op0,1},
  {"SRL_MEM_HL        ",aff_op0,1},
  {"SRL_A             ",aff_op0,1},
  {"BIT_0_B           ",aff_op0,1},
  {"BIT_0_C           ",aff_op0,1},
  {"BIT_0_D           ",aff_op0,1},
  {"BIT_0_E           ",aff_op0,1},
  {"BIT_0_H           ",aff_op0,1},
  {"BIT_0_L           ",aff_op0,1},
  {"BIT_0_MEM_HL      ",aff_op0,1},
  {"BIT_0_A           ",aff_op0,1},
  {"BIT_1_B           ",aff_op0,1},
  {"BIT_1_C           ",aff_op0,1},
  {"BIT_1_D           ",aff_op0,1},
  {"BIT_1_E           ",aff_op0,1},
  {"BIT_1_H           ",aff_op0,1},
  {"BIT_1_L           ",aff_op0,1},
  {"BIT_1_MEM_HL      ",aff_op0,1},
  {"BIT_1_A           ",aff_op0,1},
  {"BIT_2_B           ",aff_op0,1},
  {"BIT_2_C           ",aff_op0,1},
  {"BIT_2_D           ",aff_op0,1},
  {"BIT_2_E           ",aff_op0,1},
  {"BIT_2_H           ",aff_op0,1},
  {"BIT_2_L           ",aff_op0,1},
  {"BIT_2_MEM_HL      ",aff_op0,1},
  {"BIT_2_A           ",aff_op0,1},
  {"BIT_3_B           ",aff_op0,1},
  {"BIT_3_C           ",aff_op0,1},
  {"BIT_3_D           ",aff_op0,1},
  {"BIT_3_E           ",aff_op0,1},
  {"BIT_3_H           ",aff_op0,1},
  {"BIT_3_L           ",aff_op0,1},
  {"BIT_3_MEM_HL      ",aff_op0,1},
  {"BIT_3_A           ",aff_op0,1},
  {"BIT_4_B           ",aff_op0,1},
  {"BIT_4_C           ",aff_op0,1},
  {"BIT_4_D           ",aff_op0,1},
  {"BIT_4_E           ",aff_op0,1},
  {"BIT_4_H           ",aff_op0,1},
  {"BIT_4_L           ",aff_op0,1},
  {"BIT_4_MEM_HL      ",aff_op0,1},
  {"BIT_4_A           ",aff_op0,1},
  {"BIT_5_B           ",aff_op0,1},
  {"BIT_5_C           ",aff_op0,1},
  {"BIT_5_D           ",aff_op0,1},
  {"BIT_5_E           ",aff_op0,1},
  {"BIT_5_H           ",aff_op0,1},
  {"BIT_5_L           ",aff_op0,1},
  {"BIT_5_MEM_HL      ",aff_op0,1},
  {"BIT_5_A           ",aff_op0,1},
  {"BIT_6_B           ",aff_op0,1},
  {"BIT_6_C           ",aff_op0,1},
  {"BIT_6_D           ",aff_op0,1},
  {"BIT_6_E           ",aff_op0,1},
  {"BIT_6_H           ",aff_op0,1},
  {"BIT_6_L           ",aff_op0,1},
  {"BIT_6_MEM_HL      ",aff_op0,1},
  {"BIT_6_A           ",aff_op0,1},
  {"BIT_7_B           ",aff_op0,1},
  {"BIT_7_C           ",aff_op0,1},
  {"BIT_7_D           ",aff_op0,1},
  {"BIT_7_E           ",aff_op0,1},
  {"BIT_7_H           ",aff_op0,1},
  {"BIT_7_L           ",aff_op0,1},
  {"BIT_7_MEM_HL      ",aff_op0,1},
  {"BIT_7_A           ",aff_op0,1},
  {"RES_0_B           ",aff_op0,1},
  {"RES_0_C           ",aff_op0,1},
  {"RES_0_D           ",aff_op0,1},
  {"RES_0_E           ",aff_op0,1},
  {"RES_0_H           ",aff_op0,1},
  {"RES_0_L           ",aff_op0,1},
  {"RES_0_MEM_HL      ",aff_op0,1},
  {"RES_0_A           ",aff_op0,1},
  {"RES_1_B           ",aff_op0,1},
  {"RES_1_C           ",aff_op0,1},
  {"RES_1_D           ",aff_op0,1},
  {"RES_1_E           ",aff_op0,1},
  {"RES_1_H           ",aff_op0,1},
  {"RES_1_L           ",aff_op0,1},
  {"RES_1_MEM_HL      ",aff_op0,1},
  {"RES_1_A           ",aff_op0,1},
  {"RES_2_B           ",aff_op0,1},
  {"RES_2_C           ",aff_op0,1},
  {"RES_2_D           ",aff_op0,1},
  {"RES_2_E           ",aff_op0,1},
  {"RES_2_H           ",aff_op0,1},
  {"RES_2_L           ",aff_op0,1},
  {"RES_2_MEM_HL      ",aff_op0,1},
  {"RES_2_A           ",aff_op0,1},
  {"RES_3_B           ",aff_op0,1},
  {"RES_3_C           ",aff_op0,1},
  {"RES_3_D           ",aff_op0,1},
  {"RES_3_E           ",aff_op0,1},
  {"RES_3_H           ",aff_op0,1},
  {"RES_3_L           ",aff_op0,1},
  {"RES_3_MEM_HL      ",aff_op0,1},
  {"RES_3_A           ",aff_op0,1},
  {"RES_4_B           ",aff_op0,1},
  {"RES_4_C           ",aff_op0,1},
  {"RES_4_D           ",aff_op0,1},
  {"RES_4_E           ",aff_op0,1},
  {"RES_4_H           ",aff_op0,1},
  {"RES_4_L           ",aff_op0,1},
  {"RES_4_MEM_HL      ",aff_op0,1},
  {"RES_4_A           ",aff_op0,1},
  {"RES_5_B           ",aff_op0,1},
  {"RES_5_C           ",aff_op0,1},
  {"RES_5_D           ",aff_op0,1},
  {"RES_5_E           ",aff_op0,1},
  {"RES_5_H           ",aff_op0,1},
  {"RES_5_L           ",aff_op0,1},
  {"RES_5_MEM_HL      ",aff_op0,1},
  {"RES_5_A           ",aff_op0,1},
  {"RES_6_B           ",aff_op0,1},
  {"RES_6_C           ",aff_op0,1},
  {"RES_6_D           ",aff_op0,1},
  {"RES_6_E           ",aff_op0,1},
  {"RES_6_H           ",aff_op0,1},
  {"RES_6_L           ",aff_op0,1},
  {"RES_6_MEM_HL      ",aff_op0,1},
  {"RES_6_A           ",aff_op0,1},
  {"RES_7_B           ",aff_op0,1},
  {"RES_7_C           ",aff_op0,1},
  {"RES_7_D           ",aff_op0,1},
  {"RES_7_E           ",aff_op0,1},
  {"RES_7_H           ",aff_op0,1},
  {"RES_7_L           ",aff_op0,1},
  {"RES_7_MEM_HL      ",aff_op0,1},
  {"RES_7_A           ",aff_op0,1},
  {"SET_0_B           ",aff_op0,1},
  {"SET_0_C           ",aff_op0,1},
  {"SET_0_D           ",aff_op0,1},
  {"SET_0_E           ",aff_op0,1},
  {"SET_0_H           ",aff_op0,1},
  {"SET_0_L           ",aff_op0,1},
  {"SET_0_MEM_HL      ",aff_op0,1},
  {"SET_0_A           ",aff_op0,1},
  {"SET_1_B           ",aff_op0,1},
  {"SET_1_C           ",aff_op0,1},
  {"SET_1_D           ",aff_op0,1},
  {"SET_1_E           ",aff_op0,1},
  {"SET_1_H           ",aff_op0,1},
  {"SET_1_L           ",aff_op0,1},
  {"SET_1_MEM_HL      ",aff_op0,1},
  {"SET_1_A           ",aff_op0,1},
  {"SET_2_B           ",aff_op0,1},
  {"SET_2_C           ",aff_op0,1},
  {"SET_2_D           ",aff_op0,1},
  {"SET_2_E           ",aff_op0,1},
  {"SET_2_H           ",aff_op0,1},
  {"SET_2_L           ",aff_op0,1},
  {"SET_2_MEM_HL      ",aff_op0,1},
  {"SET_2_A           ",aff_op0,1},
  {"SET_3_B           ",aff_op0,1},
  {"SET_3_C           ",aff_op0,1},
  {"SET_3_D           ",aff_op0,1},
  {"SET_3_E           ",aff_op0,1},
  {"SET_3_H           ",aff_op0,1},
  {"SET_3_L           ",aff_op0,1},
  {"SET_3_MEM_HL      ",aff_op0,1},
  {"SET_3_A           ",aff_op0,1},
  {"SET_4_B           ",aff_op0,1},
  {"SET_4_C           ",aff_op0,1},
  {"SET_4_D           ",aff_op0,1},
  {"SET_4_E           ",aff_op0,1},
  {"SET_4_H           ",aff_op0,1},
  {"SET_4_L           ",aff_op0,1},
  {"SET_4_MEM_HL      ",aff_op0,1},
  {"SET_4_A           ",aff_op0,1},
  {"SET_5_B           ",aff_op0,1},
  {"SET_5_C           ",aff_op0,1},
  {"SET_5_D           ",aff_op0,1},
  {"SET_5_E           ",aff_op0,1},
  {"SET_5_H           ",aff_op0,1},
  {"SET_5_L           ",aff_op0,1},
  {"SET_5_MEM_HL      ",aff_op0,1},
  {"SET_5_A           ",aff_op0,1},
  {"SET_6_B           ",aff_op0,1},
  {"SET_6_C           ",aff_op0,1},
  {"SET_6_D           ",aff_op0,1},
  {"SET_6_E           ",aff_op0,1},
  {"SET_6_H           ",aff_op0,1},
  {"SET_6_L           ",aff_op0,1},
  {"SET_6_MEM_HL      ",aff_op0,1},
  {"SET_6_A           ",aff_op0,1},
  {"SET_7_B           ",aff_op0,1},
  {"SET_7_C           ",aff_op0,1},
  {"SET_7_D           ",aff_op0,1},
  {"SET_7_E           ",aff_op0,1},
  {"SET_7_H           ",aff_op0,1},
  {"SET_7_L           ",aff_op0,1},
  {"SET_7_MEM_HL      ",aff_op0,1},
  {"SET_7_A           ",aff_op0,1}};

int aff_op(UINT8 op,UINT16 pc,char *ret) {
  return tab_op[op].aff_op(tab_op[op].format,pc,ret);  
}

int get_nb_byte(UINT8 op) {
  return tab_op[op].nb_byte;
}

/*****************************************************/

int aff_op0(char *format,UINT16 pc,char *ret) {
  ret[0]=0;
  sprintf(ret,format);
  return 0;
}

int aff_op1(char *format,UINT16 pc,char *ret) {
  ret[0]=0;
  sprintf(ret,format,mem_read(pc+1));
  return 1;
}

int aff_op2(char *format,UINT16 pc,char *ret) {
  ret[0]=0;
  sprintf(ret,format,(mem_read(pc+2)<<8)|mem_read(pc+1));
  return 2;
}

int aff_op_cb(char *format,UINT16 pc,char *ret) {
  tab_op[mem_read(pc+1)+256].aff_op(tab_op[mem_read(pc+1)+256].format,pc+1,ret);  
  return 1;
}
