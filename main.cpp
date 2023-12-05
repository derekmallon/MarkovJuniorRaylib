#include <iterator>
#include <stdio.h>
#include <string>
#include <vector>
#include <tuple>
#include <algorithm>
#include <raylib.h>
#include <math.h>
#include <stdlib.h>
#include <cstring>

std::tuple<std::string,bool> ApplyRule(std::string w,std::string rule){
  auto x = rule.substr(0,rule.find("="));
  auto y = rule.substr(rule.find("=")+1,rule.size());
  auto pos = w.find(x);
  if(pos == std::string::npos){
    return {w,false};
  }
  if( y == "\\empty"){
    y = "";
  }
  
  return {w.replace(w.find(x),x.size(),y),true};
}


typedef std::vector<char> State;

std::vector<int> VectorFromNtoP(int n,int p){
  std::vector<int> output(p-n,0);
  for(int i=0;i<(p-n);i++){
    output[i] = n+i;
  }
  return output;
}

std::vector<std::vector<int>> FindAllMatchesFromPoint(std::vector<std::vector<int>> matches,const State &state, int point,const std::string& xMapping){
  
  const int width = 100;
  const int mappingSize = xMapping.size();
  if(state[point] != xMapping[0]){
    return matches;
  }
  int x = point % width;
  int y = point / width;
  //check left
  if(x + xMapping.size() < width){
    if(std::memcmp(xMapping.data(),&state[point],xMapping.size()) == 0){
      matches.push_back(VectorFromNtoP(point,point+xMapping.size()));
    }
    /*
    std::vector<int> potentialMatch = {};
    for(int i=0;i<xMapping.size();i++){
      int newPoint = y*width + (i+x);
      if(state[newPoint] != xMapping[i]){
        break;
      }else{
        potentialMatch.push_back(newPoint);
      }
      if(i == xMapping.size() -1){
        matches.push_back(potentialMatch);
        break;
      }
    }
    */
  }
  //check right
  if(x > xMapping.size()){
    std::vector<int> potentialMatch = {};
    for(int i=0;i<xMapping.size();i++){
      int newPoint = y*width + (x-i);
      if(state[newPoint] != xMapping[i]){
        break;
      }else{
        potentialMatch.push_back(newPoint);
      }
      if(i == xMapping.size() -1){
        matches.push_back(potentialMatch);
        break;
      }
    }
  }

  //check up
  if(y > xMapping.size()){
    std::vector<int> potentialMatch = {};
    for(int i=0;i<xMapping.size();i++){
      int newPoint = (y-i)*width + x;
      if(state[newPoint] != xMapping[i]){
        break;
      }else{
        potentialMatch.push_back(newPoint);
      }
      if(i == xMapping.size() -1){
        matches.push_back(potentialMatch);
        break;
      }
    }
  }

  //check down
  if(y + xMapping.size() < width){
    std::vector<int> potentialMatch = {};
    for(int i=0;i<xMapping.size();i++){
      int newPoint = (y+i)*width + x;
      if(state[newPoint] != xMapping[i]){
        break;
      }else{
        potentialMatch.push_back(newPoint);
      }
      if(i == xMapping.size() -1){
        matches.push_back(potentialMatch);
        break;
      }
    }
  }
  return matches;
}

struct Rule{
  Rectangle size;
  std::vector<int> rowWise;
  std::vector<int> columnWise;


  Rule(const std::string& ruleText){
  }
}

std::vector<std::vector<int>> FindMatchesForIndexs(std::vector<std::vector<int>> matches,const State &state,const std::vector<int>& indexes,std::string rule){
  auto x = (std::string)rule.substr(0,rule.find("="));
  auto y = (std::string)rule.substr(rule.find("=")+1,rule.size());
  const int width = 100;
  for(int i=0;i<indexes.size();i++){
    matches = FindAllMatchesFromPoint(std::move(matches), state, indexes[i], x);
  }

  return matches;
}

std::vector<std::vector<int>> FindAllMatches(const State &state,std::string rule){

  auto x = (std::string)rule.substr(0,rule.find("="));
  auto y = (std::string)rule.substr(rule.find("=")+1,rule.size());
  const int width = 100;
  std::vector<std::vector<int>> matches;
  for(int i=0;i<state.size();i++){
    matches = FindAllMatchesFromPoint(std::move(matches), state, i, x);
    if(matches.size() > 2){
      return matches;
    }
  }

  return matches;
}

State ApplyMatch(State state,std::vector<int> match,std::string rule){
  auto y = rule.substr(rule.find("=")+1,rule.size());
  for(int indexInMatch = 0;indexInMatch < match.size();indexInMatch++){
    if(y[indexInMatch] == '*'){
      
    }else{
      state[match[indexInMatch]] = y[indexInMatch];
    }
  }
  return state;
}

State InitState(){
  auto state = std::vector<char>(100 * 100,'B'); 
  state[50 * 100 + 50] = 'R';
  return state;
}

void DrawState(const State& state){
  for(int index = 0;index < state.size();index++) {

    int x = index % 100;
    int y = index / 100;
    switch(state[index]){
      case 'W':
        DrawRectangle(x*8,y*8,8,8,WHITE);
        break;
      case 'A':
        DrawRectangle(x*8,y*8,8,8,GRAY);
        break;
      case 'R':
        DrawRectangle(x*8,y*8,8,8,RED);
        break;
      case 'G':
        DrawRectangle(x*8,y*8,8,8,GREEN);
        break;
      case 'P':
        DrawRectangle(x*8,y*8,8,8,PURPLE);
        break;
      case 'U':
        DrawRectangle(x*8,y*8,8,8,BLUE);
        break;
      default:
        break;
    }
  }
}

std::tuple<std::vector<int>,int> GetRandomMatch(const std::vector<std::vector<int>>& matches){
  auto index = rand() % matches.size();
  return {matches[index],index};
}

int main(void){

  // Initialization
  //--------------------------------------------------------------------------------------
  const int gridSize = 8;
  const int screenWidth = gridSize * 100;
  const int screenHeight =gridSize * 100;

  InitWindow(screenWidth, screenWidth, "raylib [core] example - basic window");

  SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
  const std::vector<std::string> rules = {
    "RBB=**R",
    "RBRBRB=WWWWWWW"
  };

  int current_rule = 0;
  std::string rule = rules[current_rule];


  
  auto state = InitState();
  auto matches = FindAllMatches(state, rule);
  std::vector<int> randomMatch;
  int turn = 0;

  while (!WindowShouldClose())    // Detect window close button or ESC key
  {
    BeginDrawing();

    ClearBackground(BLACK);

    DrawState(state);
    
    if(current_rule < rules.size()){
      do{
        //take the first rule
        rule = rules[current_rule];

        //Find all matches
        matches = FindAllMatches(state,rule);

        //If we reloop look at the next rule
        current_rule++;

        //If we have looked at all the rules stop
        if(current_rule >= rules.size()){
          break;
        }
      }while(matches.size() == 0);

      if(matches.size() > 0){
        auto [randomMatchTemp,index] = GetRandomMatch(matches);
        randomMatch = std::move(randomMatchTemp);
        matches.erase(matches.begin()+index);
        state = ApplyMatch(state,randomMatch,rule);

        //If we apply a rule continue
        current_rule = 0;
      }
    }


    DrawFPS(10,10);
    EndDrawing();
    //----------------------------------------------------------------------------------
  }

  // De-Initialization
  //--------------------------------------------------------------------------------------
  CloseWindow();

}
