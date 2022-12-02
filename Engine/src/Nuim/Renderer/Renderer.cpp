#include "nmpch.h"
#include "Renderer.h"
#include "Renderer2D.h"
#include "Renderer3D.h"

namespace Nuim {
	void Renderer::Init() {
		Renderer3D::Init();
	}

	void Renderer::ShutDown() {
		Renderer3D::ShutDown();
	}

	void Renderer::OnWindowResize(unsigned int widht, unsigned int height) {

	}

	void Renderer::SceneBegin() {

	}

	void Renderer::SceneEnd() {

	}
}

