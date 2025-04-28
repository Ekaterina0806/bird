#include <iostream>
#include <string>
#include <vector>
#include <list>
#include "birdPattern3.h"
#include <stdio.h>
#include <stdlib.h>

using namespace std;

enum class BirdColor : int
{
    White,
    Yellow,
    Black,
    Unknown
};

// Реализация паттерна "Стратегия" для активности птиц

enum class ActivityStrategyEnum : int
{
    Fly,
    Sing,
    Swim,
    None
};

class ActivityStrategy
{
public:
    virtual ~ActivityStrategy() {}
    virtual void Activity() = 0;
};

class FlyStrategy : public ActivityStrategy
{
    void Activity() override { cout << "Flying high..."; }
};

class SingStrategy : public ActivityStrategy
{
    void Activity() override { cout << "Singing beautifully..."; }
};

class SwimStrategy : public ActivityStrategy
{
    void Activity() override { cout << "Swimming fast..."; }
};

// Фабричный метод для создания стратегий активности
ActivityStrategy* CreateActivityStrategy(ActivityStrategyEnum activnoct)
{
    switch(activnoct)
    {
        case ActivityStrategyEnum::Fly: return new FlyStrategy;
        case ActivityStrategyEnum::Sing: return new SingStrategy;
        case ActivityStrategyEnum::Swim: return new SwimStrategy;
        default: return nullptr;
    }
}

class Bird
{
private:
     BirdColor Color;
     double Weight;

     ActivityStrategy* ActivityCases;

void ActivityUsingStrategy()
    {
        if(ActivityCases == nullptr)
        {
            cout << "No activity";
            return;
        }

        else
        {
            ActivityCases->Activity();
        }
    }

void DetectActivityStatus()
    {
        if(IsActive())
        {
            cout << "ACTIVE";
        }
        else
        {
            cout << "RESTING";
        }
    }

protected:
    bool BirdIsActive;

public:
    Bird(BirdColor color) : Color(color), Weight(0.0), BirdIsActive(false), ActivityCases (nullptr)
    {
        BirdIsActive = static_cast<bool>(rand()%2);
    }

virtual ~Bird()
{
    if(ActivityCases != nullptr) delete ActivityCases;
}


bool IsActive() const { return BirdIsActive; }

BirdColor GetColor() const { return Color; }

double GetWeight() const { return Weight; }

virtual void PrintType() = 0;
virtual void StrannoePovedenie() = 0;

   void Activity()
    {
        // 1. Вывести тип птицы
        PrintType();
        cout << " : ";

        // 2. Определить состояние активности
        DetectActivityStatus();
        cout << " : ";

        // 3. Происходит странное поведение
        StrannoePovedenie();
        cout << " : ";

        // 4. Выполнить активность согласно стратегии
        ActivityUsingStrategy();

        cout << endl;
    }

    void SetActivityStrategy(ActivityStrategy* activnoct) { ActivityCases = activnoct; }
};

class Parrot : public Bird
{
public:
    Parrot();
    ~Parrot() {}
    void PrintType() { cout << "Parrot"; }
    void StrannoePovedenie() { cout << "Hiding his head..."; }
};

Parrot :: Parrot() : Bird (BirdColor::Yellow)
{
    SetActivityStrategy(CreateActivityStrategy(ActivityStrategyEnum::Sing));
}

class Penguin : public Bird
{
public:
    Penguin();
    ~Penguin() {}
    void PrintType() { cout << "Penguin"; }
    void StrannoePovedenie() { cout << "Pushes the other one..."; }
};

Penguin :: Penguin() : Bird (BirdColor::White)
{
    SetActivityStrategy(CreateActivityStrategy(ActivityStrategyEnum::Swim));
}

class Vorona : public Bird
{
public:
    Vorona();
    ~Vorona() {}
    void PrintType() { cout << "Vorona"; }
    void StrannoePovedenie() { cout << "Throws stones..."; }
};

Vorona :: Vorona() : Bird (BirdColor::Black)
{
    SetActivityStrategy(CreateActivityStrategy(ActivityStrategyEnum::Fly));
}

//Реализация паттерна "Фабричный метод"
enum class BirdType : int
{
    Penguin = 1,
    Vorona = 2,
    Parrot = 3,

    Undefined = 0
};

Bird *CreateBird(BirdType type)
{
    Bird *newBird = nullptr;

    if (type == BirdType::Penguin)
    {
        newBird = new Penguin;
    }
    else if (type == BirdType::Vorona)
    {
        newBird = new Vorona;
    }
    else if (type == BirdType::Parrot)
    {
        newBird = new Parrot;
    }
    return newBird;
}

class BirdColorDecorator : public IteratorDecorator<class Bird*>
{
private:
    BirdColor NecessaryColor;

public:
    BirdColorDecorator(Iterator<Bird*> *it, BirdColor color)
    : IteratorDecorator<Bird*>(it), NecessaryColor(color) {}

    void First()
    {
        It->First();
        while(!It->IsDone() && It->GetCurrent()->GetColor() != NecessaryColor)
        {
            It->Next();
        }
    }

    void Next()
    {
        do
        {
            It->Next();

        } while(!It->IsDone() && It->GetCurrent()->GetColor() != NecessaryColor);
    }
};

class BirdActiveDecorator : public IteratorDecorator<class Bird*>
{
private:
    bool TargetActive;

public:
    BirdActiveDecorator(Iterator<Bird*> *it, bool isActive)
    : IteratorDecorator<Bird*>(it), TargetActive(isActive) {}

    void First()
    {
        It->First();
        while(!It->IsDone() && It->GetCurrent()->IsActive() != TargetActive)
        {
            It->Next();
        }
    }

    void Next()
    {
        do
        {
            It->Next();

        } while(!It->IsDone() && It->GetCurrent()->IsActive() != TargetActive);
    }
};

class BirdWeightDecorator : public IteratorDecorator<class Bird*>
{
private:
    double TargetWeight;

public:
    BirdWeightDecorator(Iterator<Bird*> *it, double weight)
    : IteratorDecorator<Bird*>(it), TargetWeight(weight) {}

    void First()
    {
        It->First();
        while(!It->IsDone() && It->GetCurrent()->GetWeight() != TargetWeight)
        {
            It->Next();
        }
    }

    void Next()
    {
        do
        {
            It->Next();

        } while(!It->IsDone() && It->GetCurrent()->GetWeight() != TargetWeight);
    }
};

void ActiveAll(Iterator<Bird*>* it){
    for (it -> First(); !it -> IsDone(); it -> Next()){
        Bird *currentBird = it -> GetCurrent();
        currentBird->Activity();
    }
}

void ActiveAllActive(Iterator<Bird*>* it){
    for (it -> First(); !it -> IsDone(); it -> Next()){
        Bird *currentBird = it -> GetCurrent();
        if (!currentBird -> IsActive()) continue;
        currentBird->Activity();
    }
}

void ActiveAllYellow(Iterator<Bird*>* it){
    for (it -> First(); !it -> IsDone(); it -> Next()){
        Bird *currentBird = it -> GetCurrent();
        if (currentBird -> GetColor() != BirdColor::Yellow) continue;
        currentBird->Activity();
    }
}

int main()
{

    setlocale (LC_ALL, "Russian");

    size_t N = 0;
    cout << "Vvedite kolvo ";
    cin >> N;

    ArrayClass<Bird*> birdArray;

    for (size_t i = 0; i < N; i++)
    {
        int bird_num = rand()%3+1;
        BirdType bird_type = static_cast<BirdType>(bird_num);
        Bird *newBird = CreateBird(bird_type);
        birdArray.Add(newBird);
    }

    cout << "size massiv bird: " << birdArray.Size() << endl;

    list<Bird*> birdVector;
    for(size_t i=0; i<N; i++)
    {
        int bird_num = rand()%3+1;
        BirdType bird_type = static_cast<BirdType>(bird_num);
        Bird *newBird = CreateBird(bird_type);
        birdVector.push_back(newBird);
    }

    cout << "size spicok bird " << birdVector.size() << endl;

    cout << endl << "All in a simple loop:" << endl;
    for(size_t i=0; i<birdArray.Size(); i++)
    {
        Bird* currentBird = birdArray[i];
        currentBird->Activity();
    }

    cout << endl << "All using iterator:" << endl;
    Iterator<Bird*> *all_It = birdArray.GetIterator();
    ActiveAll(all_It);
    delete all_It;

    cout << endl << "All active bird using iterator:" << endl;
    Iterator<Bird*> *activeIt = new BirdActiveDecorator(birdArray.GetIterator(), true);
    ActiveAll(activeIt);
    delete activeIt;

    cout << endl << "All yellow bird using iterator:" << endl;
    Iterator<Bird*> *yellowIt = new  BirdColorDecorator(birdArray.GetIterator(), BirdColor::Yellow);
    ActiveAll(yellowIt);
    delete yellowIt;

    cout << endl << "All active yellow bird using iterator:" << endl;
    Iterator<Bird*> *activeYellowIt =
        new BirdActiveDecorator(new BirdColorDecorator(birdArray.GetIterator(),BirdColor::Yellow), true);
    ActiveAll(activeYellowIt);
    delete activeYellowIt;

    cout << endl << "All active yellow using adapted iterator:" << endl;
    Iterator<Bird*> *adaptedIt = new ConstIteratorAdapter<list<Bird*>, Bird*>(&birdVector);
    Iterator<Bird*> *adaptedYellowIt = new BirdActiveDecorator(new BirdColorDecorator(adaptedIt, BirdColor::Yellow), true);
    ActiveAll(adaptedYellowIt);
    delete adaptedYellowIt;

    return 0;
}
