#pragma once
#ifndef ENTRY_POINT_H
	#define ENTRY_POINT_H

int main() {
	Nuim::Application* NuimEngine = Nuim::CreateApplication();
	NuimEngine->Run();
	delete NuimEngine;
}
#endif // !ENTRY_POINT_H

