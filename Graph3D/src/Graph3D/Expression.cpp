//Includes
#include <pch.h>
#include "Expression.h"

extern exprtk::parser<float> g_Parser; //Global variable (defined at main.cpp)

//Constructor
Expression::Expression(std::string&& exp, const std::string& var1, const std::string& var2) :
	m_StrExpression(std::move(exp)), //m_StrExpression = exp
	m_VarName1(var1),
	m_VarName2(var2)
{
	Compile(); //Call the Compile function
}

Expression::Expression(const Expression& rhs)
{
	m_StrExpression = rhs.m_StrExpression;
	m_VarName1 = rhs.m_VarName1;
	m_VarName2 = rhs.m_VarName2;
	m_Compiled = rhs.m_Compiled;
	m_Value1 = rhs.m_Value1;
	m_Value2 = rhs.m_Value2;
	m_VarTable = rhs.m_VarTable;
	m_Expression = rhs.m_Expression;
}

Expression::Expression(Expression&& rhs) noexcept
{
	m_StrExpression = std::move(rhs.m_StrExpression);
	m_VarName1 = std::move(rhs.m_VarName1);
	m_VarName2 = std::move(rhs.m_VarName2);
	m_Compiled = rhs.m_Compiled;
	m_Value1 = rhs.m_Value1;
	m_Value2 = rhs.m_Value2;
	m_VarTable = std::move(rhs.m_VarTable);
	m_Expression = std::move(rhs.m_Expression);
}

Expression & Expression::operator=(const Expression & rhs)
{
	m_Compiled = rhs.m_Compiled;
	m_StrExpression = rhs.m_StrExpression;
	m_VarName1 = rhs.m_VarName1;
	m_VarName2 = rhs.m_VarName2;
	m_Value1 = rhs.m_Value1;
	m_Value2 = rhs.m_Value2;
	m_VarTable = rhs. m_VarTable;
	m_Expression = rhs.m_Expression;
	return *this; //Return a dereferenced pointer
}

Expression& Expression::operator=(Expression&& rhs) noexcept
{
	m_StrExpression = std::move(rhs.m_StrExpression);
	m_VarName1 = std::move(rhs.m_VarName1);
	m_VarName2 = std::move(rhs.m_VarName2);
	m_Compiled = rhs.m_Compiled;
	m_Value1 = rhs.m_Value1;
	m_Value2 = rhs.m_Value2;
	m_VarTable = std::move(rhs.m_VarTable);
	m_Expression = std::move(rhs.m_Expression);
	return *this;
}

void Expression::SetExpression(const std::string& exp, const std::string& var1, const std::string& var2)
{
	m_StrExpression = exp; //Changes the m_StrExpression to the new mathematical expression
	m_VarName1 = var1;
	m_VarName2 = var2;
	m_Compiled = false; //Changes the compilation status to false, because the mathematical expression needs to be recompiled

	Compile(); //Call the Compile function
}

std::string Expression::GetStrExpression() const
{
	return m_StrExpression; //Return the current mathematical expression in string format (m_StrExpression)
}

bool Expression::IsCompiled() const
{
	return m_Compiled; //Return the current state of the compilation (m_Compiled)
}


std::vector<float> Expression::Calculate(const glm::vec2& pos, unsigned int AreaSide, float step)
{
	//Checks if the expression is compiled
	if (AreaSide > 0 && step > 0.f)
	{
		if (m_Compiled)
		{
			//unsigned int totalPoints = round(pow(AreaSide + 2, 2) / step);
			unsigned int totalPoints = (unsigned int)round(AreaSide/step) + 1;
			std::vector<float> xyz; //Creation of the xy vector
			xyz.reserve(3 * totalPoints * totalPoints); //Reservation of space

			//Calculates y point for each x in the m_Range
			m_Value1 = pos.x - AreaSide/2.f;
			for (unsigned int i = 0; i < totalPoints; i++)
			{
				m_Value2 = pos.y - AreaSide/2.f;
				for (unsigned int j = 0; j < totalPoints; j++)
				{
					float z = m_Expression.value();
					xyz.push_back(m_Value1);
					xyz.push_back(m_Value2);
					xyz.push_back(z);
					m_Value2 += step;
				}
				m_Value1 += step;
			}

			return xyz; //Returns xy vector
		}
		else
		{
			std::cout << "The expression: " << m_StrExpression << " it's not compiled!!\n"; //Error message
			return {}; //Returns a empty vector
		}
	}
	else
	{
		std::cout << "Side Length or step are less then 0!!\n";
		return {};
	}

}

void Expression::Compile()
{
	m_Compiled = false; //Sets the m_Compiled to false for safety

	//Compile the expression mathematical expression
	m_VarTable.add_variable(m_VarName1, m_Value1); //Adds the variable name (m_VarName) and a reference to their value (m_ValueX)
	m_VarTable.add_variable(m_VarName2, m_Value2); //Adds the variable name (m_VarName) and a reference to their value (m_ValueX)
	m_VarTable.add_pi(); //Adds "pi" as symbol to the compiler
	m_VarTable.add_constant("e", exprtk::details::numeric::constant::e); //Adds "e" as symbol to the compiler
	m_Expression.register_symbol_table(m_VarTable); //Register the m_VarTable

	//Compile and checks if it failed
	if (!g_Parser.compile(m_StrExpression, m_Expression))
	{
		std::cout << "Error: " << g_Parser.error().c_str() << "\n"; //Error message
	}
	else
	{
		m_Compiled = true; //Sets the compilation status (m_Compiled) to true
	}
}
