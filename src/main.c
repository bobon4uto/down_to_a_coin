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
#define DISCLAIMER_CUTSCENE 5





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

#define CH_WHITE 0
#define CH_WHITE_WB 1
#define CH_BLACK_MOUSTAGE 2
#define CH_BLUE_POLICE 3

typedef struct {

	size_t characters;
	char text[255];

} cutscene_frame;

#define INTRO_FRAME_NUMBER 22
void DrawDialogue( cutscene_frame* intro_cutscene,size_t frame, Texture2D w, Texture2D bm, Texture2D bp) {
	Vector2 position = {0,0};
	Vector2 positionr = {50,0};
	Vector2 positionl = {-50,0};
  Vector2 textpos = {100,320};
	Color textColor = WHITE;
	switch (intro_cutscene[frame].characters	) {
	case CH_WHITE: 
            DrawTextureV(w, position, WHITE);
            DrawTextureV(bm, positionr, WHITE);
						textpos.x=100;
						textColor=WHITE;
						break;
	case CH_WHITE_WB: 
            DrawTextureV(w, position, WHITE);
            DrawTextureV(bp, positionr, WHITE);
						textpos.x=100;
						textColor=WHITE;
						break;
	case CH_BLACK_MOUSTAGE: 
            DrawTextureV(w, positionl, WHITE);
            DrawTextureV(bm, position, WHITE);
						textpos.x=100;
						textColor=BLACK;
						break;
	case CH_BLUE_POLICE: 
            DrawTextureV(w, positionl, WHITE);
            DrawTextureV(bp, position, WHITE);
						textpos.x=100;
						textColor=DARKBLUE;
						break;
	default : 
            DrawTextureV(w, position, WHITE);

	}

  DrawText(intro_cutscene[frame].text, textpos.x, textpos.y, 20, textColor);
	
}

void playSlide(Sound slides[3],int seed)
{
	if (seed%2) {
	PlaySound(slides[0]);
	} else if (seed%3==0) {
	PlaySound(slides[1]);
	} else {
	PlaySound(slides[2]);
	}
	

}

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
  
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib game");
  
    InitAudioDevice();              // Initialize audio device


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
		size_t cutscene = DISCLAIMER_CUTSCENE;
    
		Image im_white = LoadImage("assets/white.png"); 
    Texture2D tex_white = LoadTextureFromImage(im_white);  
    UnloadImage(im_white);

		Image im_black_moustage = LoadImage("assets/black_moustage.png"); 
    Texture2D tex_black_moustage = LoadTextureFromImage(im_black_moustage);  
    UnloadImage(im_black_moustage);

		Image im_blue_police = LoadImage("assets/blue_police.png"); 
    Texture2D tex_blue_police = LoadTextureFromImage(im_blue_police);  
    UnloadImage(im_blue_police);
		cutscene_frame intro_cutscene[] = 
		{
			{5,"Oh my god,\n I'm getting\na promotion to CEO???"},
			{CH_BLACK_MOUSTAGE, "yes.\nAnd You seem very exited about it.\nDidn't you read my letter?\nOr do I have to\nexplain everything?"},
			{CH_WHITE, "Explain, please.\n\n\nI know everything already."},
			{CH_BLACK_MOUSTAGE, "So our company is in a\nHuge debt.\nWe barely have\nhalf the money\nto pay it."},
			{CH_WHITE, "Yes, I know, but\nI will do my best to-"},
			{CH_BLACK_MOUSTAGE, "Shhh... Let me finish.\nBut this situation\nis exactly what I planned."},
			{CH_WHITE, "What?"},
			{CH_BLACK_MOUSTAGE, "Haven't you realized?\nI'm going to drive\nthis business\nto the very bottom,\nacquiring bankruptcy\nand getting all the\nmoney leftovers\n(which is a lot)\nto myself."}, 
			{CH_WHITE, "But, Isn't it illegal?"},
			{CH_BLACK_MOUSTAGE, "That's why you\nwill do the dirty work.\nIf something goes south\n- You will go to prison,\nnot me!"},
			{CH_WHITE, "And what if\nI tell the police\nright now?"},
			{CH_BLACK_MOUSTAGE, "Then they will\nalso hear a word\nor two\nabout the way\nyou got passport\nin this city."},
			{CH_WHITE, "..."},
			{CH_BLACK_MOUSTAGE, "Look, you don't\nreally\nhave an option,\nso listen carefully\nhow to do your job\nright."}, 
			{CH_BLACK_MOUSTAGE, "At the bottom of\nthe screen you have\n\"decisions\",\nthey have different effects\n- raise or drop\nprices in a shop,\nincrease or\ndecrease rating.\nAlso, there are\nsuspicious decisions,\nwhich are tinted RED. "},
			{CH_BLACK_MOUSTAGE,	"Drag decision onto\na shop to affect it.\nShops that have\nno stars are\ndestroyed by government .\nShops without money\nsymbol are giving\naway everything\nfor free,\nso both methods work.\nShop income is\ncalculated by\nmultiplying cost\nby number of stars. "},
			{CH_BLACK_MOUSTAGE, "If you played\nsuspicious decision -\nmeter on the bottom-right\ncorner will rise.\nIf you play too many\nsuspicious decisions,\npolice will catch you.\nDoing normal stuff\ncalms down the police."}, 
{CH_BLACK_MOUSTAGE, "Your final goal\nis to drop\nthe money meter\nto 5 or lower.\nThen our company\nwill go bankrupt,\nand I win.\nDon't even\nthink\nabout getting\nmore money,\neven I cold never\nreach 90 Fierce!\n(made up currency in\nthe game world,\nreaching 90 does\ngive different end"},

{CH_BLACK_MOUSTAGE, "That's it.\nDo I need to\nrepeat the rules?\n(They are also\nin the description\nof the game.)"},

{CH_WHITE, "Repeat, please.\n\n\nNo, I remember the rules."},
{CH_BLACK_MOUSTAGE, "Good, but not\nlike I expect\nanything from you."},
		};
#define POLICE_FRAME_NUMBER	8
cutscene_frame policeCutscene[] = {
	{CH_BLUE_POLICE, "HANDS IN THE AIR!"},
	{CH_WHITE_WB, "Ahh! whats hapening?"},
	{CH_BLUE_POLICE, "You're being arrested."},
	{CH_WHITE_WB, "But why?!"},
	{CH_BLUE_POLICE, "Don't act innocent,\nwe have a lot\nof evidence\nagainst you."},
	{CH_WHITE_WB, "I need a lawyer."},
	{CH_BLUE_POLICE, "You sure need one.\nBut you wont be getting\na lawyer with\nthis money youve\nbeen stealing from\nyour company."},
	{CH_WHITE_WB, "BAD END:\nyou got arrested."}




	};


#define BANKRUPT_FRAME_NUMBER	19
cutscene_frame bankruptCutscene[] = {
{CH_WHITE, "* after ravaging\nthe company,\nyou ended up\nwith a lot of money,\nand made sure\nto put it to\n'good' use. *"},
{CH_BLACK_MOUSTAGE, "You did a\nsplendid job!"},
{CH_WHITE, "Sure.\nBut what are\nyou doing here?"},
{CH_BLACK_MOUSTAGE, "I'm here\nfor my money."},
{CH_WHITE, "But I was the CEO."},
{CH_BLACK_MOUSTAGE, "Don't make me laugh!\nYou know it's\nnot about\nwho's the boss\non papers."}, 
{CH_WHITE, "I know.\nThat's why\nI'm telling you\nto get out\nof here before\nit's too late."},
{CH_BLACK_MOUSTAGE, "Alright,\nI knew you would\ndo something like this.\nSo I asked some folk\nfrom local mafia\nto help me.\nBoys, teach her\nsome manners."},
{CH_WHITE, "* but nobody moved *"},
{CH_BLACK_MOUSTAGE, "What's wrong guys??"},
{CH_WHITE, "They just know\nnot to attack\ntheir own\nmafia boss."},
{CH_WHITE_WB, "* some time later *"},
{CH_BLUE_POLICE, "We\n..\nUh...\nInvestigating\nBlack Moustage\ndissapearence.\nAnd..."}, 
{CH_WHITE_WB, "Be my guest."}, 
{CH_BLUE_POLICE, "* backs off *"},
{CH_WHITE_WB, "why did you,\nFreier,\nback off?"},
{CH_BLUE_POLICE, "well,\nwe...\nUhm..\nMaybe we will\ncome back later."},
{CH_WHITE_WB, "* they never returned. *"},
{5, "THE END:\nYou did what\nyour boss\nasked you\nto do."},
};

#define BUISNESS_FRAME_NUMBER	10
cutscene_frame businessCutscene[] = {


{CH_WHITE_WB, "* You managed\nto pay off\ncompany's debt\nand now it's\nthe biggest supermarket\ncompany in the world. *"},
{CH_WHITE_WB, "I'm glad\nI didn't listen\nto him,\nthank you\nfor your help!"},
{CH_BLUE_POLICE, "No worries, miss.\nBut you could've\nasked for help\nright away - that\nforged passport of yours\nisn't as big of a deal\nas a fall of the\nbest stores\nin the city!"},
{CH_WHITE_WB, "But then\nYou would have\ndeparted me..."},
{CH_BLUE_POLICE, "No, why?\nIn the end\nyou had to pay\na fine, and make\na real passport,\nit would've been\nthe same then."},
{CH_WHITE_WB, "I see.\nI still worry\nthat he might return."},
{CH_BLUE_POLICE, "What?\nHe won't touch\nour city's soil\nin a million years!\nIf he dares to step,\nwe will catch\nhim in no time!"},
{CH_WHITE_WB," Thank you. "},
{CH_BLUE_POLICE, "No,\nI have to thank you,\nmiss White Tail."},
{5, "THE END:\nYou did the opposite\nof what you were\ntold to do.\nIt still worked\nout well."},
};






		size_t frame = 0;
		bool press=false;
		Rectangle noBox={90,310,170,40};
		Rectangle yesBox={90,375,290,40};
		bool choose = false;


		Sound slides[3];
		slides[0]=LoadSound("assets/slide_decision1.wav");
		slides[1]=LoadSound("assets/slide_decision2.wav");
		slides[2]=LoadSound("assets/slide_decision3.wav");
		Sound moneys[3];
		moneys[0]=LoadSound("assets/money_talk1.wav");
		moneys[1]=LoadSound("assets/money_talk2.wav");
		moneys[2]=LoadSound("assets/money_talk3.wav");

		// Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
			if (cutscene==DISCLAIMER_CUTSCENE) {
				
				if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) press=true; 
          if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
						if (press) {	cutscene=INTRO_CUTSCENE;	}	
					press = false;
					} 
				
        BeginDrawing();

            ClearBackground(BLACK);
            DrawText("DISCLAIMER:\nTHIS GAME\nDOES NOT\nPROVIDE\nANY FINANCIAL\nOR\nLEGAL ADVICE\nALL EVENTS\nARE MADE UP.\nclick to continue.", SCREEN_WIDTH/6, SCREEN_HEIGHT/3, 30, RED);


        EndDrawing();
			}
			else if (cutscene==INTRO_CUTSCENE)
			{
				if (frame == 2 || frame == 19) {choose = true;}	else {choose=false;}
				if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) press=true; 
          if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
					if (choose) {
        mousePosition = GetMousePosition();
						if ( mousePosition.x >noBox.x && mousePosition.x < noBox.x+noBox.width 
						&& mousePosition.y > noBox.y && mousePosition.y < noBox.y+noBox.height ) {
									frame=3;
									press=false;

						}
						if ( mousePosition.x >yesBox.x && mousePosition.x < yesBox.x+yesBox.width 
						&& mousePosition.y > yesBox.y && mousePosition.y < yesBox.y+yesBox.height ) {
								frame=20;
								press=false;
						}
					}
					else {
						if (press) {	++frame;	}	
					press = false;
					}
					} 
					
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawTextureV(bg_texture, position, WHITE);
						if (choose) {
							DrawRectangleRec(yesBox, RED);
							DrawRectangleRec(noBox, RED);
						}
						DrawDialogue(intro_cutscene,frame, tex_white,tex_black_moustage,tex_blue_police);

        EndDrawing();
				if (frame>= INTRO_FRAME_NUMBER-1) cutscene = NO_CUTSCENE;
			}
			else if (cutscene == NO_CUTSCENE) {
				frame=0;
				press =false;
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
					playSlide(slides, mousePosition.y);
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
					playSlide(moneys, mousePosition.x);
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
				if (!(frame>= POLICE_FRAME_NUMBER-1) ) {
				if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) press=true; 
          if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
						if (press) {	++frame;	}	
					press = false;
					} 
				}
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawTextureV(bg_texture, position, WHITE);
						DrawDialogue(policeCutscene,frame, tex_white,tex_black_moustage,tex_blue_police);

        EndDrawing();


			}
			else if (cutscene==BANKRUPT_CUTSCENE) {
				if (!(frame>= BANKRUPT_FRAME_NUMBER-1) ) {
				if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) press=true; 
          if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
						if (press) {	++frame;	}	
					press = false;
					} 
				}
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawTextureV(bg_texture, position, WHITE);
						DrawDialogue(bankruptCutscene,frame, tex_white,tex_black_moustage,tex_blue_police);

        EndDrawing();
			}
			else if (cutscene==BUISNESS_CUTSCENE) {
				if (!(frame>= BUISNESS_FRAME_NUMBER-1) ) {
				if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) press=true; 
          if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
						if (press) {	++frame;	}	
					press = false;
					} 
				}
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawTextureV(bg_texture, position, WHITE);
						DrawDialogue(businessCutscene,frame, tex_white,tex_black_moustage,tex_blue_police);

        EndDrawing();
			}

		}

    // De-Initialization
    //--------------------------------------------------------------------------------------


    CloseAudioDevice(); 
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

