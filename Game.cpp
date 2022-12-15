//
// Created by kali on 2/28/22.
//

#include "Game.h"
#include <SDL2/SDL.h>
#include <iostream>
#include "arduino/controller.h"

void fatalError(std::string errorString)
{
    std::cout << errorString << std::endl;
    std::cout << "Enter any key to quit...";
    int tmp;
    std::cin >> tmp;
    SDL_Quit();
}

const Game* gameInstance;
arduino::Controller* pc;
Game::Game(int screenWidth, int screenHeight)
: window(nullptr)
, screenWidth(screenWidth)
, screenHeight(screenHeight)
, gameState(GameState::START)
{
    gameInstance = this;
    // for pi, port must be manually specified to be "/dev/ttyACM0"
    pc = new arduino::Controller("/dev/ttyACM0");
    int scoreMult = HIGH_SCORES;
    for(int& highScore : highScores) highScore = pow(10,--scoreMult);
}

// sigh...
Game::Rect::Rect(int x, int y, int w, int h) : SDL_Rect({
    gameInstance->tileLength() * (GRID_LEFT + x),
    gameInstance->tileLength() * (PADDING + y),
    gameInstance->tileLength() * w,
    gameInstance->tileLength() * h,
}){}

bool Shape::isInvalidPosition(int x,int y)
{
    return x < 0
        || y < 0
        || x >= Game::COLS
        || y >= Game::ROWS
        || !(gameInstance->grid[x][y] == Color());
}

Game::~Game()
{
    destructScene();
}

void Game::run() {
    initScene();
    gameLoop();
}

int curPiece = 0;
Shape* cyclePiece(int inc) {
    int N = Shape::T+1;
    curPiece = (curPiece + N + inc) % N;
    return new Shape(static_cast<Shape::Piece>(curPiece));
}

void Game::setCurShape(Shape *shape) {
    // todo it might be nice to make curShape a reference rather than a pointer, but it would require either a getter method that autocalls this one or more delicate handling of the curShape variable to ensure it never holds a null pointer.
    shape->setPos(0);
    shape->x = 5; // move roughly to the center of the grid
    if(shape->isInvalidState()) {
        // this is when the game terminates
        gameState = GameState::GAME_OVER;
        // todo in the future this should probably manually set lights to be off to remove the need to manually reset the tower lights.
        pc->playAnimation(arduino::Controller::Animation::FLATLINE);
        // record high score
        int toAdd = this->score;
        bool added = false;
        for(int &highScore : highScores) {
            // this is lazy since after the first substitution the next ones will all be shifted.
            if(toAdd > highScore) {
                added = true;
                int temp = highScore;
                highScore = toAdd;
                toAdd = temp;
            }
        }
        return; // terminate logic
    }
    pc->setKeyLights(shape->color);
    delete curShape;
    curShape = shape;
    // reset fall-related variables
    time = dropDelay();
    locked = false;
    fastFall = false;
}

bool holdUsed = false;
void Game::holdShape() {
    if(holdUsed) return;
    auto tmp = heldShape;
    heldShape = new Shape(curShape->piece);
    if(tmp == nullptr) {
        loadNewShape();
    } else {
        holdUsed = true;
        setCurShape(tmp);
    }
}

void Game::loadNewShape() {
    holdUsed = false;
    setCurShape(nxtShape);
    nxtShape = new Shape(bag.draw());
}

bool Game::moveCurShapeDown() {
    if(curShape->moveDown()) return true;
    placeShape();
    return false;
}

bool isLevelingUp = false;

void Game::incLevel() {
    //std::cout << "level: " << ++level << " (speed = " << DELAY/(time = dropDelay()) << "G/" << dropDelay() << "ms)" << std::endl;
    ++level;
    pc->playAnimation(arduino::Controller::Animation::TOWER_LIGHT);
    toNextLevel = LEVEL_CLEAR;
    isLevelingUp = true;
}

// the lowest y value containing a color (in other words the one closest to the top).
// used with tower light tracking, unused otherwise.
int progress;
// todo wonder if I should add a sleep call to give it some time to execute
void updateProgress() {
    switch (progress*3/Game::ROWS) {
        case 0: pc->setTowerLights(false,false,true,false); break;
        case 1: pc->setTowerLights(false,true,false,false); break;
        case 2: pc->setTowerLights(true,false,false,false); break;
    }
}
// whether to enable progress lights
bool towerLightProgressTracking = false;
void Game::placeShape() {
    Shape& s = *curShape;
    int rowsComplete = 0;
    int lY=ROWS,hY=-1; // range of rows to check
    for(int i=0; i < Shape::N_SQUARES; i++) {
        int y = s[i].y + s.y;
        if(y < lY) lY = y;
        if(y > hY) hY = y;
        grid[s[i].x+s.x][y] = curShape->color;
    }
    int y = hY;
    while(y >= lY)
    {
        if(rowComplete(y))
        {
            moveRows(y);
            rowsComplete++;
            // rows have been moved, everything is shifted one "down"
            lY++;
        }
        else y--;
    }
    progress = std::min(lY, progress+rowsComplete);
// this is explicitly opt-in given its general tendency to ruin the arduino.
    calcScore(rowsComplete);
    // todo move before tower light directive, creates increased consistency in key-light colors
    loadNewShape();
    // update tower lights in accordance to progress
    if(gameState == GameState::PLAY && towerLightProgressTracking) updateProgress();
}

#include<chrono>
using ms = std::chrono::milliseconds;
using sc = std::chrono::steady_clock;
auto curTime = sc::now();
auto getDuration()
{
    auto now = sc::now();
    auto r = std::chrono::duration_cast<ms>(now - curTime);
    curTime = now;
    return r.count();
}
void Game::play() {
    if(gameState == GameState::GAME_OVER) {
        for (auto &col: grid)
            for (auto &cell: col)
                cell = Color();
    }
    gameState = GameState::PLAY;
    level=0; incLevel();
    score = 0;
    bag = Bag(); // refresh the bag
    // gravity logic
    getDuration(); // reset reference time
    delete heldShape;
    heldShape = nullptr;
    delete nxtShape;
    nxtShape = new Shape(bag.draw());
    progress = ROWS; // lowest Y is at the bottom of the board now.
    loadNewShape();
}

struct {
    int move=0, rotate=0;
    bool instantDrop=false,fastDrop=false;
    bool holdPiece=false;
} cur, prev;

bool held = true; // press a button to start the game.

bool handleArduinoCommands(SDL_Keysym key, bool enable) {
    switch (key.scancode) {
        // 1-3 are green/yellow/red respectively. 0 is buzzer.
        case SDL_SCANCODE_KP_0:
            pc->towerLights.buzzer = enable;
            pc->updateTowerLights();
            break;
        case SDL_SCANCODE_KP_1:
            pc->towerLights.green = enable;
            pc->updateTowerLights();
            break;
        case SDL_SCANCODE_KP_2:
            pc->towerLights.yellow = enable;
            pc->updateTowerLights();
            break;
        case SDL_SCANCODE_KP_3:
            pc->towerLights.red = enable;
            pc->updateTowerLights();
            break;
        case SDL_SCANCODE_KP_DIVIDE: if(enable) {
            // toggle tower light progress tracking. Disabled by default due to being unstable and breaking things.
            towerLightProgressTracking = !towerLightProgressTracking;
            if (towerLightProgressTracking) {
                updateProgress();
            } else {
                pc->setTowerLights(false,false,false,false);
            }
            break;
        }
        // after this point everything only happens if enabled.
        case SDL_SCANCODE_KP_MULTIPLY: if(enable) {
            if(pc->nunchuckEnabled) pc->disableNunchuck();
            else pc->enableNunchuck();
            break;
        }
        case SDL_SCANCODE_KP_PLUS: if(enable) {
            pc->playAnimation(arduino::Controller::Animation::TOWER_LIGHT);
            break;
        }
            // fall into next
        case SDL_SCANCODE_KP_ENTER: if(enable) {
            pc->playAnimation(arduino::Controller::Animation::FLATLINE);
            break;
        }
        default: return false;
    }
    return true;
}

int map(int val, int l1, int l2, int r1, int r2) {
    if(val > l1 && val < l2) return -1;
    if(val > r1 && val < r2) return 1;
    return 0;
}

void Game::processInput()
{
    // fixme there is massive duplication here
    SDL_Event evnt;
    while(SDL_PollEvent(&evnt))
    {
        switch(evnt.type)
        {
            case SDL_WINDOWEVENT:
                switch(evnt.window.event) {
                    case SDL_WINDOWEVENT_RESIZED:
                        screenWidth = evnt.window.data1;
                        screenHeight = evnt.window.data2;
                        break;
                };
                break;
            case SDL_QUIT:
                gameState = GameState::EXIT;
                break;
//            case SDL_MOUSEMOTION:
//                //prepareScene(/*evnt.motion.x * 255 / screenWidth, evnt.motion.y * 255 / screenHeight, 255*/);
//                std::cout << evnt.motion.x << " " << evnt.motion.y << std::endl;
//                break;
            case SDL_KEYUP:
                if(handleArduinoCommands(evnt.key.keysym, false)) continue;
                held = false;
                if(gameState != GameState::PLAY) break;
                if(evnt.key.keysym.scancode == SDL_SCANCODE_S) {
                    toggleFastDrop(false);
                    //std::cout << "fast fall off" << std::endl;
                }
                break;
            case SDL_KEYDOWN:
                if(handleArduinoCommands(evnt.key.keysym, true)) continue;
                if(held) break; else held = true;
                if(gameState == GameState::START || gameState == GameState::GAME_OVER) {
                    if(evnt.key.keysym.scancode == 40) {
                        play();
                    }
                    return;
                }
                // interact with our piece
                // break locks, continue does not.
                bool resetTime = false;
                switch (evnt.key.keysym.scancode) {
                    case SDL_SCANCODE_W:
                        curShape->y--;
                        break;
                    case SDL_SCANCODE_S:
                        toggleFastDrop(true);
                        //std::cout << "fast fall on" << std::endl;
                        break; // already reset timer.
                    case SDL_SCANCODE_A:
                        resetTime = curShape->moveL();
                        break;
                    case SDL_SCANCODE_D:
                        resetTime = curShape->moveR();
                        break;
                    case SDL_SCANCODE_Q:
                        resetTime = curShape->rotateL();
                        break;
                    case SDL_SCANCODE_E:
                        resetTime = curShape->rotateR();
                        break;
                    case SDL_SCANCODE_TAB:
                        holdShape();
                        continue; // timer handled.
                    case SDL_SCANCODE_EQUALS:
                        incLevel();
                        break;
                    case SDL_SCANCODE_MINUS:
                        std::cout << "level: " << --level << " --- speed=" << (time = dropDelay()) << std::endl;
                        break;
                        // worth noting this cycles draw, so if you want it to start cycling you need to do it twice.
                    case SDL_SCANCODE_RIGHT:
                        loadNewShape();
                        nxtShape = cyclePiece(+1);
                        break;
                    case SDL_SCANCODE_LEFT:
                        loadNewShape();
                        nxtShape = cyclePiece(-1);
                        break;
                    case 40:
                        instantDrop();
                        break;
                    case SDL_SCANCODE_DELETE:
                        loadNewShape();
                        continue;
                    default:
                        continue;
                }
                if(locked && resetTime) time = LOCK_DELAY;
                return; // don't check arduino
                //std::cout << evnt.key.keysym.scancode << std::endl;
        }
    }
    // ARDUINO CONTROLS
    if(gameState == GameState::START || gameState == GameState::GAME_OVER) {
        if(pc->startButton || pc->nunchuckEnabled && pc->nunchuck.btnC) play(); else return;
    }
    prev = cur;
    cur = {
            (int)pc->moveRight - pc->moveLeft,
            pc->rotateLeft - (int)pc->rotateRight,
            pc->instantDrop,
            pc->fastDrop,
            pc->selectButton,
    };
    if(pc->nunchuckEnabled) {
        // nunchuck controls
        auto &n = pc->nunchuck;


        int move = map(n.joyX,32,100,165,225);
        if(move != cur.move) cur.move += move;
        if(map(n.joyY,35,80,165,225) == -1) cur.fastDrop |= true;

        cur.holdPiece |= n.btnC;
        cur.instantDrop |= n.btnZ;
        // apparently reversed?
        int roll = map(pc->nunchuck.accel.roll,40,100,150,220);
        if(roll != cur.rotate) cur.rotate += roll;
    }
    if(cur.holdPiece && !prev.holdPiece) holdShape();
    bool resetLock = false;
    if(cur.move != prev.move) resetLock = curShape->move(cur.move);
    if(cur.rotate != prev.rotate) resetLock = curShape-> rotate(cur.rotate) || resetLock;
    if(cur.fastDrop != prev.fastDrop) {
        resetLock = false;
        toggleFastDrop(cur.fastDrop);
    }
    if(cur.instantDrop && !prev.instantDrop) {
        resetLock = false;
        instantDrop();
    }
    if(locked && resetLock) time = LOCK_DELAY;
}

// ／(^ㅅ^)＼ Checks if a given row y is complete
bool Game::rowComplete(int y) const {
    // ／(^ㅅ^)＼ Loop through columns in the row
    for(int i = 0; i < COLS; i++)
    {
        // ／(^ㅅ^)＼ If a square in the row is equal to the default, the row is incomplete
        if(grid[i][y] == Color())
        {
            return false;
        }
    }
    // ／(^ㅅ^)＼ If no squares in the row are equal to default, the row is complete
    return true;
}

// ／(^ㅅ^)＼ Move all rows above y down a single row
bool Game::moveRows(int y)
{
    // ／(^ㅅ^)＼ Loop from y to 1
    for(int i = y; i > 0; i--)
    {
        // ／(^ㅅ^)＼ Move the row above to the current row
        for(int j = 0; j < COLS; j++)
        {
            grid[j][i] = grid[j][i - 1];
        }
    }
    // ／(^ㅅ^)＼ Clear row 0
    return clearRow(0);
}

// ／(^ㅅ^)＼ Clears a given row y
bool Game::clearRow(int y)
{
    // ／(^ㅅ^)＼ Loops through all columns in the row and sets each square to default
    for(int i = 0; i < COLS; i++)
    {
        grid[i][y] = Color();
    }
    // ／(^ㅅ^)＼ Increment level if required
    if(--toNextLevel == 0) {
        incLevel();
    }
    return true;
}

void Game::calcScore(int rowsCleared)
{
    if(rowsCleared == 0)
    {
        return;
    }
    else if(boardClear())
    {
        switch(rowsCleared)
        {
            case 1:
                score += 800 * level;
                break;
            case 2:
                score += 1200 * level;
                break;
            case 3:
                score += 1800 * level;
                break;
            case 4:
                score += 2000 * level;
                break;
            default:
                std::cout << "score calculation error" << std::endl;
        }
    }
    else
    {
        switch(rowsCleared)
        {
            case 1:
                score += 100 * level;
                break;
            case 2:
                score += 300 * level;
                break;
            case 3:
                score += 500 * level;
                break;
            case 4:
                score += 800 * level;
                break;
            default:
                std::cout << "score calculation error" << std::endl;
        }
    }
}

bool Game::boardClear()
{
    for(int i = 0; i < ROWS; ++i)
    {
        if(!(grid[i][COLS] == Color()))
            return false;
    }
    return true;
}

void Game::gameLoop()
{
    while (true)
    {
        prepareScene();
        presentScene();
        if(isLevelingUp) {
            isLevelingUp = false;
            sleep(2);
            getDuration();

        }
        if(pc->connected) pc->refreshArduinoStatus();
        processInput();
        if(gameState == GameState::EXIT) break;
        if(gameState == GameState::PLAY) applyGravity();
    }
}
void Game::applyGravity()
{
    time -= getDuration();
    while(time <= 0) {
        if(curShape->moveDown()) {
            time += dropDelay();
            if(fastFall){
                score += 1;
            }
            locked = false;
        } else {
            // shape cannot move down anymore.
            fastFall = false;
            if(locked) {
                // lock delay has been spent, place the shape.
                placeShape();
                // placeShape can change the gameState if it was unable to place the new shape.
                if(gameState == GameState::GAME_OVER) return;
            }
            else
            {
                // this prevents the shape from being placed instantly
                locked = true;
                time = LOCK_DELAY;
            }
        }
    }
}
