//B��������ͷ�ļ�
//
//by baizhongwei
//blog http://haku.webcpp.net
#ifndef BTree_H
#define BTree_H

#include "BTreeNode.h"
#include <fstream>
#include <sstream>
class BTree{
	
private:
	/**���������
	*���ڱ�����ʱ���
	*@access private
	*/
	vector<string> k;
	/**���ڵ�
	*@access private
	*/
	BTreeNode* root;
	/**��ӽڵ�
	 *@access private
	 *@param BTreeNode* p �ڵ�
	 *@param string value �ڵ�ֵ
	 *@param int key �ڵ�key
	 *@param int m �㼶 Ĭ��Ϊ1
	 */
	BTreeNode* addnode(BTreeNode* p,string value,int key,int m=1);
	/**����p�ڵ㼰���ӽڵ�..������ļ�
	  *@param ofstream* fout
	  *@param BTreeNode* p �ڵ�
	  */
	int dump(ofstream* fout,BTreeNode* p);
	/**ɾ��������
	*��ɾ���ڵ�ռ�
	*@param BTreeNode* p �ڵ�
	*return BTreeNode*
	*/
	BTreeNode* del(BTreeNode* p);
	/**����B��
	*@param BTreeNode* p �ڵ�
	*@param int key �ڵ�����keyֵ
	*@return BTreeNode*
	*/
	BTreeNode* searchNode(BTreeNode* p,int key);
	/**����p�ڵ����ڲ㼶�Ľڵ�����
	*@param BTreeNode* p �ڵ�
	*@return int �ڵ�����
	*/
	int count(BTreeNode* p);
	/**����p�ڵ����ڲ㼶�����нڵ�..�㼶Ϊm
	*@param BTreeNode* p �ڵ�
	*@param int m �ڵ�㼶
	*@return void
	*/
	void splitnode(BTreeNode* p,int m);
	/**����������key=�Ľڵ�
	*@return value
	*/
	string BTree::search(BTreeNode* p,int key,string pit="\0");
	/**�����ڵ����ڲ㼶�����ӽڵ���Ѱ����С�Ľڵ�
	*@return string
	*/
	string BTree::tovector_le(BTreeNode* p,int max_key,int m=0);
	/**����B��������key��maxkeyС�Ľڵ�
	*@param BTreeNode* p �ڵ�
	*@param int key �ڵ�����keyֵ
	*@return int
	*/
	int BTree::searchNode_le(int maxkey,BTreeNode* w,int dep=0);
	/**�����ڵ����ڲ㼶�����ӽڵ���Ѱ������Ľڵ�
	*@return string
	*/
	string BTree::tovector_gt(BTreeNode* p,int min_key,int m=0);
	/**����B��������key��minkey��Ľڵ�
	*@param BTreeNode* p �ڵ�
	*@param int key �ڵ�����keyֵ
	*@return int
	*/
	int BTree::searchNode_gt(int minkey,BTreeNode* w,int dep=0);
public:
	/**�չ��캯��
	  */
	BTree();
	/**����������key=�Ľڵ�
	*@return BTreeNode*
	*/
	BTreeNode* BTree::searchNode(int key);
	/**Ĭ�Ϲ��캯��
	  *@param string rootvalue ���ڵ�ֵ
	  *@param int rootkey ���ڵ�����keyֵ
	  */
	BTree(string rootvalue,int rootkey);
	/**�����½ڵ㵽B��
	  *@param string value �ڵ�ֵ
	  *@param int key �ڵ�����keyֵ
	  *@return BTreeNode*
	  */
	BTreeNode* insert(string value,int key);
	/**�����ڵ����ӽڵ�..���value��ȵ��ӽڵ�..�ͽڵ�Ŀռ�
	*@param BTreeNode* p �ڵ�
	*/
	bool del(int key);
	/**��������B��..������ļ�
	  *@param ofstream* fout
	  */
	int dump(ofstream* fout);
	/**��ȡ�ļ�..��ȡǰ������յ�ǰ�ڵ�	 
	  *@param ifstream* fin
	  */
	int load(ifstream* fin);
	/**�������B��
	  */
	void clear();
	/**����������key=�Ľڵ�
	 *@return value
	 */
	string BTree::search(int key){
		return search(this->root,key);
	}
	/**����������key<maxkey�Ľڵ�
	*@return vector<string>
	*/
	vector<string> BTree::searchNode_le(int maxkey,int dep=0);
	/**����������key>minkey�Ľڵ�
	*@return vector<string>
	*/
	vector<string> BTree::searchNode_gt(int minkey,int dep=0);

};


#endif