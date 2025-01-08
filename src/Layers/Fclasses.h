#pragma once
#include "SimulationLayer.h"
#include "Platform.h"
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>

class EnviromentPixel
{
public:
    int type;
    int Xpos;
    int Ypos;
    int Zpos;

    EnviromentPixel(int t,int x,int y,int z)
        : type(t),Xpos(x),Ypos(y),Zpos(z) {}
    virtual ~EnviromentPixel() = default;
};

class Air : public EnviromentPixel
{
public:
    Air(int Xpos, int Ypos, int Zpos)
        : EnviromentPixel(0, Xpos, Ypos, Zpos) {}
};

class Water : public EnviromentPixel
{
public:
    Water(int Xpos, int Ypos, int Zpos)
        : EnviromentPixel(2, Xpos, Ypos, Zpos) {}
};

class Soil : public EnviromentPixel
{
public:
    Soil(int Xpos, int Ypos, int Zpos)
        : EnviromentPixel(6, Xpos, Ypos, Zpos) {}
};

class PlantPart : public EnviromentPixel
{
public:
    PlantPart(int Ptype, int Xpos, int Ypos,int Zpos)
        : EnviromentPixel(Ptype, Xpos, Ypos, Zpos) {}
};

class Stem : public PlantPart
{
public:
    Stem(int Xpos, int Ypos, int Zpos)
        : PlantPart(4, Xpos, Ypos, Zpos) {}
};

class Leaf : public PlantPart
{
public:
    Leaf(int Xpos, int Ypos, int Zpos)
        : PlantPart(5, Xpos, Ypos, Zpos) {}
};

class Fruit : public PlantPart
{
public:
    Fruit(int Xpos, int Ypos, int Zpos)
        : PlantPart(3, Xpos, Ypos, Zpos) {}
};

class Root : public PlantPart
{
public:
    Root(int Xpos, int Ypos, int Zpos)
        : PlantPart(1, Xpos, Ypos, Zpos) {}
};

class SubstrateData
{
public:
    int NLevel;
    int PLevel;
    int KLevel;
    int WaterLevel;
    int XPos;
    int YPos;
    int ZPos;
    SubstrateData(int Nitrogen, int Phosporus, int Potassium, int Water, int Xcoords, int Ycoords, int Zcoords)
        : NLevel(Nitrogen), PLevel(Phosporus), KLevel(Potassium), WaterLevel(Water), XPos(Xcoords), YPos(Ycoords), ZPos(Zcoords) {}
    virtual ~SubstrateData() = default;
};

class SoilData : public SubstrateData
{
public:
    SoilData(int Nitrogen, int Phosporus, int Potassium, int Xcoords, int Ycoords, int Zcoords)
        : SubstrateData(Nitrogen, Phosporus, Potassium, 10, Xcoords, Ycoords, Zcoords) {}
};

class WaterData : public SubstrateData
{
public:
    WaterData(int Xcoords, int Ycoords, int Zcoords)
        : SubstrateData(0, 0, 0, 30, Xcoords, Ycoords,Zcoords) {}
};

int const STEMSTORAGE = 300;
int const x = 32;
int const z = 32;
int const y = 32;
Vector3D<std::shared_ptr<EnviromentPixel>> enviroment(glm::uvec3(x,y,z));
Vector3D<std::shared_ptr<SubstrateData>> substrates(glm::uvec3(x,y,z));
Vector3D<std::shared_ptr<SubstrateData>> currentSubstrates(glm::uvec3(x,y,z));
Vector3D<std::shared_ptr<int>> renderingP(glm::uvec3(x,y,z));
int const DIFFICULTY = 5;

class PlantObj
{
public:
    //misc
    int WaterLevel = 0;
    int LightLevel = 0;
    int KLevel = 0;
    int PLevel = 0;
    int NLevel = 0;
    float KBonus = 1;
    float NBonus = 1;
    float PBonus = 1;

    //DNA
    std::vector<int> RootLikelyhood;
    std::vector<int> LeafLikelyhood;
    std::vector<int> growthGenetics;

    //PPV
    std::vector<std::shared_ptr<PlantPart>> PPV;

    //metody
    /*void birthPlant(std::vector<std::shared_ptr<PlantObj>> &plants)
    {
        bool planted = false;


        for (int j = 0; j < 10; j++)
        {
            int randLockX = rand() % x;
            int randLockZ = rand() % z;

           for (int i = y-1; i = 0; i--)
           {
            if (std::dynamic_pointer_cast<Air>(enviroment[glm::uvec3(x,y,z)]) &&
                    std::dynamic_pointer_cast<Soil>(enviroment[glm::uvec3(x,y+1,z)]) &&
                    i >= 1 &&
                    !renderingP[glm::uvec3(x,y,z)] &&
                    !renderingP[glm::uvec3(x,y+1,z)])
                {
                    std::vector<std::shared_ptr<PlantPart>> PPs =
                    {
                        std::make_shared<Leaf>
                        // tuto som skoncil
                    }
                }
           
            if (planted)
                return;
        }

    }
    
    void check(int x, int y, int r, int SubstrateData::*member)
    {
        for (int i = std::max(0, x - r); i <= std::min(ROWS - 1, x + r); i++)
        {
            for (int j = std::max(0, y - r); j <= std::min(COLS - 1, y + r); j++)
            {
                if (currentSubstrates[i][j] != nullptr)
                {
                    int availableNutrients = currentSubstrates[i][j].get()->*member;
                    int subtraction = 0;
                    if (member == &SubstrateData::WaterLevel)
                    {
                        subtraction = std::min(availableNutrients, static_cast<int>(10 * KBonus));
                        WaterLevel += subtraction;
                    }
                    else
                    {
                        int subtraction = std::min(availableNutrients, static_cast<int>(10 * PBonus));
                        if (member == &SubstrateData::KLevel)
                        {
                            KLevel += subtraction;
                        }
                        else if (member == &SubstrateData::NLevel)
                        {
                            NLevel += subtraction;
                        }
                        else if (member == &SubstrateData::PLevel)
                        {
                            PLevel += subtraction;
                        }
                    }
                    currentSubstrates[i][j].get()->*member -= subtraction;
                }
            }
        }
    }

    std::tuple<int, int> leafchoosing()
    {
        std::vector<Leaf> parts;
        for (auto &PP : PPV)
        {
            if (PP->EData == '_')
            {
                parts.emplace_back(PP->Xpos, PP->Ypos);
            }
        }
        
        if (parts.empty())
        {
            return std::make_tuple(-1, -1);
        }

        auto &chosen = parts[rand() % parts.size()];
        return std::make_tuple(chosen.Xpos, chosen.Ypos);
    }

    std::tuple<int, int> rootchoosing()
    {
        std::vector<Root> parts;
        for (auto &PP : PPV)
        {
            if (PP->EData == '@')
            {
                parts.emplace_back(PP->Xpos, PP->Ypos);
            }
        }
        if (!parts.empty())
        {
            auto &chosen = parts[rand() % parts.size()];
            return std::make_tuple(chosen.Xpos, chosen.Ypos);
        }
        return std::make_tuple(-1, -1);
    }

    bool soilPlaceable(int x, int y)
    {
        if(x < 0 || y < 0 || x >= renderingP.size() || y >= renderingP[0].size())
        {
            return false;   
        }
        if(renderingP[x][y] == nullptr)
        {
            return enviroment[x][y]->EData == '#';              
        }
        else
        {
            return false;
        }
    }

    bool airPlaceable(int x, int y)
    {
        if(x < 0 || y < 0 || x >= renderingP.size() || y >= renderingP[0].size())
        {
            return false;   
        }
        if(renderingP[x][y] == nullptr)
        {
            return enviroment[x][y]->EData == ' ';              
        }
        else
        {
            return false;
        }
    }

    void growStem()
    {

        std::vector<std::shared_ptr<PlantPart>> newParts;
        for (auto &PP : PPV)
        {
            if (PP->Xpos == 0)
            {
                return;
            }
            
        }

        for (auto &PP : PPV)
        {
            if (PP->Xpos > 0)
            {
                if (PP->EData == '_')
                {
                    PP->Xpos--;
                }
                else if (PP->EData == '|' && std::dynamic_pointer_cast<Air>(enviroment[PP->Xpos-1][PP->Ypos]))
                {
                    newParts.push_back(std::make_shared<Stem>(PP->Xpos-1, PP->Ypos));
                }
            }
        }

        for (auto &newPart : newParts)
        {
            PPV.push_back(newPart);
        }
    }

    std::vector<std::tuple<int, int>> likelyHoods()
    {
        std::vector<int> list1 = {-1, 0, 1};
        std::vector<int> list2 = {-1, 0, 1};
        std::vector<std::tuple<int, int>> returnVector;

        for (int dx : list1)
        {
            for (int dy : list2)
            {
                if (!(dx == 0 && dy == 0))
                {
                    returnVector.emplace_back(dx, dy);
                }
            }
        }

        return returnVector;
    }

   void growLeaf() 
   {
        bool chosen = false;
        auto possibleDirections = likelyHoods();

        int i = 0;
        while (!chosen && !possibleDirections.empty() && i < 10) {
            i++;

            int lx, ly, nx, ny;
            std::tie(lx, ly) = leafchoosing();
            auto &likeHoodVector = possibleDirections[rand() % possibleDirections.size()];
            std::tie(nx, ny) = likeHoodVector;

            int newX = nx + lx;
            int newY = ny + ly;


            if (airPlaceable(newX, newY)) {
                PPV.emplace_back(std::make_shared<Leaf>(newX, newY));
                chosen = true;
            } else {
            }
        }
    }

    void growRoot()
    {
        bool chosen = false;
        auto possibleDirections = likelyHoods();

        int i = 0;
        while (!chosen && !possibleDirections.empty() && i < 10)
        {
            i++;
            int lx, ly, nx, ny;

            std::tie(lx, ly) = rootchoosing();

            auto &likeHoodVector = possibleDirections[rand() % possibleDirections.size()];
            std::tie(nx, ny) = likeHoodVector;

            if (soilPlaceable(nx + lx, ny + ly))
            {
                PPV.emplace_back(std::make_shared<Root>(nx + lx, ny + ly));
                chosen = true;
            }
        }
    }

    void growFruit()
    {
        int i = 0;
        bool chosen = false;

        int x,y;
        while (!chosen && i<10)
        {
            std::tie(x, y) = leafchoosing();

            int fruitx = x + 1;
            if (airPlaceable(fruitx,y) && !renderingP[fruitx][y])
            {
                PPV.emplace_back(std::make_shared<Fruit>(fruitx, y));
                PPV.back()->EData = 'o';
                chosen = true;
            }
            if (chosen || i == 10)
            {
                return;
            }
            i++;
        }
    }

    void geneticGrowth()
    {
        int sum = 0;
        int choice = 0;
        int pickedPart = 0;
        for (int i = 0; i < growthGenetics.size(); i++)
        {
            sum += growthGenetics[i];
        }
        choice = rand()%sum;
        for (int i = 0; i < growthGenetics.size(); i++)
        {
            if (choice <= growthGenetics[i])
            {
                pickedPart++;
            }
            
        }
        switch (pickedPart)
        {
        case 0:
            growRoot();
            break;
        case 1:
            growStem();
            break;
        case 2:
            growLeaf();
            break;
        case 3:
            growFruit();
            break;
        default:
            break;
        }
        
    }*/

    PlantObj(std::vector<int> RLikelyhood, std::vector<int> LLikelyhood,std::vector<int> gGrowth, std::vector<std::shared_ptr<PlantPart>> PPs)
        : RootLikelyhood(RLikelyhood), LeafLikelyhood(LLikelyhood),growthGenetics(gGrowth),PPV(PPs){}

    virtual ~PlantObj() = default;
};

std::vector<std::shared_ptr<PlantObj>> plants;