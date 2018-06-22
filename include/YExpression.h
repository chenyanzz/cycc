#pragma once

/**
 * Created by cy on 2018/5/26.
 *
 * A class for parsing expressions kind like "(i++)", "1+1" , "int_ret_func()" etc..
 */

#include <string>
#include <unordered_map>

#include "CYCC.h"
#include "Excutable.h"
#include "YVal.h"
#include <stack>

class YExpression: public Executable {
public:
	YVal* execute();
	void print() override;
	const char* className() const override;

	static bool parse(const char* str, YExpression* pExp);

	~YExpression();
	friend void test_YExpression();
	friend void calculator();

protected:
	//EOperatorType = val when **r_operand** is just a val to be excuted
	enum EOperatorType { add, sub, mul, div, val, UNDEFINED };

	class OperationNode: public Executable {
	public:
		/** Usages:
		* - l_operand++
		* - l_operand*r_operand
		* - condition?l_operand:r_operand
		* - l_operand[r_operand]
		*/

		Executable *l_operand = nullptr, *r_operand = nullptr, *condition = nullptr;

		EOperatorType opType = UNDEFINED;

		OperationNode(EOperatorType opType, Executable* l_operand,
			Executable* r_operand = nullptr, Executable* condition = nullptr);

		OperationNode();
		~OperationNode();

		const char* className() const override;
		void print() override;
		YVal* execute() override;
	};

	typedef unsigned int priority_t;
	typedef std::stack<OperationNode*> operation_stack_t;

protected:
	const std::string s_expr;
	static const std::unordered_map<EOperatorType, priority_t> operator_priority;
	OperationNode* operation_tree = nullptr;

protected:
	YExpression(const char* expr);
	static OperationNode* makeOperationTree(const char* str);
	static OperationNode* makeTestOperationTree();

	static EOperatorType parseSign(const char*& str);

	//currently just parse str as a YVal
	//TODO: parse YFunction s
	
	//it returns a YVal* for literal
	static Executable* parseIdentifier(const char*& str);

	static constexpr bool isCharInIdentifier(const char c);
	static priority_t getPriority(EOperatorType type);
	static OperationNode* getFatherNode(operation_stack_t& stack, priority_t priority);
};
