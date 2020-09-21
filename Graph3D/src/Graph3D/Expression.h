#pragma once
//Include
#include <pch.h>

class Expression
{
public:
	Expression(std::string&& exp, const std::string& var1 = "x", const std::string& var2 = "y"); //Expression constructor

	Expression() = default;
	Expression(const Expression& rhs);
	Expression(Expression&& rhs) noexcept;
	Expression& operator=(const Expression& rhs); // = Operator Overload
	Expression& operator=(Expression&& rhs) noexcept; // = Operator Overload

	~Expression() = default; //Expression destructor

	void SetExpression(const std::string& exp, const std::string& var1 = "x", const std::string& var2 = "y"); //Change the current mathematical expression

	std::string GetStrExpression() const; //Return the current mathematical expression in string format (m_StrExpression)
	bool IsCompiled() const; //Return the current state of the compilation (m_Compiled)

	std::vector<float> Calculate(const glm::vec2& pos, unsigned int AreaSide, float step); //Return a vector of X and Y values based on the given range and step

private:
	void Compile(); //Compile the mathematical expression

	//Member variables

	std::string m_StrExpression, m_VarName1, m_VarName2; //Mathematical expression in string format (m_StrExpression) and the name of the variable (m_VarName) (Usually X)

	bool m_Compiled = false; //State of the compilation
	float m_Value1 = 0, m_Value2 = 0; //This variable is passed by reference to the m_VarTable, so if changed the mathematical expression evaluation also changes to new value of m_ValueX

	exprtk::symbol_table<float> m_VarTable; //Table that hold the variables reference and the their names
	exprtk::expression<float> m_Expression; //Register of the m_VarTable
};