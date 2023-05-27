#ifndef __DISP_H
#define __DISP_H

#include "ch32f20x.h"
#include "common.h"


#define SMG_A       GPIO_Pin_9      /* PB9 */
#define SMG_B       GPIO_Pin_8      /* PB8 */
#define SMG_C       GPIO_Pin_7      /* PB7 */
#define SMG_D       GPIO_Pin_6      /* PB6 */
#define SMG_E       GPIO_Pin_5      /* PB5 */
#define SMG_F       GPIO_Pin_4      /* PB4 */
#define SMG_G       GPIO_Pin_3      /* PB3 */
#define SMG_DP      GPIO_Pin_15     /* PA15 */

#define SMG_N1      GPIO_Pin_5      /* PA5 */
#define SMG_N2      GPIO_Pin_6      /* PA6 */
#define SMG_N3      GPIO_Pin_7      /* PA7 */
#define SMG_N4      GPIO_Pin_8      /* PA8 */

void disp_init(void);
void disp_set_light_level(uint8_t light_level[SMG_NUM]);
void disp_set_code(uint8_t dat[SMG_NUM]);
void disp_set_string(char *str);
void disp_set_flick_status(uint8_t addr, uint8_t status);
uint8_t disp_get_flick_status(uint8_t addr);
void disp_set_flick_cycle(uint8_t addr, uint16_t cycle);
void disp_set_all_flick_cycle(uint16_t cycle);
uint16_t disp_get_flick_cycle(uint8_t addr);
void disp_code(void);

#endif
