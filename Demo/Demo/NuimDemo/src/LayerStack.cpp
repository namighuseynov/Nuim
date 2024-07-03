#include "NuimDemoPCH.h"
#include "LayerStack.hpp"

namespace NuimDemo {
	void LayerStack::PushLayer(Layer* layer)
	{
		iterator = layers.emplace(iterator, layer);
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		layers.push_back(overlay);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto item = std::find(layers.begin(), layers.end(), layer);
		layers.erase(item);
		iterator--;
	}
	void LayerStack::PopOverlay(Layer* layer)
	{
		auto item = std::find(layers.begin(), layers.end(), layer);
		layers.erase(item);
	}
}