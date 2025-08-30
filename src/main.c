#include <stdio.h>

#include "../../raylib/src/raylib.h"

#define SCREEN_WIDTH 450
#define SCREEN_HEIGHT 800

#define DECISION_WIDTH 100
#define DECISION_HEIGHT 180

#define SMALL_STAR_HEIGHT 20
#define SMALL_STAR_WIDTH 20

#define SMALL_MONEY_HEIGHT 25
#define SMALL_MONEY_WIDTH 25

#define DECISION_NUMBER 3

#define STORE_NUMBER 6

#define NO_CUTSCENE 0
#define POLICE_CUTSCENE 1
#define BUISNESS_CUTSCENE 2
#define BANKRUPT_CUTSCENE 3
#define INTRO_CUTSCENE 4





void playCutscenePolice()
{

    while (!WindowShouldClose())    // Detect window close button or ESC key
		{     

			BeginDrawing();

      ClearBackground(RAYWHITE);
			DrawText("You got caught py police", 10, 10, 50, DARKGRAY);
			EndDrawing();
		}     //  DrawTextureV(bg_texture, position, WHITE);
}

void playCutsceneBuisness()
{

    while (!WindowShouldClose())    // Detect window close button or ESC key
		{
			BeginDrawing();

			DrawText("You payed the debt", 10, 10, 50, DARKGRAY);
			EndDrawing();
		}
}

void playCutsceneBankrupt()
{

    while (!WindowShouldClose())    // Detect window close button or ESC key
		{
			BeginDrawing();

			DrawText("You gone bankrupt", 10, 10, 50, DARKGRAY);
			EndDrawing();
		}
}


typedef struct {

	Vector2 position;
	Rectangle box;
	//Texture2D texture; They are all same.
	size_t rating;
	size_t income;

} store;
typedef struct {
	Vector2 position;
	size_t rating;
	size_t income;

}init_Store_Opt;
store initStoreOpt(init_Store_Opt opt)
{
	store new;
	new.rating=opt.rating;
	new.income=opt.income;
	new.position = opt.position;
	Rectangle box = { new.position.x,new.position.y,50,50 };
	new.box = box;
	return new;
}


#define initStore(...) initStoreOpt((init_Store_Opt){__VA_ARGS__})

void drawStore(store s, Texture2D textureStore, Texture2D textureStar, Texture2D textureMoney)
{
	if (s.rating<1) {return;}
	DrawTextureV(textureStore, s.position, WHITE);

	for (size_t i=0;i<s.rating;++i)
	{
		Vector2 offset = {s.position.x-SMALL_STAR_WIDTH-5+i*SMALL_STAR_WIDTH,s.position.y+50}; 
	DrawTextureV(textureStar, offset, WHITE);
	}
	if (s.income>0) {
		Vector2 offset = {s.position.x+5,s.position.y-SMALL_MONEY_HEIGHT};
	DrawTextureV(textureMoney, offset, WHITE);
  char income[2] = "0";
	income[0]=s.income+'0';
	DrawText(income, offset.x+SMALL_MONEY_WIDTH,offset.y, SMALL_MONEY_HEIGHT, GREEN);

	}
}

size_t calcMoney(store stores[STORE_NUMBER]) {
	size_t money = 0;
	for (size_t i=0;i<STORE_NUMBER;++i)
	{
		money += stores[i].rating*stores[i].income;
	}
	return money;
}

typedef struct {
	Vector2 position;
	bool grabbed;

	bool sus;
	bool star;
	bool money;


	Rectangle box;
	Texture2D texture;
	Vector2 initPosition;


} decision;
typedef struct {
	bool sus;
	bool star;
	bool money;
	Vector2 initPosition;
} init_Decision_Opt;

void moveDecision (decision* d, Vector2 pos)
{
	d->position=pos;
	d->box.x = pos.x;
	d->box.y = pos.y;
}
void moveDecisionHome (decision* d)
{
	moveDecision(d,d->initPosition);	
}

Texture2D getDecisionTexture(init_Decision_Opt opt) {
	Image im_decision;
	if (opt.sus) {
			im_decision= LoadImage("assets/sus_decision.png"); 
	}
	else {
			im_decision= LoadImage("assets/ok_decision.png"); 
	}
  Rectangle starBox = { (float)(0), (float)(0), 50, 50 };
  Rectangle starPosBox = { (float)(48), (float)(4), 50, 50 };
  Image im_star; 

	if (opt.star) {
			im_star= LoadImage("assets/star.png"); 
	}
	else {
			im_star= LoadImage("assets/no_star.png"); 
	}
  ImageDraw(&im_decision, im_star, starBox, starPosBox, WHITE);

  Rectangle moneyBox = { (float)(0), (float)(0), 50, 50 };
  Rectangle moneyPosBox = { (float)(2), (float)(4), 50, 50 };
  Image im_money; 

	if (opt.money) {
			im_money= LoadImage("assets/money.png"); 
	}
	else {
			im_money= LoadImage("assets/no_money.png"); 
	}
  ImageDraw(&im_decision, im_money, moneyBox, moneyPosBox, WHITE);
    UnloadImage(im_money);   
    UnloadImage(im_star);   
    Texture2D texture = LoadTextureFromImage(im_decision);  
    UnloadImage(im_decision);   
	return texture;
}

decision initDecisionOpt (init_Decision_Opt opt) {
		decision new;  
		 Texture2D texture = getDecisionTexture(opt);
    Vector2 dec_position = opt.initPosition;
    Rectangle decisionBox = { dec_position.x, dec_position.y , DECISION_WIDTH, DECISION_HEIGHT };
		
		new.position = dec_position;
		new.initPosition = dec_position;
		new.box = decisionBox;
		new.texture = texture;

		new.grabbed=false;

		return new;

}
#define initDecision(...) initDecisionOpt((init_Decision_Opt){__VA_ARGS__})

void DrawArrayOf(decision decisions[DECISION_NUMBER]) {
 for (size_t i =0;i<DECISION_NUMBER;++i){
            DrawTextureV(decisions[i].texture, decisions[i].position, WHITE);
 }

}
void jsonstringifyorsmthing(char str_money[3],size_t money) {
	str_money[0]=(money/10)+'0';
	str_money[1]=(money%10)+'0';

}
bool apply_to_store(store* stores, decision d) {
	for (size_t i=0; i<STORE_NUMBER;++i)
	{
		if (stores[i].rating>0) {
		bool collision = CheckCollisionRecs(d.box, stores[i].box);
		Rectangle		boxCollision = {0,0,0,0}; 
		if (collision) boxCollision = GetCollisionRec(d.box, stores[i].box); 
		if (( (int)boxCollision.width*(int)boxCollision.height) > 2400) {
			if (d.star) 
			{
				if (stores[i].rating<5) stores[i].rating += 1;
			}
			else {
				if (stores[i].rating>0) stores[i].rating -= 1;
			}
			if (d.money) {
			if (stores[i].income<5) stores[i].income += 1;
			}
			else{
			if (stores[i].income>0) stores[i].income -= 1;

			}
			return true;}
		}
	}
	return false;
}

void reroll(decision* d, size_t seed, size_t seed2, size_t seed3) {
	d->star = (bool)(seed % 2);
	d->money = (bool)(seed2 % 2);
	d->sus = (bool)(seed3%2);
	init_Decision_Opt opt;
	opt.star = d->star;
	opt.money = d->money;
	opt.sus = d->sus ;

	opt.initPosition = d->initPosition;
  UnloadTexture(d->texture);
	d->texture = getDecisionTexture(opt); 
}
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
  
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib game");
    Image background = LoadImage("assets/background.png"); 
    Texture2D bg_texture = LoadTextureFromImage(background);  
    UnloadImage(background);   
	
    Image im_susmeter = LoadImage("assets/susmeter.png"); 
    Texture2D tex_susmeter = LoadTextureFromImage(im_susmeter);  
    UnloadImage(im_susmeter);   

    Image im_susmeterbg = LoadImage("assets/susmeterbg.png"); 
    Texture2D tex_susmeterbg = LoadTextureFromImage(im_susmeterbg);  
    UnloadImage(im_susmeterbg);   

    Image im_store = LoadImage("assets/store.png"); 
    Texture2D textureStore = LoadTextureFromImage(im_store);  
    UnloadImage(im_store);   
    Image im_star = LoadImage("assets/star.png"); 
    ImageResize(&im_star, SMALL_STAR_WIDTH, SMALL_STAR_HEIGHT);    
    Texture2D textureStar = LoadTextureFromImage(im_star);  
		UnloadImage(im_star);   
    Image im_money = LoadImage("assets/money.png"); 
    ImageResize(&im_money, SMALL_MONEY_WIDTH, SMALL_MONEY_HEIGHT);    
    Texture2D textureMoney = LoadTextureFromImage(im_money);  
    UnloadImage(im_money);   

    Vector2 position = { (float)(0), (float)(0) };
		Vector2 mousePosition = { -100.0f, -100.0f };
		Rectangle cursorBox ={ 30, 30, 10, 10 };
    SetTargetFPS(60);
    bool collisions[] = {false,false,false};
		Vector2 position0 = { (float)20, (float)SCREEN_HEIGHT-DECISION_HEIGHT-20  };
		Vector2 position1 = { (float)DECISION_WIDTH+40, (float)SCREEN_HEIGHT-DECISION_HEIGHT-20  };
		Vector2 position2 = { (float)DECISION_WIDTH*2+60, (float)SCREEN_HEIGHT-DECISION_HEIGHT-20  };
    decision decisions[] = {initDecision(.initPosition = position0),
			initDecision(.initPosition=position1),
			initDecision(.initPosition=position2) } ;
		Vector2 anchor={ (float)(0), (float)(0) } ;
		
		Rectangle moneyBoxbg = { SCREEN_WIDTH-50, 40, 50, SCREEN_HEIGHT-250 };
		Rectangle moneyBox = { SCREEN_WIDTH-50, 40, 50, SCREEN_HEIGHT-250 };

		Vector2 central = { (float)SCREEN_WIDTH/2,(float)SCREEN_HEIGHT/2 };
		Vector2 topleft = { (float)SCREEN_WIDTH/4-59,(float)SCREEN_HEIGHT/4+20 };
		Vector2 topleft2 = { (float)SCREEN_WIDTH/4+40,(float)SCREEN_HEIGHT/4-120 };
		Vector2 topright = { (float)SCREEN_WIDTH/2 + 40,(float)SCREEN_HEIGHT/4 };
		Vector2 botleft = { (float)SCREEN_WIDTH/4-45,(float)SCREEN_HEIGHT/2+55 };
		Vector2 botright = { (float)SCREEN_WIDTH/2 + 100,(float)SCREEN_HEIGHT/2+100 };
		store stores[STORE_NUMBER];
		size_t money=69;
		char money_str[3]="50";
		stores[0] = initStore(.income=5, .rating = 5, .position=central);
		stores[1] = initStore(.income=2, .rating = 2, .position=topleft);
		stores[2] = initStore(.income=2, .rating = 3, .position=topright);
		stores[3] = initStore(.income=3, .rating = 1, .position=botleft);
		stores[4] = initStore(.income=2, .rating = 4, .position=botright);
		stores[5] = initStore(.income=2, .rating = 3, .position=topleft2);
		size_t sus_meter =0;
		Vector2 susPos = { SCREEN_WIDTH-90, SCREEN_HEIGHT-200 };
		Rectangle susBox = { SCREEN_WIDTH-90, SCREEN_HEIGHT-200, 50, 200 };
		size_t cutscene = INTRO_CUTSCENE;
		// Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
			if (cutscene==INTRO_CUTSCENE)
			{
				cutscene = NO_CUTSCENE;
			}
			else if (cutscene == NO_CUTSCENE) {
				money = calcMoney(stores);
				if (money >= 90) {
					cutscene=BUISNESS_CUTSCENE;
				} 
				if (money <= 5) {
					cutscene=BANKRUPT_CUTSCENE;
				} 

				jsonstringifyorsmthing(money_str, money);
				moneyBox.y = (40 + (SCREEN_HEIGHT-250)) - (money * (SCREEN_HEIGHT-250)/100);
				moneyBox.height = (money * (SCREEN_HEIGHT-250)/100);

        mousePosition = GetMousePosition();
				cursorBox.x = GetMouseX() - cursorBox.width/2;
        cursorBox.y = GetMouseY() - cursorBox.height/2;
        collisions[0] = CheckCollisionRecs(decisions[0].box, cursorBox);
        collisions[1] = CheckCollisionRecs(decisions[1].box, cursorBox);
        collisions[2] = CheckCollisionRecs(decisions[2].box, cursorBox);

				if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) 
						&& !decisions[0].grabbed	&& !decisions[1].grabbed	&& !decisions[2].grabbed) {
					for (size_t i =0 ; i<DECISION_NUMBER ; ++i)
					{
						if (collisions[i]){
					anchor.x = - decisions[i].position.x + GetMousePosition().x;
					anchor.y= - decisions[i].position.y + GetMousePosition().y; 
					decisions[i].grabbed = true;
						}
					}
				};
					for (size_t i =0 ; i<DECISION_NUMBER ; ++i){
				if (decisions[i].grabbed) {
					Vector2 adjusted = { mousePosition.x-anchor.x,mousePosition.y-anchor.y };
					moveDecision(&decisions[i],adjusted );
          if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
						decisions[i].grabbed=false;
						
						if (apply_to_store(stores, decisions[i])) {
							if (decisions[i].sus) {
								sus_meter +=1;
								if (sus_meter>=10) {
									cutscene=POLICE_CUTSCENE;
								}
							}
							else {
								if (sus_meter>0) sus_meter -=1;
							}
							reroll(&decisions[i], (mousePosition.x + mousePosition.y) /2, mousePosition.x,mousePosition.y);
						}
						moveDecisionHome(&decisions[i]);
					}
				}
					}
				susBox.y = SCREEN_HEIGHT - (sus_meter * 200/10);
				susBox.height = (sus_meter * 200/10);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawTextureV(bg_texture, position, WHITE);
						DrawRectangleRec(moneyBoxbg, DARKGRAY);
						DrawRectangleRec(moneyBox, GREEN);
            DrawTextureV(tex_susmeterbg, susPos, WHITE);
						DrawRectangleRec(susBox, RED);
            DrawTextureV(tex_susmeter, susPos, WHITE);
            DrawText(money_str, moneyBoxbg.x-60, moneyBoxbg.y, 50, DARKGRAY);
						DrawArrayOf(decisions);
						for (size_t i=0 ; i< STORE_NUMBER;++i)
						{
							drawStore(stores[i], textureStore, textureStar, textureMoney);
						}

            //if (collision) DrawText("COLLISION", 20, 60, 20, RED);

        EndDrawing();
        //----------------------------------------------------------------------------------
			}
			else if (cutscene==POLICE_CUTSCENE) {
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawTextureV(bg_texture, position, WHITE);

        EndDrawing();


			}
			else if (cutscene==BANKRUPT_CUTSCENE) {
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawTextureV(bg_texture, position, WHITE);

        EndDrawing();
			}
			else if (cutscene==BUISNESS_CUTSCENE) {
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawTextureV(bg_texture, position, WHITE);

        EndDrawing();
			}

		}

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

