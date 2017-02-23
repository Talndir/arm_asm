#pragma once

/*
	gui_include.h

	Weird inbetween file that contains stuff some of the GUI classes need.
*/

#define wxUSE_GUI true	// This forces a bunch of GUI files to be #included in wx\wxprec.h
#include <wx\wxprec.h>	// Included main wxWidgets files

// Reprensents RGB colour format
struct rgb
{
	rgb(double x, double y, double z) : r(x), g(y), b(z) {}
	rgb() {}
	double r;		// [0, 1]
	double g;		// [0, 1]
	double b;		// [0, 1]
};

// Represents HSV colour format
struct hsv
{
	hsv(double x, double y, double z) : h(x), s(y), v(z) {}
	hsv() {}
	double h;		// Angle in degrees [0, 359]
	double s;		// [0, 1]
	double v;		// [0, 1]
};

static hsv   rgb2hsv(rgb in);
static rgb   hsv2rgb(hsv in);

// Converts from RGB to HSV
// Taken from a website, source unknown
// I'll rewrite this later once I understand how it actually works
// Comments are original programmer's
inline hsv rgb2hsv(rgb in)
{
	hsv         out;
	double      min, max, delta;

	min = in.r < in.g ? in.r : in.g;
	min = min  < in.b ? min : in.b;

	max = in.r > in.g ? in.r : in.g;
	max = max  > in.b ? max : in.b;

	out.v = max;                                // v
	delta = max - min;
	if (delta < 0.00001)
	{
		out.s = 0;
		out.h = 0; // undefined, maybe nan?
		return out;
	}
	if (max > 0.0) { // NOTE: if Max is == 0, this divide would cause a crash
		out.s = (delta / max);                  // s
	}
	else {
		// if max is 0, then r = g = b = 0              
		// s = 0, v is undefined
		out.s = 0.0;
		out.h = NAN;                            // its now undefined
		return out;
	}
	if (in.r >= max)                           // > is bogus, just keeps compilor happy
		out.h = (in.g - in.b) / delta;        // between yellow & magenta
	else
		if (in.g >= max)
			out.h = 2.0 + (in.b - in.r) / delta;  // between cyan & yellow
		else
			out.h = 4.0 + (in.r - in.g) / delta;  // between magenta & cyan

	out.h *= 60.0;                              // degrees

	if (out.h < 0.0)
		out.h += 360.0;

	return out;
}

// Converts from HSV to RGB
// Taken from a website, source unknown
// I'll rewrite this later once I understand how it actually works
// Comments are original programmer's
inline rgb hsv2rgb(hsv in)
{
	double      hh, p, q, t, ff;
	long        i;
	rgb         out;

	if (in.s <= 0.0) {       // < is bogus, just shuts up warnings
		out.r = in.v;
		out.g = in.v;
		out.b = in.v;
		return out;
	}
	hh = in.h;
	if (hh >= 360.0) hh = 0.0;
	hh /= 60.0;
	i = (long)hh;
	ff = hh - i;
	p = in.v * (1.0 - in.s);
	q = in.v * (1.0 - (in.s * ff));
	t = in.v * (1.0 - (in.s * (1.0 - ff)));

	switch (i) {
	case 0:
		out.r = in.v;
		out.g = t;
		out.b = p;
		break;
	case 1:
		out.r = q;
		out.g = in.v;
		out.b = p;
		break;
	case 2:
		out.r = p;
		out.g = in.v;
		out.b = t;
		break;

	case 3:
		out.r = p;
		out.g = q;
		out.b = in.v;
		break;
	case 4:
		out.r = t;
		out.g = p;
		out.b = in.v;
		break;
	case 5:
	default:
		out.r = in.v;
		out.g = p;
		out.b = q;
		break;
	}
	return out;
}

// Turns wxColour (yay! correct spelling for once!) into rgb struct
inline rgb wx2rgb(wxColor& c)
{
	return rgb(c.Red() / 255.0, c.Green() / 255.0, c.Blue() / 255.0);
}

// Turns rgb struct into wxColour
inline wxColor rgb2wx(rgb& c)
{
	return wxColor(c.r * 255, c.g * 255, c.b * 255);
}