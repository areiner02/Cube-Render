#include <iostream>
#include <vector>
#include <SDL.h>

#define SCREEN_WIDTH	300
#define SCREEN_HEIGHT	300

int main(int argc, char* argv[]) 
{
	SDL_Window* window;
	SDL_Renderer* renderer;
	std::vector<SDL_FPoint> pixels;

	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow(
		"Cube Render",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN
	);
	renderer = SDL_CreateRenderer(
		window,
		0,
		SDL_RENDERER_ACCELERATED
	);

	struct Vector3 
	{
		float x, y, z;
	};

	struct Edge
	{
		int a, b;
	};

	auto DrawLine = [&](float x1, float x2, float y1, float y2)
	{
		float dx = x2 - x1;
		float dy = y2 - y1;

		float length = std::sqrt(dx * dx + dy * dy);
		float angle = std::atan2(dy, dx);

		for (float i = 0; i < length; i++)
		{
			SDL_FPoint pixel = { x1 + std::cos(angle) * i, y1 + std::sin(angle) * i };
			pixels.emplace_back(pixel);
		}
	};

	auto Rotate = [&](Vector3& point, float x, float y, float z)
	{
		float rad = 0;
		rad = x;
		point.y =	std::cos(rad)	* point.y - std::sin(rad) * point.z;
		point.z =	std::sin(rad)	* point.y + std::cos(rad) * point.z;
		rad = y;
		point.x =	std::cos(rad)	* point.x + std::sin(rad) * point.z;
		point.z =	-std::sin(rad)	* point.x + std::cos(rad) * point.z;
		rad = z;
		point.x =	std::cos(rad)	* point.x - std::sin(rad) * point.y;
		point.y =	std::sin(rad)	* point.x + std::cos(rad) * point.y;
	};

	std::vector<Vector3> vertices
	{
		{100, 100, 100},
		{200, 100, 100},
		{200, 200, 100},
		{100, 200, 100},

		{100, 100, 200},
		{200, 100, 200},
		{200, 200, 200},
		{100, 200, 200}
	};

	std::vector<Edge> edges
	{
		{ 0, 4 },
		{ 1, 5 },
		{ 2, 6 },
		{ 3, 7 },

		{ 0, 1 },
		{ 1, 2 },
		{ 2, 3 },
		{ 3, 0 },

		{ 4, 5 },
		{ 5, 6 },
		{ 6, 7 },
		{ 7, 4 }
	};

	Vector3 c = { 0, 0, 0 };
	for (auto& v : vertices)
	{
		c.x += v.x;
		c.y += v.y;
		c.z += v.z;
	}
	c.x /= vertices.size();
	c.y /= vertices.size();
	c.z /= vertices.size();

	SDL_Event e = {};
	bool quit = false;
	while (!quit) 
	{	
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT) quit = true;
		}

		for (auto& v : vertices)
		{
			v.x -= c.x;
			v.y -= c.y;
			v.z -= c.z;
			Rotate(v, 0.002, 0.001, 0.004);
			v.x += c.x;
			v.y += c.y;
			v.z += c.z;
			SDL_FPoint pixel = { v.x, v.y };
			pixels.emplace_back(pixel);
		}

		for (auto& e : edges)
		{
			DrawLine(
				vertices[e.b].x,
				vertices[e.a].x,
				vertices[e.b].y,
				vertices[e.a].y
			);
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		for (auto& p : pixels)
		{
			SDL_RenderDrawPointF(renderer, p.x, p.y);
		}
		SDL_RenderPresent(renderer);
		pixels.clear();
		SDL_Delay(15);
	}

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}