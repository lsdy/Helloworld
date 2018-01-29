/** 
 * @file         FileRW.cpp
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

#include "FileRW.h"

#include <fcntl.h>
#include <unistd.h>
#include <cstdio>

FileRW::FileRW() {
	// TODO Auto-generated constructor stub

}

FileRW::~FileRW() {
	// TODO Auto-generated destructor stub
}

int FileRW::readFile(unsigned char* read_buff, uint &read_bytes, char *path) {
	FILE* file;
	if ((file = fopen(path, "r")) == NULL) {
		perror("fopen() error.Open file failed\n");
		return -1;
	}
	read_bytes=fread(read_buff,sizeof(unsigned char),MAX_BUFF,file);
	fclose(file);
	return read_bytes;
}

int FileRW::writeFile(const unsigned char *write_buff, uint write_bytes,
		char *path) {
	FILE *file;
	if ((file = fopen(path, "aw+")) == NULL) {
		perror("fopen() error.Open file failed\n");
		return -1;
	}
	int res = fwrite(write_buff, sizeof(unsigned char), write_bytes, file);
	fflush(file);
	fclose(file);
	return res;
}
