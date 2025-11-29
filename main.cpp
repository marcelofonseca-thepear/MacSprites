#include <print>
#include <memory>
#include <raylib.h>
#include <string>
#include <vector>

#define SCREENWIDTH 450
#define SCREENHEIGHT 450

struct AddCube;
struct ChangeColor;
struct EraseCube;

static int GRID = 16;

std::vector<Rectangle> cubes;
std::vector<Color> colors;

struct Action {
  virtual void execute() = 0;
};

struct AddCube final : Action {
  int x;
  int y;
  Color chosenColor;

  AddCube(Vector2& pos, Color& color) : x(pos.x), y(pos.y), chosenColor(color){
    std::println("x:{} y:{}", x, y);
  }
  
  void execute() override{
    cubes.emplace_back(x, y, GRID + 1, GRID + 1);
    colors.emplace_back(chosenColor);
  }
};

struct ChangeColor final : Action{
  std::string color;

  ChangeColor(std::string& c) : color(c){
    std::println("Cor alterada!");
  }

  void execute() override{ 
    color = "0x" + color + "ff";
    int getHex = std::stol(color, nullptr, 16);
  
    Color chosenColor = GetColor(getHex);
    colors.back() = chosenColor;;

    std::println("string: {}", getHex);
    std::println("hex: {} {} {}", colors.back().r, colors.back().g, colors.back().b);
  }
};

struct EraseCube final : Action{
    Vector2 posMouse;  

    EraseCube(Vector2& v) : posMouse(v){ 
      std::println("Cubo removido em x:{} e y:{}", posMouse.x, posMouse.y);
    }

    void execute() override{
     for(int i{0}; i < cubes.size(); i++){
        if(cubes[i].x == posMouse.x && cubes[i].y == posMouse.y){
         cubes.erase(cubes.begin() + i);

        if(i < colors.size()){
         colors.erase(colors.begin() + i);
        }

       break;
      }
    }
  }
};

  void drawgrid(int size, bool ON_OFF){
   if(ON_OFF){
    for(int i{0}; i < SCREENHEIGHT / size; i++){
       int x = i * size;
        for(int j{0}; j < SCREENWIDTH / size; j++){
         int y = j * size;
         DrawLine(x, 0, x, SCREENHEIGHT, WHITE);
         DrawLine(0, y, SCREENWIDTH, y , WHITE);
        }
      }
    }
  }

  void colorWindow(bool& ON_OFF, auto& actions){
    Rectangle recWindow = {25, 280, SCREENWIDTH / 3, SCREENHEIGHT / 3}; 
    static std::string getColor = "";
    char key = GetCharPressed();

    if(!ON_OFF){
     recWindow = {0, 0, 0, 0};
     getColor.clear();
    }
    else{
     if(key != 0 && !IsKeyPressed(KEY_ENTER) && getColor.length() < 8) getColor += static_cast<char>(key);
     if(IsKeyDown(KEY_BACKSPACE) && !getColor.empty()) getColor.pop_back();
     if(IsKeyPressed(KEY_ENTER)) actions.emplace_back(std::make_unique<ChangeColor>(getColor));

     DrawRectangleRec(recWindow, Color{116,124,139,255});
     DrawRectangleLinesEx(recWindow, 5.0, BLACK);
     DrawText("color(in hex)", recWindow.x * 1.9, recWindow.y * 1.07, 13, WHITE);
     DrawText(getColor.c_str(), recWindow.x * 2, recWindow.y * 1.13, 15, Color{219,219,163,255});
     DrawLine(recWindow.x * 1.8, recWindow.y * 1.2, recWindow.x * 5.0, recWindow.y * 1.2, Color{219,219,163,255});
    }
  }

  Vector2 snap2Grid(Vector2 Pos);

  void Update(){
    for(int i{0}; i < cubes.size(); i++){
     Vector2 newcubePos = snap2Grid(Vector2{cubes[i].x, cubes[i].y});
     cubes[i].x = newcubePos.x;
     cubes[i].y = newcubePos.y;
     cubes[i].height = GRID - 1;
     cubes[i].width = GRID - 1;
    } 
  }

  Vector2 snap2Grid(Vector2 Pos){
    float snapX = (float)((int)(Pos.x / GRID) * GRID); 
    float snapY = (float)((int)(Pos.y / GRID) * GRID);

    return (Vector2){ snapX, snapY };
  }

  int main(){
    bool colorWindow_on_off = false;
    bool grid_on_off = true;
    int colorWindow_buffer = 0;
    int grid_buffer = 0;
    if(colors.empty()) colors.emplace_back(WHITE);

    InitWindow(SCREENHEIGHT, SCREENWIDTH, "MacSprites");
    SetTargetFPS(60);

    auto actions = std::vector<std::unique_ptr<Action>>{};

    while(!WindowShouldClose()){
      if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        Vector2 posMouse = GetMousePosition();
        Vector2 posMouse_snap = snap2Grid(posMouse);
        Color currentColor = colors.back();
        actions.emplace_back(std::make_unique<AddCube>(posMouse_snap, currentColor));
      }
      if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT)){
       Vector2 posMouse = GetMousePosition();
       Vector2 posMouse_snap = snap2Grid(posMouse);
       actions.emplace_back(std::make_unique<EraseCube>(posMouse_snap));
      }
      if(IsKeyPressed(KEY_C)){
        ++colorWindow_buffer;
        colorWindow_on_off = colorWindow_buffer % 2;
      }
      if(IsKeyPressed(KEY_V)){
        ++grid_buffer;
        grid_on_off = grid_buffer % 2;
      }
      if(IsKeyPressed(KEY_UP)){
        GRID *= 2;
      }
      if(IsKeyPressed(KEY_DOWN)){
        GRID /= 2;
      }


      for(const auto& act: actions){
        act->execute();
      }

      actions.clear();

     BeginDrawing();
       ClearBackground(GRAY);
       Update();
       drawgrid(GRID, grid_on_off);

       for(int i{0}; i < cubes.size(); i++){
        DrawRectangleRec(cubes[i], colors[i]);
       }

       colorWindow(colorWindow_on_off, actions);

    EndDrawing();

  }
}

