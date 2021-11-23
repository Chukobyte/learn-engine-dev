# Initialize SDL

## GameEngine Class

The `GameEngine` class will tie the game loop and engine systems together.

```c++
#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include "game_engine_context.h"
#include <game_lib/utils/logger.h>

class GameEngine {
  private:
    GameEngineContext *engineContext = nullptr;
    Logger *logger = nullptr;

    void Initialize();

    void InitializeSDL();
  public:
    GameEngine();

    ~GameEngine();

    void ProcessInput();

    void Update();

    void Render();

    bool IsRunning() const;
};

#endif //GAME_ENGINE_H
```

```c++
#include "game_engine.h"

#include <SDL2/SDL.h>

GameEngine::GameEngine() :
        engineContext(GameEngineContext::GetInstance()),
        fpsCounter(FPSCounter::GetInstance()),
        logger(Logger::GetInstance()) {
    Initialize();
}

GameEngine::~GameEngine() {
    SDL_Quit();
    logger->Info("%s Engine has shut down!", engineContext->GetEngineName());
}

void GameEngine::Initialize() {
    InitializeSDL();
    logger->Info("%s Engine v%s", engineContext->GetEngineName(), engineContext->GetEngineVersion());
//    engineContext->SetRunning(true);
}

void GameEngine::InitializeSDL() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        logger->Error("Error on initializing SDL\n%s", SDL_GetError());
        return;
    }
}

void GameEngine::ProcessInput() {}

void GameEngine::Update() {
    // Sleep until FRAME_TARGET_TIME has elapsed since last frame
    const unsigned int MILLISECONDS_PER_TICK = 1000;
    const unsigned int TARGET_FPS = 60;
    static Uint32 lastFrameTime = 0;
    const unsigned int FRAME_TARGET_TIME = MILLISECONDS_PER_TICK / TARGET_FPS;
    unsigned int timeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - lastFrameTime);
    if (timeToWait > 0 && timeToWait <= FRAME_TARGET_TIME) {
        SDL_Delay(timeToWait);
    }

    lastFrameTime = SDL_GetTicks();
}

void GameEngine::Render() {}

bool GameEngine::IsRunning() const {
    return engineContext->IsRunning();
}
```

The constructor calls the `Initialize` function which initializes SDL via [SDL_Init](https://wiki.libsdl.org/SDL_Init).  We pass in `SDL_INIT_EVERYTHING` which initializes all SDL subsystems.  After initializing the engine we will set `GameEngineContext` running property to `true` in the next section, but for now we are commenting it out.  The destructor calls [SDL_Quit](https://wiki.libsdl.org/SDL_Quit) which shuts down all SDL subsystems.

The `Update` function will be used to update things such as physics, A.I., game state, etc...  We will want our game to run at a target frames per second and the current `TARGET_FPS` variable controls that.  `lastFrameTime` is used to keep track of what the frame time was the previous frame.  [SDL_Delay](https://wiki.libsdl.org/SDL_Delay) is used to not completely occupy the cpu during a game loop.  Experiment by commenting out [SDL_Delay](https://wiki.libsdl.org/SDL_Delay) and compare the cpu utilization with and without using it.

Last but not least the `IsRunning` function just returns the value of `GameEngineContext` running.

## Main Loop

Now that we have our `GameEngine` class defined, we can now test it to make sure everything works.

```cpp
#include "game_engine.h"

int main(int argv, char** args) {
    GameEngine gameEngine;

    while (gameEngine.IsRunning()) {
        gameEngine.ProcessInput();
        gameEngine.Update();
        gameEngine.Render();
    }

    return 0;
}
```

After executing the above the following two log statements should be printed to the terminal:

```
[INFO] Simple Engine v0.1.0
[INFO] Simple Engine has shut down!
```

## Measuring FPS

Last but not least, we should check performance to ensure we are getting the frames per second that we are expecting.  We will first start by defining a `Timer` class that will be used by the `FPSCounter` and other classes.

### Timer

The `Timer` class is the straight forward.

```c++
#ifndef TIMER_H
#define TIMER_H

#include <SDL2/SDL.h>

class Timer {
  private:
    Uint32 waitTime;
    bool loops;
    Uint32 startTicks;
    Uint32 pausedTicks;
    bool isPaused;
    bool hasStarted;

  public:
    Timer(Uint32 waitTimeInMilliseconds, bool loops = false);
    Uint32 GetWaitTime() const;
    Uint32 GetTimeLeft() const;
    void SetWaitTime(Uint32 waitTimeInMilliseconds);
    bool DoesLoop() const;
    void SetLoop(bool loops);
    void Start();
    void Stop();
    void Pause();
    void UnPause();
    bool HasStopped() const;
    bool HasReachedTimeOut() const;
    Uint32 GetTicks() const;
    bool HasStarted() const;
    bool IsPaused() const;
};

#endif
```

```c++
#include "timer.h"

#include <iostream>

Timer::Timer(Uint32 waitTimeInMilliseconds, bool loops) {
    this->waitTime = waitTimeInMilliseconds;
    this->loops = loops;
    startTicks = 0;
    pausedTicks = 0;
    isPaused = false;
    hasStarted = false;
}

Uint32 Timer::GetWaitTime() const {
    return waitTime;
}

Uint32 Timer::GetTimeLeft() const {
    if(!hasStarted) {
        return 0;
    }
    Uint32 timeLeft = waitTime - (SDL_GetTicks() - startTicks);
    return timeLeft > 0 && timeLeft < waitTime ? timeLeft : 0;
}

void Timer::SetWaitTime(Uint32 waitTimeInMilliseconds) {
    waitTime = waitTimeInMilliseconds;
}

bool Timer::DoesLoop() const {
    return loops;
}

void Timer::SetLoop(bool loops) {
    this->loops = loops;
}

void Timer::Start() {
    hasStarted = true;
    isPaused = false;
    startTicks = SDL_GetTicks();
    pausedTicks = 0;
}

void Timer::Stop() {
    hasStarted = false;
    isPaused = false;
    startTicks = 0;
    pausedTicks = 0;
}

void Timer::Pause() {
    if(hasStarted && !isPaused) {
        isPaused = true;
        pausedTicks = SDL_GetTicks() - startTicks;
        startTicks = 0;
    }
}

void Timer::UnPause() {
    if(hasStarted && isPaused) {
        isPaused = false;
        startTicks = SDL_GetTicks() - pausedTicks;
        pausedTicks = 0;
    }
}

bool Timer::HasStopped() const {
    return !hasStarted && !isPaused;
}

bool Timer::HasReachedTimeOut() const {
    Uint32 time = GetTicks();
    if(time >= waitTime) {
        return true;
    }
    return false;
}

Uint32 Timer::GetTicks() const {
    Uint32 time = 0;
    if(hasStarted) {
        if(isPaused) {
            time = pausedTicks;
        } else {
            time = SDL_GetTicks() - startTicks;
        }
    }
    return time;
}

bool Timer::HasStarted() const {
    return hasStarted;
}

bool Timer::IsPaused() const {
    return isPaused && hasStarted;
}
```

`waitTime` refers to how long the timer will run for and `loops` determines if the timer restarts after timing out.  I think all the functions are self-explanatory and we will see the `Timer` class in use next!

### FPS Counter

```c++
#ifndef FPS_COUNTER_H
#define FPS_COUNTER_H

#include "./game_lib/utils/timer.h"

class FPSCounter {
  private:
    unsigned int fps;
    unsigned int fpsCount;
    Timer *timer = nullptr;

    FPSCounter();
  public:
    ~FPSCounter();

    static FPSCounter* GetInstance();

    void Update();

    unsigned int GetFPS() const;
};

#endif //FPS_COUNTER_H
```

```c++
#include "fps_counter.h"

#include "./game_lib/utils/logger.h"

FPSCounter::FPSCounter() : timer(new Timer(1000, true)) {
    timer->Start();
}

FPSCounter::~FPSCounter() {
    delete timer;
}

FPSCounter* FPSCounter::GetInstance() {
    static FPSCounter *instance = new FPSCounter();
    return instance;
}

void FPSCounter::Update() {
    if (timer->HasReachedTimeOut()) {
        timer->Stop();

        fps = fpsCount;
        fpsCount = 0;
        Logger::GetInstance()->Debug("fps = %d", fps);

        timer->Start();
    }
    fpsCount++;
}

unsigned int FPSCounter::GetFPS() const {
    return fps;
}
```

The first thing to note about the `FPSCounter` class is that it is also a singleton.  The `Update` function uses the timer to determine if a second has passed. Once a second has passed, we update the `fps` member variable and log the fps to the console temporarily as we'll use the `GetFPS` function to obtain the value in a future section.   Now that we have the FPS Counter class defined, let's use it within the `Update` function in our game loop.

```c++
void GameEngine::Update() {
    // Sleep until FRAME_TARGET_TIME has elapsed since last frame
    const unsigned int MILLISECONDS_PER_TICK = 1000;
    const unsigned int TARGET_FPS = 60;
    static Uint32 lastFrameTime = 0;
    const unsigned int FRAME_TARGET_TIME = MILLISECONDS_PER_TICK / TARGET_FPS;
    unsigned int timeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - lastFrameTime);
    if (timeToWait > 0 && timeToWait <= FRAME_TARGET_TIME) {
        SDL_Delay(timeToWait);
    }

    fpsCounter->Update();

    lastFrameTime = SDL_GetTicks();
}
```

Also in the `Initialize` function uncomment setting `GameEngineContext` running to `true`.

```c++
engineContext->SetRunning(true);
```

If we execute our game engine now, it should print to the terminal the fps of our game loop once per second.  All the source code for this chapter can be found [here](https://github.com/Chukobyte/learn-engine-dev/tree/main/src/2.game_loop_and_sdl/2.0.game_loop_and_sdl).  We have now finished setting up SDL2 and our game loop!  Next we'll tackle creating a renderer to draw things to the screen!
