#pragma once
#include "../../includes.hpp"
#include "../../../sdk/interfaces/i_client_mode.hpp"

namespace function {
	class thirdperson {
	public:
		void setup( ClientFrameStage_t curStage );
	};
	extern thirdperson c_thirdperson;
}