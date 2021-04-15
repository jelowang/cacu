
#ifdef WIN32
#include "h3Std.h"
#include "CacuParticleEditor.h"
#include "imgui.h"
#include "H3Util.h"

extern ImVec2 mousePosition ; 
extern int cacuMainWindowWidth ;
extern int cacuMainWindowHeight ;

static bool gEnableParticleEditor = false ;

namespace CacuParticleEditor {

void showFpsOverlay()
{
	static bool opened = false ;
    ImGui::SetNextWindowPos(ImVec2(5,cacuMainWindowHeight-65));
    if (!ImGui::Begin("System Overlay", &opened, ImVec2(0,0), 0.3f, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoSavedSettings))
    {
        ImGui::End();
        return;
    }
    
    ImGui::TextColored(ImVec4(100,100,100,255),"(%.1f,%.1f)", mousePosition.x, mousePosition.y);
	ImGui::TextColored(ImVec4(100,100,100,255),"(%.1f FPS)", ImGui::GetIO().Framerate);
    ImGui::End();
}

void enableParticleEditor( bool enable ) 
{
	gEnableParticleEditor = enable ;
}

void drawParticleEditor ( HpEmitter* et , HpParticle* particle ) 
{

	if ( false == gEnableParticleEditor ) 
		return ;

	static bool opened ;
    if (!ImGui::Begin(H3UtilGbk2Utf8(string("Particle Editor")).data(), &opened))
    {
        ImGui::End();
        return;
    }

	ImGui::PushItemWidth(100);
	ImGui::Combo(H3UtilGbk2Utf8(string("运动模式")).data(), (int* )&et->m_mode, "Gravity\0Circle\0"); 
	ImGui::Combo(H3UtilGbk2Utf8(string("随机数机制")).data(), (int* )&et->m_randmode, "Once\0Reset\0"); 
	ImGui::InputFloat(H3UtilGbk2Utf8(string("系统重力")).data(), &et->m_gravity);
	ImGui::Separator();
	float force2[2] = { et->m_orignal_data.m_force.m_x,et->m_orignal_data.m_force.m_y};
	ImGui::InputFloat(H3UtilGbk2Utf8(string("横向力")).data(), &force2[0]);
	ImGui::InputFloat(H3UtilGbk2Utf8(string("纵向力")).data(), &force2[1]);
	particle->m_force.m_x = force2[0] ;
	particle->m_force.m_y = force2[1] ;
	ImGui::Separator();
	ImGui::DragFloat(H3UtilGbk2Utf8(string("生命周期")).data(), &particle->m_lifeSpan, 0.005f, 0.0f, 1000.0f, "%1.3f") ;
	ImGui::DragFloat(H3UtilGbk2Utf8(string("生命周期变化范围")).data(), &particle->m_lifeSpanVariance, 0.005f, 0.0f, 1000.0f, "%1.3f") ;
	ImGui::DragFloat(H3UtilGbk2Utf8(string("摩擦系数")).data(), &particle->m_friction, 0.005f, 0.0f, 1000.0f, "%1.3f") ;
	ImGui::DragFloat(H3UtilGbk2Utf8(string("质量")).data(), &particle->m_mass, 0.005f, 0.0f, 1000.0f, "%1.3f") ;
	
	ImGui::Separator();
	ImGui::PushItemWidth(200);
	float startCol4[4] = { 
		particle->m_color.m_startColor.r,
		particle->m_color.m_startColor.g,
		particle->m_color.m_startColor.b,
		particle->m_color.m_startColor.a };
	float startColVariance4[4] = { 		
		particle->m_color.m_startColorVariance.r,
		particle->m_color.m_startColorVariance.g,
		particle->m_color.m_startColorVariance.b,
		particle->m_color.m_startColorVariance.a };
	ImGui::ColorEdit4(H3UtilGbk2Utf8(string("起点颜色")).data(), startCol4);
	ImGui::ColorEdit4(H3UtilGbk2Utf8(string("起点颜色变化范围")).data(), startColVariance4);
	particle->m_color.m_startColor.r = startCol4[0] ;
	particle->m_color.m_startColor.g = startCol4[1] ;
	particle->m_color.m_startColor.b = startCol4[2] ;
	particle->m_color.m_startColor.a = startCol4[3] ;
	particle->m_color.m_startColorVariance.r = startColVariance4[0] ;
	particle->m_color.m_startColorVariance.g = startColVariance4[1] ;
	particle->m_color.m_startColorVariance.b = startColVariance4[2] ;
	particle->m_color.m_startColorVariance.a = startColVariance4[3] ;

	ImGui::Separator();
	float endCol4[4] = { 
		particle->m_color.m_endColor.r,
		particle->m_color.m_endColor.g,
		particle->m_color.m_endColor.b,
		particle->m_color.m_endColor.a };
	float endColVariance4[4] = { 		
		particle->m_color.m_endColorVariance.r,
		particle->m_color.m_endColorVariance.g,
		particle->m_color.m_endColorVariance.b,
		particle->m_color.m_endColorVariance.a };
	ImGui::ColorEdit4(H3UtilGbk2Utf8(string("终点颜色")).data(), endCol4);
	ImGui::ColorEdit4(H3UtilGbk2Utf8(string("终点颜色变化范围")).data(), endColVariance4);
	particle->m_color.m_endColor.r = endCol4[0] ;
	particle->m_color.m_endColor.g = endCol4[1] ;
	particle->m_color.m_endColor.b = endCol4[2] ;
	particle->m_color.m_endColor.a = endCol4[3] ;
	particle->m_color.m_endColorVariance.r = endColVariance4[0] ;
	particle->m_color.m_endColorVariance.g = endColVariance4[1] ;
	particle->m_color.m_endColorVariance.b = endColVariance4[2] ;
	particle->m_color.m_endColorVariance.a = endColVariance4[3] ;
	ImGui::PopItemWidth();
	ImGui::Separator();

    ImVec2 ClipSize(100, 100), offset(50, 20);
    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImVec4 clip_rect(pos.x, pos.y, pos.x+ClipSize.x, pos.y+ClipSize.y);
    ImGui::InvisibleButton("##dummy", ClipSize);
    ImGui::GetWindowDrawList()->PushTextureID((ImTextureID* )et->m_hpeTexture.m_framelist[0].m_id);
	ImGui::GetWindowDrawList()->AddRectFilled(pos, ImVec2(pos.x+ClipSize.x,pos.y+ClipSize.y), ImColor(90,90,120,255));
	ImGui::GetWindowDrawList()->AddImage ( (ImTextureID* )et->m_hpeTexture.m_framelist[0].m_id , pos , ImVec2(pos.x+ClipSize.x,pos.y+ClipSize.y) ) ;
	ImGui::GetWindowDrawList()->PopTextureID();

	int textureStartWidth = (int )particle->m_size.m_startSizeHorizontal ;
	int textureStartWidthVariance = (int )particle->m_size.m_startSizeHorizontalVariance ;
	int textureStartHeight = (int )particle->m_size.m_startSizeVertical ;
	int textureStartHeightVariance = (int )particle->m_size.m_startSizeVerticalVariance ;
	ImGui::InputInt(H3UtilGbk2Utf8(string("起点宽度")).data(), &textureStartWidth );
	ImGui::InputInt(H3UtilGbk2Utf8(string("起点宽度变化范围")).data(), &textureStartWidthVariance );
	ImGui::InputInt(H3UtilGbk2Utf8(string("起点高度")).data(), &textureStartHeight );
	ImGui::InputInt(H3UtilGbk2Utf8(string("起点高度变化范围")).data(), &textureStartHeightVariance );
	particle->m_size.m_startSizeHorizontal = (float ) textureStartWidth ;
	particle->m_size.m_startSizeHorizontalVariance = (float ) textureStartWidthVariance ;
	particle->m_size.m_startSizeVertical = (float ) textureStartHeight ;
	particle->m_size.m_startSizeVerticalVariance = (float ) textureStartHeightVariance ;

	ImGui::Separator();
	int textureEndWidth = (int )particle->m_size.m_endSizeHorizontal ;
	int textureEndWidthVariance = (int )particle->m_size.m_endSizeHorizontalVariance ;
	int textureEndHeight = (int )particle->m_size.m_endSizeVertical ;
	int textureEndHeightVariance = (int )particle->m_size.m_endSizeVerticalVariance ;
	ImGui::InputInt(H3UtilGbk2Utf8(string("终点宽度")).data(), &textureEndWidth );
	ImGui::InputInt(H3UtilGbk2Utf8(string("终点宽度变化范围")).data(), &textureEndWidthVariance );
	ImGui::InputInt(H3UtilGbk2Utf8(string("终点高度")).data(), &textureEndHeight );
	ImGui::InputInt(H3UtilGbk2Utf8(string("终点高度变化范围")).data(), &textureEndHeightVariance );
	particle->m_size.m_endSizeHorizontal = (float ) textureEndWidth ;
	particle->m_size.m_endSizeHorizontalVariance = (float ) textureEndWidthVariance ;
	particle->m_size.m_endSizeVertical = (float ) textureEndHeight ;
	particle->m_size.m_endSizeVerticalVariance = (float ) textureEndHeightVariance ;	

	ImGui::Separator();
	ImGui::LabelText("", H3UtilGbk2Utf8(string("自转参数编辑")).data());
	ImGui::DragFloat(H3UtilGbk2Utf8(string("起点角度")).data(), &particle->m_rotation.m_startAngle, 0.005f, 0.0f, 360.0f, "%1.3f") ;
	ImGui::DragFloat(H3UtilGbk2Utf8(string("起点角度变化范围")).data(), &particle->m_rotation.m_startAngleVariance, 0.005f, 0.0f, 360.0f, "%1.3f") ;
	ImGui::DragFloat(H3UtilGbk2Utf8(string("终点角度")).data(), &particle->m_rotation.m_endAngle, 0.005f, 0.0f, 360.0f, "%1.3f") ;
	ImGui::DragFloat(H3UtilGbk2Utf8(string("终点角度变化范围")).data(), &particle->m_rotation.m_endAngleVariance, 0.005f, 0.0f, 360.0f, "%1.3f") ;
	ImGui::Separator();
	ImGui::LabelText("", H3UtilGbk2Utf8(string("公转参数编辑")).data());
	ImGui::DragFloat(H3UtilGbk2Utf8(string("角速度")).data(), &particle->m_revolution.m_angularSpeed, 0.005f, 0.0f, 360.0f, "%1.3f") ;
	ImGui::DragFloat(H3UtilGbk2Utf8(string("角速度变化范围")).data(), &particle->m_revolution.m_angularSpeedVariance, 0.005f, 0.0f, 360.0f, "%1.3f") ;
	ImGui::Separator();
	ImGui::DragFloat(H3UtilGbk2Utf8(string("起点半径")).data(), &particle->m_revolution.m_startRadius, 0.005f, 0.0f, 360.0f, "%1.3f") ;
	ImGui::DragFloat(H3UtilGbk2Utf8(string("起点半径变化范围")).data(), &particle->m_revolution.m_startRadiusVariance, 0.005f, 0.0f, 360.0f, "%1.3f") ;
	ImGui::DragFloat(H3UtilGbk2Utf8(string("终点半径")).data(), &particle->m_revolution.m_endRadius, 0.005f, 0.0f, 360.0f, "%1.3f") ;
	ImGui::DragFloat(H3UtilGbk2Utf8(string("终点半径变化范围")).data(), &particle->m_revolution.m_endRadiusVariance, 0.005f, 0.0f, 360.0f, "%1.3f") ;
	ImGui::PopItemWidth();
	ImGui::End() ;

	if ( particle->m_size.m_startSizeHorizontal != particle->m_size.m_startSizeVertical ) {
		et->m_sizemode = HpSizeModeVHAsDiffRatio ;
	} else {
		et->m_sizemode = HpSizeModeVHAsSameRatio ;
	}

	

}	
	

} ;
#endif	//#ifdef WIN32