/** 
 * @file         FileRW.h
 * @brief         
 * @details  
 * @author       Pengxiang Jin
 * @date     	2017年11月1日
 * @version  	Version  
 * @par Copyright (c):  
 *       桂林优利特医疗电子有限公司 
 * @par History:          
 *   version: author, date, desc\n 
 */

#ifndef FILERW_H_
#define FILERW_H_

#include <sys/types.h>

#define MAX_BUFF 2000

/*
 *
 */
class FileRW {
public:
	FileRW();
	virtual ~FileRW();
	static int readFile(unsigned char* read_buff, uint &read_bytes,char *path);
	static int writeFile(const unsigned char *write_buff,uint write_bytes,char *path);
};

#endif /* FILERW_H_ */
