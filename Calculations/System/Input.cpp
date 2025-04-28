#include "pch.h"
#include "Input.h"

Input::Input()
{
    m_MouseButtons[0] = false;
    m_MouseButtons[1] = false;
    m_MouseButtons[2] = false;
    m_MouseX = 0;
    m_MouseY = 0;
}

Input::~Input()
{
    m_MouseButtons[0] = false;
    m_MouseButtons[1] = false;
    m_MouseButtons[2] = false;
    m_MouseX = 0;
    m_MouseY = 0;
}

void Input::HandleEvent(const SDL_Event& e)
{
    switch (e.type)
    {
    case SDL_EVENT_MOUSE_MOTION:
    {
        m_MouseX = e.motion.x;
        m_MouseY = e.motion.y;
    }
    break;

    case SDL_EVENT_MOUSE_BUTTON_UP:
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    {
        bool state = false;
        if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
        {
            state = true;
        }

        m_MouseButtons[e.button.button - 1] = state;
    }
    break;

    default:
        break;
    }
}

bool Input::GetMouseButtonDown(const MOUSE_BUTTON& button) const
{
    return m_MouseButtons[button];
}

const int& Input::GetMouseX() const
{
    return m_MouseX;
}

const int& Input::GetMouseY() const
{
    return m_MouseY;
}
