#include "NuimDemoPCH.h"
#include "LayerStack.hpp"

namespace NuimDemo {
	void LayerStack::PushLayer(Layer* layer)
	{
		this->layers.push_back(layer);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto item = std::find(layers.begin(), layers.end(), layer);
	}

}