/****************************************************************************
 Copyright (c) 2010 cocos2d-x.org
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
#ifndef __CC_FILEUTILS_ANDROID_H__
#define __CC_FILEUTILS_ANDROID_H__

#include <string>
#include <vector>
#include "CCFileUtils.h"

/**
 * @addtogroup platform
 * @{getFullPathForDirectoryAndFilename
 */

//! @brief  Helper class to handle file operations
class CCFileUtilsAndroid : public CCFileUtils
{
	friend class CCFileUtils;
	public:
    CCFileUtilsAndroid();
    ~CCFileUtilsAndroid();
	static CCFileUtilsAndroid* sharedFileUtilsAndroid();
	
    /* override funtions */
    bool init();
    virtual unsigned char* getFileData(const char* pszFileName, const char* pszMode, unsigned long * pSize);
    virtual std::string getWritablePath();
    virtual bool isFileExist(const std::string& strFilePath);
    virtual bool isAbsolutePath(const std::string& strPath);
	std::string getFullPathForDirectoryAndFilename(const std::string& strDirectory, const std::string& strFilename) ;
	std::string getPathForFilename(const std::string& filename, const std::string& resolutionDirectory, const std::string& searchPath) ;
    std::string fullPathForFilename(const char* pszFileName) ;

	std::vector<std::string> m_searchResolutionsOrderArray;
	std::vector<std::string> m_searchPathArray;
	std::string m_strDefaultResRootPath;
	
private:

    unsigned char* doGetFileData(const char* pszFileName, const char* pszMode, unsigned long * pSize, bool forAsync);
};


#endif    // __CC_FILEUTILS_ANDROID_H__

