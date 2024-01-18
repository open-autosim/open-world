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
#include "editors/crossing_editor.h"
#include "editors/start_editor.h"
#include "editors/light_editor.h"   
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

    World world(graph);
    Viewport viewport(window);
    GraphEditor graphEditor(world, viewport, graph);
    graphEditor.disable();

    std::vector<std::unique_ptr<MarkingEditor>> editors;
    editors.push_back(std::make_unique<StopEditor>(window, world, viewport));
    editors.push_back(std::make_unique<CrossingEditor>(window, world, viewport));
    editors.push_back(std::make_unique<StartEditor>(window, world, viewport));
    editors.push_back(std::make_unique<LightEditor>(window, world, viewport));

    // StopEditor stopEditor(window, world, viewport);
    // CrossingEditor crossingEditor(window, world, viewport);

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
                    for (auto& editor : editors) {
                        editor->handleEvent(event);
                    }
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
                for (auto& editor : editors) {
                    editor->disable(); // 
                }
            }
            ImGui::PopStyleColor();
        } else {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
            if (ImGui::Button("Graph Editor: Disabled")) {
                graphEditor.enable();
                for (auto& editor : editors) {
                    editor->disable(); // Disable other editors
                }
            }
            ImGui::PopStyleColor();
        }

        // Iterate through other editors
        for (auto& editor : editors) {
            std::string editorName = editor->getName(); // Assuming getName() is implemented
            if (editor->isMouseEnabled()) {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.8f, 0.2f, 1.0f));
                if (ImGui::Button((editorName + ": Enabled").c_str())) {
                    editor->disable();
                    graphEditor.disable(); // Disable Graph Editor
                    //disble other editors

                }
                ImGui::PopStyleColor();
            } else {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
                if (ImGui::Button((editorName + ": Disabled").c_str())) {
                    editor->enable();
                    graphEditor.disable(); // Disable Graph Editor
                    //disble other editors
                    for (auto& otherEditor : editors) {
                        if (otherEditor != editor) {
                            otherEditor->disable();
                        }
                    }
                }
                ImGui::PopStyleColor();
            }
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

        for (auto& editor : editors) {
            editor->display();
        }
        // stopEditor.display();
        // crossingEditor.display();

        ImGui::SFML::Render(window);

        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}

