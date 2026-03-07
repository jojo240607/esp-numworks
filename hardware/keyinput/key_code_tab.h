//
// Created by zhiwei.gong on 2026/1/29.
//

#ifndef KEYSCAN_KEY_CODE_TAB_H
#define KEYSCAN_KEY_CODE_TAB_H

typedef enum _key_code_tab KeycodeTab;

enum _key_code_tab {
    Left=0,  Up=1,     Down=2,    Right=3,           OK=4,              Back=5,
 //   Home=6,  /*B2=7,*/ OnOff=8,   /*B4=9,            B5=10,             B6=11, */
    Shift=6,Alpha=7, XNT=8,    Var=9,            Toolbox=10,        Backspace=11,
    Exp=12,  Ln=13,    Log=14,    Imaginary=15,      Comma=16,          Power=17,
    Sine=18, Cosine=19,Tangent=20,Pi=21,             Sqrt=22,           Square=23,
    Seven=24,Eight=25, Nine=26,   LeftParenthesis=27,RightParenthesis=28,// F6=29,
    Four=30, Five=31,  Six=32,    Multiplication=33, Division=34,        // G6=35,
    One=36,  Two=37,   Three=38,  Plus=39,           Minus=40,           // H6=41,
    Zero=42, Dot=43,   EE=44,     Ans=45,            EXE=46,             // I6=47,
    None = 48
};

#endif //KEYSCAN_KEY_CODE_TAB_H
