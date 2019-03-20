#include "stdafx.hpp"
#include "Audio.hpp"

Audio::Audio() : mVolume(20.F)
{
}

Audio::~Audio()
{
}

void Audio::loadSound(const std::string& filePath)
{
	if (!this->mBuffer.loadFromFile(filePath))
		std::cerr << "Fucking sound file didn't fucking load!!!" << "\n";

	this->mSound.setBuffer(this->mBuffer);
}

void Audio::play(){ this->mSound.play(); }

void Audio::stop() { this->mSound.stop(); }

void Audio::setVolume(float volume)
{
	this->mVolume = volume;
	this->mSound.setVolume(this->mVolume);
}


