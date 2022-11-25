#pragma once

namespace Nuim {
	class Renderer {
	public:
		static void Init();
		static void ShutDown();

		static void OnWindowResize(unsigned int widht, unsigned int height);

		static void SceneBegin();
		static void SceneEnd();
	};
}