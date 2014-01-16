#pragma once

#include "platform_config.h"
#if defined(WINDOWS_PLATFORM)

#include "../voice.h"
#include <vector>
#include <string>

#include <comdef.h>
#include <sphelper.h>

namespace text_to_speech
{
	void init_voice(ISpVoice * ispVoice, const language_code& language, const std::string& gender, const tts_age& age, const long& rate=0);

	const wchar_t* lang_as_attribute(const language_code& language);
	wchar_t* gender_as_attribute(const std::string& gender);
	wchar_t* age_as_attribute(const tts_age& age);
}

#endif
