#include "raylib.h"
#include "raymath.h"
#include "SPHSolver.h"
#include "CacheWriter.h"
#include <algorithm>
#include <array>
#include <string>

namespace {
constexpr Color kBg{20,22,26,255}, kPanel{31,34,39,255}, kPanel2{40,43,49,255};
constexpr Color kLine{60,64,72,255}, kText{218,221,225,255}, kMuted{139,145,153,255};
constexpr Color kAccent{37,151,224,255};

bool Button(Rectangle r,const char* label,bool active=false){
    Vector2 m=GetMousePosition();bool hover=CheckCollisionPointRec(m,r);
    DrawRectangleRec(r,active?kAccent:(hover?Color{58,63,71,255}:kPanel2));
    DrawRectangleLinesEx(r,1,kLine);int w=MeasureText(label,16);
    DrawText(label,(int)(r.x+(r.width-w)/2),(int)(r.y+(r.height-16)/2),16,kText);
    return hover&&IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}
void Label(const char* text,int x,int y,int size=16,Color c=kText){DrawText(text,x,y,size,c);}
void Field(const char* name,const std::string& value,int x,int y,int width){
    Label(name,x,y+5,15,kMuted);Rectangle r{(float)x+92,(float)y,(float)width-92,27};
    DrawRectangleRec(r,{25,27,31,255});DrawRectangleLinesEx(r,1,kLine);Label(value.c_str(),(int)r.x+8,y+5,15);
}
}

int main(){
    SetConfigFlags(FLAG_MSAA_4X_HINT|FLAG_WINDOW_RESIZABLE);InitWindow(1400,850,"FluidLab - SPH Fluid Simulator");SetTargetFPS(60);
    Camera3D camera{{8,6,9},{0,2,0},{0,1,0},45,CAMERA_PERSPECTIVE};SPHSolver solver;
    bool running=false,record=false;int frame=0;float accumulator=0;int selected=1;
    while(!WindowShouldClose()){
        const int sw=GetScreenWidth(),sh=GetScreenHeight();const int top=66,left=245,right=300,bottom=118;
        Rectangle viewport{(float)left,(float)top,(float)(sw-left-right),(float)(sh-top-bottom)};
        if(CheckCollisionPointRec(GetMousePosition(),viewport))UpdateCamera(&camera,CAMERA_ORBITAL);
        if(IsKeyPressed(KEY_SPACE))running=!running;if(IsKeyPressed(KEY_R)){solver.reset();frame=0;}if(IsKeyPressed(KEY_E))record=!record;
        if(IsKeyPressed(KEY_J))solver.emitJet({-2.7f,3.7f,0},{7,0,0},4);
        solver.settings().viscosity=std::clamp(solver.settings().viscosity+(IsKeyDown(KEY_UP)?20.0f*GetFrameTime():0)-(IsKeyDown(KEY_DOWN)?20.0f*GetFrameTime():0),0.0f,80.0f);
        if(running||IsKeyPressed(KEY_N)){accumulator+=GetFrameTime();int substeps=0;while(accumulator>=0.005f&&substeps<8){solver.step(0.005f);accumulator-=0.005f;++substeps;}if(record)writePlyCache("cache",frame,solver.particles());++frame;}
        BeginDrawing();ClearBackground(kBg);
        DrawRectangle(0,0,sw,top,kPanel);DrawLine(0,top-1,sw,top-1,kLine);
        Label("FLUIDLAB",18,13,24,{78,181,241,255});Label("DYNAMICS",18,39,11,kMuted);
        Button({150,17,72,30},"File");Button({226,17,72,30},"Edit");Button({302,17,88,30},"Layout");
        Button({(float)sw-286,17,82,30},"Mesh");Button({(float)sw-198,17,82,30},"Export");Button({(float)sw-110,17,92,30},"Render");
        BeginScissorMode((int)viewport.x,(int)viewport.y,(int)viewport.width,(int)viewport.height);BeginMode3D(camera);
        DrawGrid(20,1.0f);DrawCubeWires({0,2.5f,0},6,5,4,{77,115,142,210});
        for(const auto& p:solver.particles()){float speed=std::min(length(p.velocity)/8.0f,1.0f);Color c=ColorLerp({45,120,255,230},{160,240,255,240},speed);DrawSphere({p.position.x,p.position.y,p.position.z},.075f,c);}EndMode3D();
        EndScissorMode();
        DrawRectangle(left,top,(int)viewport.width,32,{28,31,35,240});Label("Perspective",left+12,top+8,14,kMuted);
        Label("Shaded",left+(int)viewport.width-75,top+8,14,kMuted);

        DrawRectangle(0,top,left,sh-top-bottom,kPanel);DrawLine(left-1,top,left-1,sh-bottom,kLine);
        Label("SCENE",14,top+14,14,kMuted);Button({12,(float)top+39,104,28},"+ Emitter");Button({122,(float)top+39,108,28},"+ Daemon");
        std::array<const char*,5> nodes{"Scene","  Domain01","    Liquid01","  Gravity01","  Collider01"};
        for(int i=0;i<(int)nodes.size();++i){Rectangle r{8.0f,(float)top+82+i*34,229,31};if(selected==i)DrawRectangleRec(r,{43,83,111,255});
            if(CheckCollisionPointRec(GetMousePosition(),r)&&IsMouseButtonPressed(MOUSE_BUTTON_LEFT))selected=i;
            Label(nodes[i],18,(int)r.y+7,15,i==2?Color{94,187,242,255}:kText);}
        Label("COLLECTIONS",14,top+284,14,kMuted);Label("Fluid",24,top+315,15,kText);Label("Colliders",24,top+345,15,kText);

        DrawRectangle(sw-right,top,right,sh-top-bottom,kPanel);DrawLine(sw-right,top,sw-right,sh-bottom,kLine);
        Label("NODE PARAMETERS",sw-right+16,top+14,14,kMuted);Label(selected==2?"Liquid01":"Domain01",sw-right+16,top+42,21,kText);
        DrawLine(sw-right+12,top+75,sw-12,top+75,kLine);
        Field("Type","SPH Liquid",sw-right+16,top+92,right-32);Field("Particles",std::to_string(solver.particles().size()),sw-right+16,top+127,right-32);
        Field("Resolution","0.210",sw-right+16,top+162,right-32);Field("Density","1000 kg/m3",sw-right+16,top+197,right-32);
        Field("Viscosity",std::to_string((int)solver.settings().viscosity),sw-right+16,top+232,right-32);
        Label("SOLVER",sw-right+16,top+286,14,kMuted);Field("Method","WCSPH",sw-right+16,top+312,right-32);Field("Substeps","8",sw-right+16,top+347,right-32);
        Field("Time scale","1.000",sw-right+16,top+382,right-32);Button({(float)sw-right+16,(float)top+430,(float)right-32,31},"Create Liquid Mesh");

        DrawRectangle(0,sh-bottom,sw,bottom,kPanel);DrawLine(0,sh-bottom,sw,sh-bottom,kLine);
        int cx=sw/2;if(Button({(float)cx-98,(float)sh-bottom+12,38,32},"|<")){frame=0;solver.reset();}
        if(Button({(float)cx-54,(float)sh-bottom+12,38,32},"<")){running=false;solver.step(.005f);}
        if(Button({(float)cx-10,(float)sh-bottom+12,52,32},running?"II":">",running))running=!running;
        if(Button({(float)cx+48,(float)sh-bottom+12,38,32},">")){solver.step(.005f);++frame;}
        if(Button({(float)cx+92,(float)sh-bottom+12,38,32},">|"))frame=240;
        Label((std::string("Frame  ")+std::to_string(frame)).c_str(),20,sh-bottom+20,16,kText);
        Label("0",20,sh-bottom+66,13,kMuted);Label("240",sw-47,sh-bottom+66,13,kMuted);
        DrawRectangle(48,sh-bottom+69,sw-96,5,{58,61,68,255});float playX=48+(sw-96)*std::clamp(frame/240.0f,0.0f,1.0f);DrawRectangle((int)playX-2,sh-bottom+61,4,21,kAccent);
        if(Button({(float)sw-178,(float)sh-bottom+12,158,32},record?"CACHE: RECORDING":"CACHE: OFF",record))record=!record;
        DrawFPS(left+12,top+42);EndDrawing();
    }CloseWindow();return 0;
}
