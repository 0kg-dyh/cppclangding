#include"头文件/简化API/raylib/jianhuaapi.h"
#include "头文件/万能头文件/pch.h";
struct Isgamexy {
	float Gamex = 0.0f;
	float Gamey = 0.0f;
};
int main() {
	float plaer1 = 400, plaer2 = 400;
	std::unique_ptr<Isgamexy>p = std::make_unique<Isgamexy>();
	Guaywindow::Cjwindow(800, 800, "game");
	Camera2D camera = Guaysx::Cjsx({ plaer1,plaer2 });
	Guayfps::Setfps(120);
	int speed = 2;
	while (Guaywindow::Sfkqwindow) {
		if (IsKeyDown(KEY_W)) p->Gamey -= speed;
		if (IsKeyDown(KEY_S)) p->Gamey += speed;
		bool facingLeft = 0;
		if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
			p->Gamex -= speed;
			facingLeft = true;    // 往左走，朝左
		}
		if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
			p->Gamex += speed;
			facingLeft = false;   // 往右走，朝右
		}
		BeginDrawing();
		Guaysx::Begin(camera);
		    Guaybg::Tcbj("a.jpg",camera);
			float h = 100;
			float w = h * (500.0f / 776.0f);
		    Guaytp::Xstp("juese.png", p->Gamex, p->Gamey, w, h, facingLeft,1.0f);
		    Guaysx::Gensui(camera, p->Gamex, p->Gamey, 0.1f);
		Guaysx::End();
		
		EndDrawing();
	}
}