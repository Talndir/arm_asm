#pragma once

/*
	anim.h

	Header file for anim struct.
	Contains the anim struct for animations, used by MainWindow class.
*/

#include <vector>
#include "../../gui/gui_include.h"

// Handles animation interpolation
struct anim
{
	std::vector<wxPoint> points;
	int index;
	float x;
	float speed;
	int source, destination;

	wxPoint Lerp()
	{
		if (index != points.size() - 1)		// Check for out of bounds
			return points.at(index) * (1 - x) + points.at(index + 1) * x;		// Interps between index and index + 1
		else
			return points.at(index);
	}

	bool Update()
	{
		if (index != points.size() - 1)		// Check for out of bounds
		{
			x += speed / std::sqrt(std::pow(points.at(index + 1).x - points.at(index).x, 2) + std::pow(points.at(index + 1).y - points.at(index).y, 2));	// Increase x

			if (x >= 1.0)	// x must be clamped between 0 and 1
			{
				x -= 1.0;
				index = std::min(index + 1, (int)points.size() - 1);	// Next point
			}

			return true;
		}
		else
			return false;
	}
};