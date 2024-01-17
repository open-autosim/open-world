#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
// #include "utils/button.h"
#include "primitives/polygon.h"
#include "editors/graph_editor.h"
#include "editors/stop_editor.h"
#include "math/graph.h"
#include "viewport.h"
#include "world.h"
#include "primitives/envelope.h"
#include "editors/marking_editor.h"
#include "markings/marking.h"

#include "imgui.h"
// #include "imgui_stdlib.h"
#include "imgui-sfml/imgui-SFML.h"


int main() {
    int windowWidth = 1200;
    int windowHeight = 1200;

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "World Editor");    
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    //load graph info from file
    Graph graph;
    if (!graph.load("graph_data.bin")) {
        std::cout << "No saved graph found, starting with a new graph." << std::endl;
    }

    //loaf font
    sf::Font font;
    if (!font.loadFromFile("/Users/shlomodahan/Desktop/MCIT/open-world/phase_2/assets/Roboto-Regular.ttf")) {
        std::cout << "Could not load font." << std::endl;
    }

    World world(graph);
    Viewport viewport(window);
    GraphEditor graphEditor(viewport, graph);
    StopEditor stopEditor(window, world, viewport);

    std::string oldGraphHash = graph.hash();

    world.generate();

    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {

            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                window.close();

            // Only process viewport and editor events if ImGui does not want to capture the mouse
            if (!ImGui::GetIO().WantCaptureMouse) {
                if (event.type == sf::Event::MouseButtonPressed ||
                    event.type == sf::Event::MouseButtonReleased ||
                    event.type == sf::Event::MouseMoved) {
                    viewport.handleEvent(event);
                    graphEditor.handleEvent(event);
                    stopEditor.handleEvent(event);
                }
            }

            // Process other events normally
            if (event.type == sf::Event::MouseWheelScrolled) {
                viewport.handleEvent(event);
             }
            
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        // ImGui window
        ImGui::Begin("Control Panel");
        if (ImGui::Button("Save")) {
            graph.save("graph_data.bin");
        }
        if (ImGui::Button("Dispose")) {
            graphEditor.dispose();
        }
        if (ImGui::Button("Refresh")) {
            graphEditor.dispose();
            graph.load("graph_data.bin");
            //set window center to graph center
        }
        // Graph Editor Button
        if (graphEditor.isMouseEnabled()) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.8f, 0.2f, 1.0f));
            if (ImGui::Button("Graph Editor: Enabled")) {
                graphEditor.disable();
                stopEditor.enable(); // Enable Stop Editor when Graph Editor is disabled
            }
            ImGui::PopStyleColor();
        } else {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
            if (ImGui::Button("Graph Editor: Disabled")) {
                graphEditor.enable();
                stopEditor.disable(); // Disable Stop Editor when Graph Editor is enabled
            }
            ImGui::PopStyleColor();
        }

        // Stop Editor Button
        if (stopEditor.isMouseEnabled()) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.8f, 0.2f, 1.0f));
            if (ImGui::Button("Stop Editor: Enabled")) {
                stopEditor.disable();
                graphEditor.enable(); // Enable Graph Editor when Stop Editor is disabled
            }
            ImGui::PopStyleColor();
        } else {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
            if (ImGui::Button("Stop Editor: Disabled")) {
                stopEditor.enable();
                graphEditor.disable(); // Disable Graph Editor when Stop Editor is enabled
            }
            ImGui::PopStyleColor();
        }
        ImGui::End();


        window.clear(sf::Color(42, 170, 85));
        // window.clear(sf::Color(255, 255, 255));

        viewport.reset();

        if (graph.hash() != oldGraphHash) {
            world.generate();
            oldGraphHash = graph.hash();
        }

        Point viewPoint = Utils::scale(viewport.getOffset(), -1);
        world.draw(window, viewPoint);
        graphEditor.display();       
        stopEditor.display();

        ImGui::SFML::Render(window);

        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}

