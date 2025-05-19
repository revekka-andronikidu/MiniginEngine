#include "ServiceLocator.h"

namespace dae
{
	std::unique_ptr<IAudio> ServiceLocator::m_pAudioService{ std::make_unique<NullAudio>() };


}
