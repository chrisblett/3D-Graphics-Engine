#include "ShadowMap.h"
#include <GL\glew.h>

ShadowMap::ShadowMap(GLsizei width, GLsizei height)
	: m_texture(nullptr)
	, m_width(width)
	, m_height(height)
	, m_shadowFrameBufferID(0)
{
	Init();
}

ShadowMap::~ShadowMap()
{
	if(m_texture)
	{
		delete m_texture;
	}

	// Delete the framebuffer
	glDeleteFramebuffers(1, &m_shadowFrameBufferID);
}

void ShadowMap::Init()
{
	// Create the texture for the shadow map
	m_texture = new Texture();

	m_texture->Bind();

	// Specify the texture for the framebuffer, configure it to contain depth information 
	// of the framebuffer it will be attached to
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, m_width, m_height,
		0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

	// Specify texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Enable the GPU to do depth comparison
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	// Prevent artifacts when sampling outside the shadow map
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	float borderColour[]{ 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColour);

	// Create the framebuffer and bind
	glGenFramebuffers(1, &m_shadowFrameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_shadowFrameBufferID);

	// Disable drawing to the colour buffer
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	// Attach the texture to the custom framebuffer
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_texture->GetTextureID(), 0);

	// Bind back to the default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMap::Write() const
{
	// Resize the viewport, since the shadow map may be larger or smaller than the window
	glViewport(0, 0, m_width, m_height);
	glBindFramebuffer(GL_FRAMEBUFFER, m_shadowFrameBufferID);
}

void ShadowMap::Read(GLenum texture) const
{
	glActiveTexture(texture);
	m_texture->Bind();
}

void ShadowMap::SetTextureCompareMode(GLint compareMode) const
{
	m_texture->SetCompareMode(compareMode);
}