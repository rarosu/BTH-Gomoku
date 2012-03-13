#include "InputManager.hpp"
#include <cstring>
#include <algorithm>

InputState::InputState()
{
	memset(Keyboard.keyIsPressed, false, sizeof(Keyboard.keyIsPressed));
	memset(Mouse.buttonIsPressed, false, sizeof(Mouse.buttonIsPressed));
	Mouse.x = 0;
	Mouse.y = 0;
}

InputManager::InputManager()
{
}

void InputManager::HandleKeyPress(WPARAM wParam, LPARAM lParam)
{
	mCurrentState.Keyboard.keyIsPressed[(unsigned char)wParam] = true;

	std::vector<KeyListener*>::iterator it;
	for(it = mKeyListeners.begin(); it != mKeyListeners.end(); it++)
		(*it)->KeyPressed((int)wParam, mCurrentState);
}

void InputManager::HandleKeyRelease(WPARAM wParam, LPARAM lParam)
{
	mCurrentState.Keyboard.keyIsPressed[(unsigned char)wParam] = false;

	std::vector<KeyListener*>::iterator it;
	for(it = mKeyListeners.begin(); it != mKeyListeners.end(); it++)
		(*it)->KeyReleased((int)wParam, mCurrentState);
}

void InputManager::HandleMousePress(WPARAM wParam, LPARAM lParam)
{
	int buttonIndex = -1;
	switch(wParam)
	{
		case MK_LBUTTON:
			mCurrentState.Mouse.buttonIsPressed[C_MOUSE_LEFT] = true;
			buttonIndex = C_MOUSE_LEFT;
			break;
		case MK_RBUTTON:
			mCurrentState.Mouse.buttonIsPressed[C_MOUSE_RIGHT] = true;
			buttonIndex = C_MOUSE_RIGHT;
			break;
		case MK_MBUTTON:
			mCurrentState.Mouse.buttonIsPressed[C_MOUSE_MIDDLE] = true;
			buttonIndex = C_MOUSE_MIDDLE;
			break;
	}

	std::vector<MouseListener*>::iterator it;
	for(it = mMouseListeners.begin(); it != mMouseListeners.end(); it++)
		(*it)->MouseButtonPressed(buttonIndex, mCurrentState);
}

void InputManager::HandleMouseRelease(int buttonIndex, WPARAM wParam, LPARAM lParam)
{
	mCurrentState.Mouse.buttonIsPressed[buttonIndex] = false;

	std::vector<MouseListener*>::iterator it;
	for(it = mMouseListeners.begin(); it != mMouseListeners.end(); it++)
		(*it)->MouseButtonReleased(buttonIndex, mCurrentState);
}

void InputManager::HandleMouseMove(WPARAM wParam, LPARAM lParam)
{
	mCurrentState.Mouse.x = LOWORD(lParam);
	mCurrentState.Mouse.y = HIWORD(lParam);
}

void InputManager::HandleMouseWheel(WPARAM wParam, LPARAM lParam)
{
	short delta = HIWORD(wParam);
	delta /= 120;

	std::vector<MouseListener*>::iterator it;
	for(it = mMouseListeners.begin(); it != mMouseListeners.end(); it++)
		(*it)->MouseWheelMoved(delta, mCurrentState);
}

void InputManager::HandleCharPress(WPARAM wParam, LPARAM lParam)
{
	unsigned char key = (unsigned char)wParam;

	if(key < 0x20 || (key > 0x7E && key < 0xC0))
		return;

	std::vector<KeyListener*>::iterator it;
	for(it = mKeyListeners.begin(); it != mKeyListeners.end(); it++)
		(*it)->CharEntered((unsigned char)wParam, mCurrentState);
}

const InputState& InputManager::GetPrevious() const
{
	return mPreviousState;
}

const InputState& InputManager::GetCurrent() const
{
	return mCurrentState;
}

void InputManager::Update()
{
	mPreviousState = mCurrentState;
}

// Methods inherited from InputSubscription

void InputManager::AddKeyListener(KeyListener* listener)
{
	mKeyListeners.push_back(listener);
}

void InputManager::RemoveKeyListener(KeyListener* listener)
{
	std::vector<KeyListener*>::iterator index = std::find(mKeyListeners.begin(), mKeyListeners.end(), listener);

	if(index != mKeyListeners.end())
		mKeyListeners.erase(index);
}

void InputManager::AddMouseListener(MouseListener* listener)
{
	mMouseListeners.push_back(listener);
}

void InputManager::RemoveMouseListener(MouseListener* listener)
{
	std::vector<MouseListener*>::iterator index = std::find(mMouseListeners.begin(), mMouseListeners.end(), listener);
	
	if(index != mMouseListeners.end())
		mMouseListeners.erase(index);
}

bool InputManager::IsListeningForKey(KeyListener* listener)
{
	return std::find(mKeyListeners.begin(), mKeyListeners.end(), listener) != mKeyListeners.end();
}

bool InputManager::IsListeningForMouse(MouseListener* listener)
{
	return std::find(mMouseListeners.begin(), mMouseListeners.end(), listener) != mMouseListeners.end();
}