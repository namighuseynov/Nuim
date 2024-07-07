#include "NuimDemoPCH.h"
#include "Application.hpp"




//if (e.GetType() == EventSystem::EventType::MouseMoveEvent) {
//	EventSystem::MouseMoveEvent* mouseMoveEvent = dynamic_cast<EventSystem::MouseMoveEvent*>(&e);
//	std::cout << std::to_string(mouseMoveEvent->GetX()) + "," + std::to_string(mouseMoveEvent->GetY()) + " Move" << std::endl;
//}
//else if (e.GetType() == EventSystem::EventType::MousePressEvent) {
//	EventSystem::MousePressEvent* mousePressEvent = dynamic_cast<EventSystem::MousePressEvent*>(&e);
//	std::cout << std::to_string(mousePressEvent->GetX()) + "," + std::to_string(mousePressEvent->GetY()) + " Pressed" << std::endl;
//}
//else if (e.GetType() == EventSystem::EventType::WindowSizeEvent) {
//	EventSystem::WindowSizeEvent* windowSizeEvent = dynamic_cast<EventSystem::WindowSizeEvent*>(&e);
//	std::cout << std::to_string(windowSizeEvent->GetHeight()) + ',' + std::to_string(windowSizeEvent->GetWidth()) + " Resize" << std::endl;
//}
//else if (e.GetType() == EventSystem::EventType::KeyPressEvent) {
//	EventSystem::KeyPressEvent* keyPressEvent = dynamic_cast<EventSystem::KeyPressEvent*>(&e);
//	char a;
//	a = keyPressEvent->GetKeyCode();
//	std::string msg = "Key" + std::to_string(a);
//	std::cout << "Key Pressed: ";
//	std::cout << a << std::endl;
//}
//else {
//	std::cout << e.GetName() << std::endl;
//}
//for (auto item : layers) {
//	item->OnEvent(e);
//}