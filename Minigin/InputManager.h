#pragma once
#include "Singleton.h"
#include "XboxController.h"
#include <unordered_map>
#include "Command.h"
#include <string>
#include <variant>
//#include "Gui.h"
#include <memory>


namespace dae
{
	enum class ButtonState
	{
		KeyUp,
		KeyDown,
		KeyPressed
	};

	struct KeyboardInput
	{
		ButtonState state{};
		SDL_Scancode key{};
		//unsigned int playerIndex{};
	};

	struct ControllerInput
	{
		ButtonState state{};
		XboxController::ControllerButton button{};
		unsigned int controllerIndex{};
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();

		void BindInput(ControllerInput input, std::unique_ptr<Command> command);
		void BindInput(KeyboardInput input, std::unique_ptr<Command> command);

		void AddController(unsigned int playerIndex);

		std::unordered_map<unsigned int, std::unique_ptr<Command>> m_Commands; //actionID, command
		std::unordered_map<unsigned int, ControllerInput> m_ControllerInputs; //actionID, input
		std::unordered_map<unsigned int, KeyboardInput> m_KeyboardInputs;

		std::vector<std::unique_ptr<XboxController>> m_Controllers;

		Uint8 m_PreviousKeyboardState[SDL_NUM_SCANCODES]{ };

		bool HandleControllerInput();
		bool HandleKeyboardInput();
	};

}
