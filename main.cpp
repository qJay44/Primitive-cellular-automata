#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"
#include <algorithm>
#include <cmath>
#include <exception>
#include <ios>
#include <iostream>
#include <bitset>
#include <map>
#include <string>

std::string decimalToBinary(int num) {
  int n = (int)(log2(num));

  std::string binary = std::bitset<8>(num).to_string().substr(8 - n - 1);

  if (binary.size() < 8)
    binary.insert(binary.begin(), 8 - binary.size(), '0');

  return binary;
};

int main() {
  srand((unsigned)time(NULL));

  const int width = 1200;
  const int height = 800;
  const int squareSize = 1;
  const int rows = height / squareSize;
  const int columns = width / squareSize;
  const int rule = rand() % 255 + 1;

  // create the window
  sf::RenderWindow window(sf::VideoMode(width, height), "Primitive cellular automata");
  window.setFramerateLimit(75);

  sf::RenderTexture renderTexture;
  renderTexture.create(width, height);

  // Get the render texture and make sprite of it
  const sf::Texture &canvasTexture = renderTexture.getTexture();
  sf::Sprite canvasSprite(canvasTexture);

  char grid[rows][columns] {};

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      grid[i][j] = '0';
    }
  }

  grid[0][columns / 2] = '1';

  const std::string ruleBinary = decimalToBinary(rule);
  std::map<std::string, char> cellRules;

  for (int i = 7; i >= 0; i--) {
    cellRules[std::bitset<3>(i).to_string()] = ruleBinary[7 - i];
  }

  for (int i = 0; i < rows - 1; i++) {
    for (int j = 0; j < columns; j++) {
      char mid = grid[i][j];
      char left;
      char right;

      if (j == 0) {
        left = grid[i][columns - 1];
        right = grid[i][j + 1];
      }
      else if (j == columns - 1 && i != rows - 1) {
        left = grid[i][j - 1];
        right = grid[i][0];
      } else {
        left = grid[i][j - 1];
        right = grid[i][j + 1];
      }

      std::string currentRule {left, mid, right};
      grid[i + 1][j] = cellRules[currentRule];

      sf::RectangleShape rect;
      rect.setSize(sf::Vector2f(squareSize, squareSize));
      rect.setPosition(j * squareSize, height - squareSize - i * squareSize);

      if (grid[i][j] == '1')
        rect.setFillColor(sf::Color::Black);
      else
        rect.setFillColor(sf::Color::White);

      renderTexture.draw(rect);
    }
  }

  // run the program as long as the window is open
  while (window.isOpen())
  {
    // check all the window's events that were triggered since the last iteration of the loop
    sf::Event event;
    while (window.pollEvent(event))
    {
        // "close requested" event: we close the window
        if (event.type == sf::Event::Closed)
            window.close();
        if (event.KeyPressed && event.key.code == sf::Keyboard::Q)
            window.close();
    }

    // end the current frame
    window.draw(canvasSprite);
    window.display();
  }

  return 0;
}

