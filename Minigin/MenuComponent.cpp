#include "MenuComponent.h"
#include "GameObject.h"
#include "TextureComponent.h"
#include "ActionComponent.h"

namespace dae
{
	//MenuComponent::MenuComponent(GameObject* pOwner) : BaseComponent(pOwner)
	//{
	//	//init menu pointer icon
	//	//func to change menu icon

	//};
 


	void MenuComponent::ChangeMenuItem(Direction direction)
	{
		if (direction == Direction::Down)
		{
			m_currentItem = (m_currentItem + 1) % m_pMenuItems.size();
		}
		else
		{
			if (m_currentItem > 0)
			{
				m_currentItem--;
			}
			else
			{
				m_currentItem = static_cast<int>(m_pMenuItems.size()) - 1;
			}
		}

		m_dirtyFlag = true;
#if _DEBUG
		std::cout << "Menu item: "  + std::to_string(m_currentItem) << std::endl;
#endif
	};



	void MenuComponent::Update()
	{
		SetMenuArrowPosition();


	}


	void MenuComponent::SetMenuArrowPosition()
	{
		// redner / display menu icon next to menu item
		if (!m_dirtyFlag) return;

			if (m_menuArrowIcon )
			{
				auto textureComp = m_menuArrowIcon->GetComponent<dae::TextureComponent>();
				if (textureComp)
				{
					//calculate pos
					//set pos
					auto transform = m_pMenuItems[m_currentItem]->GetTransform();
					auto pos = transform.GetWorldPosition();
					auto sizeX = textureComp->GetTextureSize().x * m_menuArrowIcon->GetTransform().GetScale().y;
					//auto sizeY = textureComp->GetTextureSize().y * m_menuArrowIcon->GetTransform().GetScale().y;
					
					float padding = sizeX / 4.f;
					pos.x -= sizeX + padding;
					//pos.y += sizeY / 2;

					//std::cout << "Pos: " + std::to_string(pos.x) << " , " << std::to_string(pos.y) << std::endl;
					
					m_menuArrowIcon->GetTransform().SetPosition(pos);


					m_dirtyFlag = false;
				}
				else
				{
					//no texture found error
				}

				
			}
	}
	
	void MenuComponent::EnterMenuItem()
	{
		//load appropriate scene
		m_pMenuItems[m_currentItem]->GetComponent<ActionComponent>()->Excecute();

#if _DEBUG
		std::cout << "Entered: " << std::to_string(m_currentItem) << std::endl;
#endif

		//reset input
		//deactivate cuurent scene
		//set up new scene
	}
	

}
