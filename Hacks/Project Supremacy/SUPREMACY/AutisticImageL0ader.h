#pragma once
#include "lodepng.h"
#include "includes.hpp"
std::uint8_t* load_image(unsigned short id, int width, int height, const std::string &type = "PNG")
{
	auto resource = FindResource(g_ctx.m_globals.lpReserved, MAKEINTRESOURCE(id), type.data());
	auto loaded_resource = LoadResource(g_ctx.m_globals.lpReserved, resource);
	auto resource_ptr = LockResource(loaded_resource);
	auto resource_size = SizeofResource(g_ctx.m_globals.lpReserved, resource);

	std::vector< std::uint8_t > image;

	auto w = std::uint32_t(width);
	auto h = std::uint32_t(height);

	const auto data = new std::uint8_t[image.size()];
	std::copy(image.begin(), image.end(), data);

	return data;
}
class c_texture
{
	// raw image data
	unsigned char *data_{ };

	// ISurface texture id
	unsigned int texture_id_{ 0 };

	// size
	int width_{ };
	int height_{ };

public:
	c_texture() = default;

	// create texture
	c_texture(unsigned short id, int width, int height, const std::string &type = "PNG")
		: data_{ load_image(id, width, height, type) }
		, width_(width)
		, height_(height)
	{ }

	// draw texture
	void paint(int x, int y)
	{
		if (!g_csgo.m_surface()->IsTextureIDValid(texture_id_))
		{
			texture_id_ = g_csgo.m_surface()->CreateNewTextureID(true);
			g_csgo.m_surface()->DrawSetTextureRGBA(texture_id_, data_, uint32_t(width_), uint32_t(height_));
		}

		g_csgo.m_surface()->DrawSetColor(255, 255, 255, 255);
		g_csgo.m_surface()->DrawSetTexture(texture_id_);
		g_csgo.m_surface()->DrawTexturedRect(x, y, x + width_, y + height_);
	}
};
