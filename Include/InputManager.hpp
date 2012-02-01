#ifndef INPUTMANAGER_HPP
#define INPUTMANAGER_HPP

#include <windows.h>
#include <vector>

const int			C_MOUSE_LEFT		= 0;
const int			C_MOUSE_RIGHT		= 1;
const int			C_MOUSE_MIDDLE		= 2;

struct InputState
{
public:
	struct MouseState
	{
		int			x;
		int			y;
		bool		buttonIsPressed[3];
	} Mouse;

	struct KeyboardState
	{
		bool		keyIsPressed[256];
	} Keyboard;

	InputState();

private:
};

class MouseListener
{
public:
	virtual void MouseButtonPressed(int index) = 0;
	virtual void MouseButtonReleased(int index) = 0;
};

class KeyListener
{
public:
	virtual void KeyPressed(int code) = 0;
	virtual void KeyReleased(int code) = 0;
	virtual void CharEntered(unsigned char symbol) = 0;
};

class InputSubscription
{
public:
	virtual void AddKeyListener(KeyListener* listener) = 0;
	virtual void RemoveKeyListener(KeyListener* listener) = 0;
	virtual void AddMouseListener(MouseListener* listener) = 0;
	virtual void RemoveMouseListener(MouseListener* listener) = 0;
	virtual bool IsListeningForKey(KeyListener* listener) = 0;
	virtual bool IsListeningForMouse(MouseListener* listener) = 0;
};

class InputManager : public InputSubscription
{
public:
	InputManager();
	void HandleKeyPress(WPARAM wParam, LPARAM lParam);
	void HandleKeyRelease(WPARAM wParam, LPARAM lParam);
	void HandleMousePress(WPARAM wParam, LPARAM lParam);
	void HandleMouseRelease(WPARAM wParam, LPARAM lParam);
	void HandleMouseMove(WPARAM wParam, LPARAM lParam);
	void HandleCharPress(WPARAM wParam, LPARAM lParam);

	const InputState& GetPrevious();
	const InputState& GetCurrent();
	void Update();

	// Inherited methods from InputSubscription
	void AddKeyListener(KeyListener* listener);
	void RemoveKeyListener(KeyListener* listener);
	void AddMouseListener(MouseListener* listener);
	void RemoveMouseListener(MouseListener* listener);
	bool IsListeningForKey(KeyListener* listener);
	bool IsListeningForMouse(MouseListener* listener);

private:
	std::vector<KeyListener*>			mKeyListeners;
	std::vector<MouseListener*>			mMouseListeners;
	InputState							mPreviousState;
	InputState							mCurrentState;

	template <typename T>
	typename std::vector<T>::iterator FindListener(std::vector<T>& list, T listener);
};

template <typename T>
typename std::vector<T>::iterator InputManager::FindListener(std::vector<T>& list, T listener)
{
	std::vector<T>::iterator returnIterator;

	for(returnIterator = list.begin(); returnIterator != list.end(); returnIterator++)
	{
		if(*returnIterator == listener)
			break;
	}

	return returnIterator;
}

#endif