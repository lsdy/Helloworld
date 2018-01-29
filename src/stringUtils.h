/*
 * stringtool.h
 *
 *  Created on: 2017年9月21日
 *      Author: jinpengxiang
 */

#ifndef STRINGUTILS_H_
#define STRINGUTILS_H_
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

class StringUtils{
public:
	static int compare(char *str1,char *str2){
		if(str1==NULL||str2==NULL)
		{
			return -1;
		}
		if(strlen(str1)!=strlen(str2)){
			return -1;
		}
		int len=strlen(str1);
		int i;
		for(i=0;i<len;i++){
			if(str1[i]!=str2[i])
				return -i;
		}
		return 1;
	}
};



#endif /* STRINGUTILS_H_ */
