#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <memory>
#include <unordered_map>

template <typename T>
class AssetManager {
public:
	template <typename AssetID = const std::string&, typename... Args>
	void loadAsset(AssetID id, Args&&...args) {
		std::unique_ptr<T> asset = std::make_unique<T>();
		if (asset->loadFromFile(std::forward<Args>(args)...)) {
			m_assets.try_emplace(id, std::move(asset));
		}
		// Should we throw an error here and force users to make sure the file is available ?
	}

	T* getAsset(const std::string& assetName) {
		if (const auto it = m_assets.find(assetName); it != m_assets.end()) {
			return it->second.get();
		}
		return nullptr;
		// Should we throw an error and force users to make sure the file is available ?
	}

	void releaseAllAssets() {
		m_assets.clear();
	}

private:
	std::unordered_map<std::string, std::unique_ptr<T>> m_assets;
};

using TextureManager = AssetManager<sf::Texture>;
using SfxManager = AssetManager<sf::SoundBuffer>;
using FontManager = AssetManager<sf::Font>;