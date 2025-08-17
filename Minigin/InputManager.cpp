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

	}
	void InputManager::BindSceneInput(Scene* scene, ControllerInput input, std::unique_ptr<Command> command)
	{

		auto& context = m_SceneContexts[scene];
		context.controllerBindings[input] = std::move(command);

		
	}

	// Global bindings (available in all scenes)
	void InputManager::BindGlobalInput(KeyboardInput input, std::unique_ptr<Command> command)
	{
		m_GlobalContext.keyboardBindings[input] = std::move(command);

	}

	void InputManager::BindGlobalInput(ControllerInput input, std::unique_ptr<Command> command)
	{
		m_GlobalContext.controllerBindings[input] = std::move(command);
	
	}

	void InputManager::AddController(unsigned int controllerIndex)
	{
		for (auto& controller : m_Controllers)
		{
			if (controller->GetIndex() == controllerIndex) // Assuming XboxController has GetIndex()
			{
				// Controller already added, skip
				return;
			}
		}

		// Add new controller
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
	void InputManager::RemoveSceneInput(Scene* scene)
	{
		auto it = m_SceneContexts.find(scene);
		if (it != m_SceneContexts.end())
		{
			m_SceneContexts.erase(it);
		}
	}

}