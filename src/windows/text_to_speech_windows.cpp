#include "platform_config.h"
#if defined(WINDOWS_PLATFORM)

#include "../text_to_speech.h"

#include <iostream>
#include <Windows.h>
#include "Utils.h"
#include "lexicon_manager.h"
#include "string_utils.h"
#include "helpers/com_helper.h"
#include "exception/config_exception.h"

namespace text_to_speech
{
    std::vector<std::string> available_languages()
    {
        // TODO: Find a way to implement text_to_speech::available_languages()
        return std::vector<std::string>();
    }

    /**
    * Try to initialize a voice object to check if the properties are OK.
    * @return True if parameters are correct, false othewise.
    * @throws and Exception if a problem occurs.
    */
    bool test_parameters(const std::string& language, const std::string& gender, const long& rate)
    {
        // Init COM lib

        com_handler comHandler;
        HRESULT hr;
        ISpVoice * ispVoice = nullptr;
        hr = ::CoCreateInstance(CLSID_SpVoice, nullptr, CLSCTX_ALL, IID_ISpVoice, reinterpret_cast<void**>(&ispVoice));
        com_helper::check_result("Speech::testParameters", hr);

        try {
            init_voice(ispVoice, language, gender, rate);
            return true;
        } catch (const software_exception&) {

        }

        return false;
    }

    void init_voice(ISpVoice * ispVoice, const std::string& language, const std::string& gender, const long& rate)
    {
        const wchar_t* reqAttributs = lang_to_attribute(language);
        const wchar_t* optAttributs = gender_to_attribute(gender);

        ISpObjectToken* cpTokenEng;
        if (FAILED(::SpFindBestToken(SPCAT_VOICES, reqAttributs, optAttributs, &cpTokenEng))) {
            throw software_exception("Speech::initVoice", "Couldn't find a Token with the required attributs.");
        }

        ispVoice->SetVoice(cpTokenEng);
        //TODO: Config Rate with file
        ispVoice->SetRate(rate);
    }

    bool say(const std::string& textToSpeak, const std::string& language, const std::string& gender, const long& rate)
    {
        // Init COM lib
        com_handler comHandler;
        HRESULT hr;

        ISpVoice * ispVoice = nullptr;

        bool result = false;

        hr = ::CoCreateInstance(CLSID_SpVoice, nullptr, CLSCTX_ALL, IID_ISpVoice, reinterpret_cast<void**>(&ispVoice));
        com_helper::check_result("Speech::sayB", hr);

        if (SUCCEEDED( hr )) {

            init_voice(ispVoice, language, gender, rate);

            bstr_t bstrTextToSpeak(textToSpeak.c_str());
            hr = ispVoice->Speak(bstrTextToSpeak, SPF_ASYNC, nullptr);
            if (hr == S_OK) {
                result = true;

            } else if (hr == E_INVALIDARG) {
                Utils::get_logger()->error("Speech::SayB - One or more parameters are invalid.");

            } else if (hr == E_POINTER) {
                Utils::get_logger()->error("Speech::SayB - Invalid pointer.");

            } else if (hr == E_OUTOFMEMORY) {
                Utils::get_logger()->error("Speech::SayB - Exceeded available memory.");

            } else {
                Utils::get_logger()->error("Speech::SayB - Unknown error.");
            }

            hr = ispVoice->WaitUntilDone(INFINITE);

            ispVoice->Release();
            ispVoice = nullptr;
        }

        return result;
    }

    const wchar_t* lang_to_attribute(std::string language)
    {
        // TODO: initialize Language in a map
        // Default value : 409 = English US
        const wchar_t* defaultValue = L"Language=409";

		if (language == lexicon_manager::LANG_EN_UK) {
            return L"Language=809";
		} else if (language == lexicon_manager::LANG_FR) {
            return L"Language=40C";
        }

        return defaultValue;
    }

    // TODO: initialize Gender in a map
    const wchar_t* gender_to_attribute(std::string gender)
    {
        const wchar_t* defaultValue = L"Gender=Female";

        if (gender == "M") {
            return L"Gender=Male";
        }

        return defaultValue;
    }

}
#endif
