#pragma once
#include <cstdint>
#include <d3d9.h>

struct color {
	int a, r, g, b;
	int red, green, blue, alpha;

	color() = default;
	color(int r, int g, int b, int a = 255) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
	color(uint32_t color) {
		this->a = (color >> 24) & 0xff;
		this->r = (color >> 16) & 0xff;
		this->g = (color >> 8) & 0xff;
		this->b = (color & 0xff);
	}
	color from_uint(uint32_t uint) {
		return color(uint);
	}
	static color FromHSB(float hue, float saturation, float brightness)
	{
		float h = hue == 1.0f ? 0 : hue * 6.0f;
		float f = h - (int)h;
		float p = brightness * (1.0f - saturation);
		float q = brightness * (1.0f - saturation * f);
		float t = brightness * (1.0f - (saturation * (1.0f - f)));

		if (h < 1)
		{
			return color(
				(unsigned char)(brightness * 255),
				(unsigned char)(t * 255),
				(unsigned char)(p * 255)
			);
		}
		else if (h < 2)
		{
			return color(
				(unsigned char)(q * 255),
				(unsigned char)(brightness * 255),
				(unsigned char)(p * 255)
			);
		}
		else if (h < 3)
		{
			return color(
				(unsigned char)(p * 255),
				(unsigned char)(brightness * 255),
				(unsigned char)(t * 255)
			);
		}
		else if (h < 4)
		{
			return color(
				(unsigned char)(p * 255),
				(unsigned char)(q * 255),
				(unsigned char)(brightness * 255)
			);
		}
		else if (h < 5)
		{
			return color(
				(unsigned char)(t * 255),
				(unsigned char)(p * 255),
				(unsigned char)(brightness * 255)
			);
		}
		else
		{
			return color(
				(unsigned char)(brightness * 255),
				(unsigned char)(p * 255),
				(unsigned char)(q * 255)
			);
		}
	}
	static color Black(int a = 255) { return { 0, 0, 0, a }; }
	static color Grey(int  a = 255) { return { 127, 127, 127, a }; }
	static color white(int a = 255) { return { 255, 255, 255, a }; }
	static color Red(int   a = 255) { return { 255, 0, 0, a }; }
	static color Green(int a = 255) { return { 0, 255, 0, a }; }
	static color Blue(int  a = 255) { return { 0, 0, 255, a }; }
	D3DCOLOR from_color(color col) {
		return D3DCOLOR_ARGB(col.a, col.r, col.g, col.b);
	}
};