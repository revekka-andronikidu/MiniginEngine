#include <SDL.h>
#include "InputManager.h"

namespace dae
{

	bool InputManager::ProcessInput()
	{
		{
			HandleControllerInput();
			HandleKeyboardInput();


			SDL_Event e;
			while (SDL_PollEvent(&e))
			{
				if (e.key.keysym.sym == SDLK_ESCAPE)
				{
					SDL_Quit();
					return false;
				}
				if (e.type == SDL_QUIT) {
					return false;
				}
			}


			return true;
		}
	}

	void InputManager::BindInput(ControllerInput input, std::unique_ptr<Command> command)
	{

		const unsigned int actionID = static_cast<unsigned int>(m_Commands.size());

		const auto it = m_ControllerInputs.find(actionID);
		if (it != m_ControllerInputs.end())
			return;

		m_ControllerInputs[actionID] = input;
		m_Commands[actionID] = std::move(command);
	}

	void InputManager::BindInput(KeyboardInput input, std::unique_ptr<Command> command)
	{
		const unsigned int actionID = static_cast<unsigned int>(m_Commands.size());
		//auto actionID = input.inputID;

		const auto it = m_KeyboardInputs.find(actionID);
		if (it != m_KeyboardInputs.end())
			return; //action already exists

		m_KeyboardInputs[actionID] = input;
		m_Commands[actionID] = std::move(command);
	}

	void InputManager::AddController(unsigned int controllerIndex)
	{
		m_Controllers.push_back(std::make_unique<XboxController>(controllerIndex));
	}

	bool InputManager::HandleControllerInput()
	{
		for (auto& controller : m_Controllers)
		{
			controller->Update();
		}

		for (auto& input : m_ControllerInputs)
		{
			const unsigned index = input.second.controllerIndex;

			const ButtonState buttonState = input.second.state;
			const XboxController::ControllerButton button = input.second.button;


			auto command = m_Commands.find(input.first);
			if (command == m_Commands.end())
			{
				assert(false && "Command not found for input action ID");
			}

			switch (buttonState)
			{
			case ButtonState::KeyUp:
				if (m_Controllers[index]->IsUp(button))
					command->second->Execute();
				break;
			case ButtonState::KeyDown:
				if (m_Controllers[index]->IsDown(button))
					command->second->Execute();
				break;
			case ButtonState::KeyPressed:
				if (m_Controllers[index]->IsPressed(button))
					command->second->Execute();
				break;
			}
		}
		return true;

	}

	bool InputManager::HandleKeyboardInput()
	{
		const Uint8* keyboardState = SDL_GetKeyboardState(NULL);

		for (auto& input : m_KeyboardInputs)
		{
			const auto scancode = input.second.key;
			bool keyPressed = keyboardState[scancode];
			bool keyPreviouslyPressed = m_PreviousKeyboardState[scancode];

			const ButtonState buttonState = input.second.state;
			auto it = m_Commands.find(input.first);
			if (it == m_Commands.end())
			{
				assert(false && "Command not found for input action ID");
			}
			auto command = it->second.get();

			switch (buttonState)
			{
			case ButtonState::KeyUp:
				if (!keyPressed && keyPreviouslyPressed)
				{
					command->Execute();
				}
				break;
			case ButtonState::KeyDown:
				if (keyPressed && !keyPreviouslyPressed)
				{
					command->Execute();
				}
				/*if (e.key.type == SDL_KEYDOWN)
				{
					if (e.key.repeat == 0)
					{
						if (scancode == e.key.keysym.scancode)
							command->Execute();
					}
				}*/
				break;
			case ButtonState::KeyPressed:
				if (keyPressed)
				{
					command->Execute();
				}
				/*if (e.key.type == SDL_KEYDOWN)
				{
					if (scancode == e.key.keysym.scancode)
						command->Execute();
				}*/
				/*if (e.key.keysym.sym == SDLK_ESCAPE)
				{
					SDL_Quit();
					return false;
				}
				if (e.type == SDL_QUIT) {
					return false;
				}*/
				break;
			}
		}

		// Update previous keyboard states
		std::memcpy(m_PreviousKeyboardState, keyboardState, SDL_NUM_SCANCODES);
		return true;

	}

}