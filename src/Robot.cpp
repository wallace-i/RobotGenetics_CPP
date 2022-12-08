//
//  Robot.cpp
//  CISP400 Final
//
//  Created by Ian Wallace on 6/23/22.
//  Copyright © 2022 Ian Wallace. All rights reserved.
//

#include "Robot.hpp"
#include "DNA.hpp"
#include "NumGen.hpp"
#include "GlobalDefs.h"

/**
 *  Function: Robot()
 *  Parameters: none
 *  Notes: No-arg constructor. Called when robotArray is initialized -> main.cpp line:27.
 */
Robot::Robot() 
{
    NumGen numObj(0, 99);
    
    m_position = numObj.getRandNum();
    m_battery  = 5;
    m_turns    = 0;
    
    for (int i{ 0 }; i < c_numOfGenes; i++)
    {
        m_genes[i] = DNA();
    }
}


/**
 *  Function: Robot()
 *  Parameters: parent1 => robot object
 *              parent2 => robot object
 *              counter => const int which determines even or odd
 *  Notes: parent1 and parent2 gene's are combined for 2 children per generation of robots.
 *         Child1 will get the top half of parent1's genes and child2 will get the bottom
 *         half, and vica-versa for parent2 genes. There is a mutation in the gene pool
 *         if numObj results in < 5, which will swap the genes from both parents by 1 element
 *         in the child's DNA array.
 */
Robot::Robot(const Robot& parent1, const Robot& parent2, const int counter)
{
    NumGen numObj(0, 99);
    
    m_position = numObj.getRandNum();
    m_battery  = 5;
    m_turns    = 0;
    
    // Else combine genes from 2 robots into two new robots
    // Child 1
    if (counter % 2 == 0) 
    {
        for (int i{ 0 }; i < (c_numOfGenes / 2); i++)
        {
            m_genes[i] = parent1.m_genes[i];
        }
        
        for (int i{ c_numOfGenes / 2 }; i < c_numOfGenes; i++)
        {
            m_genes[i] = parent2.m_genes[i];
        }

    //Child 2   
    } else {

        for (int i{ 0 }; i < (c_numOfGenes / 2); i++)
        {
            m_genes[i] = parent2.m_genes[i];
        }
        
        for (int i{ c_numOfGenes / 2 }; i < c_numOfGenes; i++)
        {
            m_genes[i] = parent1.m_genes[i];
        }
    }
    
    // If mutation, swap genes around by 1;
    if (numObj.getRandNum() < 5) 
    {
        DNA tempGene = m_genes[0];
        
        for (int i{ 0 }; i < 14; i++)
        {
            m_genes[i] = m_genes[i + 1];
        }

        m_genes[15] = tempGene;
    }
}

/**
 *  Function: senseArea
 *  Parameters: mapObj => Map object containing a randomized 10 x 10 grid
 *              for each robot to traverse.
 *  Notes: On every turn, the robot will 'sense' it's surroundings and load
 *         each adjacent square into its m_sensor variable.
 */
void Robot::senseArea(const Map mapObj) 
{
    
    // calibrate sensor
    for (auto& i : m_sensor)
    {
        i = 0;
    }

    // check for walls true = 1, batteries true = 2, else 0 = empty
    // North
    if (m_position - 10 < 0) // N
    {
        m_sensor[0] = 1;

    } else if (mapObj.m_batteryArray[m_position - 10] == 'b') {

        m_sensor[0] = 2;
    }

    // South
    if (m_position + 10 > 99)
    {
        m_sensor[1] = 1;

    } else if (mapObj.m_batteryArray[m_position + 10] == 'b') {

        m_sensor[1] = 2;
    }

    // East
    if ((m_position + 1) % 10 == 0 || m_position == 9)
    {
        m_sensor[2] = 1;

    } else if (mapObj.m_batteryArray[m_position + 1] == 'b') {

        m_sensor[2] = 2;
    }

    // West
    if (m_position % 10 == 0 || m_position == 0)
    {
        m_sensor[3] = 1;

    } else if (mapObj.m_batteryArray[m_position - 1] == 'b') {

        m_sensor[3] = 2;
    }
}

/**
 *  Function: move
 *  Parameters: mapObj => Map for robot to traverse
 *  Notes: The genes in the DNA of each robot is verified for a 'move order'.
 *         This order is generated by if any of the gene's in the robot's DNA
 *         Matches that of it's sensor variable. (i.e. 1021 == wall N, empty S,
 *         battery E, wall W). If there is a match, the robot will move in the
 *         direction of the 5th gene. If there is no match, the robot will move
 *         in the direction of its last gene. If there is a battery in the subsequent
 *         square, it is consumed by the robot and removed from the grid.
 */
void Robot::move(Map& mapObj) 
{
    int geneMatch{0};
    int newPosition{m_position};
    int direction{0};
    
    m_turns++;
    senseArea(mapObj);
    
    // check genes
    for (int i{ 0 }; i < c_numOfGenes; i++) 
    {
        geneMatch = 0;

        for (int j{ 0 }; j < (c_numOfProteins - 1); j++) 
        {
            if ((m_genes[i].m_proteins[j] == m_sensor[j]) || (m_genes[i].m_proteins[j] == 3)) // 3 don't care
            {
                geneMatch++;
            }
        }

        if (geneMatch == 4) 
        {
            direction = m_genes[i].m_proteins[4];
            break;
        }

        if (i == 15)
        {
            direction = m_genes[i].m_proteins[4];
        }
    }

    // If direction is 4, choose random direction for robot to move
    if (direction == 4) 
    {
        NumGen numObj(0, 3);
        direction = numObj.getRandNum();
    }

    // Move Position by adding or subtracting to current position
    if (m_sensor[direction] != 1)
    {
        switch (direction)
        {
            // North: -10
        case 0: newPosition = m_position - 10;
            break;

            // South: + 10
        case 1: newPosition = m_position + 10;
            break;

            //East: + 1
        case 2: newPosition = m_position + 1;
            break;

            // West: -1
        case 3: newPosition = m_position - 1;
            break;
        }
    }
    
    m_position = newPosition;
    
    if (mapObj.m_batteryArray[m_position] != 'b')
    {
        m_battery--;
        
    } else {

        mapObj.m_batteryArray[m_position] = 'o';
        addPower();
    }
}

/**
 *  Function: operator= overload
 *  Parameters: right => Robot obj
 *  Notes: Overwrite new robot child's data into current generation,
 *         so that the next generation of robots contains new baby robots.
 */
Robot& Robot::operator=(const Robot &right) 
{
    if (this != &right) 
    {
        m_position = right.getPosition();
        m_battery = right.getBattery();
        m_turns = right.getTurns();
        
        for (int i{0}; i < c_numOfGenes; i++) 
        {
            for (int j{0}; j < c_numOfProteins; j++) 
            {
                m_genes[i].m_proteins[j] = right.getProteins(i, j);
            }
        }
    }
    
    return *this;
}

// Setters
void Robot::addPower()         { m_battery += 5; }
void Robot::resetBattery()     { m_battery = 5; }
void Robot::resetTurns()       { m_turns = 0; }
void Robot::setPosition(int p) { m_position = p; } 

// Getters
int Robot::getBattery()  const { return m_battery; }
int Robot::getTurns()    const { return m_turns; }
int Robot::getPosition() const { return m_position; }
int Robot::getProteins(int i, int j) const { return m_genes[i].m_proteins[j]; }