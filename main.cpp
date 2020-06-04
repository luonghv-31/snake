#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>
#include<cstring>
#include<vector>
#include<iostream>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_mixer.h>

using namespace std;

const int WIDTH = 450;
const int HEIGHT= 450;
const string font_path="Caviar_Dreams_Bold.ttf";
const string SOUND ="eat.wav";

int FPS = 10;
int DELAY_time = 1000.0 / FPS;
enum KEY {
    _LEFT_ = 0,
    _RIGHT_ = 1,
    _DOWN_ = 2,
    _UP_ = 3,
    _QUIT_ = 4
};
class chunkeffect
{
  private:
     Mix_Chunk *chunk;
     string filename;
  public:
   void init(string filename) //khoi tao am thanh
   {
  	Mix_OpenAudio(44150, MIX_DEFAULT_FORMAT, 2, 2048);//khoi tao am thanh
	chunk = Mix_LoadWAV(filename.c_str());
	this->filename = filename;
	if (chunk == NULL)
	{
		cout << Mix_GetError() << endl;
	}
   }
   void playchunk() // chay am thanh
   {
      Mix_VolumeChunk(chunk, 128);//chay am thanh
	Mix_PlayChannel(-1, chunk, 0);
   }
};
class key_input // ham kiem tra phim nhap
{
private:
    vector<bool> key_button;
    int x = 0;
    int y = 0;
public:
  int  total;

    key_input()
    {
        for (int i = 0; i < 5; i++)
        {
            key_button.push_back(false);
        }
    }
    bool getkey_button(int index)
    {
        return key_button[index];
    }
    void setkey_button(int index)
    {
        key_button[index] = false;
    }
    void update() // cap nhat su kien bat phim
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                key_button[_QUIT_] = true;
            }
            if (e.type == SDL_KEYDOWN)
            {
                switch (e.key.keysym.sym)
                {
                case SDLK_LEFT:
                {
                    key_button[_LEFT_] = true;
                    break;
                }case SDLK_RIGHT:
                {
                    key_button[_RIGHT_] = true;
                    break;
                }case SDLK_DOWN:
                {
                    key_button[_DOWN_] = true;
                    break;
                }case SDLK_UP:
                {
                    key_button[_UP_] = true;
                    break;
                }
                }
            }
            if (e.type == SDL_KEYUP)
            {
                switch (e.key.keysym.sym)
                {
                case SDLK_LEFT:
                {
                    key_button[_LEFT_] = false;
                    break;
                }case SDLK_RIGHT:
                {
                    key_button[_RIGHT_] = false;
                    break;
                }case SDLK_DOWN:
                {
                    key_button[_DOWN_] = false;
                    break;
                }case SDLK_UP:
                {
                    key_button[_UP_] = false;
                    break;
                }
                }
            }
        }
    }
};
class fruit //ve fruit
{
public:
    SDL_Rect fruit_rect;
    fruit()
    {
        fruit_rect.w = 15;
        fruit_rect.h = 15;
        fruit_rect.x = rand() % (WIDTH-30)/15*15;
        fruit_rect.y = rand() % (HEIGHT-30)/15*15;

      //  fruit_rect.x = 10 * (rand() % 10);
         //fruit_rect.y = 10* (rand() % 10);
        //cout<<""<<fruit_rect.x<<endl;
       // cout<<"fruit x:"<<fruit_rect.x<<endl<<"fruit y:"<<fruit_rect.y<<endl;

    }
    void drawfruit(SDL_Renderer* render)// fill all hinh fruit
    {
        //SDL_SetRenderDrawColor(render, 255, 0, 0, 0);
        SDL_RenderFillRect(render, &fruit_rect);
    }
};
class snake
{
private:
    int vecX, vecY;
    SDL_Rect rect;
    vector<SDL_Rect>tail; // duoi ran
    fruit _fruit;
    bool check = true;
    int total;
   chunkeffect eat;
public:
    snake() //khoi tao con ran
    {
        eat.init(SOUND);
        total = 0;
        vecY = 0;
        vecX = 15;
        rect.x = 0;
        rect.y = 0;
        rect.w = 15;
        rect.h = 15;
    }
    void draw(SDL_Renderer* render) // ve ran
    {
        SDL_SetRenderDrawColor(render, 255, 0, 150, 0);
        _fruit.drawfruit(render);
        SDL_SetRenderDrawColor(render, 0, 0, 90, 0);
        rect.x = (rect.x + WIDTH) % WIDTH;
        rect.y = (rect.y + (HEIGHT-30)) % (HEIGHT-30);
        SDL_RenderFillRect(render, &rect);
         int blue =120;
        for (int i = 0; i < tail.size(); i++)
        {
           blue+=i*5;
           if(blue>254)
           {
              blue=254;
           }
            SDL_SetRenderDrawColor(render, 0, 0,blue,0);
            SDL_RenderFillRect(render, &tail[i]);

        }

    }
    bool checkcollision() //kiem tra toa do cua ran
    {
        for (int i = 0; i < tail.size(); i++)
        {
            if (rect.x == tail[i].x && rect.y == tail[i].y)
            {
                check = false;
                return check;
            }
        }
        check = true;
        return check;
    }
    void update(key_input& a) // cap nhat ran
    {
        if (check == true)
        {
            if (tail.size() >= 1)
            {
                if (tail.size() > 0)
                {
                    for (int i = tail.size()-1; i >= 0; i--)
                    {
                        tail[i] = tail[i -1];
                    }
                }
                tail[0] = rect;
            }
            if (a.getkey_button(_RIGHT_) == true)
            {
                if (vecX != -15)
                {
                    vecX = 15;
                    vecY = 0;
                }
            }
            else if (a.getkey_button(_LEFT_) == true)
            {
                if (vecX != 15)
                {
                    vecX = -15;
                    vecY = 0;
                }
            }
            if (a.getkey_button(_UP_) == true)
            {
                if (vecY != 15)
                {
                    vecX = 0;
                    vecY = -15;
                }
            }
            if (a.getkey_button(_DOWN_) == true)
            {
                if (vecY != -15)
                {
                    vecX = 0;
                    vecY = 15;
                }
            }

            rect.x += vecX;
            rect.y += vecY;
            checkcollision();
            spanTail();
        }
        else {
            vecX = 0;
            vecY = 0;
        }
    }
    void spanTail() // xu li va cham va tang do dai duoi
{
     if (rect.x + 7 >= _fruit.fruit_rect.x
                    && rect.x + 7 <= _fruit.fruit_rect.x + 15
                    && rect.y+1>= _fruit.fruit_rect.y + 0
                    && rect.y+1<= _fruit.fruit_rect.y + 15)
                {
                   eat.playchunk();
                    tail.push_back(rect);
                    _fruit.fruit_rect.x = rand() % WIDTH/15*15;
                    _fruit.fruit_rect.y = rand() % (HEIGHT-30)/15*15;
                    total++;
                }
}
    int getTotal()//diem
    {
        return total*10;
    }
    bool getcheck()
    {
       return check;
    }
};


SDL_Texture*initscore(string a,SDL_Renderer*renderer,int size) // hien thi diem
{
      TTF_Init();
      TTF_Font *font = TTF_OpenFont(font_path.c_str(), size);
      SDL_Color color={255, 0, 0, 0};
      SDL_Surface *surface= TTF_RenderText_Solid(font,a.c_str(), color);//chuyen tu string -> cstr
      SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);//tao texture de ve diem
      return texture;

}
void drawtext(SDL_Renderer*renderer,SDL_Texture*text,int x,int y) //ham khoi tao chu
{
      int textW;
      int textH;
      SDL_QueryTexture(text, NULL, NULL, &textW, &textH);
      SDL_Rect dstrect = { x, y, textW, textH };
      SDL_RenderCopy(renderer, text, NULL, &dstrect);
}
SDL_Texture *backg(SDL_Renderer*renderer)
{
     SDL_Surface *tempSurface=IMG_Load("background.jpg");
         if (tempSurface == 0)
         {
            cout << "No image in folder " << endl;
            return NULL;
         }
	 SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, tempSurface);
	 SDL_FreeSurface(tempSurface);
	 tempSurface = NULL;
	 return text;
}
int main(int argv, char** argc)
{
    srand(time(0));
    snake a; key_input b;
    SDL_Window* window = SDL_CreateWindow("snake", SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          WIDTH, HEIGHT, SDL_WINDOW_SHOWN);// khoi tao cua so

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);// rendered cua so len man hinh
    bool running = true;
    bool gameover=true;
    SDL_Event *event;
    int framestart, frametime;
    SDL_Texture*background=backg(renderer);
    SDL_Texture*score=NULL;
    SDL_Texture*namescore=NULL;
    SDL_Texture*gameov=NULL;
    namescore= initscore("Score:",renderer,20);
    gameov=initscore("GAMEOVER",renderer,60);
    int scorebefore,scoreafter;
    score=initscore("0",renderer,20);
    SDL_Rect boder;
    boder.x=0;
    boder.y=0;
    boder.w=450;
    boder.h=420;
    while (running)
    {

        scorebefore=a.getTotal();//diem truoc khi update
        framestart = SDL_GetTicks();//lay tong so mili giay tu khi sdl khoi tao
        b.update();
        if (b.getkey_button(_QUIT_) == true)
        {
            running = false;
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        if(a.getcheck()==true)
        {
        SDL_RenderCopy(renderer,background,0,0);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
        SDL_RenderDrawRect(renderer,&boder);

        a.draw(renderer);

        drawtext(renderer,namescore,0,430); //ve chu Score
        drawtext(renderer,score,80,430); //ve diem
        a.update(b);
        }
        else{
         if(gameover==true)
         {
           score=initscore(to_string(scoreafter),renderer,60);//set size font to len luc voi
           gameover=false;
         }
         SDL_RenderCopy(renderer,background,0,0);
         drawtext(renderer,score,200,000);
         drawtext(renderer,gameov,50,200);
        }
        SDL_RenderPresent(renderer);
        scoreafter=a.getTotal();
      if(scorebefore!=scoreafter)
      {
          score=initscore(to_string(scoreafter),renderer,20);//cap nhat score
      }
        frametime = SDL_GetTicks() - framestart; //
        if (frametime < DELAY_time)
        {
            SDL_Delay(DELAY_time - frametime);
        }

    }
    TTF_Quit();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    return 0;


}

