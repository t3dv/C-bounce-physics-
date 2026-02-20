#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <format>
#include <random>

#include "SFML/Audio/Listener.hpp"

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
    float REST_THRESHOLD = 15.f;
    CircleShape shape;
    Vector2f velocity;
    bool isStatic = false;
    ballsInSim(float  radius) : shape(radius), velocity(1.f, 1.f), isStatic(false) {
        shape.setFillColor(Color::Red);

        float x = static_cast<float>(rand() % 600 + 100);
        float y = static_cast<float>(rand() % 100 + 10);
        shape.setPosition({x,y});
    }

    void update (float dt, float gravity, float jumpImpulse, float limX, float limY) {// This function updates the ball
        if (isStatic) return;
        velocity.y += gravity * dt;
        shape.move(velocity*dt);

        if (shape.getPosition().y > limY-shape.getRadius()*2.f) {
            shape.setPosition({shape.getPosition().x, limY-shape.getRadius()*2.f});
            velocity.y *= -jumpImpulse;
            if (std::abs(velocity.y) < REST_THRESHOLD) { // Threshold for stopping the ball so it doesn't vibrate forever.
                velocity = {0.f, 0.f};
                isStatic = true;
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

struct Node { // linked list used intentionally to practice manual memory management
    ballsInSim balls;
    Node* next;

    Node(float radius) : balls(radius), next(nullptr) {}
};

int main() {
    float mass = 0;
    srand(time(NULL));
    unsigned int width = 500;
    unsigned int height = 500;
    RenderWindow window(VideoMode({width, height}), "SFML 3 Physics Test");
    Font font;
    if (!font.openFromFile("Arial_Bold.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
    }

    PhysHUD gui(font, width, height);

    int ballNum=0;
    Node* head = nullptr;
    const float gravity = 980.f; // change to affect gravity, duuh
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
                    newNode->balls.shape.setPosition(window.mapPixelToCoords(mousePressed->position));
                    newNode->next = head;
                    head = newNode;
                    ballNum++;
                }

            }
        }

        float dt = clock.restart().asSeconds(); //constant time, not dependent on refresh rate
        if (dt > 0.1f) dt = 0.1f;

        Node* current = head;
        while (current != nullptr) {
            current->balls.update(dt, gravity, jumpImpulse, width, height);
            current = current->next;
        }

        Node* i = head;
        while (i != nullptr) {
            float r = i->balls.shape.getRadius();
            Node* j = i->next;
            while (j != nullptr) {
                Vector2f diff = i->balls.shape.getPosition() - j->balls.shape.getPosition();
                float distSq = (diff.x * diff.x) + (diff.y * diff.y);
                float minDist = r + r;
                float minDistSq = minDist * minDist;

                if (distSq < minDistSq) {
                    float actualDist = std::sqrt(distSq);
                    if (actualDist == 0.f) {
                        i->balls.shape.move({0.1f, 0.1f});
                        j = j->next;
                        continue;
                    }

                    Vector2f normal = diff / actualDist;
                    float overlap = minDist - actualDist;
                    i->balls.shape.move(normal * (overlap * 0.5f));
                    j->balls.shape.move(normal * (-overlap * 0.5f));

                    Vector2f relativeVelocity = i->balls.velocity - j->balls.velocity;

                    float velocityAlongNormal = (relativeVelocity.x * normal.x) + (relativeVelocity.y * normal.y);

                    if (velocityAlongNormal < 0) {

                        float restitution = 0.8f;

                        float jImpulse = -(1.f + restitution) * velocityAlongNormal;// Had to look up the impulse formula simplified it for equal mass balls to keep the math manageable.
                        jImpulse *= 0.5f;

                        Vector2f impulse = jImpulse * normal;

                        i->balls.velocity += impulse;
                        j->balls.velocity -= impulse;
                    }
                }
                j = j->next;
            }
            i = i->next;
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
