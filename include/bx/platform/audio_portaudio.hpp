#pragma once

#include <bx/platform/audio.hpp>

class BX_PLATFORM_API AudioPortAudio final : public Audio
{
	RTTR_ENABLE(Audio)

public:
	AudioPortAudio() = default;

public:
	bool Initialize() override;
	void Reload() override;
	void Shutdown() override;

	AudioHandle GetDefaultChannel() override;

	void CreateChannel(const ChannelInfo& info) override;
	void DestroyChannel(const AudioHandle channel) override;
	void SetChannelVolume(const AudioHandle channel, f32 volume) override;

	void CreateAudio(const AudioInfo& info) override;
	void DestroyAudio(const AudioHandle audio) override;
	void PlayAudio(const AudioHandle channel, const AudioHandle audio) override;
	void StopAudio(const AudioHandle channel, const AudioHandle audio) override;
};