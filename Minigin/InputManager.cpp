#include <SDL.h>
#include "InputManager.h"
#include "SceneManager.h"
#include "Scene.h"

namespace dae
{

	bool InputManager::ProcessInput()
	{
		for (auto& controller : m_Controllers)
		{
			controller->Update();
		}

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
		//process global context
		ProcessContext(m_GlobalContext);

		// Process current scene context
		auto& activeScene = SceneManager::GetInstance().GetActiveScene();
		if (m_SceneContexts.find(&activeScene) != m_SceneContexts.end()) {
			ProcessContext(m_SceneContexts[&activeScene]);
		}


		// Update keyboard state history ONCE per frame
		const Uint8* currentState = SDL_GetKeyboardState(nullptr);
		std::memcpy(m_PreviousKeyboardState, currentState, SDL_NUM_SCANCODES);


		//HandleControllerInput();
		//HandleKeyboardInput();
	
		return true;
	}

	void InputManager::BindSceneInput(Scene* scene, KeyboardInput input, std::unique_ptr<Command> command)
	{
		auto& context = m_SceneContexts[scene];
		context.keyboardBindings[input] = std::move(command);

		/*const unsigned int actionID = static_cast<unsigned int>(m_Commands.size());

		auto& context = m_SceneContexts[scene];
		if (context.keyboardBindings.find(actionID) != context.keyboardBindings.end())
			return;

		context.keyboardBindings[actionID] = input;
		m_Commands[actionID] = std::move(command);*/

	}
	void InputManager::BindSceneInput(Scene* scene, ControllerInput input, std::unique_ptr<Command> command)
	{

		auto& context = m_SceneContexts[scene];
		context.controllerBindings[input] = std::move(command);

		//const unsigned int actionID = static_cast<unsigned int>(m_Commands.size());
		//
		//auto& context = m_SceneContexts[scene];
		//if (context.controllerBindings.find(actionID) != context.controllerBindings.end())
		//	return;
		//
		//context.controllerBindings[actionID] = input;
		//m_Commands[actionID] = std::move(command);
	}

	// Global bindings (available in all scenes)
	void InputManager::BindGlobalInput(KeyboardInput input, std::unique_ptr<Command> command)
	{
		m_GlobalContext.keyboardBindings[input] = std::move(command);

		//const unsigned int actionID = static_cast<unsigned int>(m_Commands.size());
		//
		//const auto it = m_GlobalContext.keyboardBindings.find(actionID);
		//if (it != m_GlobalContext.keyboardBindings.end())
		//	return; //action already exists
		//
		//m_GlobalContext.keyboardBindings[actionID] = input;
		//m_Commands[actionID] = std::move(command);
	}

	void InputManager::BindGlobalInput(ControllerInput input, std::unique_ptr<Command> command)
	{
		m_GlobalContext.controllerBindings[input] = std::move(command);
		/*const unsigned int actionID = static_cast<unsigned int>(m_Commands.size());

		const auto it = m_GlobalContext.controllerBindings.find(actionID);
		if (it != m_GlobalContext.controllerBindings.end())
			return;

		m_GlobalContext.controllerBindings[actionID] = input;
		m_Commands[actionID] = std::move(command);*/
	}

	void InputManager::AddController(unsigned int controllerIndex)
	{
		m_Controllers.push_back(std::make_unique<XboxController>(controllerIndex));
	}

	void InputManager::ProcessContext(InputContext& context)
	{
		// Process keyboard bindings - direct command access
		for (auto& [input, command] : context.keyboardBindings)
		{
			if (IsKeyboardTriggered(input))
			{
				command->Execute();
			}
		}

		// Process controller bindings - direct command access
		for (auto& [input, command] : context.controllerBindings)
		{
			if (IsControllerTriggered(input))
			{
				command->Execute();
			}
		}

		//// Process keyboard bindings
		//for (auto& [actionID, input] : context.keyboardBindings)
		//{
		//	if (IsKeyboardTriggered(input)) 
		//	{
		//		auto command = m_Commands.find(actionID);
		//		if (command == m_Commands.end())
		//		{
		//			assert(false && "Command not found for input action ID");
		//		}
		//
		//		command->second->Execute();
		//	}
		//}
		//
		//// Process controller bindings
		//for (auto& [actionID, input] : context.controllerBindings)
		//{
		//	if (IsControllerTriggered(input)) 
		//	{
		//		auto command = m_Commands.find(actionID);
		//		if (command == m_Commands.end())
		//		{
		//			assert(false && "Command not found for input action ID");
		//		}
		//
		//		command->second->Execute();
		//	}
		//}
		//
		//// Update keyboard state history
		//const Uint8* currentState = SDL_GetKeyboardState(nullptr);
		//std::memcpy(m_PreviousKeyboardState, currentState, SDL_NUM_SCANCODES);

	}

	bool InputManager::IsControllerTriggered(const ControllerInput& input) const
	{
		
			const unsigned index = input.controllerIndex;
			const ButtonState buttonState = input.state;
			const XboxController::ControllerButton button = input.button;

			switch (buttonState)
			{
			case ButtonState::KeyUp:
				return m_Controllers[index]->IsUp(button);
				break;
			case ButtonState::KeyDown:
				return m_Controllers[index]->IsDown(button);
				break;
			case ButtonState::KeyPressed:
				return m_Controllers[index]->IsPressed(button);
				break;
			default:
				return false;
			}
	}

	bool InputManager::IsKeyboardTriggered(const KeyboardInput& input) const
	{
		const Uint8* state = SDL_GetKeyboardState(nullptr);
		const bool isPressed = state[input.key];
		const bool wasPressed = m_PreviousKeyboardState[input.key];

		switch (input.state) 
		{
		case ButtonState::KeyDown: 
			return isPressed && !wasPressed;
			break;
		case ButtonState::KeyUp: 
			return !isPressed && wasPressed;
			break;
		case ButtonState::KeyPressed: 
			return isPressed;
			break;
		default: 
			return false;
			break;
		}
	}

	/*bool InputManager::HandleControllerInput()
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

	}*/

	//bool InputManager::HandleKeyboardInput()
	//{
	//	const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
	//
	//	for (auto& input : m_KeyboardInputs)
	//	{
	//		const auto scancode = input.second.key;
	//		bool keyPressed = keyboardState[scancode];
	//		bool keyPreviouslyPressed = m_PreviousKeyboardState[scancode];
	//
	//		const ButtonState buttonState = input.second.state;
	//		auto it = m_Commands.find(input.first);
	//		if (it == m_Commands.end())
	//		{
	//			assert(false && "Command not found for input action ID");
	//		}
	//		auto command = it->second.get();
	//
	//		switch (buttonState)
	//		{
	//		case ButtonState::KeyUp:
	//			if (!keyPressed && keyPreviouslyPressed)
	//			{
	//				command->Execute();
	//			}
	//			break;
	//		case ButtonState::KeyDown:
	//			if (keyPressed && !keyPreviouslyPressed)
	//			{
	//				command->Execute();
	//			}
	//			/*if (e.key.type == SDL_KEYDOWN)
	//			{
	//				if (e.key.repeat == 0)
	//				{
	//					if (scancode == e.key.keysym.scancode)
	//						command->Execute();
	//				}
	//			}*/
	//			break;
	//		case ButtonState::KeyPressed:
	//			if (keyPressed)
	//			{
	//				command->Execute();
	//			}
	//			/*if (e.key.type == SDL_KEYDOWN)
	//			{
	//				if (scancode == e.key.keysym.scancode)
	//					command->Execute();
	//			}*/
	//			/*if (e.key.keysym.sym == SDLK_ESCAPE)
	//			{
	//				SDL_Quit();
	//				return false;
	//			}
	//			if (e.type == SDL_QUIT) {
	//				return false;
	//			}*/
	//			break;
	//		}
	//	}
	//	// Update previous keyboard states
	//	std::memcpy(m_PreviousKeyboardState, keyboardState, SDL_NUM_SCANCODES);
	//	return true;
	//}

}