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

class Bird
{
private:
     BirdColor Color;
     double Weight;

protected:
    bool BirdIsActive;

public:
    Bird(BirdColor color) : Color(color), Weight(0.0), BirdIsActive(false)
    {
        BirdIsActive = static_cast<bool>(rand()%2);
    }
virtual ~Bird() {}
bool IsActive() const { return BirdIsActive; }

BirdColor GetColor() const { return Color; }

double GetWeight() const { return Weight; }
virtual void Activity()
    {
        if(IsActive())
        {
            cout << "Bird is ACTIVE... ";
        }
        else
        {
            cout << "Bird is RESTING... ";
        }
    }
};

class Parrot : public Bird
{
public:
    Parrot();
    ~Parrot() {}
    void Activity() override;
};

Parrot :: Parrot() : Bird (BirdColor::Yellow){}

void Parrot::Activity()
{
    Bird::Activity();
    cout << "Parrot mimics sounds" << endl;
}

class Penguin : public Bird
{
public:
    Penguin();
    ~Penguin() {}
    void Activity() override;
};

Penguin :: Penguin() : Bird (BirdColor::White){}

void Penguin::Activity()
{
    Bird::Activity();
    cout << "Penguin waddles" << endl;
}

class Vorona : public Bird
{
public:
    Vorona();
    ~Vorona() {}
    void Activity() override;
};

Vorona :: Vorona() : Bird (BirdColor::Black){}

void Vorona::Activity()
{
    Bird::Activity();
    cout << "Vorona scavenges" << endl;
}

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
