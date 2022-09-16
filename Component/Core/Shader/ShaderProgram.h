#pragma once

#include <string>
class ShaderProgram
{
public:
	ShaderProgram(std::string vertexShader, std::string fragmentShader);

	~ShaderProgram();

	void use() const;

	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void set2Float(const std::string& name, float x, float y) const;
	void set4Float(const std::string& name, float x, float y, float z, float w) const;

private:
	unsigned int __id;
};