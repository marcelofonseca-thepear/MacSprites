#pragma once

#include <print>
#include <raylib.h>
#include <string>
#include <vector>

struct AddCube;
struct ChangeColor;
struct EraseCube;

std::vector<Rectangle> cubes;
std::vector<Color> colors;

struct State{
  int currentState = 0;
};

struct Commands {
  virtual ~Commands() {};
  virtual void execute(State& state) = 0;
  virtual void undo(State& state) = 0;
};

struct AddCube final : Commands {
  int x;
  int y;
  int grid;
  Color chosenColor;

  AddCube(Vector2& pos, Color& color, int& GRID) : x(pos.x), y(pos.y), chosenColor(color), grid(GRID){
    std::println("x:{} y:{}", x, y);
  }
  
  void execute(State& state) override{
    cubes.emplace_back(x, y, grid, grid);
    colors.emplace_back(chosenColor);
  }
  
  void undo(State& state) override{
    if (!cubes.empty()) {
     cubes.pop_back();
      colors.pop_back();
    }
  }
};

struct ChangeColor final : Commands{
  bool wasChanged;
  Color NewColor;
  Color LastColor;

  ChangeColor(std::string& c){
    int getHex = std::stol("0x" + c + "ff", nullptr, 16);
    NewColor = GetColor(getHex);
  }

  void execute(State& state) override{ 
    LastColor = colors.back();
     colors.back() = NewColor;;
      wasChanged = true;
  }
  void undo(State& state) override{
    if(wasChanged && !colors.empty()){
     colors.pop_back();
    }
  }
};

struct EraseBlock final : Commands{
    Vector2 posMouse;  
    Rectangle lastBlock;
    Color lastColor;
    int goBack;

    EraseBlock(Vector2& v) : posMouse(v){ 
      std::println("Cubo removido em x:{} e y:{}", posMouse.x, posMouse.y);
    }

    void execute(State& state) override{
     for(int i{0}; i < cubes.size(); i++){
      if(cubes[i].x == posMouse.x && cubes[i].y == posMouse.y){
       lastColor = colors[i];
        lastBlock = cubes[i];
         goBack = i;

       cubes.erase(cubes.begin() + i);
        if(i < colors.size()){
         colors.erase(colors.begin() + i);
        }
       break;
      }
    }
  }

  void undo(State& state) override{
    if(!cubes.empty() && !colors.empty()){
     cubes.insert(cubes.begin() + goBack, lastBlock);
     colors.insert(colors.begin() + goBack, lastColor);
   }
  }
};

struct ChangeSize{

  std::string chosenSize;
  int finalWidth, finalHeight;

  ChangeSize(std::string& Width, std::string& Height) {
    finalWidth = std::atoi(Width.c_str());
    finalHeight = std::atoi(Height.c_str());
  };

  Vector2 Change(){
    std::println("Width: {}, Height: {}", (float)finalWidth, (float)finalHeight);
    return (Vector2) { (float)finalWidth, (float)finalHeight };
  }
};

