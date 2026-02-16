#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <format>
#include <random>

using namespace sf;

struct PhysHUD {
    Text display;

    PhysHUD(const Font& font, float limX, float limY) : display(font, "", 18) {
        display.setFont(font);
        display.setCharacterSize(20);
        display.setFillColor(sf::Color::Yellow);
        display.setPosition({limX-250, 0});
        display.setString("INITIALIZING");
    }

    void update(int num, float grav, float jumpImpulse) {
        display.setString(std::format("Number of balls: {:d}\nGravity: {:.2f}\nJump coeficient: {:.2f}",num, grav, jumpImpulse));
    }

};


struct ballsInSim {
    CircleShape shape;
    Vector2f velocity;

    ballsInSim(float  radius) : shape(radius), velocity(1.f, 1.f) {
        shape.setFillColor(Color::Red);

        float x = static_cast<float>(rand() % 600 + 100);
        float y = static_cast<float>(rand() % 100 + 10);
        shape.setPosition({x,y});
    }

    void update (float dt, float gravity, float jumpImpulse, float limX, float limY) {
        if (velocity.y != 0 || shape.getPosition().y < limY) {
            velocity.y += gravity * dt;
            shape.move(velocity*dt);
        }

        if (shape.getPosition().y > limY-shape.getRadius()) {
            shape.setPosition({shape.getPosition().x, limY-shape.getRadius()});
            velocity.y *= -jumpImpulse;
            if (velocity.y < 15.f && velocity.y > -15.f) {
                velocity.y = 0;
                shape.setPosition({shape.getPosition().x, limY-20});
                shape.setFillColor(Color::Green);
            }
        }
        if (shape.getPosition().x < 0+ shape.getRadius()) {
            shape.setPosition({0+shape.getRadius()*0.5f, shape.getPosition().y});
            velocity.x *= -jumpImpulse;
        }
        if (shape.getPosition().x > limX-shape.getRadius()*2.f) {
            shape.setPosition({limX-shape.getRadius()*2.f, shape.getPosition().y});
            velocity.x *= -jumpImpulse;
        }
    }
};

struct Node {
    ballsInSim balls;
    Node* next;

    Node(float radius) : balls(radius), next(nullptr) {}
};

int main() {
    srand(time(NULL));
    unsigned int width = 500;
    unsigned int height = 500;
    RenderWindow window(VideoMode({width, height}), "SFML 3 Physics Test");
    Font font;
    if (!font.openFromFile("Arial_Bold.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
    }
    // counters
    PhysHUD gui(font, width, height);

    int ballNum=0;
    Node* head = nullptr;
    //constants
    const float gravity = 980.f;
    const float jumpImpulse = 0.8;
    Clock clock;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<Event::Closed>()) {
                window.close();
            }
            if (const auto* mousePressed = event->getIf<Event::MouseButtonPressed>()) {

                if (mousePressed->button == Mouse::Button::Left) {
                    Node* newNode = new Node(10.f);
                    Vector2f mousePos = window.mapPixelToCoords(mousePressed->position);
                    newNode->balls.shape.setPosition(mousePos);
                    newNode->next = head;
                    head = newNode;
                    ballNum++;
                }

            }
        }
        float dt = clock.restart().asSeconds();
        if (dt > 0.1f) dt = 0.1f;
        Node* current = head;
        while (current != nullptr) {
            current->balls.update(dt, gravity, jumpImpulse, width, height);
            current = current->next;
        }
        gui.update(ballNum, gravity,jumpImpulse);

        window.clear();
        window.draw(gui.display);
        current = head;
        while (current != nullptr) {
            window.draw(current->balls.shape);
            current = current->next;
        }
        window.display();
        }

        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
    }


        return 0;
}
