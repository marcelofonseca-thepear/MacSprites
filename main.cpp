#include <memory>
#include <raylib.h>
#include <string>
#include "commands.hpp"

#define SCREENHEIGHT (float)GetMonitorHeight(GetCurrentMonitor()) 
#define SCREENWIDTH (float)GetMonitorWidth(GetCurrentMonitor()) 

static Vector2 whiteboardSize{640, 480};
static int GRID = 16;

void drawGrid(bool& draw){
  float posX = (SCREENWIDTH/2.0f) - (whiteboardSize.x/2.0f);
  float posY = (SCREENHEIGHT/2.0f) - (whiteboardSize.y/2.0f);

  if(draw){
    for(int i{0}; i < whiteboardSize.x / GRID; i++){
      int offsetX = i * GRID;
       for(int j{0}; j < whiteboardSize.y / GRID; j++){
        int offsetY = j * GRID;
          DrawLine(posX + offsetX, posY, posX + offsetX, posY + whiteboardSize.y, WHITE);
          DrawLine(posX, posY + offsetY, posX + whiteboardSize.x, posY + offsetY, WHITE);
      }
    }
  }
}

void drawWhiteBoardWindow(bool& draw){
  float posX = (SCREENWIDTH/2.0f) - (whiteboardSize.x/2.0f);
  float posY = (SCREENHEIGHT/2.0f) - (whiteboardSize.y/2.0f);

  Rectangle recWindow{posX, posY, whiteboardSize.x, whiteboardSize.y};
   DrawRectangleRec(recWindow, GRAY);
    DrawRectangleLinesEx(recWindow, 3.0, DARKGRAY);
     drawGrid(draw);
}

bool IsInsideWhiteboard(Vector2& point) {
  float posX = (SCREENWIDTH / 2.0f) - (whiteboardSize.x / 2.0f);
  float posY = (SCREENHEIGHT / 2.0f) - (whiteboardSize.y / 2.0f);

  Rectangle bounds = { posX, posY, whiteboardSize.x, whiteboardSize.y };
   return CheckCollisionPointRec(point, bounds);
}

void drawColorWindow(auto& commit){

   char key = GetCharPressed();
   Rectangle recWindow{25, 280, SCREENWIDTH / 8, SCREENHEIGHT / 4}; 
     static std::string getColor = "";
     static bool inputDelayer = true;

     while(inputDelayer){
       inputDelayer = false;
       key = 0;
     }

    if(key != 0 && !IsKeyPressed(KEY_ENTER) && getColor.length() < 8) getColor += static_cast<char>(key);
      if(IsKeyDown(KEY_BACKSPACE) && !getColor.empty()) getColor.pop_back();
        if(IsKeyPressed(KEY_ENTER)){
         commit(std::make_unique<ChangeColor>(getColor));
         getColor.clear();
         }

     DrawRectangleRec(recWindow, WHITE);
      DrawRectangleLinesEx(recWindow, 5.0, BLACK);

    DrawText("color -in hex-", recWindow.x * 1.9, recWindow.y * 1.05, 16, GetColor(0xFF94C6FF));
     DrawText(getColor.c_str(), recWindow.x * 1.9, recWindow.y * 1.15, 13, GetColor(0xFF94C6FF));
      DrawLine(recWindow.x * 1.8, recWindow.y * 1.2, recWindow.x * 5.0, recWindow.y * 1.2, GetColor(0xFF94C6FF));

    recWindow = {0, 0, 0, 0};
    }

void drawChangeSizeWindow(){
    Rectangle recWindow = {SCREENWIDTH/4.5f, SCREENHEIGHT/3.0f, SCREENWIDTH/1.75f, SCREENHEIGHT/8.0f};
     DrawRectangleRec(recWindow, WHITE);
      DrawRectangleLinesEx(recWindow, 5.0, BLACK);

    static std::string chosenSize = " ";
     static bool editWidth{true}, editHeight{false};

    char key = GetCharPressed();
      while (key > 0) { 
       if ((key >= 48) && (key <= 57) && (chosenSize.length() <= 4)) {
          chosenSize += (char)key;
        }
          key = GetCharPressed();
        }

      if (IsKeyPressed(KEY_BACKSPACE) && !chosenSize.empty()) {
        chosenSize.pop_back();
      }

      static std::string getWidth;
       static std::string getHeight;

      if (IsKeyPressed(KEY_ENTER)) {
       if(editWidth){
        getWidth = chosenSize;
         chosenSize.clear();
          editHeight = true;
           editWidth = false;
      }else{ 
        getHeight = chosenSize;
         chosenSize.clear();
          editWidth = true;
           editHeight = false;
            ChangeSize change(getWidth, getHeight);
              whiteboardSize = change.Change();
      }
     }

    DrawText("Width", recWindow.x * 1.15f, recWindow.y * 1.05f, 16, GetColor(0xFF94C6FF));
     if (editWidth) DrawText(chosenSize.c_str(), recWindow.x * 1.15f, recWindow.y * 1.20f, 16, RED);
     else DrawText(getWidth.c_str(), recWindow.x * 1.15f, recWindow.y * 1.20f, 16, BLACK);

    DrawText("Height", recWindow.x * 2.45f, recWindow.y * 1.05f, 16, GetColor(0xFF94C6FF));
     if (editHeight) DrawText(chosenSize.c_str(), recWindow.x * 2.45f, recWindow.y * 1.20f, 16, RED);
     else DrawText(getHeight.c_str(), recWindow.x * 2.45f, recWindow.y * 1.20f, 16, BLACK);
    
}

void drawHelpWindow(){
     DrawText("CTRL + C - Open the color picker window", SCREENHEIGHT / 20, SCREENWIDTH / 10, 20, BLACK);
     DrawText("CTRL + V - Hide/Show grid", SCREENHEIGHT / 20 , SCREENWIDTH / 8.65, 20, BLACK);
     DrawText("CTRL + R - Change drawable screen size", SCREENHEIGHT / 20, SCREENWIDTH / 7.65, 20, BLACK);
     DrawText("CTRL + Z - Go back", SCREENHEIGHT / 20, SCREENWIDTH / 6.65, 20, BLACK);
     DrawText("H - Close the instruction window", SCREENHEIGHT / 20, SCREENWIDTH / 6, 20, BLACK);
     DrawText("UpArrow - Double the grid size", SCREENHEIGHT / 20, SCREENWIDTH / 5.45, 20, BLACK);
     DrawText("DownArrow - Half the grid size", SCREENHEIGHT / 20, SCREENWIDTH / 5, 20, BLACK);
}

Vector2 snap2Grid(Vector2 Pos){
   float snapX = (float)((int)(Pos.x / GRID) * GRID); 
   float snapY = (float)((int)(Pos.y / GRID) * GRID);

   return (Vector2){ snapX, snapY };
}

void Update(){
   for(int i{0}; i < cubes.size(); i++){
    Vector2 newcubePos = snap2Grid(Vector2{cubes[i].x, cubes[i].y});
      cubes[i].x = newcubePos.x;
        cubes[i].y = newcubePos.y;
          cubes[i].height = GRID + 1;
            cubes[i].width = GRID + 1;
  } 
}

bool switchBool(bool& on_off){
 if(on_off)
  return on_off = false;
 else
   return on_off = true;
}

int main(){
   int switch_screen{0}; 
   bool switch_PaintMode{false};
   bool switch_grid{true}, switch_help{true}, switch_colorwindow{false}, switch_sizewindow{false};
   if(colors.empty()) colors.emplace_back(WHITE);

   InitWindow(SCREENHEIGHT, SCREENWIDTH, "MacSprites");
   SetTargetFPS(30);

   State state;
   std::vector<std::unique_ptr<Commands>> Historic;
    auto commit = [&](auto&& command) { 
      command->execute(state); 

      Historic.emplace_back(std::move(command)); 
    };
    auto undo = [&](){
     if(!Historic.empty()){
      Historic.back()->undo(state);
      Historic.pop_back();
     }
    };

   while(!WindowShouldClose()){
    if(switch_PaintMode){
     if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
       Vector2 posMouse = GetMousePosition();
       Vector2 posMouse_snap = snap2Grid(posMouse);

       if(IsInsideWhiteboard(posMouse_snap)){
       Color currentColor = colors.back();
       commit(std::make_unique<AddCube>(posMouse_snap, currentColor, GRID));
       }
      }
     } else {
     if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
       Vector2 posMouse = GetMousePosition();
       Vector2 posMouse_snap = snap2Grid(posMouse);

       if(IsInsideWhiteboard(posMouse_snap)){
       Color currentColor = colors.back();
       commit(std::make_unique<AddCube>(posMouse_snap, currentColor, GRID));
       }
      }
     }
     if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT)){
      Vector2 posMouse = GetMousePosition();
      Vector2 posMouse_snap = snap2Grid(posMouse);
      commit(std::make_unique<EraseBlock>(posMouse_snap));
     }
     if(IsKeyPressed(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_B)){
      switchBool(switch_PaintMode);
     } 
     if(IsKeyPressed(KEY_H)){
      switchBool(switch_help);
     }
     if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_C)){
      switchBool(switch_colorwindow);
     }
     if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_R)){
      switchBool(switch_sizewindow);
     }
     if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_V)){
      switchBool(switch_grid);
     }
     if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_Z)){
      undo();
     }
     if(IsKeyPressed(KEY_UP) && GRID < 128){
       GRID *= 2;
     }
     if(IsKeyPressed(KEY_DOWN) && GRID > 8){
       GRID /= 2;
     }

    BeginDrawing();

     ClearBackground(DARKGRAY);
      drawWhiteBoardWindow(switch_grid);    
       Update();


     for(int i{0}; i < cubes.size(); i++){
      DrawRectangleRec(cubes[i], colors[i]);
     }

      if(switch_help)
        drawHelpWindow();
      if(switch_colorwindow)
        drawColorWindow(commit);
      if(switch_sizewindow)
        drawChangeSizeWindow();

    EndDrawing();

  }
}

