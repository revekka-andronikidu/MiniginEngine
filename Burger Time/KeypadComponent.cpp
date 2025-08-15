#include "KeypadComponent.h"
#include <TextureComponent.h>
#include <TextComponent.h>
#include <TimeManager.h>
#include "BurgerTimeGame.h"
#include "HighScoresManager.h"

using namespace dae;
KeypadComponent::KeypadComponent(GameObject* pOwner)
	: BaseComponent(pOwner)
	{
		BuildKeyMap();
	}


void KeypadComponent::BuildKeyMap()
{
    // Each row of characters
    m_KeyGrid = 
    {
        {'A','B','C','D','E','F','G','H','I','J'},
        {'K','L','M','N','O','P','Q','R','S','T'},
        {'U','V','W','X','Y','Z','-',',','?','!'},
        {' ', '<', '\n'} // Space, Rub, End
    };
}

void KeypadComponent::Update()
{
    //if (m_nameEntered && !m_isDirty)
    //    return;


   /* if (m_name.size() < m_charLimit && !m_nameEntered)
    {*/
        if (m_nameEntered)
        {
            // Remove underscore instantly
            if (m_showCursor) {
                m_showCursor = false;
                m_isDirty = true;
            }

            // Force redraw once without underscore
            if (m_isDirty && m_pTextComponent) {
                m_pTextComponent->SetText(m_name);
                m_isDirty = false;
            }

            // Wait for the delay before transitioning
            m_transitionTimer += TimeManager::GetInstance().GetDeltaTime();
            if (m_transitionTimer >= 1.f) 
            {
                auto game = GameManager::GetInstance().GetActiveGame();
                auto burgerTime = dynamic_cast<BurgerTimeGame*>(game);
                burgerTime->m_GameModeMachine.EnterState<HighScoresState>();
            }
            return;
        }

        // Original blinking logic here...
        if (m_name.size() < m_charLimit)
        {
            m_blinkTimer += TimeManager::GetInstance().GetDeltaTime();
            if (m_blinkTimer >= m_blinkInterval) {
                m_showCursor = !m_showCursor;
                m_blinkTimer = 0.f;
                m_isDirty = true;
            }
        }
        else
        {
            if (m_showCursor) {
                m_showCursor = false;
                m_isDirty = true;
            }
        }

        if (m_isDirty && m_pTextComponent)
        {
            std::string displayName = m_name;
            if (m_name.size() < m_charLimit && m_showCursor) {
                displayName += "_";
            }
            m_pTextComponent->SetText(displayName);
            m_isDirty = false;
        }
    //}
}





glm::vec3 KeypadComponent::GetCursorWorldPos() const
{
    // Convert cursor grid coords to world pos
    int cellSize = GameSettings::cellSize/2 * GameSettings::scale.x;
    int startX = 4 * cellSize;
    int startY = 4 * cellSize;

    int x, y;

    if(m_CursorY < static_cast<int>(m_KeyGrid.size()) - 1)
    {
        // Normal letter rows
        x = startX + m_CursorX * (cellSize * 2); // 1-cell gap
		y = startY + m_CursorY * (cellSize * 3); // 2-cell gap
    }
    else
    {
        // Last row (Space, Rub, End)
        int buttonWidthCells = 3;
        int gapCells = 1;

        // Middle cell offset inside the button
        int middleOffset = buttonWidthCells / 2;

        // Calculate total width before this button
        int totalCellsBefore = m_CursorX * (buttonWidthCells + gapCells);

        x = startX + (totalCellsBefore + middleOffset) * cellSize;
        y = startY + m_CursorY * (cellSize * 3);
    }
    return glm::vec3(x, y, 0.0f);
}

void KeypadComponent::HandleMove(Direction dir)
{
    switch (dir)
    {
    case Direction::Up:
        m_CursorY = (m_CursorY - 1 + m_KeyGrid.size()) % m_KeyGrid.size();
        if (m_CursorX >= m_KeyGrid[m_CursorY].size())
            m_CursorX = m_KeyGrid[m_CursorY].size() - 1; // clamp
        break;

    case Direction::Down:
        m_CursorY = (m_CursorY + 1) % m_KeyGrid.size();
        if (m_CursorX >= m_KeyGrid[m_CursorY].size())
            m_CursorX = m_KeyGrid[m_CursorY].size() - 1; // clamp
        break;

    case Direction::Left:
        m_CursorX = (m_CursorX - 1 + m_KeyGrid[m_CursorY].size()) % m_KeyGrid[m_CursorY].size();
        break;

    case Direction::Right:
        m_CursorX = (m_CursorX + 1) % m_KeyGrid[m_CursorY].size();
        break;
    }

	//std::cout << "Cursor Position: (" << m_CursorX << ", " << m_CursorY << ")\n";
	//update cursor position
    SetMenuArrowPosition();
   
}

void KeypadComponent::Select()
{
    char selected = m_KeyGrid[m_CursorY][m_CursorX];
    if (selected == '<') // RUB
    {
        if (m_nameEntered)
            return;
        if (!m_name.empty())
        {
            m_name.pop_back();
            m_isDirty = true;
        }
    }
    else if (selected == '\n') // END
    {
        if (m_nameEntered)
            return;

        if (!m_name.empty())
        {
            auto game = GameManager::GetInstance().GetActiveGame();
            auto burgerTime = dynamic_cast<BurgerTimeGame*>(game);

            dae::HighScoreManager::GetInstance().AddScore(m_name, burgerTime->m_Score);

            m_showCursor = false;
            m_isDirty = true;
            m_nameEntered = true;
			m_transitionTimer = 0.f; 
        }
    }
    else
    {
        if (m_nameEntered)
            return;

        if (m_name.size() < m_charLimit)
        {
            m_name.push_back(selected);
            m_isDirty = true;
        }
    }

	std::cout << "Selected: " << m_name << "\n";
}


void KeypadComponent::SetMenuArrowPosition()
{
    if (m_menuArrowIcon)
    {
        auto textureComp = m_menuArrowIcon->GetComponent<dae::TextureComponent>();
        if (textureComp)
        {
           
            auto pos = GetCursorWorldPos();

            pos.y -= textureComp->GetTextureSize().y * textureComp->GetOwner()->GetTransform().GetScale().y;
            pos.x += 2;
            m_menuArrowIcon->GetTransform().SetPosition(pos);


           // m_dirtyFlag = false;
        }
       // else
       // {
            //no texture found error
       // }

    }
}