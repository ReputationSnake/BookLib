//B���ڵ�cpp�ļ�
//
//by baizhongwei
//blog http://haku.webcpp.net
#include "stdafx.h"
#include "BTreeNode.h"

BTreeNode::BTreeNode(string value,int key){
		this->value     =   value;
		this->key       =   key;
		this->parent    =   NULL;   /*���ڵ�*/
		this->pre       =   NULL;	/*ǰ���ڵ�*/
		this->next      =   NULL;   /*��̽ڵ�*/
		this->leftchild =   NULL;	/*����*/
		this->rightchild=   NULL;   /*�Һ���*/
	}