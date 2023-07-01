#include "disp.h"


uint8_t _disp_code[SMG_NUM]     = {0, 0, 0, 0};
uint8_t _flick_status[SMG_NUM]  = {1, 1, 1, 1};
uint16_t _flick_cycle[SMG_NUM]  = {0, 0, 0, 0};     /* 闪烁半周期，单位ms */


/******************************************************************************/
void disp_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure; 

    /* 使能相关时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    
    /* 禁用JTAG，启用SWJ。(PB3/PB4/PA15 可重映射为其他功能) */
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    
    /* GPIO配置 */
    GPIO_InitStructure.GPIO_Pin = SMG_A | SMG_B | SMG_C | SMG_D | SMG_E | SMG_F | SMG_G;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = SMG_DP | SMG_N1 | SMG_N2 | SMG_N3 | SMG_N4;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_SetBits(GPIOA, SMG_N1);
    GPIO_SetBits(GPIOA, SMG_N2);
    GPIO_SetBits(GPIOA, SMG_N3);
    GPIO_SetBits(GPIOA, SMG_N4);
}

void _disp_set_seg_code(uint8_t code)
{ 
    GPIO_WriteBit(GPIOB, SMG_A, (BitAction)(code & 0x01));
    GPIO_WriteBit(GPIOB, SMG_B, (BitAction)(code & 0x02));
    GPIO_WriteBit(GPIOB, SMG_C, (BitAction)(code & 0x04));
    GPIO_WriteBit(GPIOB, SMG_D, (BitAction)(code & 0x08));
    GPIO_WriteBit(GPIOB, SMG_E, (BitAction)(code & 0x10));
    GPIO_WriteBit(GPIOB, SMG_F, (BitAction)(code & 0x20));
    GPIO_WriteBit(GPIOB, SMG_G, (BitAction)(code & 0x40));
    GPIO_WriteBit(GPIOA, SMG_DP, (BitAction)(code & 0x80));
}

void _disp_set_addr(uint8_t addr)
{
    if (addr >= SMG_NUM)
    {
        return;
    }
    
    GPIO_SetBits(GPIOA, SMG_N1);
    GPIO_SetBits(GPIOA, SMG_N2);
    GPIO_SetBits(GPIOA, SMG_N3);
    GPIO_SetBits(GPIOA, SMG_N4);
    
    switch (addr)
    {
        case 0: GPIO_ResetBits(GPIOA, SMG_N1); break;
        case 1: GPIO_ResetBits(GPIOA, SMG_N2); break;
        case 2: GPIO_ResetBits(GPIOA, SMG_N3); break;
        case 3: GPIO_ResetBits(GPIOA, SMG_N4); break;
        default: break;
    }
}

/******************************************************************************/
void disp_set_code(uint8_t dat[SMG_NUM])
{
    uint8_t i;
    
    for (i = 0; i < SMG_NUM; i++)
    {
        _disp_code[i] = dat[i];
    }
}

/******************************************************************************/
void disp_set_string(char *str)
{
    uint8_t i, j;
    
    for (i = 0, j = 0; j < SMG_NUM && str[i] != '\0'; i++, j++)
    {
        switch (str[i])
        {
            case '0': _disp_code[j] = 0x3F; break;
            case '1': _disp_code[j] = 0x06; break;
            case '2': _disp_code[j] = 0x5B; break;
            case '3': _disp_code[j] = 0x4F; break;
            case '4': _disp_code[j] = 0x66; break;
            case '5': _disp_code[j] = 0x6D; break;
            case '6': _disp_code[j] = 0x7D; break;
            case '7': _disp_code[j] = 0x07; break;
            case '8': _disp_code[j] = 0x7F; break;
            case '9': _disp_code[j] = 0x6F; break;
            case 'A':
            case 'a': _disp_code[j] = 0x77; break;
            case 'B':
            case 'b': _disp_code[j] = 0x7C; break;
            case 'C': _disp_code[j] = 0x39; break;
            case 'c': _disp_code[j] = 0x58; break;
            case 'D':
            case 'd': _disp_code[j] = 0x5E; break;
            case 'E':
            case 'e': _disp_code[j] = 0x79; break;
            case 'F':
            case 'f': _disp_code[j] = 0x71; break;
            case 'G':
            case 'g': _disp_code[j] = 0x3d; break;
            case 'H': _disp_code[j] = 0x76; break;
            case 'h': _disp_code[j] = 0x74; break;
            case 'J':
            case 'j': _disp_code[j] = 0x1E; break;
            case 'K':
            case 'k': _disp_code[j] = 0x70; break;
            case 'L':
            case 'l': _disp_code[j] = 0x38; break;
            case 'N':
            case 'n': _disp_code[j] = 0x37; break;
            case 'O': _disp_code[j] = 0x3F; break;
            case 'o': _disp_code[j] = 0x5C; break;
            case 'P':
            case 'p': _disp_code[j] = 0x73; break;
            case 'Q':
            case 'q': _disp_code[j] = 0x67; break;
            case 'R':
            case 'r': _disp_code[j] = 0x50; break;
            case 'T':
            case 't': _disp_code[j] = 0x78; break;
            case 'U':
            case 'u': _disp_code[j] = 0x3E; break;
            case 'Y':
            case 'y': _disp_code[j] = 0x6e; break;            
            case ' ': _disp_code[j] = 0x00; break;
            case '-': _disp_code[j] = 0x40; break;
            default: _disp_code[j] = 0x00; break;
        }
        if (str[i+1] == '.')
        {
            _disp_code[j] |= 0x80;
            i++;
        }
    }
    if (j != SMG_NUM)
    {
        for (; j < SMG_NUM; j++)
        {
            _disp_code[j] = 0x00;
        }
    }
}

/******************************************************************************/
void disp_set_flick_status(uint8_t addr, uint8_t status)
{
    if (addr >= SMG_NUM || status > 1)
    {
        return;
    }
    _flick_status[addr] = status;
}

/******************************************************************************/
uint8_t disp_get_flick_status(uint8_t addr)
{
    if (addr >= SMG_NUM)
    {
        return 0;
    }
    return _flick_status[addr];
}

/******************************************************************************/
void disp_set_flick_cycle(uint8_t addr, uint16_t cycle)
{
    if (addr >= SMG_NUM)
    {
        return;
    }
    _flick_cycle[addr] = cycle;
    _flick_status[addr] = 1;
}

/******************************************************************************/
void disp_set_all_flick_cycle(uint16_t cycle)
{
    uint8_t i = 0;
    
    for (i = 0; i < SMG_NUM; i++)
    {
        _flick_cycle[i] = cycle;
        _flick_status[i] = 1;
    }
}

/******************************************************************************/
uint16_t disp_get_flick_cycle(uint8_t addr)
{
    if (addr >= SMG_NUM)
    {
        return 0;
    }
    return _flick_cycle[addr];
}

/******************************************************************************/
void disp_code(void)
{
    static uint8_t addr = 0;

    _disp_set_seg_code((_flick_status[addr] == 1) ? _disp_code[addr] : 0x00);
    _disp_set_addr(addr);
    
    addr++;
    if (addr == SMG_NUM)
    {
        addr = 0;
    }
}


