#include <SFML/Graphics.hpp>
#include <iostream>
#include "math/graph.h"
#include <cstdlib>
#include <ctime>
#include "utils/button.h"
#include "primitives/polygon.h"
#include "graph_editor.h"
#include "viewport.h"
#include "world.h"
#include "primitives/envelope.h"

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
    World world(graph);
    Viewport viewport(window);
    GraphEditor graphEditor(viewport, graph);

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
        ImGui::End();


        window.clear(sf::Color(42, 170, 85));

        viewport.reset();

        if (graph.hash() != oldGraphHash) {
            world.generate();
            oldGraphHash = graph.hash();
        }

        Point viewPoint = Utils::scale(viewport.getOffset(), -1);
        world.draw(window, viewPoint);
        graphEditor.display();       

        ImGui::SFML::Render(window);

        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}

