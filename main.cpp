#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"
#include <cmath>
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
}

int main() {
  srand((unsigned)time(NULL));

  const int width = 1200;
  const int height = 900;
  const int squareSize = 10;
  const int amountX = width / squareSize;
  const int amountY = height / squareSize;
  const int rule = rand() % 255 + 1;

  // create the window
  sf::RenderWindow window(sf::VideoMode(width, height), "Primitive cellular automata");
  window.setFramerateLimit(75);

  sf::RenderTexture renderTexture;
  renderTexture.create(width, height);

  // Get the render texture and make sprite of it
  const sf::Texture &canvasTexture = renderTexture.getTexture();
  sf::Sprite canvasSprite(canvasTexture);

  char grid[amountX][amountY] {};
  grid[amountX / 2][amountY / 2] = '1';

  for (int i = 0; i < amountX; i++) {
    for (int j = 0; j < amountY; j++) {
      if (i == 0 && j == 0) continue;
      grid[i][j] = '0';
    }
  }

  const std::string ruleBinary = decimalToBinary(110);
  std::map<std::string, char> cellRules;

  for (int i = 7; i >= 0; i--) {
    cellRules[std::bitset<3>(i).to_string()] = ruleBinary[7 - i];
  }

  for (int i = 0; i < amountX; i++) {
    for (int j = 0; j < amountY; j++) {
      std::string mid = std::to_string(grid[i][j]);
      std::string left;
      std::string right;
      if (j == 0) {
        std::string left  = std::to_string(grid[i][amountY - 1]);
        std::string right = std::to_string(grid[i][j + 1]);
      }
      else if (j == amountY - 1 && i != amountX - 1) {
        std::string left  = std::to_string(grid[i][j - 1]);
        std::string right = std::to_string(grid[i][0]);
      } else {
        std::string left  = std::to_string(grid[i][j - 1]);
        std::string right = std::to_string(grid[i][j + 1]);
      }

      std::string currentRule = left + mid + right;
      grid[i + 1][j] = cellRules[currentRule];

      sf::RectangleShape rect;
      rect.setSize(sf::Vector2f(squareSize, squareSize));
      rect.setPosition(i * squareSize, j * squareSize);

      if (grid[i][j] == '1')
        rect.setFillColor(sf::Color::Black);
      else
        rect.setFillColor(sf::Color::White);

      renderTexture.draw(rect);
    }
  }

  bool doOnce = true;

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
    }

    // end the current frame
    window.draw(canvasSprite);
    window.display();
  }

  return 0;
}

