
#ifndef StudioWindow_H
#define StudioWindow_H

#include "H3Std.h"
#include "H3Texture.h"
#include "H3Scene.h"
#include "editor.h"
#include "H3DrawPrimitives.h"
#include "H3Image.h"

typedef struct {

	iF32 x ;
	iF32 y ;
	iF32 z ;

} SodaVector3f ;

typedef struct {

	iF32 red ;
	iF32 green ;
	iF32 blue ;
	iF32 alpha ;

}SodaColor;

typedef struct {

	SodaVector3f axis ; 
	iF32 amount ;

}SodaRotate;

typedef struct {

	SodaColor ambientColor ;
	SodaColor diffuseColor ;
	SodaColor specularColor ;

	iF32 specularIntensity ;
	iF32 shininess ;

	iS8 diffuseTextureName[128] ;
	iU32 totallDiffuseTexCoords ;
	iF32* diffuseTexCoords ;
		
	iS8 normalTextureName[128] ;

}SodaMaterial;

typedef struct {

	iS8 type[4];
	iS8* version ;

	iU32 totalMapTable ;
	iU32 mapTableOffset ;

}SODAMAPHEADER;

typedef struct {

	iU32 totalLight ;
	iU32 lightTableOffset ;

	iU32 totalRoot ;
	iU32 totalObject ;
	iU32 objectTableOffset ;

	iU32 totalBillboard ;
	iU32 billboardTableOffset ;

	iU32 totalMesh ;
	iU32 meshTableOffset ;

}SODAMAPTABLE;

typedef struct {

	iS8 InfoFilePath[128] ;

	SodaVector3f position ;
	SodaVector3f scale ;
	SodaRotate rotate ;

	//	如果是广告牌的话，billboard保存半径
	iU32 billboard ;
	iU32 parent ;
	iU32 index ;
	iU32 childReference1 ;//child index
	iU32 childReference2 ;
	//iS8 childReference[256] ;
	iU32 totalChild ;
	
	SodaMaterial material ;
	//SodaMaterial* materials ;
	iU32 totallMaterial ;

}SODAOBJECTTABLE;

typedef struct {

	iU32 lightType ;
	SodaColor color ;
	SodaVector3f position ;
	SodaVector3f direction ;
	iF32 intensity ;
	
}SODALIGHTTABLE;

typedef struct{ 

	iU32 objectIndex ;
	iU32 meshId ;

	iS8 name[128] ;
	iS8 texture[128] ;
	iF32 raduis ;
	SodaVector3f position ;
	SodaVector3f scale ;
	SodaColor color ;

} SodaBillboardTable ;

typedef struct{
	iU32 objectIndex ;
	iU32 meshId ;
	iS8 name[128] ;
	SodaColor color ;
	iF32 sss ;
	iF32 shininess ;
	iF32 specularIntensity ;
	iF32 fresnel ;
}SODAMESHTABLE ;

typedef struct {
	iU32 type ;
	void* lightRef ;
} H3LightAtom ;

typedef struct {

	//20160408 add
	bool isclick ;      //默认false ，用于界面选择标识

	bool isBillboard ;

	H3Object* objectRef ;
	//void* billboardRef ;

	iS32 objectIndex ;       //加载时自动生成     object为自身索引号，billboard为所属object索引号
	iS32 meshId ;            //广告牌
	iS32 size ;
	iS8 name[128] ;
	SodaColor color ;
	SodaVector3f scale ;
	SodaRotate rotate ;

	iS8 filePath[128] ;
	iS8 texturePath[128] ;


}SodaObjectInfo;

typedef struct {
	iS32 objectIndex ;
	iS32 meshId ;
	iS8 name[128] ;
	SodaColor color ;
	SodaVector3f position ;
	iF32 sss ;
	iF32 shininess ;
	iF32 specularIntensity ;
	iF32 fresnel ;
}SodaMesh ;

typedef enum
{
	StudioCameraType_Orthographics = 0,
	StudioCameraType_Perspective = 1 ,

} SodaStudioCameraType ;

class StudioWindow : public H3Scene , virtual public H3Memory
{
public :

	/*
     brief :
    */
	StudioWindow() ;

	/*
     brief :
    */
	~StudioWindow () ;

	/*
     brief :
    */
	bool init ( iU32 width , iU32 height ) ;

	/*
     brief :
    */
	virtual void update ( iF32 deltaT ) ;

    /*
     brief :
    */
	virtual void resize ( iU32 width , iU32 height ) ;

    /*
     brief :
    */
	virtual void keyEvent ( H3KeyEvent k ) ;

    /*
     brief :
    */
	virtual void mouseEvent ( H3MouseEvent e ) ;

	/*
	brief :
	*/
	void draw(iF32 deltaT);

	/*
     brief :
    */
	void addObject ( H3Object* object ) ;

	/*
     brief :
    */
	void deleteObject ( H3Object* object ) ;

	/*
	 brief :
	*/
	H3Object* generateObject ( iS8* filepath ) ;

	/*
	 brief :
	*/
	H3Image* createLogoImage ( iS8* filepath ) ;

	/*
	 brief :
	*/
	H3Image* createTextImage ( iS8* words ) ;

	/*
	 brief :
	*/
	H3Image* creatBillboardImage ( H3Image* logoImage , H3Image* wordsImage ) ;

	/*
	 brief :
	*/
	H3Image* setImageRoundRect ( H3Image* image , iF32 radiusRatio ) ;

	/*
	 brief :
	*/
	H3Image* setFontScale ( H3Image* image , iS32 radius ) ;

	/*
	 brief :
	*/
	H3Image* setImageBorder ( H3Image* image) ;

	/*
	 brief :
	*/
	void addLight ( iS32 type ) ;

	/*
     brief : 判定选中哪一个物体
    */
	void chooseObject( SCClList* objects ) ;
		
	/*
     brief : 判定是否选中物体
    */
	int isCurrentObjectNull ( H3Object* object ) ;

	/*
	 brief : 20160303
	*/
	void loadFile ( iS8* path ) ;

	/*
	 brief : 
	*/
	void setBlendShapeWeight ( iF32 weight ) ;

	/*
	 breif :
	*/
	iF32 getBlendShapeWeight () ;

	/*
	 brief : 20160304
	*/
	SodaObjectInfo* selectObjectInfo ( H3Object* object ) ;

	/*
	 brief : 20160309
	*/
	iS32 selectObjectIndex ( H3Object* object ) ;

	/*
	 brief : 20160310
	*/
	SodaObjectInfo* selectBillboardInfo ( iS32 objectIndex , iS32 meshId ) ;

	/*
	 brief : 20160315
	*/
	SodaMesh* selectMeshInfo ( iS32 objectIndex , iS32 meshId ) ;

	/*
	 brief : 20160314
	*/
	void scanObject ( SCClList* objectList ) ;

	/*
	 brief : 
	*/
	iS8* filePathTransform( iS8* filepath ) ;

	/*
	 brief :
	*/
	void clean () ;

	/*
     brief :
    */
	virtual void accelerometerEvent ( H3AccelerometerEvent e ) ;
	virtual void loadObject ( const iS8* model , const iS8* mtl ){}


public :

	/*
	 brief : hold SodaObjectInfo* here
	*/
	SCClList objectInfoList ;
	SCClList objectList ;
	SCClList billboardList ;
	SCClList lightList ;
	SCClList meshList ;

	H3AmbientLight* h3AmbientLight ;
	H3Object* currentObject ;
	H3Vector3f intersectPoint ;
	iS32 currentSelectedMeshId ;
	H3Object* currentSelectedObject ;

	//20160409 add
	H3Texture* fontTexture ;
	void* fontData ;
	iS32 fontWidth ;
	iS32 fontHeight ;

	bool show_details_windows ;
	void* gDetailsRef ;
	iS32 gDetailsType ;
	iS32 camerType ;
	iS32 lastCameraType ;

    /*
     brief :
    */
	iS32 width ;
	iS32 height ;

private :

    /*
     brief :
    */
	bool inited ;

	iF32 blendShapeWeight ;

    /*
     brief :
    */
	H3Ray ray ;
	H3Vector2f click ;
	H3Vector2f screen ;

    /*
     brief :
    */
	H3Texture* texture ;

	bool mouseClicked ;
	//H3Vector2f mouseClickedPosition ;

} ;

#endif