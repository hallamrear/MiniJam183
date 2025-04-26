// Calculations.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <pch.h>
#include <System/Services.h>
#include <Application.h>

int main(int argc, char* args[])
{
    srand(time(nullptr));

    std::cout << "Hello World!\n";

    Application* app = new Application();

    if (app->Initialise() == false)
    {
        std::cout << "Failed on startup.\n";
        return 0;
    }

    Uint64 currentTime = 0;
    Uint64 previousTime = 0;
    float deltaTime = 0.0f;

    while (app->IsRunning())
    {
        currentTime = SDL_GetTicks();
        deltaTime = (currentTime - previousTime) / 1000.0f;

        app->ProcessEvents(deltaTime);
        app->Update(deltaTime);
        app->Render();

        previousTime = currentTime;
    }

    app->Shutdown();

    if (app != nullptr)
    {
        delete app;
        app = nullptr;
    }

    std::cout << "Goodbye World!\n";

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file