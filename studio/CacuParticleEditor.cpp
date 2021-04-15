
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
	ImGui::Combo(H3UtilGbk2Utf8(string("�˶�ģʽ")).data(), (int* )&et->m_mode, "Gravity\0Circle\0"); 
	ImGui::Combo(H3UtilGbk2Utf8(string("���������")).data(), (int* )&et->m_randmode, "Once\0Reset\0"); 
	ImGui::InputFloat(H3UtilGbk2Utf8(string("ϵͳ����")).data(), &et->m_gravity);
	ImGui::Separator();
	float force2[2] = { et->m_orignal_data.m_force.m_x,et->m_orignal_data.m_force.m_y};
	ImGui::InputFloat(H3UtilGbk2Utf8(string("������")).data(), &force2[0]);
	ImGui::InputFloat(H3UtilGbk2Utf8(string("������")).data(), &force2[1]);
	particle->m_force.m_x = force2[0] ;
	particle->m_force.m_y = force2[1] ;
	ImGui::Separator();
	ImGui::DragFloat(H3UtilGbk2Utf8(string("��������")).data(), &particle->m_lifeSpan, 0.005f, 0.0f, 1000.0f, "%1.3f") ;
	ImGui::DragFloat(H3UtilGbk2Utf8(string("�������ڱ仯��Χ")).data(), &particle->m_lifeSpanVariance, 0.005f, 0.0f, 1000.0f, "%1.3f") ;
	ImGui::DragFloat(H3UtilGbk2Utf8(string("Ħ��ϵ��")).data(), &particle->m_friction, 0.005f, 0.0f, 1000.0f, "%1.3f") ;
	ImGui::DragFloat(H3UtilGbk2Utf8(string("����")).data(), &particle->m_mass, 0.005f, 0.0f, 1000.0f, "%1.3f") ;
	
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
	ImGui::ColorEdit4(H3UtilGbk2Utf8(string("�����ɫ")).data(), startCol4);
	ImGui::ColorEdit4(H3UtilGbk2Utf8(string("�����ɫ�仯��Χ")).data(), startColVariance4);
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
	ImGui::ColorEdit4(H3UtilGbk2Utf8(string("�յ���ɫ")).data(), endCol4);
	ImGui::ColorEdit4(H3UtilGbk2Utf8(string("�յ���ɫ�仯��Χ")).data(), endColVariance4);
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
	ImGui::InputInt(H3UtilGbk2Utf8(string("�����")).data(), &textureStartWidth );
	ImGui::InputInt(H3UtilGbk2Utf8(string("����ȱ仯��Χ")).data(), &textureStartWidthVariance );
	ImGui::InputInt(H3UtilGbk2Utf8(string("���߶�")).data(), &textureStartHeight );
	ImGui::InputInt(H3UtilGbk2Utf8(string("���߶ȱ仯��Χ")).data(), &textureStartHeightVariance );
	particle->m_size.m_startSizeHorizontal = (float ) textureStartWidth ;
	particle->m_size.m_startSizeHorizontalVariance = (float ) textureStartWidthVariance ;
	particle->m_size.m_startSizeVertical = (float ) textureStartHeight ;
	particle->m_size.m_startSizeVerticalVariance = (float ) textureStartHeightVariance ;

	ImGui::Separator();
	int textureEndWidth = (int )particle->m_size.m_endSizeHorizontal ;
	int textureEndWidthVariance = (int )particle->m_size.m_endSizeHorizontalVariance ;
	int textureEndHeight = (int )particle->m_size.m_endSizeVertical ;
	int textureEndHeightVariance = (int )particle->m_size.m_endSizeVerticalVariance ;
	ImGui::InputInt(H3UtilGbk2Utf8(string("�յ���")).data(), &textureEndWidth );
	ImGui::InputInt(H3UtilGbk2Utf8(string("�յ��ȱ仯��Χ")).data(), &textureEndWidthVariance );
	ImGui::InputInt(H3UtilGbk2Utf8(string("�յ�߶�")).data(), &textureEndHeight );
	ImGui::InputInt(H3UtilGbk2Utf8(string("�յ�߶ȱ仯��Χ")).data(), &textureEndHeightVariance );
	particle->m_size.m_endSizeHorizontal = (float ) textureEndWidth ;
	particle->m_size.m_endSizeHorizontalVariance = (float ) textureEndWidthVariance ;
	particle->m_size.m_endSizeVertical = (float ) textureEndHeight ;
	particle->m_size.m_endSizeVerticalVariance = (float ) textureEndHeightVariance ;	

	ImGui::Separator();
	ImGui::LabelText("", H3UtilGbk2Utf8(string("��ת�����༭")).data());
	ImGui::DragFloat(H3UtilGbk2Utf8(string("���Ƕ�")).data(), &particle->m_rotation.m_startAngle, 0.005f, 0.0f, 360.0f, "%1.3f") ;
	ImGui::DragFloat(H3UtilGbk2Utf8(string("���Ƕȱ仯��Χ")).data(), &particle->m_rotation.m_startAngleVariance, 0.005f, 0.0f, 360.0f, "%1.3f") ;
	ImGui::DragFloat(H3UtilGbk2Utf8(string("�յ�Ƕ�")).data(), &particle->m_rotation.m_endAngle, 0.005f, 0.0f, 360.0f, "%1.3f") ;
	ImGui::DragFloat(H3UtilGbk2Utf8(string("�յ�Ƕȱ仯��Χ")).data(), &particle->m_rotation.m_endAngleVariance, 0.005f, 0.0f, 360.0f, "%1.3f") ;
	ImGui::Separator();
	ImGui::LabelText("", H3UtilGbk2Utf8(string("��ת�����༭")).data());
	ImGui::DragFloat(H3UtilGbk2Utf8(string("���ٶ�")).data(), &particle->m_revolution.m_angularSpeed, 0.005f, 0.0f, 360.0f, "%1.3f") ;
	ImGui::DragFloat(H3UtilGbk2Utf8(string("���ٶȱ仯��Χ")).data(), &particle->m_revolution.m_angularSpeedVariance, 0.005f, 0.0f, 360.0f, "%1.3f") ;
	ImGui::Separator();
	ImGui::DragFloat(H3UtilGbk2Utf8(string("���뾶")).data(), &particle->m_revolution.m_startRadius, 0.005f, 0.0f, 360.0f, "%1.3f") ;
	ImGui::DragFloat(H3UtilGbk2Utf8(string("���뾶�仯��Χ")).data(), &particle->m_revolution.m_startRadiusVariance, 0.005f, 0.0f, 360.0f, "%1.3f") ;
	ImGui::DragFloat(H3UtilGbk2Utf8(string("�յ�뾶")).data(), &particle->m_revolution.m_endRadius, 0.005f, 0.0f, 360.0f, "%1.3f") ;
	ImGui::DragFloat(H3UtilGbk2Utf8(string("�յ�뾶�仯��Χ")).data(), &particle->m_revolution.m_endRadiusVariance, 0.005f, 0.0f, 360.0f, "%1.3f") ;
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