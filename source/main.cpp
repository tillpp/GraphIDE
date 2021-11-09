#include <iostream>
#include <math.h>
#include "Application/Application.h"
#include "Application/FPSCounter.h"
#include "Application/GuiHandler.h"
#include "Util/Log.h"

int main()
{
  Log::info("Starting...");  
  Application *app = new Application("title", 1280, 720);
  app->registerFeature(new FPSCounter);


  struct BackgroundDisco
      : public ApplicationFeature
  {
    float i = 0;
    void init(Application*)override {}
    void draw(Application *app) {}
    void update(Application *app)
    {
	    glClearColor(0.1,0.1,0.3, 1);
      //glClearColor(0,pow(sin(i / 10),0.5) / 2 + 0.5, pow(sin(i / 10+M_PI),0.5) / 2 + 0.5, 1);
      i++;
    }
  };
  app->registerFeature(new BackgroundDisco);
  app->registerFeature(new GuiHandler);
  getchar();
}