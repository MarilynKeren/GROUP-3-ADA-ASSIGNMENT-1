#include <Arduino.h>

#define MAX_MENU_DEPTH 10

// Menu IDs
#define MENU_MAIN 0
#define MENU_SETTINGS 1
#define MENU_DISPLAY_SETTINGS 2
#define MENU_AUDIO_SETTINGS 3
#define MENU_BRIGHTNESS 4
#define MENU_CONTRAST 5

class MenuStack {
private:
    int stack[MAX_MENU_DEPTH];
    int top;

public:
    MenuStack() : top(-1) {}

    void push(int menuId) {
        if (top >= MAX_MENU_DEPTH - 1) {
            Serial.println("Stack overflow! Cannot push.");
            return;
        }
        stack[++top] = menuId;
    }

    int pop() {
        if (isEmpty()) {
            Serial.println("Stack underflow! No menu to pop.");
            return -1;
        }
        return stack[top--];
    }

    int peek() {
        if (isEmpty()) return -1;
        return stack[top];
    }

    bool isEmpty() {
        return top == -1;
    }
};

MenuStack navStack;
int currentMenu = MENU_MAIN;

void displayMenu(int menuId) {
    Serial.println("\n---Menu---");
    switch (menuId) {
        case MENU_MAIN:
            Serial.println("1. Main Menu");
            Serial.println(" Enter 's' for Settings");
            break;
        case MENU_SETTINGS:
            Serial.println("2. Settings Menu");
            Serial.println(" Enter 'd' for Display Settings");
            Serial.println(" Enter 'a' for Audio Settings");
            Serial.println(" Enter 'b' to go Back");
            break;
        case MENU_DISPLAY_SETTINGS:
            Serial.println("3. Display Settings Menu");
            Serial.println(" Enter 'r' for Brightness");
            Serial.println(" Enter 'c' for Contrast");
            Serial.println(" Enter 'b' to go Back");
            break;
        case MENU_AUDIO_SETTINGS:
            Serial.println("3. Audio Settings Menu");
            Serial.println(" (No sub-menus)");
            Serial.println(" Enter 'b' to go Back");
            break;
        case MENU_BRIGHTNESS:
            Serial.println("4. Brightness Menu (Setting value not implemented)");
            Serial.println(" Enter 'b' to go Back");
            break;
        default:
            Serial.println("Unknown Menu");
            break;
    }
    Serial.print("> ");
}

void setup() {
    Serial.begin(115200);
    // Small delay to ensure serial is ready
    delay(100);
    displayMenu(currentMenu);
}

void loop() {
    if (Serial.available() > 0) {
        char command = Serial.read();
        // Convert to lowercase for consistency
        if (command >= 'A' && command <= 'Z') {
            command = command + 32; // to lowercase
        }

        // Ignore newline and carriage return
        if (command == '\n' || command == '\r') {
            return;
        }

        if (command == 'b') { // BACK command
            if (!navStack.isEmpty()) {
                currentMenu = navStack.pop();
            } else {
                Serial.println("Already at main menu. Cannot go back.");
            }
        } else { // Navigation command
            int newMenu = -1;

            if (currentMenu == MENU_MAIN && command == 's') {
                newMenu = MENU_SETTINGS;
            }
            else if (currentMenu == MENU_SETTINGS) {
                if (command == 'd') newMenu = MENU_DISPLAY_SETTINGS;
                else if (command == 'a') newMenu = MENU_AUDIO_SETTINGS;
            }
            else if (currentMenu == MENU_DISPLAY_SETTINGS) {
                if (command == 'r') newMenu = MENU_BRIGHTNESS;
                else if (command == 'c') newMenu = MENU_CONTRAST;
            }

            if (newMenu != -1) {
                navStack.push(currentMenu);
                currentMenu = newMenu;
            } else {
                Serial.println("Invalid command for this menu.");
            }
        }

        displayMenu(currentMenu);
    }
}
/*

This program implements a simple menu navigation system using a stack.

- Each menu is identified by a unique ID.
- The MenuStack class stores previously visited menus.
- When entering a submenu, the current menu is pushed onto the stack.
- When 'b' is pressed, the previous menu is popped from the stack (go back).
- Serial input is used to navigate between menus.
- displayMenu() prints the current menu options to the Serial Monitor.

The stack allows multi-level menu navigation similar to real user interfaces.
*/