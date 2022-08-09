#pragma once
#include <memory>
#include <string>
#include "Function/ITick.h"

namespace function {

class Mouse;
class Keyboard;
class Window;
class GameTimer;

class InputSystem : public ITick {
public:
	InputSystem(const std::string &title, int width, int height);
	InputSystem(const InputSystem &) = delete;
	InputSystem &operator=(const InputSystem &) = delete;
	InputSystem() = default;
	~InputSystem();
	bool shouldClose() const;
	virtual void beginTick(std::shared_ptr<GameTimer> pGameTimer) override;
	virtual void tick(std::shared_ptr<GameTimer> pGameTimer) override;
	virtual void endTick(std::shared_ptr<GameTimer> pGameTimer) override;
public:
	std::unique_ptr<Mouse>		pMouse;
	std::unique_ptr<Keyboard>	pKeyboard;
	std::unique_ptr<Window>		pWindow;
};

}