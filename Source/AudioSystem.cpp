#include "AudioSystem.h"

#include <filesystem>

#include "SDL.h"
#include "SDL_mixer.h"

SoundHandle SoundHandle::Invalid;

// Create the AudioSystem with specified number of channels
// (Defaults to 8 channels)
AudioSystem::AudioSystem(int numChannels) {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
        SDL_Log("Failed to initialize SDL_mixer");
        return;
    }

    Mix_AllocateChannels(numChannels);
    mChannels.resize(numChannels);
    for (auto &handle : mChannels) {
        handle.Reset();
    }
}

// Destroy the AudioSystem
AudioSystem::~AudioSystem() {
    for (auto &[_, sound] : mSounds) {
        Mix_FreeChunk(sound);
    }
    Mix_CloseAudio();
}

// Updates the status of all the active sounds every frame
void AudioSystem::Update(float deltaTime) {
    for (SoundHandle &handle : mChannels) {
        if (handle.IsValid()) continue;
        mHandleMap.erase(handle);
        handle.Reset();
    }
}

// Plays the sound with the specified name and loops if looping is true
// Returns the SoundHandle which is used to perform any other actions on the
// sound when active
// NOTE: The soundName is without the "Assets/Sounds/" part of the file
//       For example, pass in "ChompLoop.wav" rather than
//       "Assets/Sounds/ChompLoop.wav".
SoundHandle AudioSystem::PlaySound(const std::string &soundName, bool looping) {
    // Get the sound with the given name
    Mix_Chunk *sound = GetSound(soundName);

    if (sound == nullptr) {
        SDL_Log("Error loading sound");
        return SoundHandle::Invalid;
    }

    int availableChannel = -1;

    for (auto &[handle, info] : mHandleMap) {
        if (info.mSoundName == soundName) {
            availableChannel = info.mChannel;
            break;
        }
    }

    if (availableChannel == -1) {
        for (int i = 0; i < mChannels.size(); i++) {
            // It's the same sound playing
            if (!mChannels[i].IsValid()) {
                availableChannel = i;
            }
        }
    }

    if (availableChannel == -1) {
        for (auto &[handle, info] : mHandleMap) {
            if (info.mIsLooping) {
                StopSound(handle);
                availableChannel = info.mChannel;
                break;
            }
        }
    }

    if (availableChannel == -1) {
        auto &[handle, info] = *mHandleMap.begin();
        StopSound(handle);
        availableChannel = info.mChannel;
    }

    mLastHandle++;
    HandleInfo info = {.mSoundName = soundName,
                       .mChannel = availableChannel,
                       .mIsLooping = looping,
                       .mIsPaused = false};

    mHandleMap[mLastHandle] = info;
    mChannels[availableChannel] = mLastHandle;
    Mix_PlayChannel(availableChannel, sound, looping ? -1 : 0);
    return mLastHandle;
}

// Stops the sound if it is currently playing
void AudioSystem::StopSound(SoundHandle sound) {
    if (!mHandleMap.count(sound)) {
        SDL_Log("No such sound handle!");
        return;
    }
    int channel = mHandleMap[sound].mChannel;
    Mix_HaltChannel(channel);
    mChannels[channel].Reset();
    mHandleMap.erase(sound);
}

// Pauses the sound if it is currently playing
void AudioSystem::PauseSound(SoundHandle sound) {
    if (!mHandleMap.count(sound)) {
        SDL_Log("No such sound handle!");
        return;
    }
    auto &info = mHandleMap[sound];
    if (info.mIsPaused) return;
    Mix_Pause(info.mChannel);
    info.mIsPaused = true;
}

// Resumes the sound if it is currently paused
void AudioSystem::ResumeSound(SoundHandle sound) {
    if (!mHandleMap.count(sound)) {
        SDL_Log("No such sound handle!");
        return;
    }
    auto &info = mHandleMap[sound];
    if (!info.mIsPaused) return;

    Mix_Resume(info.mChannel);
    info.mIsPaused = false;
}

// Returns the current state of the sound
SoundState AudioSystem::GetSoundState(SoundHandle sound) {
    if (mHandleMap.find(sound) == mHandleMap.end()) {
        return SoundState::Stopped;
    }

    if (mHandleMap[sound].mIsPaused) {
        return SoundState::Paused;
    }

    return SoundState::Playing;
}

// Stops all sounds on all channels
void AudioSystem::StopAllSounds() {
    Mix_HaltChannel(-1);

    for (auto &mChannel : mChannels) {
        mChannel.Reset();
    }

    mHandleMap.clear();
}

// Cache all sounds under Assets/Sounds
void AudioSystem::CacheAllSounds() {
#ifndef __clang_analyzer__
    std::error_code ec{};
    for (const auto &rootDirEntry :
         std::filesystem::directory_iterator{"Assets/Sounds", ec}) {
        std::string extension = rootDirEntry.path().extension().string();
        if (extension == ".ogg" || extension == ".wav") {
            std::string fileName = rootDirEntry.path().stem().string();
            fileName += extension;
            CacheSound(fileName);
        }
    }
#endif
}

// Used to preload the sound data of a sound
// NOTE: The soundName is without the "Assets/Sounds/" part of the file
//       For example, pass in "ChompLoop.wav" rather than
//       "Assets/Sounds/ChompLoop.wav".
void AudioSystem::CacheSound(const std::string &soundName) {
    GetSound(soundName);
}

// If the sound is already loaded, returns Mix_Chunk from the map.
// Otherwise, will attempt to load the file and save it in the map.
// Returns nullptr if sound is not found.
// NOTE: The soundName is without the "Assets/Sounds/" part of the file
//       For example, pass in "ChompLoop.wav" rather than
//       "Assets/Sounds/ChompLoop.wav".
Mix_Chunk *AudioSystem::GetSound(const std::string &soundName) {
    std::string fileName = "../Assets/Sounds/";
    fileName += soundName;

    Mix_Chunk *chunk = nullptr;
    auto iter = mSounds.find(fileName);
    if (iter != mSounds.end()) {
        chunk = iter->second;
    } else {
        chunk = Mix_LoadWAV(fileName.c_str());
        if (!chunk) {
            SDL_Log("[AudioSystem] Failed to load sound file %s",
                    fileName.c_str());
            return nullptr;
        }

        mSounds.emplace(fileName, chunk);
    }
    return chunk;
}

// Input for debugging purposes
void AudioSystem::ProcessInput(const Uint8 *keyState) {
    // Debugging code that outputs all active sounds on leading edge of period
    // key
    if (keyState[SDL_SCANCODE_PERIOD] && !mLastDebugKey) {
        SDL_Log("[AudioSystem] Active Sounds:");
        for (size_t i = 0; i < mChannels.size(); i++) {
            if (mChannels[i].IsValid()) {
                auto iter = mHandleMap.find(mChannels[i]);
                if (iter != mHandleMap.end()) {
                    HandleInfo &hi = iter->second;
                    SDL_Log("Channel %d: %s, %s, looping = %d, paused = %d",
                            static_cast<unsigned>(i),
                            mChannels[i].GetDebugStr(), hi.mSoundName.c_str(),
                            hi.mIsLooping, hi.mIsPaused);
                } else {
                    SDL_Log("Channel %d: %s INVALID", static_cast<unsigned>(i),
                            mChannels[i].GetDebugStr());
                }
            }
        }
    }

    mLastDebugKey = keyState[SDL_SCANCODE_PERIOD];
}
