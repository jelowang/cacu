/****************************************************************************
Copyright (c) 2010-2013 cocos2d-x.org

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
#ifndef __CC_FILEUTILS_H__
#define __CC_FILEUTILS_H__

#include <string>
#include <vector>
#include <map>

/**
 * @addtogroup platform
 * @{
 */

//! @brief  Helper class to handle file operations
class CCFileUtils 
{

public:
  
    
    /**
     *  Gets the instance of CCFileUtils.
     *  @js getInstance
     */
    static CCFileUtils* sharedFileUtils();
    
	CCFileUtils () ;
    /**
     *  The destructor of CCFileUtils.
     *  @js NA
     *  @lua NA
     */
    virtual ~CCFileUtils();
    
    /**
     *  Gets resource file data
     *
     *  @param[in]  pszFileName The resource file name which contains the path.
     *  @param[in]  pszMode The read mode of the file.
     *  @param[out] pSize If the file read operation succeeds, it will be the data size, otherwise 0.
     *  @return Upon success, a pointer to the data is returned, otherwise NULL.
     *  @warning Recall: you are responsible for calling delete[] on any Non-NULL pointer returned.
     *  @js NA
     */
    virtual unsigned char* getFileData(const char* pszFileName, const char* pszMode, unsigned long * pSize);

    
    /** Returns the fullpath for a given filename.
     
     First it will try to get a new filename from the "filenameLookup" dictionary.
     If a new filename can't be found on the dictionary, it will use the original filename.
     Then it will try to obtain the full path of the filename using the CCFileUtils search rules: resolutions, and search paths.
     The file search is based on the array element order of search paths and resolution directories.
     
     For instance:

     	We set two elements("/mnt/sdcard/", "internal_dir/") to search paths vector by setSearchPaths,
     	and set three elements("resources-ipadhd/", "resources-ipad/", "resources-iphonehd")
     	to resolutions vector by setSearchResolutionsOrder. The "internal_dir" is relative to "Resources/".

		If we have a file named 'sprite.png', the mapping in fileLookup dictionary contains `key: sprite.png -> value: sprite.pvr.gz`.
     	Firstly, it will replace 'sprite.png' with 'sprite.pvr.gz', then searching the file sprite.pvr.gz as follows:

     	    /mnt/sdcard/resources-ipadhd/sprite.pvr.gz      (if not found, search next)
     	    /mnt/sdcard/resources-ipad/sprite.pvr.gz        (if not found, search next)
     	    /mnt/sdcard/resources-iphonehd/sprite.pvr.gz    (if not found, search next)
     	    /mnt/sdcard/sprite.pvr.gz                       (if not found, search next)
     	    internal_dir/resources-ipadhd/sprite.pvr.gz     (if not found, search next)
     	    internal_dir/resources-ipad/sprite.pvr.gz       (if not found, search next)
     	    internal_dir/resources-iphonehd/sprite.pvr.gz   (if not found, search next)
     	    internal_dir/sprite.pvr.gz                      (if not found, return "sprite.png")

        If the filename contains relative path like "gamescene/uilayer/sprite.png",
        and the mapping in fileLookup dictionary contains `key: gamescene/uilayer/sprite.png -> value: gamescene/uilayer/sprite.pvr.gz`.
        The file search order will be:

     	    /mnt/sdcard/gamescene/uilayer/resources-ipadhd/sprite.pvr.gz      (if not found, search next)
     	    /mnt/sdcard/gamescene/uilayer/resources-ipad/sprite.pvr.gz        (if not found, search next)
     	    /mnt/sdcard/gamescene/uilayer/resources-iphonehd/sprite.pvr.gz    (if not found, search next)
     	    /mnt/sdcard/gamescene/uilayer/sprite.pvr.gz                       (if not found, search next)
     	    internal_dir/gamescene/uilayer/resources-ipadhd/sprite.pvr.gz     (if not found, search next)
     	    internal_dir/gamescene/uilayer/resources-ipad/sprite.pvr.gz       (if not found, search next)
     	    internal_dir/gamescene/uilayer/resources-iphonehd/sprite.pvr.gz   (if not found, search next)
     	    internal_dir/gamescene/uilayer/sprite.pvr.gz                      (if not found, return "gamescene/uilayer/sprite.png")

     If the new file can't be found on the file system, it will return the parameter pszFileName directly.
     
     This method was added to simplify multiplatform support. Whether you are using cocos2d-js or any cross-compilation toolchain like StellaSDK or Apportable,
     you might need to load different resources for a given file in the different platforms.

     @since v2.1
     */
    virtual std::string fullPathForFilename(const char* pszFileName);
    
    
    /**
     *  Gets the writable path.
     *  @return  The path that can be write/read a file in
     *  @lua NA
     */
    virtual std::string getWritablePath() = 0;
    
    /**
     *  Checks whether a file exists.
     *
     *  @note If a relative path was passed in, it will be inserted a default root path at the beginning.
     *  @param strFilePath The path of the file, it could be a relative or absolute path.
     *  @return true if the file exists, otherwise it will return false.
     *  @lua NA
     */
    virtual bool isFileExist(const std::string& strFilePath) = 0;
    
    /**
     *  Checks whether the path is an absolute path.
     *
     *  @note On Android, if the parameter passed in is relative to "assets/", this method will treat it as an absolute path.
     *        Also on Blackberry, path starts with "app/native/Resources/" is treated as an absolute path.
     *
     *  @param strPath The path that needs to be checked.
     *  @return true if it's an absolute path, otherwise it will return false.
     *  @lua NA
     */
    virtual bool isAbsolutePath(const std::string& strPath);
    

protected:

    
    /**
     *  Initializes the instance of CCFileUtils. It will set m_searchPathArray and m_searchResolutionsOrderArray to default values.
     *
     *  @note When you are porting Cocos2d-x to a new platform, you may need to take care of this method.
     *        You could assign a default value to m_strDefaultResRootPath in the subclass of CCFileUtils(e.g. CCFileUtilsAndroid). Then invoke the CCFileUtils::init().
     *  @return true if successed, otherwise it returns false.
     *
     */
    virtual bool init();
    
    /**
     *  Gets the new filename from the filename lookup dictionary.
     *  @param pszFileName The original filename.
     *  @return The new filename after searching in the filename lookup dictionary.
     *          If the original filename wasn't in the dictionary, it will return the original filename.
     */
    virtual std::string getNewFilename(const char* pszFileName);
    
    /**
     *  Gets full path for filename, resolution directory and search path.
     *
     *  @param filename The file name.
     *  @param resolutionDirectory The resolution directory.
     *  @param searchPath The search path.
     *  @return The full path of the file. It will return an empty string if the full path of the file doesn't exist.
     */
    virtual std::string getPathForFilename(const std::string& filename, const std::string& resolutionDirectory, const std::string& searchPath);
    
    /**
     *  Gets full path for the directory and the filename.
     *
     *  @note Only iOS and Mac need to override this method since they are using
     *        `[[NSBundle mainBundle] pathForResource: ofType: inDirectory:]` to make a full path.
     *        Other platforms will use the default implementation of this method.
     *  @param strDirectory The directory contains the file we are looking for.
     *  @param strFilename  The name of the file.
     *  @return The full path of the file, if the file can't be found, it will return an empty string.
     */
    virtual std::string getFullPathForDirectoryAndFilename(const std::string& strDirectory, const std::string& strFilename);
    

    
    /** 
     *  The vector contains resolution folders.
     *  The lower index of the element in this vector, the higher priority for this resolution directory.
     */
    std::vector<std::string> m_searchResolutionsOrderArray;
    
    /**
     * The vector contains search paths.
     * The lower index of the element in this vector, the higher priority for this search path.
     */
    std::vector<std::string> m_searchPathArray;
    
    /**
     *  The default root path of resources.
     *  If the default root path of resources needs to be changed, do it in the `init` method of CCFileUtils's subclass.
     *  For instance:
     *  On Android, the default root path of resources will be assigned with "assets/" in CCFileUtilsAndroid::init().
     *  Similarly on Blackberry, we assign "app/native/Resources/" to this variable in CCFileUtilsBlackberry::init().
     */
    std::string m_strDefaultResRootPath;
    
    /**
     *  The singleton pointer of CCFileUtils.
     */
    static CCFileUtils* s_sharedFileUtils;
    
};


#endif    // __CC_FILEUTILS_H__
