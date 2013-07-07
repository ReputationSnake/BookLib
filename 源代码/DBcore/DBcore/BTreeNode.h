//B���ڵ�ͷ�ļ�
//
//by baizhongwei
//blog http://haku.webcpp.net
#ifndef BTreeNode_H
#define BTreeNode_H

class BTreeNode{
	friend class BTree;
private:
	//�ڵ�����
	string value;
	//�ؼ�key
	int key;
	//���ڵ�
	BTreeNode* parent;
	//ǰ���ڵ�
	BTreeNode* pre;
	//����
	BTreeNode* next;
	//���ӽڵ�
	BTreeNode* leftchild;
	//���ӽڵ�
	BTreeNode* rightchild;
public:
	//���캯��
	BTreeNode(string value,int key);
};

#endif