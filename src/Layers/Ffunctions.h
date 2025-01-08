#pragma once
#include "Fclasses.h"

void Generate()
{
    srand(time(NULL));

    std::vector<std::vector<int>> height(x, std::vector<int>(z,0));
    srand(time(NULL));
    std::vector<int> perlinNoiseRandomVector;

    for (int i = 0; i < 4; i++)
    {
        perlinNoiseRandomVector.emplace_back(rand()%16);
    }
    double Pie = 3.14159265358979323846;

    for (int X = 0; X < x; X++) {
        for (int Z = 0; Z < z; Z++) {
            double finalHeight = 0.0;

            for (int i = 0; i < 4; i++) {
                double cornerX, cornerZ;

                switch (i) {
                case 0: cornerX = X + 0.5; cornerZ = Z + 0.5; break;
                case 1: cornerX = x - X - 0.5; cornerZ = Z + 0.5; break;
                case 2: cornerX = X + 0.5; cornerZ = z - Z - 0.5; break;
                case 3: cornerX = x - X - 0.5; cornerZ = z - Z - 0.5; break;
                }

                double magnitude = sqrt(cornerX * cornerX + cornerZ * cornerZ);
                double normX = cornerX / magnitude;
                double normZ = cornerZ / magnitude;

                double randomAngle = perlinNoiseRandomVector[i] / 16.0 * 2.0 * Pie;
                double randX = cos(randomAngle);
                double randZ = sin(randomAngle);

                double dotProduct = normX * randX + normZ * randZ;

                double normalizedHeight = ((dotProduct + 1.0) / 2.0) * 31.0;

                finalHeight += normalizedHeight;
            }

            height[X][Z] = static_cast<int>(finalHeight / 4.0);

            for (int Y = 0; Y < height[X][Z]; Y++)
            {
                enviroment[glm::uvec3(X,Y,Z)] = std::make_shared<Soil>(X,Y,Z);
            }

        }
    }
}