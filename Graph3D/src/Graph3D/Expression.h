#pragma once
//Include
#include <pch.h>

class Expression
{
public:
	Expression(std::string&& exp, std::string&& var1, std::string&& var2); //Expression constructor

	Expression() = default;
	Expression(const Expression& rhs);
	Expression(Expression&& rhs) noexcept;
	Expression& operator=(const Expression& rhs); // = Operator Overload
	Expression& operator=(Expression&& rhs) noexcept; // = Operator Overload

	~Expression() = default; //Expression destructor

	void SetExpression(const std::string& exp, const std::string& var1, const std::string& var2); //Change the current mathematical expression

	std::string GetStrExpression() const; //Return the current mathematical expression in string format (m_StrExpression)
	std::string GetVar1() const; //Return the current mathematical expression in string format (m_StrExpression)
	std::string GetVar2() const; //Return the current mathematical expression in string format (m_StrExpression)
	bool IsCompiled() const; //Return the current state of the compilation (m_Compiled)

	std::vector<float> Calculate(const glm::vec2& pos, unsigned int AreaSide, float step); //Return a vector of X and Y values based on the given range and step

private:
	static exprtk::parser<float> s_Parser; //Mathematical expression parser

	void Compile(); //Compile the mathematical expression
	//Member variables

	std::string m_StrExpression;// , m_VarName1, m_VarName2; //Mathematical expression in string format (m_StrExpression) and the name of the variable (m_VarName) (Usually X)
	std::array<std::string, 2> m_VarName;

	bool m_Compiled = false; //State of the compilation
	std::array<float, 2> m_Value = { 0.f,0.f };

	exprtk::symbol_table<float> m_VarTable; //Table that hold the variables reference and the their names
	exprtk::expression<float> m_Expression; //Register of the m_VarTable
};