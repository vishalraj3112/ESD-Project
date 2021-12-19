/**
 * @file    :   my_file_func.h
 * @brief   :   An abstraction for my file functions
 *
 *              This header file provides abstraction of my file functions which are wrapper functions
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


#ifndef INC_MY_FILE_FUNC_H_
#define INC_MY_FILE_FUNC_H_

#include <fatfs/ff.h>

/*********************************************************************************
 * @function:   file_write
 *
 * @brief   :   wrapper function for f_write
 *              writes a string to the file mentioned
 *
 * @param   :   fp          - file object
 *              filename    - name of the file
 *              buff        - string to be written to the file
 *              btw         - no of bytes to be written
 *
 * @return  :   void
 *
**********************************************************************************/
void my_file_write(FIL fp, char* filename, const void* buff, UINT btw);

/*********************************************************************************
 * @function:   file_append
 *
 * @brief   :   appends a string to the file mentioned
 *
 * @param   :   fp          - file object
 *              filename    - name of the file
 *              buff        - string to be appended to the file
 *              btw         - no of bytes to be appended
 *
 * @return  :   void
 *
**********************************************************************************/
void my_file_append(FIL fp, char* filename, const void* buff, UINT btw);


/*********************************************************************************
 * @function:   file_read
 *
 * @brief   :   wrapper function for f_read
 *              reads full contents of a file line by line
 *
 * @param   :   fp          - file object
 *              filename    - name of the file
 *
 * @return  :   void
 *
**********************************************************************************/
void my_file_read(FIL fp, char* filename);

/*********************************************************************************
 * @function:   file_stat
 *
 * @brief   :   wrapper function for f_stat
 *              gets the info of the file into the fileinfo parameter
 *
 * @param   :   fp          - file object
 *              filename    - name of the file
 *
 * @return  :   void
 *
**********************************************************************************/
void my_file_stat(char* filename, FILINFO *fileinfo);

/*********************************************************************************
 * @function:   print_file_attributes
 *
 * @brief   :   prints the attributes of the file
 *
 * @param   :   fileinfo - FILINFO parameter
 *
 * @return  :   void
 *
**********************************************************************************/
void my_file_print_attributes(FILINFO fileinfo);



#endif /* INC_MY_FILE_FUNC_H_ */
