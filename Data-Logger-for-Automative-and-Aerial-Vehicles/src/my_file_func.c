/**
 * @file    :   my_file_func.c
 * @brief   :   An abstraction for my file functions
 *
 *              This source file provides abstraction of my file functions which are wrapper functions
 *              around the FAT file functions. They help in reducing the code duplication in the main logic
 *
 * @author  :   Sanish Kharade
 * @date    :   November 28, 2021
 * @version :   1.0
 *
 * @tools   :   Code Composer Studio
 *
 * @link    :   http://elm-chan.org/fsw/ff/00index_e.html
*/

#include <string.h>

#include <fatfs/ff.h>
#include <fatfs/diskio.h>
#include <Devices/MSPIO.h>

#include "my_file_func.h"

void my_file_write(FIL fp, char* filename, const void* buff, UINT btw)
{
    FRESULT fr;
    UINT bw;
    fr = f_open(&fp, filename, FA_WRITE);
    if(fr != FR_OK)
    {
        MSPrintf(EUSCI_A0_BASE, "Error opening file/directory\r\n");
        while(1);
    }

    fr = f_write(&fp, buff, btw, &bw);
    if(fr != FR_OK)
    {
        MSPrintf(EUSCI_A0_BASE, "Error writing to  file/directory\r\n");
        while(1);
    }

    f_close(&fp);
}
void my_file_append(FIL fp, char* filename, const void* buff, UINT btw)
{
    FRESULT fr;
    UINT bw;
    fr = f_open(&fp, filename, (FA_OPEN_APPEND | FA_WRITE) );
    if(fr != FR_OK)
    {
        MSPrintf(EUSCI_A0_BASE, "Error opening %s file/directory\r\n", filename);

        /* Uncomment below line if you want to halt the code at the first error in opening file */
        //while(1);
    }

    fr = f_write(&fp, buff, btw, &bw);
    if(fr != FR_OK)
    {
        MSPrintf(EUSCI_A0_BASE, "Error writing to  file/directory\r\n");

        /* Uncomment below line if you want to halt the code at the first error in opening file */
        //while(1);
    }

    f_close(&fp);
}

void my_file_read(FIL fp, char* filename)
{
    FRESULT fr;
    //UINT bw;
    char line[50] = {0};
    fr = f_open(&fp, filename, FA_READ );
    if(fr != FR_OK)
    {
        MSPrintf(EUSCI_A0_BASE, "Error opening file/directory\r\n");
        while(1);
    }

    MSPrintf(EUSCI_A0_BASE, "\n\rReading output4.txt :\n\r");
    while (f_gets(line, sizeof line, &fp))
    {
        /* Data is printed out on the UART */
        MSPrintf(EUSCI_A0_BASE, line);
        memset(line, 0, sizeof(line));
    }
    f_close(&fp);
}

void my_file_stat(char* filename, FILINFO *fileinfo)
{
    char buf[70]={0};
    FRESULT fr;
    fr = f_stat(filename, fileinfo);
    if(fr == FR_NO_FILE)
    {
        MSPrintf(EUSCI_A0_BASE, "%s - File not found\r\n", filename);
    }
    else if(fr == FR_OK)
    {
        /* Data is printed out on the UART */
        sprintf(buf, "size = %d, date = %d, time = %d, attribute = %c, name= %s \n\r"
                ,(*fileinfo).fsize, (*fileinfo).fdate,(*fileinfo).ftime, (*fileinfo).fattrib, (*fileinfo).fname);
        MSPrintf(EUSCI_A0_BASE, buf);
    }

}
void my_file_print_attributes(FILINFO fileinfo)
{
    char buf[70]={0};

    /* Data is printed out on the UART */
    sprintf(buf, "size = %d, date = %d, time = %d, attribute = %c, name= %s \n\r"
            ,fileinfo.fsize, fileinfo.fdate,fileinfo.ftime, fileinfo.fattrib, fileinfo.fname);

    MSPrintf(EUSCI_A0_BASE, buf);
}

