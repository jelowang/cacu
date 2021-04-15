
#if defined(_MSC_VER)
#pragma once
#endif

#ifndef CacuParticleEditor_H
#define CacuParticleEditor_H

#include "HpParticle.h"
#include "HpEmitter.h"

namespace CacuParticleEditor {

	void showFpsOverlay() ;
	void enableParticleEditor( bool enable ) ;
	void drawParticleEditor ( HpEmitter* et , HpParticle* particle ) ;

} ;

#endif