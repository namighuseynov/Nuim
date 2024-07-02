#pragma once
#include "EventSystem.hpp"

namespace NuimDemo {
	class Layer
	{
	public:
		Layer(std::string name = "Layer") : name(name) {};
	public:
		virtual void OnUpdate() {};
		virtual void Begin() {};
		virtual void End() {};
		void OnEvent(EventSystem::Event& e) {
			std::cout << e.GetName() << std::endl;
		}
	private:
		std::string name;
	};
}