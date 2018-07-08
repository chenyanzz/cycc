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
	YVal* execute() override;
	void print() override;
	const char* className() const override;

	static YExpression* parse(const char* str);

	virtual ~YExpression();
	friend void test_YExpression();
	friend void calculator();

protected:

	//EOperatorType = val when **r_operand** is just a val to be excuted
	//NOTE that if there's only one operand, then it must be in the r_operand place
	enum EOperatorType {
		add, sub, mul, div, val, //op _ op
		neg, front_inc, front_dec, //_ op
		back_inc, back_dec, //op _
		NOTHING
	};

	class OperationNode;

	typedef unsigned int priority_t;
	typedef std::stack<OperationNode*> operation_stack_t;

protected:
	static const std::unordered_map<EOperatorType, priority_t> operator_priority;
	
	std::string s_expr;
	OperationNode* operation_tree = nullptr;

protected:
	static OperationNode* makeOperationTree(const char* str);
	static OperationNode* makeTestOperationTree();

	//@throws YParseFailedException
	static EOperatorType parseSign(const char*& str);

	//@retval NOTHING for failed
	static EOperatorType parseFrefix(const char*& str);

	//currently just parse str as a YVal
	//TODO: parse YFunction s

	//@return YVal* for literal
	static Executable* parseIdentifier(const char*& str);
	static priority_t getPriority(EOperatorType type);
	//from stack.top tillpriority
	static OperationNode* getFatherNode(operation_stack_t& stack, priority_t priority);

	static OperationNode* parsePrefixes(const char*& first, operation_stack_t& node_stack);
	static OperationNode* parseParentheses(const char*& first);
};

class YExpression::OperationNode: public Executable {
public:
	/** Usages:
	* - l_operand++
	* - l_operand*r_operand
	* - condition?l_operand:r_operand
	* - l_operand[r_operand]
	*/

	Executable *l_operand = nullptr, *r_operand = nullptr, *condition = nullptr;

	EOperatorType opType;

	OperationNode(EOperatorType opType, Executable* l_operand = nullptr,
		Executable* r_operand = nullptr, Executable* condition = nullptr);

	OperationNode();
	virtual ~OperationNode();

	const char* className() const override;
	void print() override;
	YVal* execute() override;
};
