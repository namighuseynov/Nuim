#pragma once
#include "Layer.hpp"

namespace NuimDemo {
	class LayerStack
	{
	public:
		LayerStack() {
			iterator = layers.begin();
		}
		~LayerStack() {
			for (auto layer : layers) {
				delete layer;
			}
		}
	public:
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* layer);
		std::vector<Layer*>::iterator Begin() { return this->layers.begin(); }
		std::vector<Layer*>::iterator End() { return this->layers.end(); }
	private:
		std::vector<Layer*> layers;
		std::vector<Layer*>::iterator iterator;
	};
}