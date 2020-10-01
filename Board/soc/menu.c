#include "GLOBAL.h"
#include "stm32f10x.h"
#include "oled.h"
#include "wheel.h"
#include "menu.h"



/**
 * @brief	显示菜单
 */ 
uint8_t menu_active(char **menuText, uint8_t numItems)
{
    uint8_t i;
    uint8_t position = 0;
    uint8_t lastPosition = 9;

    if (numItems > 6){                                              // Screen is 8 lines tall = 1
                                                                    // title line + 7 items max
        numItems = 6;
    }
    OLED_clear();
    OLED_show8x16string(0, 0, menuText[0]); // Print the title
		
		key_1 = 0;
    key_2 = 0;
    while (key_2 != 1 && key_1 != 1)                                         // Menu active until selection
                                                                    // is made
    {
        position = get_wheel_position(numItems);
        if (position == numItems){		//此处这样写可防止闪烁
            position = numItems - 1;
        }
        //else if (position == 0){
          //  position = 1;
        //}
        if (position != lastPosition)                               // Update position if it is
                                                                    // changed
        {
            for (i = 1; i < numItems + 1; i++)                      // Display menu items
            {
                if (i != position+1){
                    OLED_show6x8string(0, i + 1, menuText[i]);
                }
                else {
                    // Highlight item at current position
                    OLED_show6x8string_inverse(0, i + 1, menuText[i]);
                }
            }
            lastPosition = position;
        }
    }
		key_2 = 0;
		
		if (key_1 == 0)
			return position;
		else {
			key_1 = 0;
			return 0xff;
		}
}

