#ifndef TreeNode_H
#define TreeNode_H

#include <string>
#include <vector>


class TreeNode
{
private:
	std::string textContent;
	std::string type;

	TreeNode *parent;

	std::vector<TreeNode *> children;

	int countNodesRec(TreeNode *root, int& count);

public:
	TreeNode();
	TreeNode(std::string iTextContent, std::string iTagName);
	~TreeNode();

	void appendChild(TreeNode *child);
	void setParent(TreeNode *parent);

	void popBackChild();
	void removeChild(int pos);

	bool hasChildren();
	bool hasParent();

	TreeNode* getParent();
	TreeNode* getChild(int pos);

	int childrenNumber();
	int grandChildrenNum();

	std::string getTextContent();
	std::string getType();
};

#endif#pragma once
