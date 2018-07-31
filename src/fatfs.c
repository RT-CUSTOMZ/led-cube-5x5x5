// #include "ff_gen_drv.h"
#include "stm32f4xx_hal.h"
#include "diskio.h"
#include "ff.h"

FATFS SDFatFs;  /* File system object for SD card logical drive */
FIL MyFile;     /* File object */
char SDPath[4]; /* SD card logical drive path */

extern SD_HandleTypeDef uSdHandle;

void SD_Init(void) {
	FRESULT res;                                          /* FatFs function common result code */

	/*##-2- Register the file system object to the FatFs module ##############*/
	if(f_mount(&SDFatFs, (TCHAR const*)SDPath, 0) != FR_OK)
	{
		/* FatFs Initialization Error */
		printf("POST: Error mounting sd-card\n");
	}
	else {
		// DIR dir;
		// res = f_opendir(&dir,"/");
		// printf("opendir: %d\n",res);
		// res = f_opendir(&dir,"/");
		// printf("opendir: %d\n",res);
	}





// 	HAL_StatusTypeDef status;


// // check card status 
// 	HAL_SD_CardStatusTypedef cardStatus;
// 	status = HAL_SD_GetCardStatus(&uSdHandle, &cardStatus);
// 	if(HAL_OK != status)
// 		printf("HAL_SD_Init failes: %d", status);

}
