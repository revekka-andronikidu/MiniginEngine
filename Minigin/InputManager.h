#pragma once
#include "Singleton.h"
#include "XboxController.h"
#include <unordered_map>
#include "Command.h"
#include <string>
#include <variant>
//#include "Gui.h"
#include <memory>
#include "Scene.h"


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

	// Add hash support for KeyboardInput
	struct KeyboardInputHash 
	{
		size_t operator()(const KeyboardInput& k) const {
			return std::hash<SDL_Scancode>()(k.key) ^
				(std::hash<int>()(static_cast<int>(k.state)) << 1);
		}
	};

	// Add hash support for ControllerInput
	struct ControllerInputHash 
	{
		size_t operator()(const ControllerInput& c) const {
			return std::hash<unsigned int>()(c.controllerIndex) ^
				std::hash<int>()(static_cast<int>(c.state)) ^
				std::hash<int>()(static_cast<int>(c.button)) << 1;
		}
	};

	struct InputContext
	{
		std::unordered_map<KeyboardInput, std::unique_ptr<Command>, KeyboardInputHash> keyboardBindings;
		std::unordered_map<ControllerInput, std::unique_ptr<Command>, ControllerInputHash> controllerBindings;
		//std::unordered_map<unsigned int, KeyboardInput> keyboardBindings;
		//std::unordered_map<unsigned int, ControllerInput> controllerBindings;
	};

	inline bool operator==(const dae::KeyboardInput& lhs, const dae::KeyboardInput& rhs)
	{
		return lhs.key == rhs.key && lhs.state == rhs.state;
	}

	inline bool operator==(const dae::ControllerInput& lhs, const dae::ControllerInput& rhs)
	{
		return lhs.button == rhs.button &&
			lhs.state == rhs.state &&
			lhs.controllerIndex == rhs.controllerIndex;
	}




	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();

		void BindGlobalInput(KeyboardInput input, std::unique_ptr<Command> command);
		void BindGlobalInput(ControllerInput input, std::unique_ptr<Command> command);
		void BindSceneInput( Scene* scene, KeyboardInput input, std::unique_ptr<Command> command);
		void BindSceneInput( Scene* scene, ControllerInput input, std::unique_ptr<Command> command);


		void ProcessContext(InputContext& context) ;

		bool IsKeyboardTriggered(const KeyboardInput& input) const;
		bool IsControllerTriggered(const ControllerInput& input) const;

		void AddController(unsigned int playerIndex);

		InputContext m_GlobalContext;                 // Always active
		std::unordered_map<Scene*, InputContext> m_SceneContexts; // scene context

		//std::unordered_map<unsigned int, std::unique_ptr<Command>> m_Commands; //actionID, command
		std::vector<std::unique_ptr<XboxController>> m_Controllers;

		Uint8 m_PreviousKeyboardState[SDL_NUM_SCANCODES]{ };


		//std::unordered_map<unsigned int, ControllerInput> m_ControllerInputs; //actionID, input
		//std::unordered_map<unsigned int, KeyboardInput> m_KeyboardInputs;
		/*	void BindInput(ControllerInput input, std::unique_ptr<Command> command);
		void BindInput(KeyboardInput input, std::unique_ptr<Command> command);*/
		//bool HandleControllerInput();
		//bool HandleKeyboardInput();
	};

}

