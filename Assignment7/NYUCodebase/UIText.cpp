#include "UIText.h"


UIText::UIText(GLfloat posX, GLfloat posY, GLfloat scale_x, GLfloat scale_y)
	: UIElement(nullptr, posX, posY, scale_x, scale_y){

}

UIText::~UIText()
{
}

GLvoid UIText::setup(GLuint texID, string message, GLfloat space, GLfloat r1, GLfloat g1, GLfloat b1, GLfloat a1){
	textureID = texID;

	text = message;
	spacing = space;

	r = r1;
	g = g1;
	b = b1;
	a = a1;

}
GLvoid UIText::draw(){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glEnable(GL_BLEND);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(x, y, 0.0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	GLfloat texture_size = 1.0 / 16.0f;
	vector<GLfloat> vertexData;
	vector<GLfloat> texCoordData;
	vector<GLfloat> colorData;

	for (GLfloat i = 0/*, j = -((GLfloat)text.size() / 2)*/; i < text.size(); i++/*, j++*/) {
		GLfloat texture_x = (GLfloat)(((GLint)text[i]) % 16) / 16.0f;
		GLfloat texture_y = (GLfloat)(((GLint)text[i]) / 16) / 16.0f;
		colorData.insert(colorData.end(), { r, g, b, a, r, g, b, a, r, g, b, a, r, g, b, a });
		vertexData.insert(vertexData.end(), { ((scale_x + spacing)/* * j*/) + (-0.5f * scale_x), 0.5f * scale_y,
			((scale_x + spacing)/* * j*/) + (-0.5f * scale_x), -0.5f * scale_y,
			((scale_x + spacing)/* * j*/) + (0.5f * scale_x), -0.5f * scale_y,
			((scale_x + spacing)/* * j*/) + (0.5f * scale_x), 0.5f * scale_y });
		texCoordData.insert(texCoordData.end(), { texture_x, texture_y, texture_x, texture_y + texture_size, texture_x +
			texture_size, texture_y + texture_size, texture_x + texture_size, texture_y });
	}

	glColorPointer(4, GL_FLOAT, 0, colorData.data());
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glDrawArrays(GL_QUADS, 0, text.size() * 4);
	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glPopMatrix();

}