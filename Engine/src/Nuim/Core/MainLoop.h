#pragma once
#ifndef MAIN_LOOP_H
	#define MAIN_LOOP_H

int main() {
	Nuim::Application* NuimEngine = Nuim::CreateApplication();
	NuimEngine->Run();
	delete NuimEngine;
}
#endif // !ENTRY_POINT_H

