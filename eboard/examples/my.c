/*****************************************************************************
* | File      	:   EPD_2in15b_test.c
* | Author      :   Waveshare team
* | Function    :   2inch15 e-paper (B) test demo
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2024-08-07
* | Info        :
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to dealv
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included inv
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#include "my.h"
#include "EPD_2in15b.h"
#include <time.h> 

int my(void)
{
    printf("EPD_2IN15B_test Demo\r\n");
    if(DEV_Module_Init()!=0){
        return -1;
    }

    printf("e-Paper Init and Clear...\r\n");
    EPD_2IN15B_Init();
    EPD_2IN15B_Clear();
    DEV_Delay_ms(500);

    //Create a new image cache
    UBYTE *BlackImage, *RedImage;
    /* you have to edit the startup_stm32fxxx.s file and set a big enough heap size */
    UWORD Imagesize = ((EPD_2IN15B_WIDTH % 8 == 0)? (EPD_2IN15B_WIDTH / 8 ): (EPD_2IN15B_WIDTH / 8 + 1)) * EPD_2IN15B_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }
    if((RedImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for red memory...\r\n");
        return -1;
    }
    printf("Paint_NewImage\r\n");
    Paint_NewImage(BlackImage, EPD_2IN15B_WIDTH, EPD_2IN15B_HEIGHT, 270, WHITE);
	  //Paint_Clear(WHITE);
	  Paint_NewImage(RedImage, EPD_2IN15B_WIDTH, EPD_2IN15B_HEIGHT, 270, WHITE);
	  //Paint_Clear(WHITE);


    printf("Drawing\r\n");

    //1.Draw black image
    Paint_NewImage(BlackImage, EPD_2IN15B_WIDTH, EPD_2IN15B_HEIGHT, 270, WHITE);
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    Paint_DrawString_EN(10, 10, "vim", &Font24, BLACK, WHITE);
    Paint_DrawNum(10, 40, 239, &Font24, WHITE, BLACK);

    //2.Draw red image
    Paint_NewImage(RedImage, EPD_2IN15B_WIDTH, EPD_2IN15B_HEIGHT, 270, WHITE);
    Paint_SelectImage(RedImage);
    Paint_Clear(WHITE);
    Paint_DrawString_EN(10, 70, "tmux", &Font24, WHITE, BLACK);
    Paint_DrawNum(10, 100, 30, &Font24, BLACK, WHITE);

    EPD_2IN15B_Display(BlackImage, RedImage);
    DEV_Delay_ms(2000);

    printf("Clear...\r\n");
    //EPD_2IN15B_Clear();

    printf("Goto Sleep...\r\n");
    //EPD_2IN15B_Sleep();
    free(BlackImage);
    BlackImage = NULL;
    DEV_Delay_ms(2000);//important, at least 2s
    // close 5V
    printf("close 5V, Module enters 0 power consumption ...\r\n");
    DEV_Module_Exit();

    return 0;
}

