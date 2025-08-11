#pragma once
#include <BaseComponent.h>
#include <vector>


//#include <memory>

namespace dae
{
	class GameObject;
	class IngredientComponent; 

	class TrayComponent final : public BaseComponent
	{

    public:
        TrayComponent(GameObject* owner, int totalPieces = 4);
        ~TrayComponent();

        void Update() override {};
        void FixedUpdate([[maybe_unused]] const float fixedTimeStep) override {};



       void RegisterIngredient(IngredientComponent* ingredient);

    private:
        
        void CheckIfComplete();

    private:
        std::vector<IngredientComponent*> m_IngredientsOnTray;
        int m_TotalPieces{};  // How many burgers should be completed
	};
}

