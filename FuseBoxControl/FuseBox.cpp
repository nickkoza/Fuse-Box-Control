#include "Animately.h"
#include "Core/Timeline.h"
#include "Parts/TimeSwitch.h"
#include "Parts/Button.h"
#include "Parts/LED.h"
#include "Tweens/Flicker.h"
#include "Speaker.h"
#include "Memory.h"

using namespace Animately;

class Scene {
private:
    static const int numberOfLEDs = 3;
    Timeline *timeline;
    TimeSwitch *sparkSwitch;
    TimeSwitch *motorSwitch;
    LED *sparkLEDs[numberOfLEDs];
    Tween *sparkTweenInstance;

public:
    Scene(Timeline *timeline, TimeSwitch *sparkSwitch, TimeSwitch *motorSwitch, LED *sparkLEDs[], Tween *sparkTweenInstance) {
        this->timeline = timeline;
        this->sparkSwitch = sparkSwitch;
        this->motorSwitch = motorSwitch;
        memcpy(this->sparkLEDs, sparkLEDs, sizeof(LED*) * numberOfLEDs);
        this->sparkTweenInstance = sparkTweenInstance;
    }

    void cueMain(inputId id) {
        LOG("Scene 1");
        
        timeline->schedule(0, 2250,
            DELEGATE(sparkSwitch, &TimeSwitch::close),
            DELEGATE(sparkSwitch, &TimeSwitch::open));

        timeline->schedule(250, 2000,
            DELEGATE(motorSwitch, &TimeSwitch::close),
            DELEGATE(motorSwitch, &TimeSwitch::open));

        for (int i = 0; i < numberOfLEDs; i++) {
            timeline->schedule(0, 255, 250, 2000,
                NULL,
                DELEGATE(sparkLEDs[i], &LED::set),
                DELEGATE(sparkTweenInstance, &Tween::getValue),
                DELEGATE(sparkLEDs[i], &LED::off));
        }
    }
    
    void cueLong(inputId id) {
        LOG("Scene 2");
        
        timeline->schedule(0, 3250,
            DELEGATE(sparkSwitch, &TimeSwitch::close),
            DELEGATE(sparkSwitch, &TimeSwitch::open));

        timeline->schedule(250, 3000,
            DELEGATE(motorSwitch, &TimeSwitch::close),
            DELEGATE(motorSwitch, &TimeSwitch::open));

        for (int i = 0; i < numberOfLEDs; i++) {
            timeline->schedule(0, 255, 250, 3000,
                NULL,
                DELEGATE(sparkLEDs[i], &LED::set),
                DELEGATE(sparkTweenInstance, &Tween::getValue),
                DELEGATE(sparkLEDs[i], &LED::off));
        }
    }
    
    void cueShort(inputId id) {
        LOG("Scene 3");
        
        timeline->schedule(0, 1250,
            DELEGATE(sparkSwitch, &TimeSwitch::close),
            DELEGATE(sparkSwitch, &TimeSwitch::open));

        timeline->schedule(250, 1000,
            DELEGATE(motorSwitch, &TimeSwitch::close),
            DELEGATE(motorSwitch, &TimeSwitch::open));

        for (int i = 0; i < numberOfLEDs; i++) {
            timeline->schedule(0, 255, 250, 1000,
                NULL,
                DELEGATE(sparkLEDs[i], &LED::set),
                DELEGATE(sparkTweenInstance, &Tween::getValue),
                DELEGATE(sparkLEDs[i], &LED::off));
        }
    }
};

Timeline timeline;
TimeSwitch sparkSwitch(6, 3000L, 5000L);
TimeSwitch motorSwitch(7, 5000L, 100L);
Speaker speaker(8, 60);

Flicker sparkTween({0, 255}, {15, 50}, {2, 10}, {15, 50});

int numberOfLEDs = 3;
LED sparkLED1(9);
LED sparkLED2(10);
LED sparkLED3(11);
LED *sparkLEDs[] = {
    &sparkLED1,
    &sparkLED2,
    &sparkLED3
};

Scene scene(&timeline, &sparkSwitch, &motorSwitch, sparkLEDs, &sparkTween);

const int triggerA = A3;
const int triggerB = A1;
const int triggerC = A4;
const int triggerD = A2;
const int triggerAny = A5;
Button scene1Button(triggerA, 0, fastdelegate::MakeDelegate(&scene, &Scene::cueMain), NULL, NULL);
Button scene2Button(triggerB, 0, fastdelegate::MakeDelegate(&scene, &Scene::cueShort), NULL, NULL);
Button scene3Button(triggerC, 0, fastdelegate::MakeDelegate(&scene, &Scene::cueLong), NULL, NULL);
Button toneToggleButton(triggerD, 0, fastdelegate::MakeDelegate(&speaker, &Speaker::toggleTone), NULL, NULL);

void setup() {
    Serial.begin(9600);
    
    LOGF("Timeline: %d bytes", sizeof(timeline));
    LOGF("Free: %d bytes", freeMemory());
    LOGF("Delegates: %d bytes each", sizeof(fastdelegate::MakeDelegate(&scene, &Scene::cueMain)));
    
    speaker.playTone(60);
}

void loop() {
    timeline.tick();
    scene1Button.tick();
    scene2Button.tick();
    scene3Button.tick();
    toneToggleButton.tick();
    speaker.tick();
}
