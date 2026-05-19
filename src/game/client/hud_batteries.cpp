#include "cbase.h"
#include "hud.h"
#include "hudelement.h"
#include "hud_macros.h"
#include "hud_numericdisplay.h"
#include "iclientmode.h"
#include "c_basehlplayer.h"
#include <vgui_controls/AnimationController.h>
#include <vgui/ILocalize.h>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;

class CHudBatteries : public CHudNumericDisplay, public CHudElement
{
    DECLARE_CLASS_SIMPLE( CHudBatteries, CHudNumericDisplay );

public:
    CHudBatteries( const char *pElementName );
    void Init( void );
    void Reset( void );
    void VidInit( void );
    void OnThink( void );
    bool ShouldDraw();

private:
    int m_iBatCount;
    int m_iNewBatCount;
};

DECLARE_HUDELEMENT( CHudBatteries );

CHudBatteries::CHudBatteries( const char *pElementName ) : BaseClass(g_pClientMode->GetViewport(), "HudBatteries"), CHudElement( pElementName )
{
    SetHiddenBits( HIDEHUD_PLAYERDEAD );
    DevMsg("CHudBatteries constructed\n");
}

void CHudBatteries::Init( void )
{
    m_iBatCount = -1;
    m_iNewBatCount = 0;

    SetDisplayValue(0);

    wchar_t *tempString = g_pVGuiLocalize->Find("#Valve_Hud_BATTERIES");
    SetLabelText(tempString ? tempString : L"BATTERIES");
}

void CHudBatteries::Reset( void )
{
    m_iBatCount = 0;
    SetDisplayValue(0);
}

void CHudBatteries::VidInit( void )
{
    Reset();
}

bool CHudBatteries::ShouldDraw( void )
{
    return CHudElement::ShouldDraw();
}

void CHudBatteries::OnThink( void )
{
    BaseClass::OnThink();

    C_BaseHLPlayer *pPlayer = (C_BaseHLPlayer *)C_BasePlayer::GetLocalPlayer();
    if ( !pPlayer )
    {
        m_iNewBatCount = 0;
    }
    else
    {
        m_iNewBatCount = pPlayer->m_HL2Local.m_iBatteries;
        if ( m_iNewBatCount < 0 )
            m_iNewBatCount = 0;
    }

    if ( m_iBatCount == m_iNewBatCount )
        return;

    // Play small animation/sequence on changes if desired (hooks into HudLayout animations)
    if ( m_iBatCount == -1 || m_iBatCount == 0 || m_iNewBatCount >= 1 )
    {
        g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("BatteriesChanged");
    }

    m_iBatCount = m_iNewBatCount;
    SetDisplayValue( m_iBatCount );
    Msg("Batteries HUD: %d\n", m_iBatCount);
    InvalidateLayout(true);
	Repaint();
}