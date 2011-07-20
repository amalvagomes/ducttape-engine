
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_COMPONENT_SOUNDCOMPONENT
#define DUCTTAPE_ENGINE_COMPONENT_SOUNDCOMPONENT

#include <SFML/Audio.hpp>

#include "component/Component.hpp"
#include "event/SoundsControlEvent.hpp"
#include "Root.hpp"

namespace dt {

/**
  * Component for managing sounds in 3D enviroment.
  * @todo Positions should be calculated relatively to position of player, but there is no method to get those now.
  */
class SoundComponent : public Component {
public:
    /**
     * Advanced constructor.
     * @see Component
     */
    SoundComponent(const std::string& name = "", const std::string& sound_file = "");

    virtual void HandleEvent(std::shared_ptr<Event> e);

    void OnCreate();
    void OnDestroy();
    void OnUpdate(double time_diff);

	/**
     * Plays the sound located in mSound.
     * @param sound_file The name of the sound file to play. To get sound in 3D file should be mono.
     */
    void SetSoundFile(const std::string& sound_file);

	/**
     * Returns the name of the sound file.
     * @returns The name of the found file.
     */
    const std::string& GetSoundFile() const;

	/**
     * Returns the sf::Sound object for default action access.
     * @returns The sf::Sound object.
     */
	sf::Sound& GetSound();

    /**
     * Tries to play / continue sound located in mSound.
     */
    void PlaySound();

    /**
     * Pauses the sound if playing.
     */
    void PauseSound();

    /**
     * Stops the sound if playing.
     */
    void StopSound();

    /**
      * Sets the global master sound/music volume.
      * @param volume The global master sound/music volumn. Minimum: 0. Maximum: 100. Default: 100.
      */
    static void SetMasterVolume(float volume);

private:
	/**
     * Private method. Tries to load sound to buffer and then from buffer to mSound.
     */
    void _LoadSound();

    std::string mSoundFile; //!< Name of sound file loaded in component.
	sf::Sound mSound; //!< SFML Sound. It is set during _LoadSound().

};

}

#endif
