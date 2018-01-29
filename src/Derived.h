/** 
 * @file         Derived.h
 * @brief         
 * @details  
 * @author       Pengxiang Jin
 * @date     	2017年12月2日
 * @version  	Version  
 * @par Copyright (c):  
 *       桂林优利特医疗电子有限公司 
 * @par History:          
 *   version: author, date, desc\n 
 */

#ifndef DERIVED_H_
#define DERIVED_H_

#include "Base.h"

/**
 *
 */
class Derived :public Base{
public:
	Derived();
	 ~Derived();

	 void test();
};

#endif /* DERIVED_H_ */
